#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include "json.hpp"

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

        JSON::ERROR ec;

	try {
		JSON json = JSON::parse(blob);
		std::cout << /*"dump:\n" <<*/ json.dump() << std::endl;

	} catch ( const JSON::exception& e ) {
		std::cout << "ERROR: " << e.what() << std::endl;
	}

	return 0;
}
