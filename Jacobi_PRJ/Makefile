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
				build/jacobi_threadPool \
				build/jacobi_async \
				build/jacobi_barrier 


.PHONY: all clean

.SUFFIXES: .cpp 

all: $(TARGETS)

build/jacobi_sequential: Jacobi_sequential.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

build/jacobi_threadPool: Jacobi_threadPool.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

build/jacobi_async: Jacobi_async.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)
	
build/jacobi_barrier: Jacobi_barrier.cpp
	$(CXX11) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf build/*