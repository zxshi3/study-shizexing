from abc import abstractmethod, ABCMeta
# http://stackoverflow.com/questions/1167617/in-python-how-do-i-indicate-im-overriding-a-method
# http://stackoverflow.com/questions/5856963/abstract-methods-in-python
class Parser(object):
	__metaclass__ = ABCMeta
	def __init__(self):
		pass

	''' parse html page to get RealEstate 
	@return RealEstate
	'''
	@abstractmethod
	def parse(self, html):
		pass
