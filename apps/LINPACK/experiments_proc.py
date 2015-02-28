#!/usr/gapps/asde/python/chaos_5_x86_64_ib/bin/python

import os
from subprocess import call
import random
import subprocess
import re
import math

#SIGHT_ROOT = "/g/g15/bronevet/code/sight"

os.environ["SIGHT_FILE_OUT"] = "1"

if("N" in os.environ) :
  NList = map(int, os.environ["N"].split(","))
else :
  NList = [100,200]

experiments = [1]
if("EXP_ID" in os.environ) :
  experiments = [int(os.environ["EXP_ID"])]

def sys(command, verbose=True) :
    print command;
    os.system(command);
    return

for idx in range(len(NList)) :
	n = str(NList[idx])
	command = "./linpack " + n
	sys(command)

