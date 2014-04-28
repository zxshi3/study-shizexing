#!/usr/bin/python

import subprocess, time, os, platform, sys

zips = []
zips.append('94086')
zips.append('95035')
zips.append('95051')
zips.append('95119')
zips.append('95120')
zips.append('95121')
zips.append('95123')
zips.append('95124')
zips.append('95132')
zips.append('95135')
zips.append('95148')
zips.append('94536')
zips.append('94538')

for zip in zips:
	print 'searching... ', zip
	result = zip + '.txt'
	cmd = 'python realestatefilter.py ' + zip + ' > ' + zip + '.txt'
	print cmd 
	os.system(cmd)
	print 'sleep 3 seconds to avoid banned'
	time.sleep(3)
