from httpreader import HttpReader
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
		self.parseRealEstateInfo(self.html)

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
		if idx < 0:
			idx = 0
		# idx is not used for price
		idx = h.find('class="price-large"', idx)
		if (idx == -1):
			print 'error\tcannot find price'
			return -1
		idx = h.find('$', idx)
		if (idx == -1):
			print 'error\tcannot find price mark $'
			return -2
		idx2 = h.find('</strong>', idx)
		if (idx2 == -1):
			print 'cannot identify price end tag'
			return -3
		price = h[idx:idx2]
		#print 'price = ' + price
		self.price = int(price.translate(None, '$,'))
		return idx2

	def parseAddress(self, h, idx):
		if idx < 0:
			idx = 0
		idx = h.find('"streetAddress">', idx)
		if (idx == -1):
			print 'cannot find streeAddress'
			return -1
		idx = idx + len('"streetAddress">')
		idx2 = h.find('</span>', idx)
		if (idx2 == -1):
			print 'cannot find streetAddress end tag'
			return -2
		streetAddress = h[idx:idx2]
		idx = h.find('"addressLocality">', idx2)
		if (idx == -1):
			print 'cannot find addressLocality'
			return -3
		idx = idx + len('"addressLocality">')
		idx2 = h.find('</span>', idx)
		if (idx2 == -1):
			print 'cannot find addressLocality end tag'
			return -4
		addressLocality = h[idx:idx2]
		idx = h.find('"addressRegion">', idx2)
		if idx == -1:
			print 'cannot find addressRegion'
			return -5
		idx = idx + len('"addressRegion">')
		idx2 = h.find('</span>', idx)
		if idx2 == -1:
			print 'cannot find addressRegion end tag'
			return -6
		addressRegion = h[idx:idx2]
		idx = h.find('"postalCode" class="hide">', idx2)
		if idx == -1:
			print 'cannot find postalCode'
			return -7
		idx = idx + len('"postalCode" class="hide">')
		idx2 = h.find('</span>', idx)
		if idx2 == -1:
			print 'cannot find postalCode end tag'
			return -8
		postalCode = h[idx:idx2]
		self.address = streetAddress + ', ' + addressLocality + ', ' + addressRegion + ' ' + postalCode
		#print 'address = ' + self.address
		return idx2

	def parseBedroom(self, h, idx):
		if idx < 0:
			idx = 0
		idx = h.find('"property-data">', idx)
		if idx == -1:
			print 'cannot find bedroom'
			return -1
		idx = idx + len('"property-data">')
		idx2 = h.find(' b', idx)
		if idx2 == -1:
			print 'cannot find bedroom end tag'
			#print '----------'
			#print h
			#print '----------'
			return -2
		self.bedroom = h[idx:idx2]
		#print 'bedroom = ' + self.bedroom
		return idx2

	def parseBathroom(self, h, idx):
		if idx < 0:
			idx = 0
		idx = h.find('<span class="hide-when-narrow">s</span>, ', idx)
		if idx == -1:
			print 'cannot find bathroom'
			return -1
		idx = idx + len('<span class="hide-when-narrow">s</span>, ')
		idx2 = h.find(' ba', idx)
		if idx2 == -1:
			print 'cannot find bathroom end tag'
			return -2
		self.bathroom = h[idx:idx2]
		#print 'bathroom = ' + self.bathroom
		#idx = h.find('<span class="hide-when-narrow">ths</span>, ', idx2) + len('<span class="hide-when-narrow">ths</span>, ')
		# if the house has only 1 bathroom, there is no 's'
		idx = h.find('<span class="hide-when-narrow">th', idx2)
		if idx == -1:
			print 'cannot pass bathroom unit'
			return -3
		idx = idx + len('<span class="hide-when-narrow">th')
		idx = h.find('</span>, ', idx)
		if idx == -1:
			print 'cannot pass bathroom end tag'
			return -4
		idx = idx + len('</span>, ')
		return idx

	def parseSpace(self, h, idx):
		if idx < 0:
			idx = 0
		idx2 = h.find('</dt>', idx)
		if idx2 == -1:
			print 'cannot find space end tag'
			return -1
		self.space = h[idx:idx2]
		#print 'space = ' + self.space
		return idx2

	def parseLot(self, h, idx):
		if idx < 0:
			idx = 0
		# some house has no lot information
		idx = h.find('<dt class="property-lot">', idx)
		if idx == -1:
			print 'cannot find lot'
			return -1
		idx = idx + len('<dt class="property-lot">')
		idx2 = h.find('</dt>', idx)
		if idx2 == -1:
			print 'cannot find lot end tag'
			return -2
		self.lot = h[idx:idx2]
		#print 'lot = ' + self.lot
		return idx2

	def parseBuiltYear(self, h, idx):
		if idx < 0:
			idx = 0
		#dt class="property-year"> Built in 2013</dt>
		idx = h.find('"property-year"> Built in ', idx)
		if idx == -1:
			print 'cannot find built year'
			#print '----------'
			#print h
			#print '----------'
			return -1
		idx = idx + len('"property-year"> Built in ')
		idx2 = h.find('</dt>', idx)
		if idx2 == -1:
			print 'cannot find built year end tag'
			return -2
		self.year = h[idx:idx2]
		#print 'built year = ' + self.year
		return idx2

	#import httpreader
	#from httpreader import HttpReader
	def querySchool(self):
		f = HttpReader.retrieveUrl(self.url)
		#print '+++++++++++++'
		#print f
		#print '+++++++++++++'
		#print f.count('<li class="nearby-school assigned-school">')
		idx = 0
		while idx != -1:
			idx = f.find('<li class="nearby-school assigned-school">', idx)
			if idx == -1:
				break
			idx = idx + len('<li class="nearby-school assigned-school">')
			idx2 = f.find('</li>', idx)
			if idx2 == -1:
				print 'cannot find school end tag'
				continue
			s = f[idx:idx2]
			#print 'ssssssssssss'
			#print s
			#print 'ssssssssssss'
			school = School(s)
			self.schools.append(school)
			if school.type == SchoolType.ELEMENTARY:
				if self.elementary == None:
					self.elementary = school
				else:
					print 'multiple elementary school'
					print 'self.elementary : ' + str(self.elementary)
					print 'school : ' + str(school)
			elif school.type == SchoolType.MIDDLE:
				if self.middle == None:
					self.middle = school
				else:
					print 'multiple middle school'
					print 'self.middle : ' + str(self.middle)
					print 'school : ' + str(school)
			elif school.type == SchoolType.HIGH:
				if self.high == None:
					self.high = school
				else:
					print 'multiple high school'
					print 'self.high : ' + str(self.high)
					print 'school : ' + str(school)
			else:
				print 'unknown type school : ' + str(school)

	def parseHouse(self, h):
		#print 'wow.... parse house'
		#print '--------------'
		#print h
		#print '--------------'
		idx = 0
		idx = self.parsePrice(h, idx)
		idx = self.parseAddress(h, idx)
		idx = self.parseBedroom(h, idx)
		idx = self.parseBathroom(h, idx)
		idx = self.parseSpace(h, idx)
		idx = self.parseLot(h, idx)
		idx = self.parseBuiltYear(h, idx)
		# TODO get school information
		self.querySchool()

	def parseRealEstateInfo(self, house):
		# 1. get id
		idx = house.find('zpid_') + len('zpid_')
		idx2 = house.find('"', idx)
		zpid = house[idx:idx2]
		self.id = zpid
		#print 'zillow property id = ' + zpid
		# 2. get lat, lon
		idx = house.find('longitude="', idx2) + len('longitude="')
		idx2 = house.find('"', idx)
		longitude = house[idx:idx2]
		self.lon = longitude
		#print 'longitude = ' + longitude
		idx = house.find('latitude="', idx2) + len('latitude="')
		idx2 = house.find('"', idx)
		latitude = house[idx:idx2]
		self.lat = latitude
		#print 'latitude = ' + latitude
		# 3. get url
		idx = house.find('/homedetails')
		idx2 = house.find('"', idx)
		url = house[idx:idx2]
		self.url = 'http://www.zillow.com' + url
		#print 'url = ' + self.url
		# 4,5 type & property type
		idx = house.find('<dl class="property-info-list col-1 column">')
		idx = house.find('<strong>', idx) + len('<strong>')
		idx2 = house.find('</strong>', idx)
		houseType = house[idx:idx2]
		if houseType.count('<span') > 0:
			idx = houseType.find('<span')
			idx = houseType.find('>', idx) + len('>')
			idx2 = houseType.find('</span>')
			houseType = houseType[idx:idx2]
		#print 'house type : ' + houseType
		self.setType(houseType)
		# TODO parse info by housetype
		{
			'House For Sale' : lambda e, h : e.parseHouse(h)
		}.get(houseType, lambda e, h : 'DO NOTHING')(self, house)

