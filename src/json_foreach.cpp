#include <stdexcept>
#include "json/json.hpp"
#include "json/json_error.hpp"
#include "json/json_foreach.hpp"

bool JSON::fe_iterator::is_object() const {
	return std::holds_alternative<std::string>(*this);
}

bool JSON::fe_iterator::is_array() const {
	return std::holds_alternative<size_t>(*this);
}

size_t JSON::fe_iterator::index() const {
	return this -> indexed() ? std::get<size_t>(*this) : -1;
}

std::string JSON::fe_iterator::name() const {
	return this -> named() ? std::get<std::string>(*this) : "";
}

bool JSON::fe_iterator::named() const {
	return this -> is_object();
}

bool JSON::fe_iterator::indexed() const {
	return this -> is_array();
}

void JSON::for_each(const JSON::for_each_function lambda) {

	if ( *this == JSON::OBJECT ) {

		auto j = std::get<std::map<std::string, JSON>>(*this);

		for ( auto it = j.begin(); it != j.end(); it++ ) {
			fe_iterator fit = it -> first;
			lambda(fit, (*this)[it -> first]);
		}

	} else if ( *this == JSON::ARRAY ) {

		auto j = std::get<std::vector<JSON>>(*this);

		for ( size_t i = 0; i < j.size(); i++ ) {
			fe_iterator fit = i;
			lambda(fit, (*this)[i]);
		}

	} else {

		fe_iterator fit = nullptr;
		lambda(fit, *this);
	}
}

void JSON::for_each(const JSON::const_for_each_function lambda) const {

	if ( *this == JSON::OBJECT ) {

		auto j = std::get<std::map<std::string, JSON>>(*this);

		for ( auto it = j.begin(); it != j.end(); it++ ) {
			fe_iterator fit = it -> first;
			lambda(fit, (*this)[it -> first]);
		}

	} else if ( *this == JSON::ARRAY ) {

		auto j = std::get<std::vector<JSON>>(*this);

		for ( size_t i = 0; i < j.size(); i++ ) {
			fe_iterator fit = i;
			lambda(fit, (*this)[i]);
		}

	} else {

		fe_iterator fit = nullptr;
		lambda(fit, *this);
	}
}
