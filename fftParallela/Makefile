H_CXX = g++
H_CXXFLAGS = -Wall -Wextra -std=c++1y #-DE_EXECUTABLE=\"$(E_EXECUTABLE)\"
H_INCLUDE = -I${EPIPHANY_HOME}/tools/host/include -le-hal
H_LD = $(H_CXX)
H_LDFLAGS = -ltiff
H_LIBS = -L${EPIPHANY_HOME}/tools/host/lib -le-hal

#TODO: device compilation

PROJECT_NAME = FFT_PARALLELA

H_EXECUTABLE = $(PROJECT_NAME)-host
H_SOURCES = tiff_handler.cpp host.cpp
H_OBJECTS = $(patsubst %.cpp, %-host.o, $(H_SOURCES))

all: $(H_EXECUTABLE)

$(H_EXECUTABLE) : $(H_OBJECTS)
	$(H_LD) $(H_LFLAGS) -o $@ $^ $(H_LIBS) $(H_LDFLAGS)

%-host.o : %.cpp
	$(H_CXX) $(H_CXXFLAGS) $(H_INCLUDE) -o $@ -c $<

.depend_host : $(H_SOURCES)
	$(H_CXX) $(H_CXXFLAGS) $(H_INCLUDE) -MM $^ | sed s/.o:/-host.o/ > $@

-include .depend-host

.PHONY: clean
clean:
	rm -f *.o 
