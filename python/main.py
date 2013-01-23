#!/usr/bin/python
import logging,os

#os.system('ls ; time')
os.environ[ 'PATH' ] = './script' + os.pathsep + os.environ['PATH']
#print 'path : ' + os.environ['PATH']
os.system('echo $PATH')
#logging.getLogger('').info('path : ' + os.environ['PATH'])
os.system('a.sh')