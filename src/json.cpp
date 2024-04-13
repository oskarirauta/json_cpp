#include <stdexcept>
#include <algorithm>
#include <charconv>

#include "json/json.hpp"
#include "json/json_error.hpp"

JSON::JSON() {
	this -> emplace<std::nullptr_t>(std::forward<decltype(nullptr)>(nullptr));
}

JSON::JSON(const std::nullptr_t& n) {
	this -> emplace<std::nullptr_t>(std::forward<decltype(nullptr)>(nullptr));
}

JSON::JSON(const bool& b) {
	this -> emplace<bool>(std::forward<decltype(b)>(b));
}

JSON::JSON(const char& c) {
	long long ll = (long long)c;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
}

JSON::JSON(const unsigned char& c) {
	long long ll = (long long)c;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
}

JSON::JSON(const int& i) {
	long long ll = (long long)i;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
}

JSON::JSON(const unsigned int& i) {
	long long ll = (long long)i;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
}

JSON::JSON(const long& l) {
	long long ll = (long long)l;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
}

JSON::JSON(const unsigned long& l) {
	long long ll = (long long)l;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
}

JSON::JSON(const long long& ll) {
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
}

JSON::JSON(const unsigned long long& ll) {
	long long _ll = (long long)ll;
	this -> emplace<long long>(std::forward<decltype(_ll)>(_ll));
}

JSON::JSON(const long double& ld) {
	this -> emplace<long double>(std::forward<decltype(ld)>(ld));
}

JSON::JSON(const double& d) {
	long long ld = (long double)d;
	this -> emplace<long double>(std::forward<decltype(ld)>(ld));
}

JSON::JSON(const float& f) {
	long long ld = (long double)f;
	this -> emplace<long double>(std::forward<decltype(ld)>(ld));
}

JSON::JSON(const std::string &s) {
	this -> emplace<std::string>(std::forward<decltype(s)>(s));
}

JSON::JSON(const char* s) {
	std::string _s(s);
	this -> emplace<std::string>(std::forward<decltype(_s)>(_s));
}

JSON::JSON(const std::map<std::string, JSON>& m) {
	this -> emplace<std::map<std::string, JSON>>(std::forward<decltype(m)>(m));
}

JSON::JSON(const std::vector<JSON>& a) {
	this -> emplace<std::vector<JSON>>(std::forward<decltype(a)>(a));
}

JSON::JSON(const JSON& other) {

	if ( other == OBJECT ) {
		std::map<std::string, JSON> obj = std::get<std::map<std::string, JSON>>(other);
		this -> emplace<std::map<std::string,JSON>>(std::forward<decltype(obj)>(obj));
	} else if ( other == ARRAY ) {
		std::vector<JSON> array = std::get<std::vector<JSON>>(other);
		this -> emplace<std::vector<JSON>>(std::forward<decltype(array)>(array));
	} else if ( other == STRING ) {
		std::string s = std::get<std::string>(other);
		this -> emplace<std::string>(std::forward<decltype(s)>(s));
	} else if ( other == FLOAT ) {
		long double d = std::get<long double>(other);
		this -> emplace<long double>(std::forward<decltype(d)>(d));
	} else if ( other == INT ) {
		long long l = std::get<long long>(other);
		this -> emplace<long long>(std::forward<decltype(l)>(l));
	} else if ( other == BOOL ) {
		bool b = std::get<bool>(other);
		this -> emplace<bool>(std::forward<decltype(b)>(b));
	} else if ( other == NULLPTR ) {
		std::nullptr_t n = nullptr;
		this -> emplace<std::nullptr_t>(std::forward<decltype(n)>(n));
	} else throw JSON::exception(JSON::ERROR_CODE::ELEMENT_COPY_ERROR);
}

const JSON::TYPE JSON::type() const {

	if ( std::holds_alternative<std::map<std::string, JSON>>(*this)) return OBJECT;
	else if ( std::holds_alternative<std::vector<JSON>>(*this)) return ARRAY;
	else if ( std::holds_alternative<std::string>(*this)) return STRING;
	else if ( std::holds_alternative<long double>(*this)) return FLOAT;
	else if ( std::holds_alternative<long long>(*this)) return INT;
	else if ( std::holds_alternative<bool>(*this)) return BOOL;
	else if ( std::holds_alternative<std::nullptr_t>(*this)) return NULLPTR;
	else throw JSON::exception(JSON::ERROR_CODE::INVALID_ELEMENT_TYPE);
}

