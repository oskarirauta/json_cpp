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

const bool JSON::iterator::is_object() const {

	return std::holds_alternative<std::map<std::string, JSON>>(*this -> v);
}

const bool JSON::iterator::is_array() const {

	return std::holds_alternative<std::vector<JSON>>(*this -> v);
}

JSON& JSON::iterator::operator *() {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return it -> second;

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> at(idx);
	}

	return *this -> v;
}

JSON* JSON::iterator::operator ->() {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return &it -> second;

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return &a -> at(idx);
	}

	return this -> v;
}


const bool JSON::iterator::indexed() const {

	return this -> is_array();
}

const bool JSON::iterator::named() const {

	return this -> is_object();
}

const size_t JSON::iterator::index() const {

	return this -> is_array() ? this -> idx : -1;
}

const std::string JSON::iterator::name() const {

	if ( !this -> is_object())
		return "";

	std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
	auto it = m -> begin();
	std::advance(it, idx);
	return it -> first;
}

const std::string JSON::iterator::key() const {

	return this -> name();
}

const size_t JSON::iterator::size() const {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		return m -> size();

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> size();

	}

	return this -> v -> size();
}

const size_t JSON::iterator::length() const {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		return m -> size();

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> size();

	}

	return this -> v -> length();
}

const JSON JSON::iterator::const_value() const {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return it -> second;

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> at(idx);
	}

	return *this -> v;
}

JSON* JSON::iterator::value() {

	return this -> operator ->();
}

const JSON::TYPE JSON::iterator::type() const {

	return this -> const_value().type();
}

const bool JSON::iterator::operator ==(const JSON::TYPE type) const {

	return this -> type() == type;
}

const bool JSON::iterator::operator !=(const JSON::TYPE type) const {

	return this -> type() != type;
}

const bool JSON::iterator::operator ==(const char& c) const {

	return this -> const_value() == c;
}

const bool JSON::iterator::operator ==(const unsigned char& c) const {

	return this -> const_value() == c;
}

const bool JSON::iterator::operator ==(const int& i) const {

	return this -> const_value() == i;
}

const bool JSON::iterator::operator ==(const unsigned int& i) const {

	return this -> const_value() == i;
}

const bool JSON::iterator::operator ==(const long& l) const {

	return this -> const_value() == l;
}

const bool JSON::iterator::operator ==(const unsigned long& l) const {

	return this -> const_value() == l;
}

const bool JSON::iterator::operator ==(const long long& ll) const {

	return this -> const_value() == ll;
}

const bool JSON::iterator::operator ==(const unsigned long long& ll) const {

	return this -> const_value() == ll;
}

const bool JSON::iterator::operator ==(const long double& ld) const {

	return this -> const_value() == ld;
}

const bool JSON::iterator::operator ==(const double& d) const {

	return this -> const_value() == d;
}

const bool JSON::iterator::operator ==(const float& f) const {

	return this -> const_value() == f;
}

const bool JSON::iterator::operator ==(const std::string& s) const {

	return this -> const_value() == s;
}

const bool JSON::iterator::operator ==(const char* s) const {

	return this -> const_value() == s;
}

const bool JSON::iterator::operator ==(const std::nullptr_t& n) const {

	return this -> const_value() == n;
}

const bool JSON::iterator::operator !=(const char& c) const {

	return this -> const_value() != c;
}

const bool JSON::iterator::operator !=(const unsigned char& c) const {

	return this -> const_value() != c;
}

const bool JSON::iterator::operator !=(const int& i) const {

	return this -> const_value() != i;
}

const bool JSON::iterator::operator !=(const unsigned int& i) const {

	return this -> const_value() != i;
}

const bool JSON::iterator::operator !=(const long& l) const {

	return this -> const_value() != l;
}

const bool JSON::iterator::operator !=(const unsigned long& l) const {

	return this -> const_value() != l;
}

const bool JSON::iterator::operator !=(const long long& ll) const {

	return this -> const_value() != ll;
}

const bool JSON::iterator::operator !=(const unsigned long long& ll) const {

	return this -> const_value() != ll;
}

const bool JSON::iterator::operator !=(const long double& ld) const {

	return this -> const_value() != ld;
}

const bool JSON::iterator::operator !=(const double& d) const {

	return this -> const_value() != d;
}

const bool JSON::iterator::operator !=(const float& f) const {

	return this -> const_value() != f;
}

const bool JSON::iterator::operator !=(const std::string& s) const {

	return this -> const_value() != s;
}

const bool JSON::iterator::operator !=(const char* s) const {

	return this -> const_value() != s;
}

const bool JSON::iterator::operator !=(const std::nullptr_t& n) const {

	return this -> const_value() != n;
}


JSON::iterator& JSON::iterator::operator =(const JSON& other) {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		it -> second = other;

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		a -> at(idx) = other;

	} else *(this -> v) = other;

	return *this;
}

JSON::iterator& JSON::iterator::operator +=(const JSON& other) {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		it -> second += other;

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		a -> at(idx) += other;

	} else *(this -> v) += other;

	return *this;
}

JSON::iterator& JSON::iterator::operator +=(const std::pair<std::string, JSON>& pair) {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		it -> second += pair;

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		a -> at(idx) += pair;

	} else *(this -> v) += pair;

	return *this;
}

