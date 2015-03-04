#!/bin/bash

rm -rf file_done
rm -rf dbg.BLACKSCHOLES_SCALAR_*
export SIGHT_FILE_OUT=1


export EXP_ID=1
#export NUM_RUN_LIST=100
export NUM_RUN_LIST=100,200,500,1000
export NUM_THREAD_LIST=1
#export NUM_THREAD_LIST=1,2,4,8,16,32
./experiments_proc.py

export EXP_ID=2
#export NUM_RUN_LIST=100
export NUM_RUN_LIST=100,200,500,1000
export NUM_THREAD_LIST=1
#export NUM_THREAD_LIST=1,2,4,8,16,32
./experiments_proc.py

export EXP_ID=3
#export NUM_RUN_LIST=100
export NUM_RUN_LIST=100,200,500,1000
export NUM_THREAD_LIST=1
#export NUM_THREAD_LIST=1,2,4,8,16,32
./experiments_proc.py


rm -rf dbg.BLACKSCHOLES_SCALAR.combined
sleep 10s
ulimit -n 2048

../../../hier_merge dbg.BLACKSCHOLES_SCALAR.combined dbg.BLACKSCHOLES_SCALAR_*_*_*_*_*/structure
../../../slayout dbg.BLACKSCHOLES_SCALAR.combined

touch file_done
