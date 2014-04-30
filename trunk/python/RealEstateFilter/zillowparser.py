from parser import Parser
from override import overrides
from realestate import RealEstate, RealEstateType, PropertyType
from httpreader import HttpReader
from school import School, SchoolType

class ZillowParser(Parser):
	"""docstring for ZillowParser"""
	def __init__(self):
		super(ZillowParser, self).__init__()

	@overrides(Parser)
	def parse(self, html):
		estate = RealEstate(html)
		estate.html = html
		# 1. get id
		idx = html.find('zpid_') + len('zpid_')
		idx2 = html.find('"', idx)
		zpid = html[idx:idx2]
		estate.id = zpid
		#print 'zillow property id = ' + zpid
		# 2. get lat, lon
		idx = html.find('longitude="', idx2) + len('longitude="')
		idx2 = html.find('"', idx)
		longitude = html[idx:idx2]
		estate.lon = longitude
		#print 'longitude = ' + longitude
		idx = html.find('latitude="', idx2) + len('latitude="')
		idx2 = html.find('"', idx)
		latitude = html[idx:idx2]
		estate.lat = latitude
		#print 'latitude = ' + latitude
		# 3. get url
		idx = html.find('/homedetails')
		idx2 = html.find('"', idx)
		url = html[idx:idx2]
		estate.url = 'http://www.zillow.com' + url
		#print 'url = ' + estate.url
		# 4,5 type & property type
		idx = html.find('<dl class="property-info-list col-1 column">')
		idx = html.find('<strong>', idx) + len('<strong>')
		idx2 = html.find('</strong>', idx)
		houseType = html[idx:idx2]
		if houseType.count('<span') > 0:
			idx = houseType.find('<span')
			idx = houseType.find('>', idx) + len('>')
			idx2 = houseType.find('</span>')
			houseType = houseType[idx:idx2]
		#print 'house type : ' + houseType
		ZillowParser.__setType(estate, houseType)
		# TODO parse info by housetype
		{
			'House For Sale' : lambda e, h : ZillowParser.__parseHouse(e, h)
		}.get(houseType, lambda e, h : 'DO NOTHING')(estate, html)
		return estate

	@staticmethod
	def __setType(estate, t):
		estate.type = {
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
		estate.propertytype = {
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

	@staticmethod
	def __parseHouse(estate, h):
		#print 'wow.... parse house'
		#print '--------------'
		#print h
		#print '--------------'
		idx = 0
		idx = ZillowParser.__parsePrice(estate, h, idx)
		idx = ZillowParser.__parseAddress(estate, h, idx)
		idx = ZillowParser.__parseBedroom(estate, h, idx)
		idx = ZillowParser.__parseBathroom(estate, h, idx)
		idx = ZillowParser.__parseSpace(estate, h, idx)
		idx = ZillowParser.__parseLot(estate, h, idx)
		idx = ZillowParser.__parseBuiltYear(estate, h, idx)
		# TODO get school information
		ZillowParser.__querySchool(estate)

	@staticmethod
	def __parsePrice(estate, h, idx):
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
		estate.price = int(price.translate(None, '$,'))
		return idx2

	@staticmethod
	def __parseAddress(estate, h, idx):
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
		estate.address = streetAddress + ', ' + addressLocality + ', ' + addressRegion + ' ' + postalCode
		#print 'address = ' + estate.address
		return idx2

	@staticmethod
	def __parseBedroom(estate, h, idx):
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
		estate.bedroom = int(h[idx:idx2])
		#print 'bedroom = ' + estate.bedroom
		return idx2

	@staticmethod
	def __parseBathroom(estate, h, idx):
		if idx < 0:
			idx = h.find('"property-data">')
			if idx == -1:
				print 'cannot locate property-data for bathroom'
				return -1
		# find bathroom end tag
		idx2 = h.find(' ba', idx)
		if idx2 == -1:
			print 'cannot find bathroom end tag'
			return -2
		idx = h.rfind('</span>, ', idx, idx2)
		if idx == -1:
			print 'cannot find bathroom start tag. print house : '
			print '<house id = ' + estate.id + '>'
			print h
			print '</house id = ' + estate.id + '>'
			return -3
		idx = idx + len('</span>, ')
		estate.bathroom = h[idx:idx2]
		#print 'bathroom = ' + estate.bathroom
		#idx = h.find('<span class="hide-when-narrow">ths</span>, ', idx2) + len('<span class="hide-when-narrow">ths</span>, ')
		# if the house has only 1 bathroom, there is no 's'
		idx = h.find('<span class="hide-when-narrow">th', idx2)
		if idx == -1:
			print 'cannot pass bathroom unit'
			return -3
		idx = idx + len('<span class="hide-when-narrow">th')
		return idx

	@staticmethod
	def __parseSpace(estate, h, idx):
		if idx < 0:
			idx = h.find('"property-data">')
			if idx == -1:
				print 'cannot locate property-data for living space'
				return -1
		idx2 = h.find(' sqft', idx)
		if idx2 == -1:
			print 'cannot find space end tag'
			return -2
		idx = h.rfind('</span>, ', idx , idx2)
		if idx == -1:
			print 'cannot find space start tag. print house : '
			print '<house id = ' + estate.id + '>'
			print h
			print '</house id = ' + estate.id + '>'
			return -3
		idx = idx + len('</span>, ')
		idx2 = idx2 + len(' sqft')
		estate.space = h[idx:idx2]
		return idx2

	@staticmethod
	def __parseLot(estate, h, idx):
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
		estate.lot = h[idx:idx2]
		#print 'lot = ' + estate.lot
		return idx2

	@staticmethod
	def __parseBuiltYear(estate, h, idx):
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
		estate.year = h[idx:idx2]
		#print 'built year = ' + estate.year
		return idx2

	@staticmethod
	def __querySchool(estate):
		f = HttpReader.retrieveUrl(estate.url)
		#print '+++++++++++++'
		#print f
		#print '+++++++++++++'
		idx = f.find('<li class="nearby-schools-header">')
		if idx != -1:
			idx = idx + len('<li class="nearby-schools-header">')
		else:
			idx = 0
		while idx != -1:
			# <li class="nearby-school">
			idx = f.find('<li class="nearby-school', idx)
			if idx == -1:
				break
			idx2 = f.find('</li>', idx)
			if idx2 == -1:
				print 'cannot find school end tag'
				continue
			idx2 = idx2 + len('</li>')
			s = f[idx:idx2]
			idx = idx2
			#print 'ssssssssssss'
			#print s
			#print 'ssssssssssss'
			school = School(s)
			estate.schools.append(school)
			if school.type == SchoolType.ELEMENTARY:
				if estate.elementary == None:
					estate.elementary = school
				else:
					if estate.elementary.isAssigned == False and school.isAssigned == True:
						estate.elementary = school
					elif estate.elementary.isAssigned == True and school.isAssigned == False:
						pass
					else:
						if estate.elementary.rating < school.rating:
							estate.elementary = school
						print 'multiple elementary school'
						print 'estate.elementary : ' + str(estate.elementary)
						print 'school : ' + str(school)
			elif school.type == SchoolType.MIDDLE:
				if estate.middle == None:
					estate.middle = school
				else:
					print 'multiple middle school'
					print 'estate.middle : ' + str(estate.middle)
					print 'school : ' + str(school)
			elif school.type == SchoolType.HIGH:
				if estate.high == None:
					estate.high = school
				else:
					print 'multiple high school'
					print 'estate.high : ' + str(estate.high)
					print 'school : ' + str(school)
			else:
				print 'unknown type school : ' + str(school)
