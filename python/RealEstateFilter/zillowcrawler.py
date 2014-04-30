from crawler import Crawler, overrides
from httpreader import HttpReader
from realestate import RealEstate

class ZillowCrawler(Crawler):
	def __init__(self):
		self.host = 'http://www.zillow.com'
		self.estates = []

	@overrides(Crawler)
	def queryByZip(self, zip):
		url = self.host + '/homes/' + zip + '_rb/'
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
		#print 'pages = ', pages
		return self.estates

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
		url = self.host + '/homes/' + zip + '_rb/' + page + '_p/'
		data = HttpReader.retrieveUrl(url)
		self.__collectEstates(data)


