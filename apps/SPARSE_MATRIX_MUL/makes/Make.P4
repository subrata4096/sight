# CITRIS-specific makefile for standalone SpMV benchmark
# author: hormozd

# Basic commands
CD           = cd
RM           = /bin/rm -f

# We're on this platform
ARCH         = $(arch)

# Compiler
CC           = g++
CFLAGS       = -O0 -g -Wall -std=c99
LINKER       = g++
LINKFLAGS    = 
ARCHIVER     = ar
ARFLAGS      = rc
RANLIB       = ranlib
