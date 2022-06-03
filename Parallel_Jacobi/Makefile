DIR := ${CURDIR}

ifndef FF_ROOT 
# FF_ROOT		= ${DIR}
FF_ROOT		= ${HOME}/fastflow
endif

CXX			= g++ -std=c++2a 
CXX11		= g++-11 -std=c++2a
INCLUDES	= -I $(FF_ROOT) 
CXXFLAGS  	= -g # -DBLOCKING_MODE -DFF_BOUNDED_BUFFER

LDFLAGS 	= -pthread
OPTFLAGS	= -finline-functions -w -DNDEBUG -O3

PRELDFLAGS 	= LD_PRELOAD=${DIR}/jemalloc/lib/libjemalloc.so.2

TARGETS 	=	build/jacobi_sequential \
				build/jacobi_std \
				build/jacobi_fastflow

.PHONY: all clean

.SUFFIXES: .cpp 

all: $(TARGETS)

build/jacobi_sequential: Jacobi_sequential.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

build/jacobi_std: Jacobi_std.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)
	
build/jacobi_fastflow: Jacobi_fastflow.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf build/*