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
			{ "optional_missing_object", JSON::OBJECT },
			{ "arr", { .type = JSON::ARRAY, .required = true }},
			{ "arr2", { .type = JSON::ARRAY, .required = false }},
			{ "hello", { .type = JSON::STRING, .required = false, .allowed_values = { "THI", "JSO" }}},
			{ "hello2", JSON::OBJECT },
			{ "missing", { .type = JSON::STRING, .required = true }}
		});
	} catch ( const JSON::exception& e ) {
		std::cout << "VALIDATION FAILED: " << e.what() << std::endl;
	}

	return 0;
}
