
from abc import abstractmethod, ABCMeta
# http://stackoverflow.com/questions/1167617/in-python-how-do-i-indicate-im-overriding-a-method
# http://stackoverflow.com/questions/5856963/abstract-methods-in-python
def overrides(interface_class):
    def overrider(method):
        assert(method.__name__ in dir(interface_class))
        return method
    return overrider

class Crawler(object):
	__metaclass__ = ABCMeta
	def __init__(self):
		self.host = ''

	''' search all estate in $zip area 
	@return RealEstate[]
	'''
	@abstractmethod
	def queryByZip(self, zip):
		pass
		#raise NotImplementedError("no queryByZip implementation")
