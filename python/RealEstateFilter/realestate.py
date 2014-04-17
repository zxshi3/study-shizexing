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

	def __init__(self):
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
		self.value = 0
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
		self.elementary = 0
		# junior school
		self.junior = 0
		# senior school
		self.senior = 0
	
	def setType(self, t):
		self.type = {
			'Apartment For Rent' : RealEstateType.APARTMENT,
			'Condo For Sale' : RealEstateType.CONDO,
			'For Sale by Owner' : RealEstateType.UNKNOWN,
			'Foreclosed' : RealEstateType.UNKNOWN,
			'House For Rent' : RealEstateType.SINGLE_FAMILY,
			'House For Sale' : RealEstateType.SINGLE_FAMILY,
			'Lot/Land For Sale' : RealEstateType.LOT,
			'Make Me Move' : RealEstateType.UNKNOWN,
			'Pre-Foreclosure' : RealEstateType.UNKNOWN,
			'Townhouse For Sale' : RealEstateType.TOWN_HOUSE,
		}.get(t, RealEstateType.UNKNOWN)
		self.propertytype = {
			'Apartment For Rent' : PropertyType.RENT,
			'Condo For Sale' : PropertyType.SALE,
			'For Sale by Owner' : PropertyType.SALE,
			'Foreclosed' : PropertyType.FORECLOSED,
			'House For Rent' : PropertyType.RENT,
			'House For Sale' : PropertyType.SALE,
			'Lot/Land For Sale' : PropertyType.SALE,
			'Make Me Move' : PropertyType.SALE,
			'Pre-Foreclosure' : PropertyType.PREFORECLOSURE,
			'Townhouse For Sale' : PropertyType.SALE,
		}.get(t, PropertyType.UNKNOWN)

	'''
	@staticmethod
	def checkTagFound(orignalstring, idx, errormessage):
		if idx == -1:
			print 'error:\t' + errormessage
			return false
		return true
	'''

	def parsePrice(self, h, idx):
		# idx is not used for price
		idx = h.find('class="price-large"', idx)
		if (idx == -1):
			print 'error\tcannot find price'
			return idx
		idx = h.find('$', idx)
		if (idx == -1):
			print 'error\tcannot find price mark $'
			return idx
		idx2 = h.find('</strong>', idx)
		if (idx2 == -1):
			print 'cannot identify price end tag'
			return
		price = h[idx:idx2]
		print 'price = ' + price
		# TODO, convert price to integer value
		return idx2

	def parseAddress(self, h, idx):
		idx = h.find('"streetAddress">', idx) + len('"streetAddress">')
		if (idx == -1):
			print 'cannot find streeAddress'
			return
		idx2 = h.find('</span>', idx)
		if (idx2 == -1):
			print 'cannot find streetAddress end tag'
			return
		streetAddress = h[idx:idx2]
		idx = h.find('<span itemprop="addressLocality">', idx2) + len('<span itemprop="addressLocality">')
		idx2 = h.find('</span', idx)
		addressLocality = h[idx:idx2]
		idx = h.find('<span itemprop="addressRegion">', idx2) + len('<span itemprop="addressRegion">')
		idx2 = h.find('</span', idx)
		addressRegion = h[idx:idx2]
		idx = h.find('<span itemprop="postalCode" class="hide">', idx2) + len('<span itemprop="postalCode" class="hide">')
		idx2 = h.find('</span', idx)
		postalCode = h[idx:idx2]
		self.address = streetAddress + ', ' + addressLocality + ', ' + addressRegion + ' ' + postalCode
		print 'address = ' + self.address
		return idx2

	def parseHouse(self, h):
		print 'wow.... parse house'
		print '--------------'
		print h
		print '--------------'
		idx = 0
		idx = self.parsePrice(h, idx)
		idx = self.parseAddress(h, idx)
		idx = h.find('<dt class="property-data">', idx) + len('<dt class="property-data">')
		idx2 = h.find(' b', idx)
		self.bedroom = h[idx:idx2]
		print 'bedroom = ' + self.bedroom
		idx = h.find('<span class="hide-when-narrow">s</span>, ', idx2) + len('<span class="hide-when-narrow">s</span>, ')
		idx2 = h.find(' ba', idx)
		self.bathroom = h[idx:idx2]
		print 'bathroom = ' + self.bathroom
		#idx = h.find('<span class="hide-when-narrow">ths</span>, ', idx2) + len('<span class="hide-when-narrow">ths</span>, ')
		# if the house has only 1 bathroom, there is no 's'
		idx = h.find('<span class="hide-when-narrow">th', idx2) + len('<span class="hide-when-narrow">th')
		idx = h.find('</span>, ', idx) + len('</span>, ')
		idx2 = h.find('</dt>', idx)
		self.space = h[idx:idx2]
		print 'space = ' + self.space
		# some house has no lot information
		idx = h.find('<dt class="property-lot">', idx2) + len('<dt class="property-lot">')
		print 'index for lot : ' + str(idx)
		print 'length : ' + str(len('<dt class="property-lot">'))
		idx2 = h.find('</dt>', idx)
		self.lot = h[idx:idx2]
		print 'lot = ' + self.lot
		idx = h.find('<dt class="property-year"> Built in ', idx2) + len('<dt class="property-year"> Built in ')
		idx2 = h.find('</dt>', idx)
		self.year = h[idx:idx2]
		print 'built year = ' + self.year