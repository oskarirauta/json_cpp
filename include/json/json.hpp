#pragma once

#include <utility>
#include <ostream>
#include <fstream>
#include <variant>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>

class JSON : public std::variant<std::map<std::string, JSON>, std::vector<JSON>, std::nullptr_t, std::string, long double, long long, bool> {

	public:

		using variant::variant;

		struct iterator;
		struct const_iterator;
		struct fe_iterator;

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
			FUNCTION_UNAVAILABLE_FOR_CONST = 132,
			HEX_EXP = 133, HEX_FLOAT = 134, ILLEGAL_HEX_NUMBER = 135, HEX_OUT_OF_RANGE = 136,

			FILE_NOT_OPEN = 201, FILE_READ_EOF = 202, FILE_READ_ERROR = 203,

			PREDICATE_ARRAY_FAIL = 301, PREDICATE_TYPE_FAILURE = 302, PREDICATE_VALIDATOR_TYPE_MISMATCH = 303,
			PREDICATE_MISMATCH = 304, PREDICATE_REQUIRED_MISSING = 305, PREDICATE_UNALLOWED_VALUE = 306
		};

		class PREDICATE {

			friend class JSON;

			public:

				struct PROPERTIES {

					public:
						JSON::TYPE type;
						bool required = false;
						std::vector<JSON> values = {};
						std::vector<JSON::PREDICATE> validate = {};
				};

				PREDICATE& operator =(const JSON::TYPE& type);
				PREDICATE& operator =(const JSON::PREDICATE::PROPERTIES& cfg);

				bool operator ==(const JSON::TYPE& type) const;
				bool operator ==(const JSON& json) const;
				bool operator !=(const JSON::TYPE& type) const;
				bool operator !=(const JSON& json) const;

				PREDICATE& require(bool state = true);

				std::string name() const;
				JSON::TYPE type() const;
				bool is_required() const;
				bool is_optional() const;
				std::vector<JSON>& values();
				const std::vector<JSON>& values() const;
				std::vector<PREDICATE>& validate();
				const std::vector<PREDICATE>& validate() const;

				PREDICATE(const std::string& name, const JSON::TYPE& type) :
					_name(name), _type(type), _required(false), _values(), _validate() {}
				PREDICATE(const std::string& name, const JSON::PREDICATE::PROPERTIES& cfg) :
					_name(name), _type(cfg.type), _required(cfg.required), _values(cfg.values), _validate(cfg.validate) {}

				friend std::ostream& operator <<(std::ostream& os, const JSON::PREDICATE& predicate);

			private:
				std::string _name;
				JSON::TYPE _type;
				bool _required = false;
				std::vector<JSON> _values = {};
				std::vector<JSON::PREDICATE> _validate = {};
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

		using for_each_function = std::function<void(fe_iterator&, JSON&)>;
		using const_for_each_function = std::function<void(fe_iterator&, const JSON&)>;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin();
		const_iterator cend();

		const TYPE type() const;
		const bool operator ==(const TYPE type) const;
		const bool operator !=(const TYPE type) const;

		const bool operator ==(const char& c) const;
		const bool operator ==(const unsigned char& c) const;
		const bool operator ==(const int& i) const;
		const bool operator ==(const unsigned int& i) const;
		const bool operator ==(const long& l) const;
		const bool operator ==(const unsigned long& l) const;
		const bool operator ==(const long long& ll) const;
		const bool operator ==(const unsigned long long& ll) const;

		const bool operator ==(const long double& ld) const;
		const bool operator ==(const double& d) const;
		const bool operator ==(const float& f) const;

		const bool operator ==(const std::string& s) const;
		const bool operator ==(const char* s) const;

		const bool operator ==(const std::nullptr_t& n) const;

		const bool operator !=(const char& c) const;
		const bool operator !=(const unsigned char& c) const;
		const bool operator !=(const int& i) const;
		const bool operator !=(const unsigned int& i) const;
		const bool operator !=(const long& l) const;
		const bool operator !=(const unsigned long& l) const;
		const bool operator !=(const long long& ll) const;
		const bool operator !=(const unsigned long long& ll) const;

		const bool operator !=(const long double& ld) const;
		const bool operator !=(const double& d) const;
		const bool operator !=(const float& f) const;

		const bool operator !=(const std::string& s) const;
		const bool operator !=(const char* s) const;

		const bool operator !=(const std::nullptr_t& n) const;

		JSON& operator =(const JSON& other);
		JSON& operator +=(const JSON& other);
		JSON& operator +=(const std::pair<std::string, JSON>& pair);
		JSON& operator +=(const std::initializer_list<std::pair<std::string, JSON>>& list);

		JSON& operator =(const char& c);
		JSON& operator =(const unsigned char& c);
		JSON& operator =(const int& i);
		JSON& operator =(const unsigned int& i);
		JSON& operator =(const long& l);
		JSON& operator =(const unsigned long& l);
		JSON& operator =(const long long& ll);
		JSON& operator =(const unsigned long long& ll);

		JSON& operator =(const long double& ld);
		JSON& operator =(const double& d);
		JSON& operator =(const float& f);

		JSON& operator =(const std::string& s);
		JSON& operator =(const char* s);
		JSON& operator =(const bool& b);
		JSON& operator =(const std::nullptr_t& n);

		JSON& operator [](const std::string& key);
		JSON& operator [](const char* key);
		JSON& operator [](size_t index);
		JSON& operator [](int index);

		const JSON operator [](const std::string& key) const;
		const JSON operator [](const char* key) const;
		const JSON operator [](size_t index) const;
		const JSON operator [](int index) const;

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
		const bool contains_path(const std::vector<std::string>& path) const;
		const bool contains_any(const std::set<std::string>& keys) const;

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

		void for_each(const for_each_function lambda);
		void for_each(const const_for_each_function lambda) const;

		void validate(const std::vector<JSON::PREDICATE>& reqs, const std::vector<std::string>& path = {}) const;

		static const std::string escape(const std::string& s);
		static const std::string unescape(const std::string& s);
		static const std::string to_lower(const std::string& s);
		static const std::string trim_special_chars(const std::string& s);
		static const std::string describe(const JSON::TYPE& type);
		static const std::string describe(const JSON::PREDICATE& predicate);
		static const std::string describe(const std::vector<JSON::PREDICATE>& predicates);

		JSON();
		JSON(const bool& b);
		JSON(const char& c);
		JSON(const unsigned char& c);
		JSON(const int& i);
		JSON(const unsigned int& i);
		JSON(const long& l);
		JSON(const unsigned long& l);
		JSON(const long long& ll);
		JSON(const unsigned long long& ll);

		JSON(const long double& ld);
		JSON(const double& d);
		JSON(const float& f);

		JSON(const std::string &s);
		JSON(const char* s);

		JSON(const std::nullptr_t& n);
		JSON(const std::map<std::string, JSON>&m);
		JSON(const std::vector<JSON>&a);
		JSON(const JSON& other);

		static JSON parse(const std::string& s);
		static JSON parse(const char* s);
		static JSON load(std::ifstream& ifd);

		friend std::ostream& operator <<(std::ostream& os, const JSON& json);

	private:

		static const std::map<JSON::TYPE, std::string> type_descs;
		static const std::map<std::string::value_type, std::string> escape_chars;
		static const std::map<std::string::value_type, std::string::value_type> unescape_chars;

		static const std::string describe(const JSON::PREDICATE& predicate, int level);
		static const std::string describe(const std::vector<JSON::PREDICATE>& predicates, int level);

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
std::ostream& operator <<(std::ostream& os, const JSON::PREDICATE& predicate);
std::ostream& operator <<(std::ostream& os, const std::vector<JSON::PREDICATE>& predicates);
std::ostream& operator <<(std::ostream& os, const JSON& json);
