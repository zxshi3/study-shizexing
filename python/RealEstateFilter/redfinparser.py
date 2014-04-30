from parser import Parser
from override import overrides

class RedfinParser(Parser):
	"""docstring for RedfinParser"""
	def __init__(self):
		super(RedfinParser, self).__init__()

	@overrides
	def parse(self, html):
		pass