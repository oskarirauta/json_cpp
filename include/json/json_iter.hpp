#pragma once

#include <utility>
#include <ostream>
#include <variant>
#include <string>
#include <vector>
#include <map>

#include "json/json.hpp"

struct JSON::iterator {

	public:
		friend class JSON;

		JSON& operator *();
		JSON* operator ->();

		JSON* value();

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

		iterator& operator =(const JSON& other);
		iterator& operator +=(const JSON& other);
		iterator& operator +=(const std::pair<std::string, JSON>& pair);
		iterator& operator +=(const std::initializer_list<std::pair<std::string, JSON>>& list);

		iterator& operator =(const char& c);
		iterator& operator =(const unsigned char& c);
		iterator& operator =(const int& i);
		iterator& operator =(const unsigned int& i);
		iterator& operator =(const long& l);
		iterator& operator =(const unsigned long& l);
		iterator& operator =(const long long& ll);
		iterator& operator =(const unsigned long long& ll);

		iterator& operator =(const long double& ld);
		iterator& operator =(const double& d);
		iterator& operator =(const float& f);

		iterator& operator =(const std::string& s);
		iterator& operator =(const char* s);
		iterator& operator =(const bool& b);

		iterator& operator =(const std::nullptr_t& n);

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

		JSON& operator[](const std::string& key);
		JSON& operator[](const char* key);
		JSON& operator[](size_t index);
		JSON& operator[](int index);

		const JSON operator[](const std::string& key) const;
		const JSON operator[](const char* key) const;
		const JSON operator[](size_t index) const;
		const JSON operator[](int index) const;

		JSON& at(const std::string& key);
		JSON& at(const char* key);
		JSON& at(const size_t index);
		JSON& at(const int index);

		const JSON at(const std::string& key) const;
		const JSON at(const char* key) const;
		const JSON at(const size_t index) const;
		const JSON at(const int index) const;

		iterator& operator ++() { idx++; return *this; }
		iterator operator ++(int) { iterator tmp = *this; ++(*this); return tmp; }
		friend bool operator ==(const iterator& a, const iterator& b) { return a.idx == b.idx; };
		friend bool operator !=(const iterator& a, const iterator& b) { return a.idx != b.idx; };

		friend std::ostream& operator <<(std::ostream& os, const JSON::iterator& it);

	private:

		JSON* v;
		size_t idx;

		const JSON const_value() const;
		iterator(JSON* v, size_t index) : v(v), idx(index) {}
};

std::ostream& operator <<(std::ostream& os, const JSON::iterator& it);
