#!/p/lscratchd/mitra3/my_sight_branch/sight/tools/python-dev/bin/python
import numpy as np
#import sklearn

import sys
import cPickle
def testTry():
	print "I am in testTry"
#	print sklearn

def loadModel(pklFName):
	print "loding: " , pklFName
	try:
		with open(pklFName, 'rb') as fid:
			print fid
                	model_loaded = cPickle.load(fid)
        	fid.close()
	except e:
		print e
	print model_loaded
        return model_loaded

def predictValue(regressor, inputArr):
	inArr = np.array(inputArr)
	print inArr
	#predictedVal = regressor.predict();
	#inputDataArr = []

#fname = sys.argv[1]

#m = loadModel(fname)

#testTry()