const bool JSON::operator ==(const TYPE type) const {

	if ( type == OBJECT && std::holds_alternative<std::map<std::string, JSON>>(*this)) return true;
	else if ( type == ARRAY && std::holds_alternative<std::vector<JSON>>(*this)) return true;
	else if ( type == STRING && std::holds_alternative<std::string>(*this)) return true;
	else if ( type == FLOAT && std::holds_alternative<long double>(*this)) return true;
	else if ( type == INT && std::holds_alternative<long long>(*this)) return true;
	else if ( type == BOOL && std::holds_alternative<bool>(*this)) return true;
	else if ( type == NULLPTR && std::holds_alternative<std::nullptr_t>(*this)) return true;
	else return false;
}

const bool JSON::operator !=(const TYPE type) const {

	return operator ==(type) ? false : true;
}

const bool JSON::operator ==(const char& c) const {
	return *this == INT && (char)this -> to_number() == c;
}

const bool JSON::operator ==(const unsigned char& c) const {
	return *this == INT && (unsigned char)this -> to_number() == c;
}

const bool JSON::operator ==(const int& i) const {
	return *this == INT && (int)this -> to_number() == i;
}

const bool JSON::operator ==(const unsigned int& i) const {
	return *this == INT && (unsigned int)this -> to_number() == i;
}

const bool JSON::operator ==(const long& l) const {
	return *this == INT && (long)this -> to_number() == l;
}

const bool JSON::operator ==(const unsigned long& l) const {
	return *this == INT && (unsigned long)this -> to_number() == l;
}

const bool JSON::operator ==(const long long& ll) const {
	return *this == INT && this -> to_number() == ll;
}

const bool JSON::operator ==(const unsigned long long& ll) const {
	return *this == INT && (unsigned long long)this -> to_number() == ll;
}

const bool JSON::operator ==(const long double& ld) const {
	return *this == FLOAT && this -> to_float() == ld;
}

const bool JSON::operator ==(const double& d) const {
	return *this == FLOAT && (double)this -> to_float() == d;
}

const bool JSON::operator ==(const float& f) const {
	return *this == FLOAT && (float)this -> to_float() == f;
}

const bool JSON::operator ==(const std::string& s) const {
	return *this == STRING && this -> to_string() == s;
}

const bool JSON::operator ==(const char* s) const {
	return *this == STRING && this -> to_string() == std::string(s);
}

const bool JSON::operator ==(const std::nullptr_t& n) const {

	return *this == NULLPTR;
}

const bool JSON::operator !=(const char& c) const {
	return !(*this == c);
}

const bool JSON::operator !=(const unsigned char& c) const {
	return !(*this == c);
}

const bool JSON::operator !=(const int& i) const {
	return !(*this == i);
}

const bool JSON::operator !=(const unsigned int& i) const {
	return !(*this == i);
}

const bool JSON::operator !=(const long& l) const {
	return !(*this == l);
}

const bool JSON::operator !=(const unsigned long& l) const {
	return !(*this == l);
}

const bool JSON::operator !=(const long long& ll) const {
	return !(*this == ll);
}

const bool JSON::operator !=(const unsigned long long& ll) const {
	return !(*this == ll);
}

const bool JSON::operator !=(const long double& ld) const {
	return !(*this == ld);
}

const bool JSON::operator !=(const double& d) const {
	return !(*this == d);
}

const bool JSON::operator !=(const float& f) const {
	return !(*this == f);
}

const bool JSON::operator !=(const std::string& s) const {
	return !(*this == s);
}

const bool JSON::operator !=(const char* s) const {
	return !(*this == std::string(s));
}

const bool JSON::operator !=(const std::nullptr_t& n) const {

	return *this != NULLPTR;
}

