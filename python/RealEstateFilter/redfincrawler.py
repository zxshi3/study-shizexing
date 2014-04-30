from crawler import Crawler
from crawler import overrides

class RedfinCrawler:
	def __init__(self):
		self.host = 'http://www.redfin.com'

	@overrides(Crawler)
	def queryByZip(self, zip):
		# url = 'www.redfin.com/zipcode/95132/'
		pass

