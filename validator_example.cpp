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

	std::vector<JSON::PREDICATE> predicates = {
		{ "optional_missing_object", JSON::OBJECT },
		{ "optional_missing_string", { .type = JSON::STRING, .values = { "value1", "value2", "value3" }}},
		{ "optional_missing_int", { .type = JSON::INT, .values = { 5, 6, 7 }}},
		{ "arr", { .type = JSON::ARRAY, .required = true }},
		{ "arr2", { .type = JSON::ARRAY, .required = false }},
		//{ "hello", { .type = JSON::STRING, .required = false, .values = { "THI", "JSO" }}},
		{ "hello2", { .type = JSON::OBJECT, .validate = {
			{ "hello", { .type = JSON::OBJECT, .validate = {
				{ "this", JSON::INT },
				{ "another", { .type = JSON::INT, .values = { 1, 2, 3 }}}
			}}}
		}}},
		//{ "missing", { .type = JSON::STRING, .required = true }}
	};

	std::cout << "JSON validation predicate:\n" << predicates << "\n" << std::endl;

	try {
		json.validate(predicates);
	} catch ( const JSON::exception& e ) {
		std::cout << "json validation failed: " << e.what() << std::endl;
		return 1;
	}

	std::cout << "json validation succeeded" << std::endl;
	return 0;
}
