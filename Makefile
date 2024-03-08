all: world
CXX?=g++
CXXFLAGS?=--std=c++17 -Wall -fPIC -g
LDFLAGS?=-L/lib -L/usr/lib

INCLUDES+= -I./examples/include

OBJS:= \
	objs/main.o

JSON_DIR:=.
include ./Makefile.inc

world: example

$(shell mkdir -p objs)

objs/main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

example: $(JSON_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -L. $(LIBS) $^ -o $@;

.PHONY: clean
clean:
	@rm -rf objs
	@rm -f example
