from redfinparser import RedfinParser
from zillowparser import ZillowParser

class ParserFactory(object):
	"""docstring for ParserFactory"""
	def __init__(self):
		super(ParserFactory, self).__init__()
	
	#@classmethod
	#def getParser(cls, key):
	@staticmethod
	def getParser(key):
		if key.count('zillow') > 0:
			return ZillowParser()
		elif key.count('redfin') > 0:
			return RedfinParser()