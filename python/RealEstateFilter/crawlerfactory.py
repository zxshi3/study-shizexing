from crawler import Crawler
from redfincrawler import RedfinCrawler
from zillowcrawler import ZillowCrawler

class CrawlerFactory(object):
	"""docstring for CrawlerFactory"""
	def __init__(self):
		super(CrawlerFactory, self).__init__()
	
	@classmethod
	def getCrawler(cls, url):
		if url.count('zillow') > 0:
			return ZillowCrawler()
		elif url.count('redfin') > 0:
			return RedfinCrawler()