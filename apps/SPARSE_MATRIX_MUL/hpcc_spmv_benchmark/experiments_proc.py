#!/usr/gapps/asde/python/chaos_5_x86_64_ib/bin/python

import os
from subprocess import call
import random
import subprocess
import re
import math

SIGHT_ROOT = "/p/lscratchd/mitra3/my_sight_branch/sight"
APP_ROOT = SIGHT_ROOT + "/apps/SPARSE_MATRIX_MUL/hpcc_spmv_benchmark"

os.environ["SIGHT_FILE_OUT"] = "1"

if("NNZ_LIST" in os.environ) :
  nnzList = map(str, os.environ["NNZ_LIST"].split(","))
else :
  nnzList = [24, 25, 26, 27]

if("BLOCK_SIZE_LIST" in os.environ) :
  blockList = map(str, os.environ["BLOCK_SIZE_LIST"].split(","))
else :
  blockList = [1,2,3,4,6,8,10]

if("T_MAX_LIST" in os.environ) :
  tList = map(str, os.environ["T_MAX_LIST"].split(","))
else :
  tList = [1]

if("M_MAX_LIST" in os.environ) :
  mList = map(str, os.environ["M_MAX_LIST"].split(","))
else :
  mList = [400]

#experiments = [1]
#if("EXP_ID" in os.environ) :
#  experiments = [int(os.environ["EXP_ID"])]



def sys(command, verbose=True) :
    print command;
    os.system(command);
    return

for tMax in tList :
	for mMax in mList :
		for blkSize in blockList :
			for nnz in nnzList :
				os.environ["NNZ"] = nnz
				os.environ["BLOCK_SIZE"] = blkSize
				command = "./hpcc_spmv_benchmark " + tMax + " " + mMax
				sys(command)

