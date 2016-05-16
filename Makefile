# Makefile for thermal simulation
# Author: 03.24.2016
# Y. Yang

TARGET  = SimCOMETCaptureQuench.exe
CXX     = g++
MAINSRC = main.cpp
VPATH   = src
LIBSRC  = IFdmException.cpp \
          IFdmAluminium.cpp \
          IFdmCopper.cpp \
          IFdmNbTi.cpp \
          IFdmKapton.cpp \
          IThermalContainer.cpp \
          IFdmThermalSolver.cpp \
          IFdmConstruction.cpp \
          IFdmBiotSavart.cpp \
          IFdmField.cpp \
          IFdmFieldHandle.cpp \
          IFdmDataflow.cpp \
          IFdmPostprocess.cpp \
          IMyPostprocess.cpp

# ----- cint -----
CINT      = rootcint
CINTARGET = IThermalContainerDict.cpp
CINTSRC   = include/IThermalContainer.h include/IThermalContainer_LinkDef.h  

SRC     = $(MAINSRC) $(LIBSRC) $(CINTARGET)
OBJS    = $(SRC:.cpp=.o)

CXXLIBS   =
CXXFLAGS  = -O3 -Wall -I include
ROOTLIBS  = `root-config --evelibs`
ROOTFLAGS = `root-config --cflags`

CXXLIBS  += $(ROOTLIBS)
CXXFLAGS += $(ROOTFLAGS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXLIBS) $^ -o $@

.o:.cpp
	$(CXX) $(CXXFLAGS) -c $<

$(CINTARGET): $(CINTSRC)
	$(CINT) -f $@ -c $^

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(CINTARGET) IThermalContainerDict*

.PHONY: install
install:
	mkdir -p bin
	cp -p $(TARGET) bin

