from crawler import Crawler

class ZillowCrawler(Crawler):
	def __init__(self):
		self.host = 'http://www.zillow.com'

	@overrides(Crawler)
	def queryByZip(self, zip):
		pass

