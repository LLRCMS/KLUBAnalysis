HDR   = ../interface/
OBJ   = ../obj/
LIB   = ../lib/
BIN   = ../bin/
PRG   = ../test/

HdrSuf  =  .h
SrcSuf  =  .cc
ObjSuf  =  .o
PrgSuf  =  .cpp
BinSuf  =  .exe
LibSuf  =  .so

HDRS  =  $(wildcard $(HDR)*$(HdrSuf))
SRCS  =  $(wildcard *$(SrcSuf))
_OBJS =  $(patsubst %$(SrcSuf), %$(ObjSuf), $(SRCS))
OBJS  =  $(addprefix $(OBJ),$(_OBJS))
PRGS  =  $(wildcard $(PRG)*$(PrgSuf))

_BINS    =  $(wildcard $(PRG)*$(PrgSuf))
__BINS   =  $(_BINS:$(PrgSuf)=$(BinSuf))
___BINS  =  $(notdir $(__BINS))
BINS	 =  $(addprefix $(BIN),${___BINS})

LINKDEF   =  $(wildcard ${HDR}*LinkDef.h)
DICTHDRS  =  $(patsubst $(LINKDEF),,$(HDRS)) $(LINKDEF)

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)

BOOSTFLAGS      = $(shell scram tool info boost | grep -e '^LIBDIR' | sed -e 's/LIBDIR=/-L/')
BOOSTLIB    = $(shell scram tool info boost | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')

TensFLowFLAGS = $(shell scram tool info tensorflow | grep -e '^LIBDIR' | sed -e 's/LIBDIR=/-L/')
TensFLowLIB   = $(shell scram tool info tensorflow | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')

PROTOBUFLIB   = $(shell scram tool info protobuf | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')
EIGENLIB   = $(shell scram tool info eigen | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')
BASELIBS = -I$(shell echo $$CMSSW_RELEASE_BASE | sed -e 's/$$/\/src/')

ARCHL = -m64
CXX  =  g++
CXXFLAGS  =  -Wall -O -fPIC -I$(HDR) $(ROOTCFLAGS) $(BOOSTFLAGS) $(BOOSTLIB)
CPP  =  g++
CPPFLAGS  = -Wall  $(ARCHL) -I$(HDR) $(ROOTCFLAGS) $(BOOSTFLAGS) $(BOOSTLIB) $(BASELIBS) $(TensFLowLIB) $(PROTOBUFLIB) $(EIGENLIB)

F    = gfortran
FFLAGS = -c

LD       =  g++
LDFLAGS  =  -rdynamic -shared -O3 $(ARCHL)
SONAME	 =  libDoubleHiggs.so
SONAME2  =  DoubleHiggs
SOFLAGS  =  -Wl,-soname,$(SONAME)

GLIBS   =  -lm -ldl -rdynamic $(ROOTGLIBS) -lGenVector -lFoam -lMinuit -lTMVA -lMLP -lXMLIO -lGpad  -lTreePlayer -lRooFit -lRooFitCore -lRooStats -lHHKinFit2 -L../../HHKinFit2 -lboost_regex -ltensorflow_cc -ltensorflow_framework $(TensFLowFLAGS)

DNNARCH = $(shell scram arch)
DNNPATH = $(shell echo $$CMSSW_BASE | grep "CMSSW" | sed -e 's/$$/\/lib/')/$(DNNARCH)/
DNNLIBS = -lcms_hh_proc_interfaceprocessing -lcms_hh_tf_inferenceinference -L$(DNNPATH)

#################################################
#if mac 64
ARCH  =  $(shell root-config --arch)
ifeq ($(ARCH),macosx64)
LibSuf  =  .dylib

ARCHL = -m64

CPPFLAGS  =  -Wall -W -Woverloaded-virtual -O2 $(ARCHL) -pipe -I$(HDR) $(ROOTCFLAGS)

CXXFLAGS  = -fPIC  -Wall -W -Woverloaded-virtual -O2 $(ARCHL) -pipe -I$(HDR) $(ROOTCFLAGS) 

LDFLAGS  =  -dynamiclib -shared -03 -single_module -undefined dynamic_lookup $(ARCHL)
SONAME	 =  libPlotter.dylib
SOFLAGS  =
endif
#################################################

.PHONY: all exe test clean exef

all: $(LIB)$(SONAME)

exe: $(BINS) 


test:
	@echo "HDRS = $(HDRS)"
	@echo "DICTHDRS = $(DICTHDRS)"
	@echo "SRCS = $(SRCS)"
	@echo "OBJS = $(OBJS)"
	@echo "PRGS = $(PRGS)"
	@echo "BINS = $(BIN)%$(BinSuf): $(PRG)%$(PrgSuf)"
	@echo "DNNLIBS   = $(DNNLIBS)"
	@echo "CMSSWPATH = $(CMSSWPATH)"


$(OBJ)%$(ObjSuf): %$(SrcSuf) $(HDRS)
	$(CXX) $< -c $(CXXFLAGS) -o $@  


$(OBJ)mydict.cc: $(DICTHDRS)
	@echo "Generating dictionary for  ..."
	rootcling -f $(OBJ)mydict.cc -c -p ${CXXFLAGS} $(DICTHDRS)

$(LIB)$(SONAME): $(OBJS) $(OBJ)mydict.o
	$(LD) $(LDFLAGS) $(OBJS) $(OBJ)mydict.o $(SOFLAGS) $(ROOTGLIBS) -o $(LIB)$(SONAME)
        @echo "Linking $(SONAME):"

$(BIN)%$(BinSuf): $(PRG)%$(PrgSuf) $(HDRS) $(LIB)$(SONAME)
	$(CPP) $<  -l$(SONAME2) $(CPPFLAGS) -L$(LIB) $(GLIBS) $(DNNLIBS) -o $@


clean:
	rm -f $(OBJ)*$(ObjSuf)  $(OBJ)mydict* $(BIN)*$(BinSuf) $(LIB)*$(LibSuf) 
