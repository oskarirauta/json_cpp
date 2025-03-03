#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include "json.hpp"

static std::string make_path(const std::vector<std::string>& path) {

	std::string s;
	for ( auto& _d : path )
		s += ( !s.empty() ? " -> " : "" ) + _d;

	return s;
}

int main(int argc, char **argv) {

	std::ifstream ifd("./sample", std::ios::in);

	if ( !ifd.is_open() || !ifd.good() || ifd.bad()) {

		if ( ifd.is_open())
			ifd.close();

		std::cerr << "abort, failed to open file \"sample\"" << std::endl;
		return 1;
	}

	std::stringstream ss;
	std::string blob;

	ss << ifd.rdbuf();
	blob = ss.str();
	ifd.close();

	try {
		JSON json = JSON::parse(blob);
		std::cout << /*"dump:\n" <<*/ json.dump() << std::endl;

		std::vector<std::string> path = { "hello2", "hello", "this" };
		std::vector<std::string> path2 = { "hello2", "hello2", "not_this" };

		std::cout << "\n\ntesting if traversable path " << make_path(path) << " exists: " << ( json.contains(path) ? "true" : "false" ) << std::endl;
		std::cout << "testing if traversable path " << make_path(path2) << " exists: " << ( json.contains(path2) ? "true" : "false" ) << std::endl;

	} catch ( const JSON::exception& e ) {
		std::cout << "ERROR: " << e.what() << std::endl;
	}

	std::cout << std::endl;

	return 0;
}
