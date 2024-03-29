#!/usr/bin/python

import sys, urllib, urllib2, subprocess, platform
#from realestate import RealEstate
#from httpreader import HttpReader
from crawlerfactory import CrawlerFactory
#from crawler import Crawler

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

	''' query estates by zip '''
	def __queryByZip(self, zip):
		crawler = CrawlerFactory.getCrawler('zillow')
		self.estates = crawler.queryByZip(zip);

if __name__ == '__main__':
	filter = RealEstateFilter(sys.argv)
	filter.query()
	#print '<search-results>'
	#print filter.estates
	#print '</search-results>'
	#quit()	# short-circuit
	filter.filter()
	print '<match-result>'
	print filter.matchedEstates
	print '</match-result>'
	#print '<match-url>'
	for estate in filter.matchedEstates:
		if platform.system() == 'Darwin':
			subprocess.call(['open', '-a', 'Safari', estate.url])
		else:
			subprocess.call(['explorer', estate.url])
	#print '</match-url>'
