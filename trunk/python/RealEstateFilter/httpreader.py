import urllib2

class HttpReader:
	@staticmethod
	def retrieveUrl(url):
		f = urllib2.urlopen(url)
		data = f.read()
		f.close()
		return data

