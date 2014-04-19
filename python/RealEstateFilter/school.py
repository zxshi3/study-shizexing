class SchoolType:
	ELEMENTARY = 'elementary'
	MIDDLE = 'middle'
	HIGH = 'high'
	UNKNOWN = 'unknown'

class School:
	def __init__(self, s):
		self.name = ''
		self.url = ''
		self.grades = ''
		self.distance = ''
		self.rating = 0
		self.type = SchoolType.UNKNOWN
		# rating
		idx = s.find('<span class="gs-rating-number">')
		if idx == -1:
			print 'cannot find rating'
		else:
			idx = idx + len('<span class="gs-rating-number">')
			idx2 = s.find('</span>', idx)
			if idx2 == -1:
				print 'cannot find rating end tag'
			else:
				self.rating = int(s[idx:idx2])
				#print 'school rating = ' + self.rating
				idx = idx2
		# name & url
		idx = s.find('<span class="nearby-schools-name">', idx)
		if idx == -1:
			print 'cannot find school name & url'
		else:
			idx = idx + len('<span class="nearby-schools-name">')
			idx = s.find('<a href="', idx)
			if idx == -1:
				print 'cannot find school url'
			else:
				idx = idx + len('<a href="')
				idx2 = s.find('"', idx)
				if idx2 == -1:
					print 'cannot find school url end tag'
				else:
					self.url = 'http://www.zillow.com' + s[idx:idx2]
					#print 'school url = ' + self.url
					idx = idx2
				idx = s.find('>', idx)
				if idx == -1:
					print 'cannot find school name'
				else:
					idx = idx + len('>')
					idx2 = s.find('</a>', idx)
					if idx2 == -1:
						print 'cannot find school name end tag'
					else:
						self.name = s[idx:idx2]
						#print 'school name = ' + self.name
						idx = idx2
		# grades
		idx = s.find('<span class="nearby-schools-grades">', idx)
		if idx == -1:
			print 'cannot find school grades'
		else:
			idx = idx + len('<span class="nearby-schools-grades">')
			idx2 = s.find('</span>', idx)
			if idx2 == -1:
				print 'cannot find school grades end tag'
			else:
				self.grades = s[idx:idx2]
				#print 'school grades = ' + self.grades
				if self.grades.count('K') > 0:
					self.type = SchoolType.ELEMENTARY
					#print 'elementary school type'
				elif self.grades.count('12') > 0:
					self.type = SchoolType.HIGH
					#print 'high school type'
				else:
					self.type = SchoolType.MIDDLE
					#print 'middle school type'
				idx = idx2
		# distance
		idx = s.find('<span class="nearby-schools-distance">')
		if idx == -1:
			print 'cannot find school distance'
		else:
			idx = idx + len('<span class="nearby-schools-distance">')
			idx2 = s.find('</span>', idx)
			if idx2 == -1:
				print 'cannot find school distance end tag'
			else:
				self.distance = s[idx:idx2]
				#print 'school distance = ' + self.distance
				idx = idx2

	def __str__(self):
		return '{\n' \
			+ '\t\t"name" : "' + self.name + '",\n' \
			+ '\t\t"url" : "' + self.url + '",\n' \
			+ '\t\t"rating" : ' + str(self.rating) + ',\n' \
			+ '\t\t"grades" : "' + self.grades + '",\n' \
			+ '\t\t"distance" : "' + self.distance + '",\n' \
			+ '\t\t"type" : "' + self.type + '"\n' \
		+ '\t}'

	def __repr__(self):
		return self.__str__()

