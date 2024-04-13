#pragma once

#include <utility>
#include <ostream>
#include <variant>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>

class JSON : public std::variant<std::map<std::string, JSON>, std::vector<JSON>, std::nullptr_t, std::string, long double, long long, bool> {

	public:

	using variant::variant;

	using number_variant = std::variant<char, signed char, unsigned char,
				short, short, unsigned short, signed short,
				int, signed, signed int, unsigned int, long, signed long, unsigned long,
				long long, unsigned long long, signed long long, size_t>;

	using float_variant = std::variant<long double, double, float>;

	using string_variant = std::variant<std::string, char*>;

	struct iterator;
	struct const_iterator;

	enum TYPE {
		NULLPTR, OBJECT, ARRAY, STRING, FLOAT, INT, BOOL
	};

	enum ERROR_CODE : int {
		NO_ERROR = 0, END_OF_BLOB = 1, UNSUPPORTED_BEGIN_OF_BLOB = 2, NULLPTR_MISREAD = 3, BOOL_MISREAD = 4,
		EXP_MISSING = 5, ILLEGAL_NUMBER = 6, FLOAT_OUT_OF_RANGE = 7, INT_OUT_OF_RANGE = 8, INVALID_UNICODE = 9,
		UNSUPPORTED_ESCAPE_CHARACTER = 10, INVALID_KEY = 11, MISSING_TICK = 12, MISSING_SEMICOLON = 13,
		MISSING_COMMA = 14,

		ELEMENT_COPY_ERROR = 101, INVALID_ELEMENT_TYPE = 102, ASSIGNMENT_ERROR = 103,
		OBJECT_SUBSCRIPT_FAIL = 104, ARRAY_SUBSCRIPT_FAIL = 105, ARRAY_SUBSCRIPT_RANGE_ERROR = 106,
		OBJECT_KEY_DOES_NOT_EXIST = 107, INVALID_OBJECT_SUBSCRIPT = 108, INVALID_ARRAY_SUBSCRIPT = 109,
		STRING_CONVERSION_FAILED = 110, FLOAT_CONVERSION_INVALID = 111, FLOAT_CONVERSION_RANGE = 112,
		FLOAT_CONVERSION_CAST = 113, FLOAT_CONVERSION_ERROR = 114,
		INT_CONVERSION_INVALID = 115, INT_CONVERSION_RANGE = 116, INT_CONVERSION_CAST = 117, INT_CONVERSION_ERROR = 118,
		BOOL_CONVERSION_INVALID = 119, BOOL_CONVERSION_RANGE = 120, BOOL_CONVERSION_CAST = 121, BOOL_CONVERSION_ERROR = 122,
		ELEMENT_CANNOT_CLEAR = 123, ERASE_FAILED_NO_KEY = 124, ELEMENT_CANNOT_ERASE_KEY = 125,
		ERASE_FAILED_NO_INDEX = 126, ELEMENT_CANNOT_ERASE_INDEX = 127,
		APPEND_ARRAY_FAILED = 128, INSERT_ARRAY_FAILED = 129, APPEND_OBJECT_FAILED = 130, INSERT_OBJECT_FAILED = 131,
		FUNCTION_UNAVAILABLE_FOR_CONST = 132
	};

	struct ERROR;
	class exception;

	using Object = std::map<std::string, JSON>;
	using Array = std::vector<JSON>;
	using Bool = bool;
	using Int = long long;
	using Float = long double;
	using String = std::string;
	using Nullptr = std::nullptr_t;
	using Null = std::nullptr_t;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	const TYPE type() const;
	const bool operator ==(const TYPE type) const;
	const bool operator !=(const TYPE type) const;

	const bool operator ==(const number_variant& v) const;
	const bool operator ==(const float_variant& v) const;
	const bool operator ==(const string_variant& v) const;
	const bool operator ==(const bool& b) const;
	const bool operator ==(const std::nullptr_t& n) const;

	const bool operator !=(const number_variant& v) const;
	const bool operator !=(const float_variant& v) const;
	const bool operator !=(const string_variant& v) const;
	const bool operator !=(const std::nullptr_t& n) const;

