#!/usr/gapps/asde/python/chaos_5_x86_64_ib/bin/python

import os
from subprocess import call
import random
import subprocess
import re
import math

SIGHT_ROOT = "/p/lscratchd/mitra3/my_sight_branch/sight"
APP_ROOT = SIGHT_ROOT + "/apps/BLACKSCHOLES/src"

os.environ["SIGHT_FILE_OUT"] = "1"

inputFileList = ["i10.txt", "i50.txt", "i100.txt", "i200.txt", "i500.txt", "i800.txt", "i1000.txt", "i2000.txt", "i5000.txt", "i8000.txt", "i10000.txt",  "i50000.txt"]

if("NUM_RUN_LIST" in os.environ) :
  numRunList = map(str, os.environ["NUM_RUN_LIST"].split(","))
else :
  numRunList = [100]

if("NUM_THREAD_LIST" in os.environ) :
  numThreadList = map(str, os.environ["NUM_THREAD_LIST"].split(","))
else :
  numThreadList = [1]

#experiments = [1]
#if("EXP_ID" in os.environ) :
#  experiments = [int(os.environ["EXP_ID"])]

isOpenMP = False
if("IS_OPENMP" in os.environ) :
	isOpenMP = map(bool, os.environ["IS_OPENMP"])
else :
	isOpenMP = False

def sys(command, verbose=True) :
    print command;
    os.system(command);
    return

for inputfile in inputFileList :
	for numRun in numRunList :
		for numThread in numThreadList :
			command = ""
			os.environ["NUM_RUNS"] = numRun
			if(isOpenMP):
				command = "./blackscholes_OMP " + numThread + " " + inputfile + " k.txt"
			else:
				command = "./blackscholes " + numThread + " " + inputfile + " k.txt"
			#print command
			sys(command)

