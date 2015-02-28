#!/bin/bash

rm -rf file_done
rm -rf dbg.MATRIX_MULTIPLY_*
export SIGHT_FILE_OUT=1

export EXP_ID=1
export DIM=2,4,6,8,10,15,20,25,30,35,40,45,45,50,60,70,80,90,100,110,120,150,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000
./experiments_proc.py


export EXP_ID=2
export DIM=2,4,6,8,10,15,20,25,30,35,40,45,45,50,60,70,80,90,100,110,120,150,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000
./experiments_proc.py


export EXP_ID=3
export DIM=2,4,6,8,10,15,20,25,30,35,40,45,45,50,60,70,80,90,100,110,120,150,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000
./experiments_proc.py

rm -rf dbg.MATRIX_MULTIPLY.combined
../../hier_merge dbg.MATRIX_MULTIPLY.combined dbg.MATRIX_MULTIPLY_*_*_*_*/structure
../../slayout dbg.MATRIX_MULTIPLY.combined

touch file_done
