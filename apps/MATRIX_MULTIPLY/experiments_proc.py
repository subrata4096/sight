#!/usr/gapps/asde/python/chaos_5_x86_64_ib/bin/python

import os
from subprocess import call
import random
import subprocess
import re
import math

#SIGHT_ROOT = "/g/g15/bronevet/code/sight"
CODE_ROOT = "/p/lscratchd/mitra3/sight_gbDevelop_fresh_cab/sight/apps/lulesh"

os.environ["SIGHT_FILE_OUT"] = "1"

if("DIM" in os.environ) :
  dimList = map(int, os.environ["DIM"].split(","))
else :
  spatial = [2, 3, 4, 5]

experiments = [1]
if("EXP_ID" in os.environ) :
  experiments = [int(os.environ["EXP_ID"])]

def sys(command, verbose=True) :
    print command;
    os.system(command);
    return

for idx in range(len(dimList)) :
	dim = str(dimList[idx])
	command = "./mm " + dim + " " + dim + " " + dim
	sys(command)

