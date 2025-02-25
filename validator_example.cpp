#include <iostream>
#include <fstream>
#include <utility>

#include "json.hpp"

int main(int argc, char **argv) {

	std::ifstream ifd("./sample", std::ios::in);

	JSON json;

	try {
		json = JSON::load(ifd);

	} catch ( const JSON::exception& e ) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}

	try {
		json.validate({
			{ "optional_missing_element", { JSON::OBJECT }},
			{ "arr", { JSON::ARRAY, false }},
			{ "arr2", { JSON::ARRAY, false }},
			{ "hello", { JSON::INT }},
			{ "missing", { .type = JSON::STRING, .optional = true }}
		});
	} catch ( const JSON::exception& e ) {
		std::cout << "VALIDATION FAILED: " << e.what() << std::endl;
	}

	return 0;
}
