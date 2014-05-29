#!/usr/bin/python

import os

dirBuild = os.path.abspath('build')
if (os.path.exists(dirBuild)) :
    print 'exists'
else :
    os.makedirs(dirBuild)
    print 'not exists'
print dirBuild
os.chdir(dirBuild)
os.system('cmake ..')
os.system('make')
os.chdir('..')
