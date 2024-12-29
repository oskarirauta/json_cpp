#include <stdexcept>
#include "json/json.hpp"
#include "json/json_error.hpp"
#include "json/json_const_iter.hpp"

JSON::const_iterator JSON::begin() const {

	return (( *this == OBJECT || *this == ARRAY ) && this -> empty()) ?
		JSON::const_iterator(this, 1) : JSON::const_iterator(this, 0);
}

JSON::const_iterator JSON::end() const {

	return JSON::const_iterator(this, this -> empty() ? 1 : this -> size());
}

JSON::const_iterator JSON::cbegin() {

	return (( *this == OBJECT || *this == ARRAY ) && this -> empty()) ?
		JSON::const_iterator(this, 1) : JSON::const_iterator(this, 0);
}

JSON::const_iterator JSON::cend() {

	return JSON::const_iterator(this, this -> empty() ? 1 : this -> size());
}

const bool JSON::const_iterator::is_object() const {

	return std::holds_alternative<std::map<std::string, JSON>>(*this -> v);
}

const bool JSON::const_iterator::is_array() const {

	return std::holds_alternative<std::vector<JSON>>(*this -> v);
}

const JSON& JSON::const_iterator::operator *() {

	if ( this -> is_object()) {

		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return it -> second.as_const();

	} else if ( this -> is_array()) {

		const std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> at(idx).as_const();
	}

	return this -> v -> as_const();
}

const JSON* JSON::const_iterator::operator ->() {

	if ( this -> is_object()) {

		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return &it -> second.as_const();

	} else if ( this -> is_array()) {

		const std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return &a -> at(idx).as_const();
	}

	return &this -> v -> as_const();
}


const bool JSON::const_iterator::indexed() const {

	return this -> is_array();
}

const bool JSON::const_iterator::named() const {

	return this -> is_object();
}

const size_t JSON::const_iterator::index() const {

	return this -> is_array() ? this -> idx : -1;
}

const std::string JSON::const_iterator::name() const {

	if ( !this -> is_object())
		return "";

	const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
	auto it = m -> begin();
	std::advance(it, idx);
	return it -> first;
}

const std::string JSON::const_iterator::key() const {

	return this -> name();
}

const size_t JSON::const_iterator::size() const {

	if ( this -> is_object()) {

		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		return m -> size();

	} else if ( this -> is_array()) {

		const std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> size();

	}

	return this -> v -> size();
}

const size_t JSON::const_iterator::length() const {

	if ( this -> is_object()) {

		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		return m -> size();

	} else if ( this -> is_array()) {

		const std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> size();

	}

	return this -> v -> length();
}

const JSON& JSON::const_iterator::const_value() const {

	if ( this -> is_object()) {

		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return it -> second.as_const();

	} else if ( this -> is_array()) {

		const std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> at(idx).as_const();
	}

	return this -> v -> as_const();
}

const JSON& JSON::const_iterator::value() const {

	if ( this -> is_object()) {

		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return it -> second.as_const();

	} else if ( this -> is_array()) {

		const std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> at(idx).as_const();
	}

	return this -> v -> as_const();
}

const JSON::TYPE JSON::const_iterator::type() const {

	return this -> const_value().type();
}

const bool JSON::const_iterator::operator ==(const JSON::TYPE type) const {

	return this -> type() == type;
}

const bool JSON::const_iterator::operator !=(const JSON::TYPE type) const {

	return this -> type() != type;
}

const bool JSON::const_iterator::operator ==(const char& c) const {

	return this -> const_value() == c;
}

const bool JSON::const_iterator::operator ==(const unsigned char& c) const {

	return this -> const_value() == c;
}

const bool JSON::const_iterator::operator ==(const int& i) const {

	return this -> const_value() == i;
}

const bool JSON::const_iterator::operator ==(const unsigned int& i) const {

	return this -> const_value() == i;
}

const bool JSON::const_iterator::operator ==(const long& l) const {

	return this -> const_value() == l;
}

const bool JSON::const_iterator::operator ==(const unsigned long& l) const {

	return this -> const_value() == l;
}

const bool JSON::const_iterator::operator ==(const long long& ll) const {

	return this -> const_value() == ll;
}

const bool JSON::const_iterator::operator ==(const unsigned long long& ll) const {

	return this -> const_value() == ll;
}

const bool JSON::const_iterator::operator ==(const long double& ld) const {

	return this -> const_value() == ld;
}

const bool JSON::const_iterator::operator ==(const double& d) const {

	return this -> const_value() == d;
}

const bool JSON::const_iterator::operator ==(const float& f) const {

	return this -> const_value() == f;
}

const bool JSON::const_iterator::operator ==(const std::string& s) const {

	return this -> const_value() == s;
}

const bool JSON::const_iterator::operator ==(const char* s) const {

	return this -> const_value() == s;
}

const bool JSON::const_iterator::operator ==(const std::nullptr_t& n) const {

	return this -> const_value() == n;
}

const bool JSON::const_iterator::operator !=(const char& c) const {

	return this -> const_value() != c;
}

const bool JSON::const_iterator::operator !=(const unsigned char& c) const {

	return this -> const_value() != c;
}