JSON& JSON::operator =(const JSON& other) {

	if ( other == OBJECT ) {
		std::map<std::string, JSON> obj = std::get<std::map<std::string, JSON>>(other);
		this -> emplace<std::map<std::string,JSON>>(std::forward<decltype(obj)>(obj));
	} else if ( other == ARRAY ) {
		std::vector<JSON> array = std::get<std::vector<JSON>>(other);
		this -> emplace<std::vector<JSON>>(std::forward<decltype(array)>(array));
	} else if ( other == STRING ) {
		std::string s = std::get<std::string>(other);
		this -> emplace<std::string>(std::forward<decltype(s)>(s));
	} else if ( other == FLOAT ) {
		long double d = std::get<long double>(other);
		this -> emplace<long double>(std::forward<decltype(d)>(d));
	} else if ( other == INT ) {
		long long l = std::get<long long>(other);
		this -> emplace<long long>(std::forward<decltype(l)>(l));
	} else if ( other == BOOL ) {
		bool b = std::get<bool>(other);
		this -> emplace<bool>(std::forward<decltype(b)>(b));
	} else if ( other == NULLPTR ) {
		std::nullptr_t n = nullptr;
		this -> emplace<std::nullptr_t>(std::forward<decltype(n)>(n));
	} else throw JSON::exception(JSON::ERROR_CODE::ASSIGNMENT_ERROR);

	return *this;
}

JSON& JSON::operator +=(const JSON& other) {

	this -> append(other);
	return *this;
}

JSON& JSON::operator +=(const std::pair<std::string, JSON>& pair) {

	this -> append(pair);
	return *this;
}

JSON& JSON::operator +=(const std::initializer_list<std::pair<std::string, JSON>>& list) {

	this -> append(list);
	return *this;
}

JSON& JSON::operator =(const char& c) {
	long long ll = (long long)c;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
	return *this;
}

JSON& JSON::operator =(const unsigned char& c) {
	long long ll = (long long)c;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
	return *this;
}

JSON& JSON::operator =(const int& i) {
	long long ll = (long long)i;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
	return *this;
}

JSON& JSON::operator =(const unsigned int& i) {
	long long ll = (long long)i;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
	return *this;
}

JSON& JSON::operator =(const long& l) {
	long long ll = (long long)l;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
	return *this;
}

JSON& JSON::operator =(const unsigned long& l) {
	long long ll = (long long)l;
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
	return *this;
}

JSON& JSON::operator =(const long long& ll) {
	this -> emplace<long long>(std::forward<decltype(ll)>(ll));
	return *this;
}

JSON& JSON::operator =(const unsigned long long& ll) {
	long long _ll = (long long)ll;
	this -> emplace<long long>(std::forward<decltype(_ll)>(_ll));
	return *this;
}

JSON& JSON::operator =(const long double& ld) {
	this -> emplace<long double>(std::forward<decltype(ld)>(ld));
	return *this;
}

JSON& JSON::operator =(const double& d) {
	long double ld = (long double)d;
	this -> emplace<long double>(std::forward<decltype(ld)>(ld));
	return *this;
}

JSON& JSON::operator =(const float& f) {
	long double ld = (long double)f;
	this -> emplace<long double>(std::forward<decltype(ld)>(ld));
	return *this;
}

JSON& JSON::operator =(const std::string& s) {
	this -> emplace<std::string>(std::forward<decltype(s)>(s));
	return *this;
}

JSON& JSON::operator =(const char* s) {
	std::string _s(s);
	this -> emplace<std::string>(std::forward<decltype(_s)>(_s));
	return *this;
}

JSON& JSON::operator =(const bool& b) {
	this -> emplace<bool>(std::forward<decltype(b)>(b));
	return *this;
}

JSON& JSON::operator [](const std::string& key) {

	if ( !std::holds_alternative<std::map<std::string, JSON>>(*this)) {
		std::map<std::string, JSON> m;
		this -> emplace<std::map<std::string, JSON>>(std::forward<decltype(m)>(m));
	}

	return std::get<std::map<std::string, JSON>>(*this)[key];
}

JSON& JSON::operator [](const char* key) {

	if ( !std::holds_alternative<std::map<std::string, JSON>>(*this)) {
		std::map<std::string, JSON> m;
		this -> emplace<std::map<std::string, JSON>>(std::forward<decltype(m)>(m));
	}

	return std::get<std::map<std::string, JSON>>(*this)[std::string(key)];
}

