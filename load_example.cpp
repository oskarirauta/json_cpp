#include <iostream>
#include <fstream>
#include <utility>

#include "json.hpp"

int main(int argc, char **argv) {

	std::ifstream ifd("./sample", std::ios::in);

	try {
		JSON json = JSON::load(ifd);
		std::cout << /*"dump:\n" <<*/ json.dump() << std::endl;

	} catch ( const JSON::exception& e ) {
		std::cout << "ERROR: " << e.what() << std::endl;
	}

	return 0;
}