	JSON& operator =(const JSON& other);
	JSON& operator +=(const JSON& other);
	JSON& operator +=(const std::pair<std::string, JSON>& pair);
	JSON& operator +=(const std::initializer_list<std::pair<std::string, JSON>>& list);
	JSON& operator =(const number_variant& v);
	JSON& operator =(const float_variant& v);
	JSON& operator =(const string_variant& v);
	JSON& operator =(const bool& b);

	JSON& operator [](const string_variant& key);
	JSON& operator [](const number_variant& index);

	const JSON operator [](const string_variant& key) const;
	const JSON operator [](const number_variant& index) const;

	JSON& at(const string_variant& key);
	JSON& at(const number_variant& index);
	const JSON at(const string_variant& key) const;
	const JSON at(const number_variant& index) const;
	const size_t length() const;
	const std::size_t size() const;
	const bool empty() const;
	const bool contains(const string_variant& key) const;

	const std::string to_string() const;
	long double to_float() const;
	long long to_number() const;
	bool to_bool() const;

	operator std::string() const;
	operator float() const;
	operator long double() const;
	operator double() const;
	operator char() const;
	operator unsigned char() const;
	operator long() const;
	operator unsigned long() const;
	operator long long() const;
	operator unsigned long long() const;
	operator int() const;
	operator unsigned int() const;
	operator bool() const;

	void clear();
	void erase(const string_variant& key);
	void erase(const number_variant& index);
	void append(const JSON& json);
	void insert(const JSON& json);
	void append(const std::pair<std::string, JSON>& pair);
	void insert(const std::pair<std::string, JSON>& pair);
	void append(const std::initializer_list<std::pair<std::string, JSON>>& list);
	void insert(const std::initializer_list<std::pair<std::string, JSON>>& list);

	void clear() const;
	void erase(const string_variant& key) const;
	void erase(const number_variant& index) const;
	void append(const JSON& json) const;
	void insert(const JSON& json) const;
	void append(const std::pair<std::string, JSON>& pair) const;
	void insert(const std::pair<std::string, JSON>& pair) const;
	void append(const std::initializer_list<std::pair<std::string, JSON>>& list) const;
	void insert(const std::initializer_list<std::pair<std::string, JSON>>& list) const;

	const JSON& as_const();
	const JSON& as_const() const;

	const bool is_convertible(const JSON::TYPE& to) const;
	const bool convertible_to(const JSON::TYPE& type) const;

	const std::string dump(const bool pretty = true);
	const std::string dump(const bool pretty = true) const;
	const std::string dump_minified();
	const std::string dump_minified() const;

	static const std::string escape(const std::string& s);
	static const std::string unescape(const std::string& s);
	static const std::string to_lower(const std::string& s);
	static const std::string trim_special_chars(const std::string& s);
	static const std::string describe(const JSON::TYPE& type);

	JSON();
	JSON(const bool& b);
	JSON(const number_variant& v);
	JSON(const float_variant& v);
	JSON(const string_variant& v);
	JSON(const std::nullptr_t& n);
	JSON(const std::map<std::string, JSON>&m);
	JSON(const std::vector<JSON>&a);
	JSON(const JSON& other);

	static JSON parse(const string_variant& s);
	static long long to_number(const number_variant& v);
	static long double to_float(const float_variant& v);
	static std::string to_string(const string_variant& v);
	friend std::ostream& operator <<(std::ostream& os, const JSON& json);

	private:

	static const std::map<JSON::TYPE, std::string> type_descs;
	static const std::map<std::string::value_type, std::string> escape_chars;
	static const std::map<std::string::value_type, std::string::value_type> unescape_chars;

	static JSON parse_nullptr(const std::string& s, size_t& pos, JSON::ERROR& ec);
	static JSON parse_object(const std::string& s, size_t& pos, JSON::ERROR& ec);
	static JSON parse_string(const std::string& s, size_t& pos, JSON::ERROR& ec);
	static JSON parse_number(const std::string& s, size_t& pos, JSON::ERROR& ec);
	static JSON parse_array(const std::string& s, size_t& pos, JSON::ERROR& ec);
	static JSON parse_bool(const std::string& s, size_t& pos, JSON::ERROR& ec);

	JSON(const std::string& s, size_t& pos, JSON::ERROR& ec);

	friend class JSON::exception;
};

std::ostream& operator <<(std::ostream& os, const JSON::TYPE& type);
std::ostream& operator <<(std::ostream& os, const JSON& json);
