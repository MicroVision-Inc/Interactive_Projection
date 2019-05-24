####### Compiler, tools and options

GIT_VERSION		:= $(shell git describe --dirty --always --tags)

PICOP_SDK     	= ../..

SDK_LIB       	= $(PICOP_SDK)/Lib/libPicoP_Api.a
LIBS          	= -L/usr/lib/x86_64-linux-gnu $(SDK_LIB) -lusb-1.0 -lbluetooth -lpthread -ludev

CC            	= gcc
CXX           	= g++
CFLAGS        	= -pipe -O2 -Wall -W -D_REENTRANT -std=c++11 -DVERSION=\"$(GIT_VERSION)\"
CXXFLAGS      	= -pipe -O2 -Wall -W -D_REENTRANT -std=c++11 -DVERSION=\"$(GIT_VERSION)\"
INCPATH       	= -I$(PICOP_SDK)/Interface -I.
LINK          	= g++
AR            	= ar cqs
DEL_FILE      	= rm -f

####### Output directory

OBJECTS_DIR   	= ./

####### Files

SOURCES       	= main.cpp
OBJECTS      	= main.o
TARGET        	= PicoP_Upgrade_FW_Demo

# dependency file list
DEPS = $(SOURCES:.cpp=.d)

first: all
	
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): $(OBJECTS)  
	$(LINK) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

clean: 
	-$(DEL_FILE) $(OBJECTS) $(TARGET) $(DEPS)

# dependency rule
%.d: %.cpp
	mkdir -p $(@D)
	$(CXX) $(INCPATH) $(CXXFLAGS) -M $< | \
	sed 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	mv $@.tmp $@

# include generated dependencies
include $(SOURCES:.cpp=.d)

# object rule
%.o: %.cpp
	$(CXX) $(INCPATH) -c $(CXXFLAGS) -o $@ $(<:.d=.cpp)
