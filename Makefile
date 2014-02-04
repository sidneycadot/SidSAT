
CC=g++
CXXFLAGS = -W -Wall -O3 -std=c++11

.PHONY : clean

SidSAT : SidSAT.o ReadDimacsCNF.o

SidSAT.o : SidSAT.cc ReadDimacsCNF.h

ReadDimacsCNF.o : ReadDimacsCNF.cc ReadDimacsCNF.h

clean :
	$(RM) *~ SidSAT SidSAT.o ReadDimacsCNF.o
