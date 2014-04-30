from school import School, SchoolType

class RealEstateType:
	APARTMENT = 'apartment'
	CONDO = 'condo'
	LOT = 'lot/land'
	SINGLE_FAMILY = 'single family'
	TOWN_HOUSE = 'town house'
	UNKNOWN = 'unknown'

class PropertyType:
	""" transaction type """
	FORECLOSED = 'foreclosed'
	PREFORECLOSURE = 'pre-foreclosure'
	SALE = 'sale'
	RENT = 'rent'
	UNKNOWN = 'unknown'

'''
class Address(object):
	"""docstring for Address"""
	def __init__(self, lat, lon):
		super(Address, self).__init__()
		self.lat = lat
		self.lon = lon

	def __repr__(self):
		return 'lat = ' + lat + ', lon = ' + lon
'''		

class RealEstate:
	"""base class for real estate"""

	def __init__(self, html):
		self.html = html
		# id
		self.id = 'INVALID_ID'
		# lon
		self.lon = 0
		# lat
		self.lat = 0
		# url
		self.url = ''
		# real estate type
		self.type = RealEstateType.UNKNOWN
		# property type
		self.propertytype = PropertyType.UNKNOWN
		# value, unit us dollar
		self.price = 0
		# address
		self.address = 'unknown'
		# number of bedroom
		self.bedroom = 0
		# number of bathroom
		self.bathroom = 0
		# living space, unit square feet
		self.space = 0
		# lot, unit square feet, 1 acres = 43560 square feet
		self.lot = 0
		# year built
		self.year = 0
		# elementary school, if unknown, assign to -1
		self.elementary = None
		# middle school
		self.middle = None
		# high school
		self.high = None
		# schools in html
		self.schools = []
		# parse from html

	def __str__(self):
		r = '{\n' \
			+ '\t"id" : "' + self.id + '",\n' \
			+ '\t"url" : "' + self.url + '",\n' \
			+ '\t"price" : ' + str(self.price) + ',\n' \
			+ '\t"bedroom" : ' + str(self.bedroom) + ',\n' \
			+ '\t"bathroom" : ' + str(self.bathroom) + ',\n' \
			+ '\t"space" : "' + str(self.space) + '",\n' \
			+ '\t"lot" : "' + str(self.lot) + '"'
		if len(self.schools) > 0:
			r += ',\n' \
				+ '\t"schools" : "' + str(self.schools) + '"\n'
		else:
			r += '\n'
		r += '}'
		return r

	def __repr__(self):
		return self.__str__()
