#pragma once

#include <utility>
#include <ostream>
#include <variant>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

class JSON : public std::variant<std::map<std::string, JSON>, std::vector<JSON>, std::nullptr_t, std::string, double, long long, bool> {

	public: using variant::variant;

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
	using Float = double;
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

	const bool operator ==(const bool& b) const;
	const bool operator ==(const int& i) const;
	const bool operator ==(const long long& l) const;
	const bool operator ==(const double& d) const;
	const bool operator ==(const float& f) const;
	const bool operator ==(const std::string& s) const;
	const bool operator ==(const char* s) const;
	const bool operator ==(const std::nullptr_t& n) const;
	const bool operator ==(const uint64_t& i) const;
	const bool operator ==(const int64_t& i) const;

	JSON& operator =(const JSON& other);
	JSON& operator +=(const JSON& other);
	JSON& operator +=(const std::pair<std::string, JSON>& pair);
	JSON& operator +=(const std::initializer_list<std::pair<std::string, JSON>>& list);
	JSON& operator =(const uint64_t& i);
	JSON& operator =(const int64_t& i);

	JSON& operator [](const std::string& key);
	JSON& operator [](const char* key);
	JSON& operator [](const size_t index);
	JSON& operator [](const int index);

	const JSON operator [](const std::string& key) const;
	const JSON operator [](const char* key) const;
	const JSON operator [](const size_t index) const;
	const JSON operator [](const int index) const;

	JSON& at(const std::string& key);
	JSON& at(const char* key);
	JSON& at(const size_t index);
	JSON& at(const int index);
	const JSON at(const std::string& key) const;
	const JSON at(const char* key) const;
	const JSON at(const size_t index) const;
	const JSON at(const int index) const;

	const size_t length() const;
	const std::size_t size() const;
	const bool empty() const;
	const bool contains(const std::string& key) const;

	const std::string to_string() const;
	double to_float() const;
	double to_double() const;
	long long to_int() const;
	bool to_bool() const;

	operator std::string() const;
	operator double() const;
	operator long long() const;
	operator int() const;
	operator bool() const;

	void clear();
	void erase(const std::string& key);
	void erase(const size_t index);
	void erase(const int index);
	void append(const JSON& json);
	void insert(const JSON& json);
	void append(const std::pair<std::string, JSON>& pair);
	void insert(const std::pair<std::string, JSON>& pair);
	void append(const std::initializer_list<std::pair<std::string, JSON>>& list);
	void insert(const std::initializer_list<std::pair<std::string, JSON>>& list);

	void clear() const;
	void erase(const std::string& key) const;
	void erase(const size_t index) const;
	void erase(const int index) const;
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
	JSON(const int& i);
	JSON(const long long& l);
	JSON(const uint64_t& i);
	JSON(const int64_t& i);
	JSON(const double& d);
	JSON(const float& f);
	JSON(const std::string& s);
	JSON(const std::nullptr_t& n);
	JSON(const std::map<std::string, JSON>&m);
	JSON(const std::vector<JSON>&a);
	JSON(const JSON& other);

	static JSON parse(const std::string& s);

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