JSON& JSON::operator [](const size_t index) {

	if ( !std::holds_alternative<std::vector<JSON>>(*this)) {
		std::vector<JSON> a;
		this -> emplace<std::vector<JSON>>(std::forward<decltype(a)>(a));
	}

	if ( size_t max = std::get<std::vector<JSON>>(*this).size(); index >= max ) {
		std::get<std::vector<JSON>>(*this).resize(max + 1);
		return std::get<std::vector<JSON>>(*this)[max];
	} else return std::get<std::vector<JSON>>(*this)[index];
}

JSON& JSON::operator [](const int index) {
	return operator[]((size_t)index);
}

const JSON JSON::operator [](const std::string& key) const {

	if ( std::holds_alternative<std::map<std::string, JSON>>(*this)) {
		std::map<std::string, JSON> m = std::get<std::map<std::string, JSON>>(*this);
		return m[key];
	} else throw JSON::exception(JSON::ERROR_CODE::OBJECT_SUBSCRIPT_FAIL);
}

const JSON JSON::operator [](const char* key) const {

	if ( std::holds_alternative<std::map<std::string, JSON>>(*this)) {
		std::map<std::string, JSON> m = std::get<std::map<std::string, JSON>>(*this);
		return m[std::string(key)];
	} else throw JSON::exception(JSON::ERROR_CODE::OBJECT_SUBSCRIPT_FAIL);
}

const JSON JSON::operator [](const size_t index) const {

	std::vector<JSON> a;
	if ( std::holds_alternative<std::vector<JSON>>(*this)) {
		std::vector<JSON> a = std::get<std::vector<JSON>>(*this);
		if ( index >= a.size())
			throw JSON::exception(JSON::ERROR_CODE::ARRAY_SUBSCRIPT_RANGE_ERROR);
		return a[index];
	} else throw JSON::exception(JSON::ERROR_CODE::ARRAY_SUBSCRIPT_FAIL);
}

const JSON JSON::operator [](const int index) const {
	return operator[]((size_t) index);
}

JSON& JSON::at(const std::string& key) {

	if ( std::holds_alternative<std::map<std::string, JSON>>(*this)) {
		if ( this -> contains(key))
			return operator[](key);
		else throw JSON::exception(JSON::ERROR_CODE::OBJECT_KEY_DOES_NOT_EXIST,
				"subscript failure with .at(key), key '" + key + "' does not exists in object");
	} else throw JSON::exception(JSON::ERROR_CODE::INVALID_OBJECT_SUBSCRIPT,
				"subscript failure with .at(key), function can be used only for object types, this is " +
						JSON::describe(this -> type()));
}

