#pragma once

#include <utility>
#include <ostream>
#include <variant>
#include <string>
#include <vector>
#include <map>

#include "json/json.hpp"

struct JSON::const_iterator {

	public:

		friend class JSON;

		const JSON& operator *();
		const JSON* operator ->();

		const JSON& value() const;

		const bool is_object() const;
		const bool is_array() const;
		const size_t index() const;
		const std::string name() const;
		const std::string key() const;
		const size_t size() const;
		const size_t length() const;
		const bool indexed() const;
		const bool named() const;

		const JSON::TYPE type() const;
		const bool operator ==(const JSON::TYPE type) const;
		const bool operator !=(const JSON::TYPE type) const;

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

		const_iterator& operator =(const JSON& other) const;
		const_iterator& operator +=(const JSON& other) const;
		const_iterator& operator +=(const std::pair<std::string, JSON>& pair) const;
		const_iterator& operator +=(const std::initializer_list<std::pair<std::string, JSON>>& list) const;

		const_iterator& operator =(const char& c) const;
		const_iterator& operator =(const unsigned char& c) const;
		const_iterator& operator =(const int& i) const;
		const_iterator& operator =(const unsigned int& i) const;
		const_iterator& operator =(const long& l) const;
		const_iterator& operator =(const unsigned long& l) const;
		const_iterator& operator =(const long long& ll) const;
		const_iterator& operator =(const unsigned long long& ll) const;

		const_iterator& operator =(const long double& ld) const;
		const_iterator& operator =(const double& d) const;
		const_iterator& operator =(const float& f) const;

		const_iterator& operator =(const std::string& s) const;
		const_iterator& operator =(const char* s) const;
		const_iterator& operator =(const bool& b) const;

		const_iterator& operator =(const std::nullptr_t& n) const;

		const bool empty() const;
		const std::string to_string() const;
		long double to_float() const;
		long to_number() const;
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

		const bool contains(const std::string& key) const;
		const bool is_convertible(const JSON::TYPE& to) const;
		const bool convertible_to(const JSON::TYPE& to) const;

		const JSON& as_const();
		const JSON& as_const() const;

		const JSON operator[](const std::string& key) const;
		const JSON operator[](const char* key) const;
		const JSON operator[](size_t index) const;
		const JSON operator[](int index) const;

		const JSON at(const std::string& key) const;
		const JSON at(const char* key) const;
		const JSON at(const size_t index) const;
		const JSON at(const int index) const;

		const_iterator& operator ++() { idx++; return *this; }
		const_iterator operator ++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
		friend bool operator ==(const const_iterator& a, const const_iterator& b) { return a.idx == b.idx; };
		friend bool operator !=(const const_iterator& a, const const_iterator& b) { return a.idx != b.idx; };

		friend std::ostream& operator <<(std::ostream& os, const JSON::const_iterator& it);

	private:

		const JSON* v;
		size_t idx;

		const JSON& const_value() const;
		const_iterator(const JSON* v, size_t index) : v(v), idx(index) {}
};

std::ostream& operator <<(std::ostream& os, const JSON::const_iterator& it);
