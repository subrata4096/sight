#!/bin/bash

rm -rf file_done
rm -rf dbg.SPARSE_MATRIX_MUL_*
export SIGHT_FILE_OUT=1


export EXP_ID=1
export NNZ_LIST=20,30,40,50,60,70,80,90,100,200,500,1000
export BLOCK_SIZE_LIST=1,2,4,6,8
export T_MAX_LIST=5
export M_MAX_LIST=100,500,1000,5000,10000,50000
./experiments_proc.py

export EXP_ID=2
export NNZ_LIST=20,30,40,50,60,70,80,90,100,200,500,1000
export BLOCK_SIZE_LIST=1,2,4,6,8
export T_MAX_LIST=5
export M_MAX_LIST=100,500,1000,5000,10000,50000
./experiments_proc.py

export EXP_ID=3
export NNZ_LIST=20,30,40,50,60,70,80,90,100,200,500,1000
export BLOCK_SIZE_LIST=1,2,4,6,8
export T_MAX_LIST=5
export M_MAX_LIST=100,500,1000,5000,10000,50000
./experiments_proc.py

rm -rf dbg.SPARSE_MATRIX_MUL.combined
../../../hier_merge dbg.SPARSE_MATRIX_MUL.combined dbg.SPARSE_MATRIX_MUL_*_*_*_*_*/structure
../../../slayout dbg.SPARSE_MATRIX_MUL.combined

touch file_done
