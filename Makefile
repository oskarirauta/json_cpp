all: world

CXX?=g++
CXXFLAGS?=--std=c++17 -Wall -fPIC -g

example_OBJS:= \
	objs/main.o

foreach_OBJS:= \
	objs/foreach_example.o

JSON_DIR:=.
include ./Makefile.inc

world: example foreach

$(shell mkdir -p objs)

objs/main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/foreach_example.o: foreach_example.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

example: $(JSON_OBJS) $(example_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@;

foreach: $(JSON_OBJS) $(foreach_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@;

.PHONY: clean
clean:
	@rm -rf objs example foreach
