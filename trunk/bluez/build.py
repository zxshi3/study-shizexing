#!/usr/bin/python

import os, sys, logging, shutil

if os.path.isdir('build'):
    shutil.rmtree('build')
os.mkdir('build')
os.chdir('build')
os.system('cmake .. -G "Unix Makefiles"')
os.system('make')

os.chdir('..')

