####### Compiler, tools and options

GIT_VERSION 	:= $(shell git describe --dirty --always --tags)

PICOP_SDK     	= ../..
CXX 			:= g++
#DEBUG 			=-g3 -DNDEBUG -O3
DEBUG 			=-g3 -O3
#INCPATH    	= -I.
SDK_LIB         = $(PICOP_SDK)/Lib/libPicoP_Api.a
#C_INCLUDES      += -I$(PICOP_SDK)/Interface
INCPATH         = -I. -I$(PICOP_SDK)/Interface

CPPFLAGS 		:= $(DEBUG) $(OPT) $(WARN) $(PTHREAD) -std=c++11 -pipe `pkg-config gtkmm-3.0 --cflags` -DVERSION=\"$(GIT_VERSION)\"
GTKLIB 			=`pkg-config --cflags --libs gtk+-3.0` `pkg-config gtkmm-3.0 --libs`
PICOPLIB 		= $(SDK_LIB) -lusb-1.0 -lbluetooth
LDFLAGS 		:= $(GTKLIB) $(PICOPLIB)
LD 				:= g++

####### Files
SRCS = main.cpp 
OBJS = $(SRCS:.cpp=.o)
TARGET := PicoP_Display_GUI_Demo

# dependency file list
DEPS = $(SRCS:.cpp=.d)

####### Implicit rules

all: $(TARGET)

clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET) $(DEPS)

# dependency rule
%.d: %.cpp
	mkdir -p $(@D)
	$(CXX) $(INCPATH) $(CPPFLAGS) -M $< | \
	sed 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	mv $@.tmp $@

# build target
$(TARGET): $(OBJS)
	$(LD) $^ -o $@ $(LDFLAGS)

# include generated dependencies
include $(SRCS:.cpp=.d)

# object rule
%.o: %.cpp
	$(CXX) $(INCPATH) -c $(CPPFLAGS) -o $@ $(<:.d=.cpp)
