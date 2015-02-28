#!/bin/bash

rm -rf file_done
rm -rf dbg.LINPACK_*
export SIGHT_FILE_OUT=1

export EXP_ID=1
export N=10,50,100,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000
./experiments_proc.py


export EXP_ID=2
export N=10,50,100,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000
./experiments_proc.py


export EXP_ID=3
export N=10,50,100,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000
./experiments_proc.py

rm -rf dbg.LINPACK.combined
../../hier_merge dbg.LINPACK.combined dbg.LINPACK_*_*/structure
../../slayout dbg.LINPACK.combined

touch file_done
