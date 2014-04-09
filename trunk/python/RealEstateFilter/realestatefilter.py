#!/usr/bin/python

import sys, urllib, urllib2

class RealEstateFilter(object):
	"""docstring for RealEstateFilter"""
	def __init__(self, arg):
		super(RealEstateFilter, self).__init__()
		self.arg = arg
		#print "arg.len = " + str(len(arg)) 
		#print 'args : ' + ', '.join(arg)
		self.zips = []
		for i in xrange(1,len(arg)):
			self.zips.append(arg[i])

	def queryByZip(self, zip):
		url = 'http://www.zillow.com/homes/' + zip + '_rb/'
		#print 'url = ' + url
		url = 'http://www.zillow.com/search/RealEstateSearch.htm?citystatezip=' + zip
		#r = urllib2.Request(url)
		#r.add_header('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8')
		#r.add_header('Referer', 'http://www.zillow.com/')
		f = urllib.urlopen(url)
		#f = urllib2.urlopen(r)
		data = f.read()
		f.close()
		print data
		idx = data.find('<div id="search-results"')
		#print data[idx:]
		houses = []
		i = 0
		while idx != -1:
			idx = data.find('<article', idx)
			if idx == -1:
				break
			idx2 = data.find('</article>', idx) + len('</article>')
			houses.append(data[idx:idx2])
			i = i + 1
			#print 'i = ' + str(i)
			idx = idx2
		print len(houses)
		print houses[0]
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