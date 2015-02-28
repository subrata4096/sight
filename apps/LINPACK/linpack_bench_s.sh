#!/bin/bash
#
BENCH_SOURCE=/g/g90/mitra3/work/LINPACK_Benchmark/from_fsu
INSTALL_DIR=/g/g90/mitra3/work/LINPACK_Benchmark//from_fsu/install
mkdir -p $INSTALL_DIR

#g++ -c -g -I $HOME/include linpack_bench_s.cpp >& compiler.txt
g++ -c -g -I $BENCH_SOURCE/include linpack_bench_s.cpp >& compiler.txt
if [ $? -ne 0 ]; then
  echo "Errors compiling linpack_bench_s.cpp"
  exit
fi
rm compiler.txt
#
g++ linpack_bench_s.o -lm
if [ $? -ne 0 ]; then
  echo "Errors linking and loading linpack_bench_s.o."
  exit
fi
#
rm linpack_bench_s.o
#
chmod ugo+x a.out
#mv a.out ~/bincpp/$ARCH/linpack_bench_s
mv a.out $INSTALL_DIR/linpack_bench_s
#
echo "Executable installed as $INSTALL_DIR/linpack_bench_s."
