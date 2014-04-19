class SchoolType:
	ELEMENTARY = 'elementary'
	MIDDLE = 'middle'
	HIGH = 'high'
	UNKNOWN = 'unknown'

class School:
	def __init__(self, html):
		self.name = ''
		self.url = ''
		self.grades = ''
		self.distance = ''
		self.rating = 0
		self.isAssigned = False
		self.type = SchoolType.UNKNOWN
		self.school_html = html
		# assigned?
		idx = self.__parseAssignedSchool()
		# rating
		idx = self.__parseRating(idx)
		# name & url
		idx = self.__parseNameAndUrl(idx)
		# grades
		idx = self.__parseGrades(idx)
		# distance
		idx = self.__parseDistance(idx)

	def __str__(self):
		return '{\n' \
			+ '\t\t"name" : "' + self.name + '",\n' \
			+ '\t\t"url" : "' + self.url + '",\n' \
			+ '\t\t"rating" : ' + str(self.rating) + ',\n' \
			+ '\t\t"grades" : "' + self.grades + '",\n' \
			+ '\t\t"distance" : "' + self.distance + '",\n' \
			+ '\t\t"type" : "' + self.type + '",\n' \
			+ '\t\t"assigned" : ' + str(self.isAssigned) + '\n' \
		+ '\t}'

	def __repr__(self):
		return self.__str__()

	def __parseAssignedSchool(self):
		idx = self.school_html.find('assigned-school')
		if idx != -1:
			self.isAssigned = True
		return idx 

	def __parseRating(self, idx):
		if idx < 0:
			idx = 0
		idx = self.school_html.find('<span class="gs-rating-number">', idx)
		if idx == -1:
			print 'cannot find rating'
			print '<school>'
			print self.school_html
			print '</school>'
			return -1
		else:
			idx = idx + len('<span class="gs-rating-number">')
			idx2 = self.school_html.find('</span>', idx)
			if idx2 == -1:
				print 'cannot find rating end tag'
				return -2
			else:
				self.rating = int(self.school_html[idx:idx2])
				#print 'school rating = ' + self.rating
				return idx2

	def __parseNameAndUrl(self, idx):
		if idx < 0:
			idx = 0
		idx = self.school_html.find('<span class="nearby-schools-name">', idx)
		if idx == -1:
			print 'cannot find school name & url'
			return -1
		else:
			idx = idx + len('<span class="nearby-schools-name">')
			idx = self.school_html.find('<a href="', idx)
			if idx == -1:
				print 'cannot find school url'
				return -2
			else:
				idx = idx + len('<a href="')
				idx2 = self.school_html.find('"', idx)
				if idx2 == -1:
					print 'cannot find school url end tag'
					#return -3
				else:
					self.url = 'http://www.zillow.com' + self.school_html[idx:idx2]
					#print 'school url = ' + self.url
					idx = idx2
				idx = self.school_html.find('>', idx)
				if idx == -1:
					print 'cannot find school name'
					return -4
				else:
					idx = idx + len('>')
					idx2 = self.school_html.find('</a>', idx)
					if idx2 == -1:
						print 'cannot find school name end tag'
						return -5
					else:
						self.name = self.school_html[idx:idx2]
						#print 'school name = ' + self.name
						return idx2

	def __parseGrades(self, idx):
		if idx < 0:
			idx = 0
		idx = self.school_html.find('<span class="nearby-schools-grades">', idx)
		if idx == -1:
			print 'cannot find school grades'
			return -1
		else:
			idx = idx + len('<span class="nearby-schools-grades">')
			idx2 = self.school_html.find('</span>', idx)
			if idx2 == -1:
				print 'cannot find school grades end tag'
				return -2
			else:
				self.grades = self.school_html[idx:idx2]
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
				return idx2

	def __parseDistance(self, idx):
		if idx < 0:
			idx = 0
		idx = self.school_html.find('<span class="nearby-schools-distance">')
		if idx == -1:
			print 'cannot find school distance'
			return -1
		else:
			idx = idx + len('<span class="nearby-schools-distance">')
			idx2 = self.school_html.find('</span>', idx)
			if idx2 == -1:
				print 'cannot find school distance end tag'
				return -2
			else:
				self.distance = self.school_html[idx:idx2]
				#print 'school distance = ' + self.distance
				return idx2
