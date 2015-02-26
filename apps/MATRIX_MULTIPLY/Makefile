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

MM_SRC  = direct.cc 
MM_INCL = matrix.h Measure_DCSL.h
MM_OBJS = $(MM_SRC:.cc=.o)
MM_DEPS = $(MM_SRC:.cc=.d)

CXXFLAGS += -std=c++0x
CXXFLAGS += -I../.. ${SIGHT_CFLAGS}
CXXFLAGS += -I.

MAKEDEPEND = gcc -MM -MT $*.o $(CXXFLAGS) -o $*.d.tmp $<

default: mm

mm: $(MM_OBJS)
	$(CXX) $(CXXFLAGS) $(MM_OBJS) $(LIBS) -o mm

%.o : %.cc
	@$(MAKEDEPEND)
	cp $*.d.tmp $*.d
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.d.tmp >> $*.d
	rm -f $*.d.tmp
	$(CXX) -c $(CXXFLAGS) -c -o $@ $< 

-include $(MM_DEPS)

clean:
	$(RM) $(MM_OBJS) $(MM_DEPS)

realclean: clean
	$(RM) mm


