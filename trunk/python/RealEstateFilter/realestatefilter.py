#!/usr/bin/python

import sys, urllib, urllib2

class RealEstateFilter(object):
	"""docstring for RealEstateFilter"""
	def __init__(self, arg):
		super(RealEstateFilter, self).__init__()
		self.arg = arg
		self.zips = []
		self.houses = []
		for i in xrange(1,len(arg)):
			self.zips.append(arg[i])

	def retrieveUrl(self, url):
		f = urllib2.urlopen(url)
		data = f.read()
		f.close()
		return data

	def parseHouseInfo(self, house):
		print '======================================'
		idx = house.find('zpid_') + len('zpid_')
		idx2 = house.find('"', idx)
		zpid = house[idx:idx2]
		print 'zillow property id = ' + zpid
		idx = house.find('longitude="', idx2) + len('longitude="')
		idx2 = house.find('"', idx)
		longitude = house[idx:idx2]
		print 'longitude = ' + longitude
		idx = house.find('latitude="', idx2) + len('latitude="')
		idx2 = house.find('"', idx)
		latitude = house[idx:idx2]
		print 'latitude = ' + latitude
		idx = house.find('/homedetails')
		idx2 = house.find('"', idx)
		url = house[idx:idx2]
		print 'url = ' + url
		idx = house.find('<dl class="property-info-list col-1 column">')
		idx = house.find('<strong>', idx) + len('<strong>')
		idx2 = house.find('</strong>', idx)
		houseType = house[idx:idx2]
		if houseType.count('<span') > 0:
			idx = houseType.find('<span')
			idx = houseType.find('>', idx) + len('>')
			idx2 = houseType.find('</span>')
			houseType = houseType[idx:idx2]
		print 'house type : ' + houseType
		# TODO parse info by housetype

	def collectHouses(self, data):
		idx = data.find('<article')
		while idx != -1:
			idx2 = data.find('</article>', idx) + len('</article>')
			house = data[idx:idx2]
			#print house
			self.parseHouseInfo(house)
			self.houses.append(data[idx:idx2])
			idx = data.find('<article', idx2)

	''' query houses by zip and continue to page $page '''
	def queryByZipMore(self, zip, page):
		url = 'http://www.zillow.com/homes/' + zip + '_rb/' + page + '_p/'
		data = self.retrieveUrl(url)
		self.collectHouses(data)

	''' query houses by zip '''
	def queryByZip(self, zip):
		url = 'http://www.zillow.com/homes/' + zip + '_rb/'
		#url = 'http://www.zillow.com/search/RealEstateSearch.htm?citystatezip=' + zip
		#r = urllib2.Request(url)
		#r.add_header('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8')
		#r.add_header('Referer', 'http://www.zillow.com/')
		data = self.retrieveUrl(url)
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
		print 'houses.length = ' + str(len(self.houses))
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