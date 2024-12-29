JSON_DIR?=./json
INCLUDES += -I$(JSON_DIR)/include

JSON_OBJS:= \
        objs/json.o \
	objs/json_conv.o \
	objs/json_iter.o \
	objs/json_dump.o \
	objs/json_const_iter.o \
	objs/json_foreach.o \
	objs/json_parser.o \
	objs/json_error.o

objs/json_conv.o: $(JSON_DIR)/src/json_conv.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/json_iter.o: $(JSON_DIR)/src/json_iter.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/json_dump.o: $(JSON_DIR)/src/json_dump.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/json_const_iter.o: $(JSON_DIR)/src/json_const_iter.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/json_foreach.o: $(JSON_DIR)/src/json_foreach.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/json_error.o: $(JSON_DIR)/src/json_error.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/json_parser.o: $(JSON_DIR)/src/json_parser.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/json.o: $(JSON_DIR)/src/json.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;