JSON::iterator& JSON::iterator::operator +=(const std::initializer_list<std::pair<std::string, JSON>>& list) {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		it -> second += list;

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		a -> at(idx) += list;

	} else *(this -> v) += list;

	return *this;
}


JSON::iterator& JSON::iterator::operator =(const char& c) {

	this -> value() -> operator=(c);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const unsigned char& c) {

	this -> value() -> operator=(c);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const int& i) {

	this -> value() -> operator=(i);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const unsigned int& i) {

	this -> value() -> operator=(i);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const long& l) {

	this -> value() -> operator=(l);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const unsigned long& l) {

	this -> value() -> operator=(l);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const long long& ll) {

	this -> value() -> operator=(ll);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const unsigned long long& ll) {

	this -> value() -> operator=(ll);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const long double& ld) {

	this -> value() -> operator=(ld);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const double& d) {

	this -> value() -> operator=(d);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const float& f) {

	this -> value() -> operator=(f);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const std::string& s) {

	this -> value() -> operator=(s);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const char* s) {

	this -> value() -> operator=(s);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const bool& b) {

	this -> value() -> operator=(b);
	return *this;
}

JSON::iterator& JSON::iterator::operator =(const std::nullptr_t& n) {

	this -> value() -> operator=(n);
	return *this;
}

const bool JSON::iterator::empty() const {

	if ( this -> is_object() || this -> is_array())
		return this -> const_value().empty();
	else if ( this -> type() == JSON::STRING )
		return this -> to_string().empty();
	else return this -> type() == JSON::NULLPTR;
}

const std::string JSON::iterator::to_string() const {

	try {
		return this -> const_value().to_string();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

long double JSON::iterator::to_float() const {

	try {
		return this -> const_value().to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

long JSON::iterator::to_number() const {

	try {
		return this -> const_value().to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

bool JSON::iterator::to_bool() const {

	try {
		return this -> const_value().to_bool();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator std::string() const {

	try {
		return this -> to_string();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator float() const {

	try {
		return (float)this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator long double() const {

	try {
		return this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator double() const {

	try {
		return (double)this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator char() const {

	try {
		return (char)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator unsigned char() const {

	try {
		return (unsigned char)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator long() const {

	try {
		return (long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator unsigned long() const {

	try {
		return (unsigned long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator long long() const {

	try {
		return this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator unsigned long long() const {

	try {
		return (unsigned long long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator int() const {

	try {
		return (int)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator unsigned int() const {

	try {
		return (unsigned int)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::iterator::operator bool() const {

	try {
		return this -> to_bool();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const bool JSON::iterator::contains(const std::string& key) const {

	return this -> const_value().contains(key);
}

const bool JSON::iterator::is_convertible(const JSON::TYPE& to) const {

	return this -> const_value().is_convertible(to);
}

const bool JSON::iterator::convertible_to(const JSON::TYPE& to) const {

	return this -> const_value().convertible_to(to);
}

const JSON& JSON::iterator::as_const() {

	if ( this -> is_object()) {

		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
		auto it = m -> begin();
		std::advance(it, idx);
		return it -> second.as_const();

	} else if ( this -> is_array()) {

		std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
		return a -> at(idx).as_const();
	}

	return this -> v -> as_const();
}

const JSON& JSON::iterator::as_const() const {

        if ( this -> is_object()) {

                std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this -> v);
                auto it = m -> begin();
                std::advance(it, idx);
                return it -> second.as_const();

        } else if ( this -> is_array()) {

                std::vector<JSON>* a = &std::get<std::vector<JSON>>(*this -> v);
                return a -> at(idx).as_const();
        }

        return this -> v -> as_const();
}

JSON& JSON::iterator::operator[](const std::string& key) {

	try {
		return this -> value() -> operator[](key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON& JSON::iterator::operator[](const char* key) {

	try {
		return this -> value() -> operator[](key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON& JSON::iterator::operator[](size_t index) {

	try {
		return this -> value() -> operator[](index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON& JSON::iterator::operator[](int index) {

	try {
		return this -> value() -> operator[](index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::iterator::operator[](const std::string& key) const {

	try {
		return this -> const_value().operator[](key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::iterator::operator[](const char* key) const {

	try {
		return this -> const_value().operator[](key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::iterator::operator[](size_t index) const {

	try {
		return this -> const_value().operator[](index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::iterator::operator[](int index) const {

	try {
		return this -> const_value().operator[](index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON& JSON::iterator::at(const std::string& key) {

	try {
		return this -> value() -> at(key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON& JSON::iterator::at(const char *key) {

	try {
		return this -> value() -> at(key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON& JSON::iterator::at(const size_t index) {

	try {
		return this -> value() -> at(index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON& JSON::iterator::at(const int index) {

	try {
		return this -> value() -> at(index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::iterator::at(const std::string& key) const {

	try {
		return this -> const_value().at(key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::iterator::at(const char* key) const {

	try {
		return this -> const_value().at(key);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::iterator::at(const size_t index) const {

	try {
		return this -> const_value().at(index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::iterator::at(const int index) const {

	try {
		return this -> const_value().at(index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

std::ostream& operator <<(std::ostream& os, const JSON::iterator& it) {

	try {
		os << it.to_string();
		return os;
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}
