####### Compiler, tools and options

GIT_VERSION		:= $(shell git describe --dirty --always --tags)

PICOP_SDK     	= ../..

STATIC = true
ifeq ($(STATIC),true)
$(info static library)
SDK_LIB       	= $(PICOP_SDK)/Lib/libPicoP_Api.a
LIBS          	= -L/usr/lib/x86_64-linux-gnu $(SDK_LIB) -lusb-1.0 -lbluetooth -lpthread -ludev
else
$(info dynamic library)
SDK_LIB       	= -lPicoP_Api
LIBRARY_PATH	= $(PICOP_SDK)/Lib
LIBS          	= -L$(LIBRARY_PATH) $(SDK_LIB) -Wl,-R$(LIBRARY_PATH) -L/usr/lib/x86_64-linux-gnu -lusb-1.0 -lbluetooth -lpthread -ludev
# shared module usage required softlinks to traverse the versions of the soname
# if released version is libPicoP_Api.so.0.12 then create following softlink in library path directory
#    $ sudo ln -sf libPicoP_Api.so.0.12.4 libPicoP_Api.so.0.12
#    $ sudo ln -sf libPicoP_Api.so.0.12 libPicoP_Api.so.0
#    $ sudo ln -sf libPicoP_Api.so.0 libPicoP_Api.so
endif

#CC            	= gcc
CXX           	= g++
#CFLAGS        	= -pipe -O2 -Wall -W -D_REENTRANT -std=c++11 -DVERSION=\"$(GIT_VERSION)\"
CXXFLAGS      	= -pipe -O2 -Wall -W -D_REENTRANT -std=c++11 -DVERSION=\"$(GIT_VERSION)\"
INCPATH       	= -I$(PICOP_SDK)/Interface -I.
LINK          	= g++
#AR            	= ar cqs
DEL_FILE      	= rm -f

####### Output directory

#OBJECTS_DIR   	= ./

####### Files
SOURCES       	= main.cpp
OBJECTS      	= main.o
TARGET        	= PicoP_Display_Console_Demo

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

#.c.o:
#	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

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