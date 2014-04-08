#!/usr/bin/python

import sys, urllib, urllib2

class RealEstateFilter(object):
	"""docstring for RealEstateFilter"""
	def __init__(self, arg):
		super(RealEstateFilter, self).__init__()
		self.arg = arg
		print "arg.len = " + str(len(arg)) 
		print 'args : ' + ', '.join(arg)
		self.zips = []
		for i in xrange(1,len(arg)):
			self.zips.append(arg[i])

	def queryByZip(self, zip):
		url = 'http://www.zillow.com/homes/' + zip + '_rb/'
		print 'url = ' + url
		f = urllib.urlopen(url)
		data = f.read()
		f.close()
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