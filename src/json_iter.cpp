#include <stdexcept>
#include "json/json.hpp"
#include "json/json_error.hpp"
#include "json/json_iter.hpp"

JSON::iterator JSON::begin() {

	return (( *this == OBJECT || *this == ARRAY ) && this -> empty()) ?
		JSON::iterator(this, 1) : JSON::iterator(this, 0);
}

JSON::iterator JSON::end() {

	return JSON::iterator(this, this -> empty() ? 1 : this -> size());
}


JSON::iterator::RESULT& JSON::iterator::operator *() {

	if ( std::holds_alternative<std::map<std::string, JSON>>(*this -> v)) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		this -> result = std::make_pair(it -> first, &it -> second);

	} else if ( std::holds_alternative<std::vector<JSON>>(*this -> v)) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		this -> result = std::make_pair(idx, &a -> at(idx));

	} else this -> result = this -> v;

	return result;
}

JSON::iterator::RESULT* JSON::iterator::operator ->() {

	if ( std::holds_alternative<std::map<std::string, JSON>>(*this -> v)) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		this -> result = std::make_pair(it -> first, &it -> second);

	} else if ( std::holds_alternative<std::vector<JSON>>(*this -> v)) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		this -> result = std::make_pair(idx, &a -> at(idx));

	} else this -> result = this -> v;

	return &result;
}

const JSON::TYPE JSON::iterator::RESULT::type() const {

	if ( this -> indexed())
		return std::get<std::pair<size_t, JSON*>>(*this).second -> type();
	else if ( this -> named())
		return std::get<std::pair<std::string, JSON*>>(*this).second -> type();
	else return std::get<JSON*>(*this) -> type();
}

const bool JSON::iterator::RESULT::operator ==(const JSON::TYPE type) const {

	if ( this -> indexed())
		return std::get<std::pair<size_t, JSON*>>(*this).second -> type() == type;
	else if ( this -> named())
		return std::get<std::pair<std::string, JSON*>>(*this).second -> type() == type;
	else return std::get<JSON*>(*this) -> type() == type;
}

const bool JSON::iterator::RESULT::operator !=(const JSON::TYPE type) const {

	return operator ==(type) ? false : true;
}

const bool JSON::iterator::RESULT::named() const {

	return std::holds_alternative<std::pair<std::string, JSON*>>(*this);
}

const bool JSON::iterator::RESULT::indexed() const {

	return std::holds_alternative<std::pair<size_t, JSON*>>(*this);
}

const size_t JSON::iterator::RESULT::index() const {

	if ( !this -> indexed())
		return -1;

	return std::get<std::pair<size_t, JSON*>>(*this).first;
}

const std::string JSON::iterator::RESULT::name() const {

	if ( !this -> named())
		return "";

	return std::get<std::pair<std::string, JSON*>>(*this).first;
}

JSON* JSON::iterator::RESULT::value() {

	if ( this -> indexed())
		return std::get<std::pair<size_t, JSON*>>(*this).second;
	else if ( this -> named())
		return std::get<std::pair<std::string, JSON*>>(*this).second;
	else return std::get<JSON*>(*this);

}

const size_t JSON::iterator::RESULT::length() const {

	if ( this -> indexed())
		return std::get<std::pair<size_t, JSON*>>(*this).second -> length();
	else if ( this -> named())
		return std::get<std::pair<std::string, JSON*>>(*this).second -> length();
	else return std::get<JSON*>(*this) -> length();
}

const size_t JSON::iterator::RESULT::size() const {

	if ( this -> indexed())
		return std::get<std::pair<size_t, JSON*>>(*this).second -> size();
	else if ( this -> named())
		return std::get<std::pair<std::string, JSON*>>(*this).second -> size();
	else return std::get<JSON*>(*this) -> size();
}

const bool JSON::iterator::RESULT::empty() const {

	if ( *this == OBJECT || *this == ARRAY )
		return this -> size() == 0;
	else if ( *this == STRING )
		return this -> to_string().empty();
	else return *this == NULLPTR ? true : false;
}

const std::string JSON::iterator::RESULT::to_string() const {

	try {
		if ( this -> indexed())
			return std::get<std::pair<size_t, JSON*>>(*this).second -> to_string();
		else if ( this -> named())
			return std::get<std::pair<std::string, JSON*>>(*this).second -> to_string();
		else return std::get<JSON*>(*this) -> to_string();

	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

double JSON::iterator::RESULT::to_float() const {

	try {
		if ( this -> indexed())
			return std::get<std::pair<size_t, JSON*>>(*this).second -> to_float();
		else if ( this -> named())
			return std::get<std::pair<std::string, JSON*>>(*this).second -> to_float();
		else return std::get<JSON*>(*this) -> to_float();

	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

double JSON::iterator::RESULT::to_double() const {

	try {
		if ( this -> indexed())
			return std::get<std::pair<size_t, JSON*>>(*this).second -> to_double();
		else if ( this -> named())
			return std::get<std::pair<std::string, JSON*>>(*this).second -> to_double();
		else return std::get<JSON*>(*this) -> to_double();

	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

long long JSON::iterator::RESULT::to_int() const {

	try {
		if ( this -> indexed())
			return std::get<std::pair<size_t, JSON*>>(*this).second -> to_int();
		else if ( this -> named())
			return std::get<std::pair<std::string, JSON*>>(*this).second -> to_int();
		else return std::get<JSON*>(*this) -> to_int();

	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

bool JSON::iterator::RESULT::to_bool() const {

	try {
		if ( this -> indexed())
			return std::get<std::pair<size_t, JSON*>>(*this).second -> to_bool();
		else if ( this -> named())
			return std::get<std::pair<std::string, JSON*>>(*this).second -> to_bool();
		else return std::get<JSON*>(*this) -> to_bool();

	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::RESULT::operator std::string() const {

	try {
		return this -> to_string();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::RESULT::operator double() const {

	try {
		return this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::RESULT::operator long long() const {

	try {
		return this -> to_int();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::RESULT::operator int() const {

	try {
		return this -> to_int();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::RESULT::operator bool() const {

	try {
		return this -> to_bool();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

std::ostream& operator <<(std::ostream& os, const JSON::iterator& it) {
	os << it.v -> to_string();
	return os;
}

std::ostream& operator <<(std::ostream& os, const JSON::iterator::RESULT& res) {
	os << res.to_string();
	return os;
}
