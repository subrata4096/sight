#!/bin/bash
#
#BENCH_SOURCE=/g/g90/mitra3/work/LINPACK_Benchmark/from_fsu
INSTALL_DIR=install
mkdir -p $INSTALL_DIR
rm -f mm
g++ -O0 direct.cc -o mm >& compiler.txt
if [ $? -ne 0 ]; then
  echo "Errors compiling direct.cc"
  exit
fi
rm compiler.txt
#
mv mm $INSTALL_DIR/mm
#
echo "Executable installed as $INSTALL_DIR/mm"
