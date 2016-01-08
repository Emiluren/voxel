CXX=clang++
#CPPFLAGS=-O2 -W -Wall -pedantic -std=c++11
CPPFLAGS=-g -std=c++11
LIBS=`pkg-config --libs --static glfw3` -lGLEW -lGLU -lpng

OBJS=block.o camera.o chunk.o chunkmanager.o lib.o main.o matrix.o mesh.o \
	quaternion.o shader.o texturedShader.o vector.o vertex.o

#SRCS=block.cpp camera.cpp chunk.cpp chunkmanager.cpp lib.c main.cpp matrix.cpp mesh.cpp \
	 quaternion.cpp shader.cpp texturedShader.cpp vector.cpp vertex.cpp

#SRCS = $(wildcard *.cpp *.c)

voxel: $(OBJS)
	$(CXX) $(OBJS) -o voxel $(LIBS)

#depend: .depend
#
#.depend: $(SRCS)
#	rm -f ./.depend
#	$(CXX) $(CPPFLAGS) -MM $^ > ./.depend;
#
#include .depend

block.o: block.cpp block.hpp mesh.hpp vertex.hpp
	$(CXX) $(CPPFLAGS) -c block.cpp

camera.o: camera.cpp camera.hpp vector.hpp matrix.hpp macros.h chunk.hpp chunkmanager.hpp
	$(CXX) $(CPPFLAGS) -c camera.cpp

chunk.o: chunk.cpp chunk.hpp block.hpp mesh.hpp shader.hpp
	$(CXX) $(CPPFLAGS) -c chunk.cpp

chunkmanager.o: chunkmanager.cpp chunkmanager.hpp chunk.hpp
	$(CXX) $(CPPFLAGS) -c chunkmanager.cpp

lib.o: lib.c lib.h
	$(CXX) $(CPPFLAGS) -c lib.c

main.o: main.cpp chunk.hpp lib.h matrix.hpp vector.hpp block.hpp camera.hpp texturedShader.hpp shader.hpp mesh.hpp vector.hpp
	$(CXX) $(CPPFLAGS) -c main.cpp

matrix.o: matrix.cpp matrix.hpp macros.h
	$(CXX) $(CPPFLAGS) -c matrix.cpp

quaternion.o: quaternion.cpp quaternion.hpp vector.hpp
	$(CXX) $(CPPFLAGS) -c quaternion.cpp

shader.o: shader.cpp shader.hpp vertex.hpp lib.h
	$(CXX) $(CPPFLAGS) -c shader.cpp

texturedShader.o: texturedShader.cpp texturedShader.hpp shader.hpp vertex.hpp lib.h
	$(CXX) $(CPPFLAGS) -c texturedShader.cpp

vector.o: vector.cpp vector.hpp macros.h
	$(CXX) $(CPPFLAGS) -c vector.cpp

vertex.o: vertex.cpp vertex.hpp vector.hpp
	$(CXX) $(CPPFLAGS) -c vertex.cpp

.PHONY: clean

clean:
	rm *.o
	rm voxel