const bool JSON::const_iterator::operator !=(const int& i) const {

	return this -> const_value() != i;
}

const bool JSON::const_iterator::operator !=(const unsigned int& i) const {

	return this -> const_value() != i;
}

const bool JSON::const_iterator::operator !=(const long& l) const {

	return this -> const_value() != l;
}

const bool JSON::const_iterator::operator !=(const unsigned long& l) const {

	return this -> const_value() != l;
}

const bool JSON::const_iterator::operator !=(const long long& ll) const {

	return this -> const_value() != ll;
}

const bool JSON::const_iterator::operator !=(const unsigned long long& ll) const {

	return this -> const_value() != ll;
}

const bool JSON::const_iterator::operator !=(const long double& ld) const {

	return this -> const_value() != ld;
}

const bool JSON::const_iterator::operator !=(const double& d) const {

	return this -> const_value() != d;
}

const bool JSON::const_iterator::operator !=(const float& f) const {

	return this -> const_value() != f;
}

const bool JSON::const_iterator::operator !=(const std::string& s) const {

	return this -> const_value() != s;
}

const bool JSON::const_iterator::operator !=(const char* s) const {

	return this -> const_value() != s;
}

const bool JSON::const_iterator::operator !=(const std::nullptr_t& n) const {

	return this -> const_value() != n;
}


JSON::const_iterator& JSON::const_iterator::operator =(const JSON& other) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "operator = is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator +=(const JSON& other) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "operator += is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator +=(const std::pair<std::string, JSON>& pair) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "operator += is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator +=(const std::initializer_list<std::pair<std::string, JSON>>& list) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "operator += is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const char& c) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const unsigned char& c) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const int& i) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const unsigned int& i) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const long& l) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const unsigned long& l) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const long long& ll) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const unsigned long long& ll) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const long double& ld) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const double& d) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const float& f) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const std::string& s) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const char* s) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const bool& b) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

JSON::const_iterator& JSON::const_iterator::operator =(const std::nullptr_t& n) const {

	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "assignment is not available for const JSON");
}

const bool JSON::const_iterator::empty() const {

	if ( this -> is_object() || this -> is_array())
		return this -> const_value().empty();
	else if ( this -> type() == JSON::STRING )
		return this -> to_string().empty();
	else return this -> type() == JSON::NULLPTR;
}

const std::string JSON::const_iterator::to_string() const {

	try {
		return this -> const_value().to_string();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

long double JSON::const_iterator::to_float() const {

	try {
		return this -> const_value().to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

long JSON::const_iterator::to_number() const {

	try {
		return this -> const_value().to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

bool JSON::const_iterator::to_bool() const {

	try {
		return this -> const_value().to_bool();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator std::string() const {

	try {
		return this -> to_string();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator float() const {

	try {
		return (float)this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator long double() const {

	try {
		return this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator double() const {

	try {
		return (double)this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator char() const {

	try {
		return (char)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator unsigned char() const {

	try {
		return (unsigned char)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator long() const {

	try {
		return (long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator unsigned long() const {

	try {
		return (unsigned long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator long long() const {

	try {
		return this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator unsigned long long() const {

	try {
		return (unsigned long long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator int() const {

	try {
		return (int)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator unsigned int() const {

	try {
		return (unsigned int)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::const_iterator::operator bool() const {

	try {
		return this -> to_bool();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const bool JSON::const_iterator::contains(const std::string& key) const {

	return this -> const_value().contains(key);
}

const bool JSON::const_iterator::is_convertible(const JSON::TYPE& to) const {

	return this -> const_value().is_convertible(to);
}

const bool JSON::const_iterator::convertible_to(const JSON::TYPE& to) const {

	return this -> const_value().convertible_to(to);
}

const JSON& JSON::const_iterator::as_const() {

	if ( this -> is_object()) {

		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return it -> second.as_const();

	} else if ( this -> is_array()) {

		const std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> at(idx).as_const();
	}

	return this -> v -> as_const();
}

const JSON& JSON::const_iterator::as_const() const {

	if ( this -> is_object()) {

		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return it -> second.as_const();

	} else if ( this -> is_array()) {

		const std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> at(idx).as_const();
	}

	return this -> v -> as_const();
}

const JSON JSON::const_iterator::operator[](const std::string& key) const {

	try {
		return this -> const_value().operator[](key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::const_iterator::operator[](const char* key) const {

	try {
		return this -> const_value().operator[](key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::const_iterator::operator[](size_t index) const {

	try {
		return this -> const_value().operator[](index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::const_iterator::operator[](int index) const {

	try {
		return this -> const_value().operator[](index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::const_iterator::at(const std::string& key) const {

	try {
		return this -> const_value().at(key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::const_iterator::at(const char* key) const {

	try {
		return this -> const_value().at(key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::const_iterator::at(const size_t index) const {

	try {
		return this -> const_value().at(index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::const_iterator::at(const int index) const {

	try {
		return this -> const_value().at(index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

std::ostream& operator <<(std::ostream& os, const JSON::const_iterator& it) {

	try {
		os << it.to_string();
		return os;
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}
