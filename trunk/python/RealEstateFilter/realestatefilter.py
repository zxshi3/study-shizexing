#!/usr/bin/python

import sys, urllib, urllib2, subprocess
from realestate import RealEstate
from httpreader import HttpReader

class RealEstateFilter(object):
	"""docstring for RealEstateFilter"""
	def __init__(self, arg):
		super(RealEstateFilter, self).__init__()
		self.arg = arg
		self.zips = []
		# all estates
		self.estates = []
		# filtered estates
		self.matchedEstates = []
		for i in xrange(1,len(arg)):
			self.zips.append(arg[i])

	def query(self):
		for zip in self.zips:
			self.__queryByZip(zip)

	def filter(self):
		for estate in self.estates:
			# 1. price
			if estate.price > 700000:
				#print '>70w. price = ' + str(estate.price) + '\turl = ' + estate.url
				continue
			# 2. bedroom
			if estate.bedroom < 3:
				#print '<3b. bedroom = ' + str(estate.bedroom) + '\turl = ' + estate.url
				continue
			#finally, we find match estate
			# 3. school
			n = len(estate.schools)
			if n > 0:
				ratings = 0
				for school in estate.schools:
					ratings += school.rating
				avg = ratings / n
				if avg < 7:
					#print 'rating < 7. avg = ' + str(avg) + '\turl = ' + estate.url
					# only avg < 7 && (estate.elementary < 8 || estate.middle < 8)
					if estate.elementary.rating < 8 or estate.middle.rating < 8:
						continue
			self.matchedEstates.append(estate)

	def __collectEstates(self, data):
		idx = data.find('<article')
		while idx != -1:
			idx2 = data.find('</article>', idx) + len('</article>')
			house_html = data[idx:idx2]
			#print house_html
			estate = RealEstate(house_html)
			self.estates.append(estate)
			idx = data.find('<article', idx2)

	''' query estates by zip and continue to page $page '''
	def __queryByZipMore(self, zip, page):
		url = 'http://www.zillow.com/homes/' + zip + '_rb/' + page + '_p/'
		data = HttpReader.retrieveUrl(url)
		self.__collectEstates(data)

	''' query estates by zip '''
	def __queryByZip(self, zip):
		url = 'http://www.zillow.com/homes/' + zip + '_rb/'
		#url = 'http://www.zillow.com/search/RealEstateSearch.htm?citystatezip=' + zip
		#r = urllib2.Request(url)
		#r.add_header('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8')
		#r.add_header('Referer', 'http://www.zillow.com/')
		data = HttpReader.retrieveUrl(url)
		#print data
		idx = data.find('<div id="search-results"')
		#print data[idx:]
		self.__collectEstates(data)
		idx = data.find('changePage(')
		pages = []
		while idx != -1:
			idx2 = data.find(')', idx)
			page = data[idx + len('changePage('):idx2]
			if page not in pages:
				pages.append(page)
				self.__queryByZipMore(zip, page)
			idx = data.find('changePage(', idx2)

if __name__ == '__main__':
	filter = RealEstateFilter(sys.argv)
	filter.query()
	#print '<search-results>'
	#print filter.estates
	#print '</search-results>'
	filter.filter()
	print '<match-result>'
	print filter.matchedEstates
	print '</match-result>'
	#print '<match-url>'
	for estate in filter.matchedEstates:
		subprocess.call(['open', '-a', 'Safari', estate.url])
	#print '</match-url>'
