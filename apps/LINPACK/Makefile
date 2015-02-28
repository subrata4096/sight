#
# ===== Build options begin here =====
#

#
# Specify the C++ compiler.  This is also used for linking the executable.
#

CXX = g++

#
# Specify compiler options (optimization level, debugging options, etc.)
#

#CXXFLAGS = -O3
CXXFLAGS = -g -O0

LIBS += -L../.. -lsight_structure ${SIGHT_LINKFLAGS}

MM_SRC = linpack_bench_s.cpp 
MM_INCL = Measure_DCSL.h
MM_OBJS = $(MM_SRC:.cpp=.o)
MM_DEPS = $(MM_SRC:.cpp=.d)

CXXFLAGS += -std=c++0x
CXXFLAGS += -I../.. ${SIGHT_CFLAGS}
CXXFLAGS += -I.

MAKEDEPEND = g++ -MM -MT $*.o $(CXXFLAGS) -o $*.d.tmp $<

default: linpack

linpack: $(MM_OBJS)
	$(CXX) $(CXXFLAGS) $(MM_OBJS) -lm $(LIBS) -o linpack

%.o : %.cpp
	@$(MAKEDEPEND)
	cp $*.d.tmp $*.d
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.d.tmp >> $*.d
	rm -f $*.d.tmp
	$(CXX) -c $(CXXFLAGS) -c -o $@ $< 

-include $(MM_DEPS)

clean:
	$(RM) $(MM_OBJS) $(MM_DEPS) linpack

realclean: clean
	$(RM) linpack


