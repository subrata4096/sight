#!/p/lscratchd/mitra3/my_sight_branch/sight/tools/python-dev/bin/python
import numpy as np
#import sklearn

import sys
import cPickle

from errorDatastructure import *
from errorAnalysis import *
from pickleDump import *
from fields import *
from detectAnomaly import *

#import pickle
def testTry():
	print "I am in testTry"
#	print sklearn
def printModule(loc):
	print "wrap", loc
	return loadModel(loc)
	#return m
	#l = []
	#inArr = [1,0.6899999999999999,1.94,2e-05,100,1]
	#l.append(inArr)
	#inArr = np.array(l)
	#print inArr
	#inArrT = np.transpose(inArr)
	#inArrB = inArr[:,None]
	#print inArrB
        #predictValue(m,inArr)	

def loadModel(pklFName):
	print "loading: " , pklFName
	with open(pklFName, 'rb') as fid:
		print fid
		#l = fid.readline()
		#print l
               	model_loaded = cPickle.load(fid)
               	#model_loaded = pickle.load(fid)
        fid.close()
	print model_loaded
        return model_loaded

def predictValue(regressor, inputArr):
	#try:
		inArr = np.array(inputArr)
		#print inArr
		predictedVal = regressor.predict(inArr);
		#inputDataArr = []
		print "PREDICTED VALUE: " , predictedVal
	#except e:
	#	print e
		return predictedVal

def loadAnomalyDetectionEngine(dumpDir):
	setGlobalObject("activeDumpDirectory",dumpDir)
	anoDetectEngine = anomalyDetectionEngine()
        anoDetectEngine.dumpDirectory = dumpDir
        anoDetectEngine.loadPerModuleObjects()
	return anoDetectEngine

def createDataPoint(prodDataPointMap):
	print "\n\n In createDataPoint: ", str(prodDataPointMap)
	fDpt = FeatureDataPoint(prodDataPointMap)
	return fDpt

def getValidRangeOfValuesWithErrorAdjustment(prodDataPointMap,anomalyDetectObject,targetName):
	print "\n\n In createDataPoint: ", str(prodDataPointMap)
	fDpt = FeatureDataPoint(prodDataPointMap)
	rangeOfVal = anomalyDetectObject.getValidRangeOfTargetValue(targetName,fDpt)
	return rangeOfVal


if __name__ == "__main__" :
	fname = sys.argv[1]

	#m = loadModel(fname)
	m = printModule(fname)
        print m
	#testTry()
