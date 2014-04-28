
from abc import abstractmethod
# http://stackoverflow.com/questions/1167617/in-python-how-do-i-indicate-im-overriding-a-method
# http://stackoverflow.com/questions/5856963/abstract-methods-in-python
def overrides(interface_class):
    def overrider(method):
        assert(method.__name__ in dir(interface_class))
        return method
    return overrider

class Crawler:
	def __init__(self):
		self.host = ''

	@abstractmethod
	def queryByZip(self, zip):
		pass
		raise NotImplementedError("no queryByZip implementation")

class RedfinCrawler:
	def __init__(self):
		self.host = 'http://www.redfin.com'

	@overrides(Crawler)
	def queryByZip(self, zip):
		# url = 'www.redfin.com/zipcode/95132/'
		pass

class CrawlerFactory(object):
	"""docstring for CrawlerFactory"""
	def __init__(self, arg):
		super(CrawlerFactory, self).__init__()
		self.arg = arg
	
	def getCrawler(url):
		if url.count('zillow') > 0:
			return ZillowCrawler()
		elif url.count('redfin') > 0:
			return RedfinCrawler()