JSON& JSON::at(const char* key) {

	try {
		return this -> at(std::string(key));
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON& JSON::at(const size_t index) {

	if ( std::holds_alternative<std::vector<JSON>>(*this)) {
		if ( this -> size() > index )
			return operator[](index);
		else throw JSON::exception(JSON::ERROR_CODE::ARRAY_SUBSCRIPT_RANGE_ERROR,
				"subscript failure with .at(index), index " + std::to_string(index) + " is out of bounds(" +
						std::to_string(this -> size()) + ")");
	} else throw JSON::exception(JSON::ERROR_CODE::INVALID_ARRAY_SUBSCRIPT,
				"subscript failure with .at(index), function can be used only for arrays, this is " +
						JSON::describe(this -> type()));
}

JSON& JSON::at(const int index) {

	try {
		return this -> at((size_t) index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::at(const std::string& key) const {

	if ( std::holds_alternative<std::map<std::string, JSON>>(*this)) {
		if ( this -> contains(key))
			return operator[](key);
		else throw JSON::exception(JSON::ERROR_CODE::OBJECT_KEY_DOES_NOT_EXIST,
				"subscript failure with .at(key), key '" + key + "' does not exist in object");
	} else throw JSON::exception(JSON::ERROR_CODE::INVALID_OBJECT_SUBSCRIPT,
				"subscript failure with .at(key), function can be used only for objects, this is " +
						JSON::describe(this -> type()));
}

const JSON JSON::at(const char* key) const {

	try {
		return this -> at(std::string(key));
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const JSON JSON::at(const size_t index) const {

	if ( std::holds_alternative<std::vector<JSON>>(*this)) {
		if ( this -> size() > index )
			return operator[](index);
		else throw JSON::exception(JSON::ERROR_CODE::ARRAY_SUBSCRIPT_RANGE_ERROR,
				"subscript failure with .at(index), index " + std::to_string(index) + " is out " +
						"bounds(" + std::to_string(this -> size()) + ")");
	} else throw JSON::exception(JSON::ERROR_CODE::INVALID_ARRAY_SUBSCRIPT,
				"subscript failure with .at(index), function can be used only for arrays, this is " +
						JSON::describe(this -> type()));
}

const JSON JSON::at(const int index) const {

	try {
		return this -> at((size_t) index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

const std::size_t JSON::length() const {
	if ( std::holds_alternative<std::vector<JSON>>(*this))
		return std::get<std::vector<JSON>>(*this).size();
	return 0;
}

const std::size_t JSON::size() const {
	if ( std::holds_alternative<std::vector<JSON>>(*this))
		return std::get<std::vector<JSON>>(*this).size();
	else if ( std::holds_alternative<std::map<std::string, JSON>>(*this))
		return std::get<std::map<std::string, JSON>>(*this).size();
	return 0;
}

const bool JSON::empty() const {

	if ( *this == ARRAY || *this == OBJECT )
		return this -> size() == 0;
	else if ( *this == STRING )
		return this -> to_string().empty();
	else return *this == NULLPTR ? true : false;
}

const bool JSON::contains(const std::string& key) const {

	if ( std::holds_alternative<std::map<std::string, JSON>>(*this)) {
		const std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this);
		if ( m -> find(key) != m -> end()) return true;
	}
	return false;
}

const std::string JSON::to_lower(const std::string& s) {

	std::string _s(s);
	std::transform(_s.cbegin(), _s.cend(), _s.begin(), [](auto ch){ return std::tolower(ch);});
	return _s;
}

const std::string JSON::to_string() const {

	if ( *this == NULLPTR ) return "null";
	else if ( *this == STRING ) return std::get<std::string>(*this);
	else if ( *this == FLOAT ) return std::to_string(std::get<long double>(*this));
	else if ( *this == INT ) return std::to_string(std::get<long long>(*this));
	else if ( *this == BOOL ) return std::get<bool>(*this) ? "true" : "false";
	else if ( *this == OBJECT || *this == ARRAY ) return this -> dump(true);
	else throw JSON::exception(JSON::ERROR_CODE::STRING_CONVERSION_FAILED);
}

long double JSON::to_float() const {

	if ( *this == NULLPTR ) return 0;
	else if ( *this == FLOAT ) return std::get<long double>(*this);
	else if ( *this == INT ) return (long double)std::get<long long>(*this);
	else if ( *this == BOOL ) return std::get<bool>(*this) ? 1 : 0;
	else if ( *this == STRING ) {
		double d = -1;
		try {
			d = std::stold(std::get<std::string>(*this));
		} catch ( const std::invalid_argument& e ) {
			throw JSON::exception(JSON::ERROR_CODE::FLOAT_CONVERSION_INVALID,
						"json conversion to float failed, value \"" + std::get<std::string>(*this) +
						"\" is not convertible to number");
		} catch ( const std::out_of_range& e ) {
			throw JSON::exception(JSON::ERROR_CODE::FLOAT_CONVERSION_RANGE,
						"json conversion to float failed, value \"" + std::get<std::string>(*this) +
						"\" is out of range");
		}
		return d;
	} else if ( *this == OBJECT )
		throw JSON::exception(JSON::ERROR_CODE::FLOAT_CONVERSION_CAST, "json object cannot be converted to float");
	else if ( *this == ARRAY )
		throw JSON::exception(JSON::ERROR_CODE::FLOAT_CONVERSION_CAST, "json array cannot be converted to float");
	else throw JSON::exception(JSON::ERROR_CODE::FLOAT_CONVERSION_ERROR);
}

long long JSON::to_number() const {

	if ( *this == NULLPTR ) return 0;
	else if ( *this == FLOAT ) return (long long)std::get<long double>(*this);
	else if ( *this == INT ) return std::get<long long>(*this);
	else if ( *this == BOOL ) return std::get<bool>(*this) ? 1 : 0;
	else if ( *this == STRING ) {
		long long ll = -1;
		try {
			ll = std::stoll(std::get<std::string>(*this));
		} catch ( const std::invalid_argument& e ) {
			throw JSON::exception(JSON::ERROR_CODE::INT_CONVERSION_INVALID,
						"json conversion to int failed, value \"" + std::get<std::string>(*this) +
						"\" is not convertible to number");
		} catch ( const std::out_of_range& e ) {
			throw JSON::exception(JSON::ERROR_CODE::INT_CONVERSION_RANGE,
						"json conversion to int failed, value \"" + std::get<std::string>(*this) +
						"\" is out of range");
		}
		return ll;
	} else if ( *this == OBJECT )
		throw JSON::exception(JSON::ERROR_CODE::INT_CONVERSION_CAST, "json object cannot be converted to int");
	else if ( *this == ARRAY )
		throw JSON::exception(JSON::ERROR_CODE::INT_CONVERSION_CAST, "json array cannot be converted to int");
	throw JSON::exception(JSON::ERROR_CODE::INT_CONVERSION_ERROR);
}

bool JSON::to_bool() const {

	if ( *this == NULLPTR ) return false;
	else if ( *this == FLOAT ) return (long long)std::get<long double>(*this) == 0 ? false : true;
	else if ( *this == INT ) return std::get<long long>(*this) == 0 ? false : true;
	else if ( *this == BOOL ) return std::get<bool>(*this);
	else if ( *this == STRING ) {

		std::string s = std::get<std::string>(*this);

		if ( JSON::to_lower(s) == "true" )
			return true;
		else if ( JSON::to_lower(s) == "false" )
			return false;

		int result{};
		auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), result);

		if ( ec == std::errc())
			return result == 0 ? false : true;
		else if ( ec == std::errc::invalid_argument )
			throw JSON::exception(BOOL_CONVERSION_INVALID,
						"json conversion to bool failed, value \"" + std::get<std::string>(*this) +
						"\" is not convertible to false, true or number");
		else if ( ec == std::errc::result_out_of_range )
			throw JSON::exception(BOOL_CONVERSION_RANGE,
						"json conversion to bool failed, value \"" + std::get<std::string>(*this) +
						"\" is out of range");
		else throw JSON::exception(BOOL_CONVERSION_ERROR);

	} else if ( *this == OBJECT )
		throw JSON::exception(BOOL_CONVERSION_CAST, "json object cannot be converted to bool");
	else if ( *this == ARRAY )
		throw JSON::exception(BOOL_CONVERSION_CAST, "json array cannot be converted to array");
	throw JSON::exception(BOOL_CONVERSION_ERROR);
}

JSON::operator std::string() const {

	try {
		return this -> to_string();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator float() const {

	try {
		return (float)this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator long double() const {

	try {
		return this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator double() const {

	try {
		return (double)this -> to_float();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator char() const {

	try {
		return (char)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator unsigned char() const {

	try {
		return (unsigned char)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator long() const {

	try {
		return (long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator unsigned long() const {

	try {
		return (unsigned long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator long long() const {

	try {
		return this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator unsigned long long() const {

	try {
		return (unsigned long long)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator int() const {

	try {
		return (int)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator unsigned int() const {

	try {
		return (unsigned int)this -> to_number();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

JSON::operator bool() const {

	try {
		return this -> to_bool();
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

void JSON::clear() {

	if ( *this == OBJECT )
		std::get<std::map<std::string, JSON>>(*this).clear();
	else if ( *this == ARRAY )
		std::get<std::vector<JSON>>(*this).clear();
	else if ( *this == STRING )
		std::get<std::string>(*this) = "";
	else throw JSON::exception(JSON::ERROR_CODE::ELEMENT_CANNOT_CLEAR,
					"json " + JSON::describe(this -> type()) +
					" element type cannot be cleared");
}

void JSON::erase(const std::string& key) {

	if ( *this == OBJECT ) {
		if ( this -> contains(key))
			std::get<std::map<std::string, JSON>>(*this).erase(key);
		else throw JSON::exception(JSON::ERROR_CODE::ERASE_FAILED_NO_KEY,
						"erase failed, key \"" + key + "\" does not exist in object");
	} else throw JSON::exception(JSON::ERROR_CODE::ELEMENT_CANNOT_ERASE_KEY);
}

void JSON::erase(const size_t index) {

	if ( *this == OBJECT ) {
		std::map<std::string, JSON>* m = &std::get<std::map<std::string, JSON>>(*this);
		if ( m -> size() > index ) {
			auto it = m -> begin();
			if ( index > 0 )
				std::advance(it, index);
			m -> erase(it);
		} else throw JSON::exception(JSON::ERROR_CODE::ERASE_FAILED_NO_INDEX,
						"erase failed, index " + std::to_string(index) + " is out of array's bounds");
	} else if ( *this == ARRAY ) {
		std::vector<JSON>* v = &std::get<std::vector<JSON>>(*this);
		if ( v -> size() > index ) {
			auto it = v -> begin();
			if ( index > 0 )
				std::advance(it, index);
			v -> erase(it);
		} else throw JSON::exception(JSON::ERROR_CODE::ERASE_FAILED_NO_INDEX,
						"erase failed, index " + std::to_string(index) + " is out of array's bounds");
	} else throw JSON::exception(JSON::ERROR_CODE::ELEMENT_CANNOT_ERASE_INDEX);
}

void JSON::erase(const int index) {

	try {
		this -> erase((size_t)index);
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}

void JSON::append(const JSON& json) {

	if ( *this == ARRAY )
		std::get<std::vector<JSON>>(*this).push_back(json);
	else throw JSON::exception(JSON::ERROR_CODE::APPEND_ARRAY_FAILED);
}

void JSON::insert(const JSON& json) {

	if ( *this == ARRAY )
		std::get<std::vector<JSON>>(*this).push_back(json);
	else throw JSON::exception(JSON::ERROR_CODE::INSERT_ARRAY_FAILED);
}

void JSON::append(const std::pair<std::string, JSON>& pair) {

	if ( *this == OBJECT )
		std::get<std::map<std::string, JSON>>(*this).insert(pair);
	else throw JSON::exception(JSON::ERROR_CODE::APPEND_OBJECT_FAILED);
}

void JSON::insert(const std::pair<std::string, JSON>& pair) {

	if ( *this == OBJECT )
		std::get<std::map<std::string, JSON>>(*this).insert(pair);
	else throw JSON::exception(JSON::ERROR_CODE::INSERT_OBJECT_FAILED);
}

void JSON::append(const std::initializer_list<std::pair<std::string, JSON>>& list) {

	for ( auto& pair : list ) {

		try {
			this -> append(pair);
		} catch ( const JSON::exception &e ) {
			throw e;
		}
	}
}

void JSON::insert(const std::initializer_list<std::pair<std::string, JSON>>& list) {

	for ( auto& pair : list ) {

		try {
			this -> insert(pair);
		} catch ( const JSON::exception &e ) {
			throw e;
		}
	}
}

void JSON::clear() const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function clear is not available for const JSON");
}

void JSON::erase(const std::string& key) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function erase is not available for const JSON");
}

void JSON::erase(const size_t index) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function erase is not available for const JSON");
}

void JSON::erase(const int index) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function erase is not available for const JSON");
}

void JSON::append(const JSON& json) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function append is not available for const JSON");
}

void JSON::insert(const JSON& json) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function insert is not available for const JSON");
}

void JSON::append(const std::pair<std::string, JSON>& pair) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function append is not available for const JSON");
}

void JSON::insert(const std::pair<std::string, JSON>& pair) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function insert is not available for const JSON");
}

void JSON::append(const std::initializer_list<std::pair<std::string, JSON>>& list) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function append is not available for const JSON");
}

void JSON::insert(const std::initializer_list<std::pair<std::string, JSON>>& list) const {
	throw JSON::exception(JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "function insert is not available for const JSON");
}

const JSON& JSON::as_const() {
	return std::as_const(*this);
}

const JSON& JSON::as_const() const {
	return *this;
}
