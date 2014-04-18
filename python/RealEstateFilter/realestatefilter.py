#!/usr/bin/python

import sys, urllib, urllib2
#import realestate
from realestate import RealEstate
from httpreader import HttpReader
#from realestate import Address

class RealEstateFilter(object):
	"""docstring for RealEstateFilter"""
	def __init__(self, arg):
		super(RealEstateFilter, self).__init__()
		self.arg = arg
		self.zips = []
		# original house information
		self.houses = []
		self.estates = []
		for i in xrange(1,len(arg)):
			self.zips.append(arg[i])

	def parseRealEstateInfo(self, house):
		estate = RealEstate()
		print '======================================'
		estate.parseRealEstateInfo(house)
		if estate.price > 700000:
			#print 'tooooooooooooooooexpensive'
			#print 'estate:\n' + str(estate)
			pass
		else:
			print 'estate:\n' + str(estate)
			self.estates.append(estate)

	def collectHouses(self, data):
		idx = data.find('<article')
		while idx != -1:
			idx2 = data.find('</article>', idx) + len('</article>')
			house = data[idx:idx2]
			#print house
			self.parseRealEstateInfo(house)
			self.houses.append(data[idx:idx2])
			idx = data.find('<article', idx2)

	''' query houses by zip and continue to page $page '''
	def queryByZipMore(self, zip, page):
		url = 'http://www.zillow.com/homes/' + zip + '_rb/' + page + '_p/'
		data = HttpReader.retrieveUrl(url)
		self.collectHouses(data)

	''' query houses by zip '''
	def queryByZip(self, zip):
		url = 'http://www.zillow.com/homes/' + zip + '_rb/'
		#url = 'http://www.zillow.com/search/RealEstateSearch.htm?citystatezip=' + zip
		#r = urllib2.Request(url)
		#r.add_header('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8')
		#r.add_header('Referer', 'http://www.zillow.com/')
		data = HttpReader.retrieveUrl(url)
		#print data
		idx = data.find('<div id="search-results"')
		#print data[idx:]
		self.collectHouses(data)
		#print 'houses.length = ' + str(len(self.houses))
		idx = data.find('changePage(')
		pages = []
		while idx != -1:
			idx2 = data.find(')', idx)
			page = data[idx + len('changePage('):idx2]
			if page not in pages:
				pages.append(page)
				self.queryByZipMore(zip, page)
			idx = data.find('changePage(', idx2)
		#print 'houses.length = ' + str(len(self.houses))
		#self.parseHouseInfo(self.houses[0])
		#for house in self.houses:
		#	print house
		#print data.count('<article')
		#print data.count('</article>')
		#print data
		#<div id="search-results"
		#<article
		#</article>

	def query(self):
		for zip in self.zips:
			self.queryByZip(zip)

if __name__ == '__main__':
	filter = RealEstateFilter(sys.argv)
	filter.query()
