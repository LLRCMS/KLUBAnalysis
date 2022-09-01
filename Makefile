BASEDIR := $(shell pwd)

HDR := $(BASEDIR)/interface
OBJ := $(BASEDIR)/obj
LIB := $(BASEDIR)/lib
BIN := $(BASEDIR)/bin
PRG := $(BASEDIR)/test
SRC := $(BASEDIR)/src

HDR_SUF := .h
SRC_SUF := .cc
OBJ_SUF := .o
PRG_SUF := .cpp
BIN_SUF := .exe
LIB_SUF := .so

HDRS := $(wildcard $(HDR)/*$(HDR_SUF))
SRCS := $(wildcard $(SRC)/*$(SRC_SUF))
OBJS := $(patsubst %$(SRC_SUF), %$(OBJ_SUF), $(SRCS))
PRGS := $(wildcard $(PRG)/*$(PRG_SUF))

BINS_TMP1 := $(wildcard $(PRG)/*$(PRG_SUF))
BINS_TMP2 := $(BINS_TMP1:$(PRG_SUF)=$(BIN_SUF))
BINS_TMP3 := $(notdir $(BINS_TMP2))
BINS	  := $(addprefix $(BIN)/,${BINS_TMP3})

LINKDEF  = $(wildcard ${HDR}/*LinkDef.h)
DICTHDRS = $(patsubst $(LINKDEF),,$(HDRS)) $(LINKDEF)

PROJDIR = $(shell ( cd $$CMSSW_BASE/src && /bin/pwd ))

ROOTCFLAGS = $(shell root-config --cflags)
ROOTGLIBS  = $(shell root-config --glibs)

BOOSTFLAGS = $(shell ( cd $(PROJDIR) && scram tool info boost ) | grep -e '^LIBDIR' | sed -e 's/LIBDIR=/-L/')
BOOSTLIB   = $(shell ( cd $(PROJDIR) && scram tool info boost ) | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')

TensFLowFLAGS = $(shell ( cd $(PROJDIR) && scram tool info tensorflow ) | grep -e '^LIBDIR' | sed -e 's/LIBDIR=/-L/')
TensFLowLIB   = $(shell ( cd $(PROJDIR) && scram tool info tensorflow ) | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')

PROTOBUFLIB = $(shell ( cd $(PROJDIR) && scram tool info protobuf ) | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')
EIGENLIB    = $(shell ( cd $(PROJDIR) && scram tool info eigen ) | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')
TBBLIB      = $(shell ( cd $(PROJDIR) && scram tool info tbb ) | grep -e '^INCLUDE' | sed -e 's/INCLUDE=/-I/')
BASELIBS    = -I$(shell echo $$CMSSW_RELEASE_BASE | sed -e 's/$$/\/src/')
CMSSWLIBS   = -I$(shell echo $$CMSSW_BASE | sed -e 's/$$/\/src/')
HHKINFITLIB = -I$(PROJDIR)/HHKinFit2/interface

ARCHL    = -m64
CXX      = g++
CXXFLAGS = -Wall -O -fPIC -I$(HDR) $(ROOTCFLAGS) $(BOOSTFLAGS) $(BOOSTLIB) $(BASELIBS) $(CMSSWLIBS) $(TensFLowLIB) $(PROTOBUFLIB) $(EIGENLIB) $(TBBLIB) $(HHKINFITLIB)
CPP      = g++
CPPFLAGS = -Wall  $(ARCHL) -I$(HDR) $(ROOTCFLAGS) $(BOOSTFLAGS) $(BOOSTLIB) $(BASELIBS) $(CMSSWLIBS) $(TensFLowLIB) $(PROTOBUFLIB) $(EIGENLIB) $(TBBLIB) $(HHKINFITLIB)

F      = gfortran
FFLAGS = -c

LD      = g++
LDFLAGS = -rdynamic -shared -O3 $(ARCHL)
LIB_NAME  = libDoubleHiggs.so
LIB_NAME_SHORT = DoubleHiggs
SOFLAGS = -Wl,-soname,$(LIB_NAME)

GLIBS = -lm -ldl -rdynamic $(ROOTGLIBS) -lGenVector -lFoam -lMinuit -lTMVA -lMLP -lXMLIO -lGpad  -lTreePlayer -lRooFit -lRooFitCore -lRooStats -L$(PROJDIR)/HHKinFit2 -lHHKinFit2 -lboost_regex -lboost_system -lboost_filesystem -ltensorflow_cc -ltensorflow_framework $(TensFLowFLAGS)

DNNARCH    = $(shell scram arch)
DNNPATH    = $(shell echo $$CMSSW_BASE | grep "CMSSW" | sed -e 's/$$/\/lib/')/$(DNNARCH)/
DNNLIBS    = -lcms_hh_proc_interfaceprocessing -lcms_hh_tf_inferenceinference -lMulticlassInferenceMulticlassInference -L$(DNNPATH)
FWCOREPATH = $(shell echo $$CMSSW_RELEASE_BASE | sed -e 's/$$/\/lib/')/$(DNNARCH)/
FWCORELIBS = -lFWCoreMessageLogger -lCondFormatsJetMETObjects -lJetMETCorrectionsModules -L$(FWCOREPATH)

SVfitLIBS  = -lTauAnalysisClassicSVfit -lTauAnalysisSVfitTF -L$(DNNPATH)
HHbtagLIBS = -lHHToolsHHbtag -L${DNNPATH}

#################################################
#if mac 64
ARCH =  $(shell root-config --arch)
ifeq ($(ARCH),macosx64)
	LIB_SUF   = .dylib
	ARCHL    = -m64
	CPPFLAGS = -Wall -W -Woverloaded-virtual -O2 $(ARCHL) -pipe -I$(HDR) $(ROOTCFLAGS)
	CXXFLAGS = -fPIC  -Wall -W -Woverloaded-virtual -O2 $(ARCHL) -pipe -I$(HDR) $(ROOTCFLAGS) 
	LDFLAGS  = -dynamiclib -shared -03 -single_module -undefined dynamic_lookup $(ARCHL)
	LIB_NAME   = libPlotter.dylib
	SOFLAGS  =
endif
#################################################

.PHONY: all exe test clean

all: $(LIB)/$(LIB_NAME)

test:
	@echo "HDRS		= $(HDRS)"
	@echo "DICTHDRS = $(DICTHDRS)"
	@echo "SRCS		= $(SRCS)"
	@echo "OBJS		= $(OBJS)"
	@echo "PRGS		= $(PRGS)"
	@echo "BINS		= $(BINS)"
	@echo "DNNLIBS	= $(DNNLIBS)"

exe: $(BINS)
	( cd "$(PROJDIR)/KLUBAnalysis/lib" && rm -f "mydict_rdict.pcm" && ln -s "../obj/mydict_rdict.pcm" . )

$(OBJ)/%$(OBJ_SUF): %$(SRC_SUF) $(HDRS)
	$(CXX) $< -c $(CXXFLAGS) -o $@  

$(OBJ)/mydict.cc: $(DICTHDRS)
	@echo "Generating dictionary for  ..."
	rootcling -f $(OBJ)/mydict.cc -c -p ${CXXFLAGS} $(DICTHDRS)

$(LIB)/$(LIB_NAME): $(OBJS) $(OBJ)/mydict.o
	$(LD) $(LDFLAGS) $(OBJS) $(OBJ)/mydict.o $(SOFLAGS) $(ROOTGLIBS) -o $(LIB)/$(LIB_NAME)
	@echo "Linking $(LIB_NAME):"

$(BIN)/%$(BIN_SUF): $(PRG)/%$(PRG_SUF) $(HDRS) $(LIB)/$(LIB_NAME)
	$(CPP) $< -l$(LIB_NAME_SHORT) $(CPPFLAGS) -L$(LIB) $(GLIBS) $(DNNLIBS) $(SVfitLIBS) $(HHbtagLIBS) $(FWCORELIBS) -o $@

clean:
	rm -f $(OBJ)/*$(OBJ_SUF) $(OBJ)/mydict* $(BIN)/*$(BIN_SUF) $(LIB)/*$(LIB_SUF) 
