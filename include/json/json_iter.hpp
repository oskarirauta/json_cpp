#pragma once

#include <utility>
#include <ostream>
#include <variant>
#include <string>
#include <vector>
#include <map>

#include "json/json.hpp"

struct JSON::iterator {

	friend class JSON;

	class RESULT : public std::variant<std::pair<std::string, JSON*>, std::pair<size_t, JSON*>, JSON*> {

		public:

		using variant::variant;

		const JSON::TYPE type() const;
		const bool operator ==(const JSON::TYPE type) const;
		const bool operator !=(const JSON::TYPE type) const;
		const bool indexed() const;
		const bool named() const;
		const size_t length() const;
		const size_t index() const;
		const std::string name() const;
		JSON* value();

		const std::size_t size() const;
		const bool empty() const;
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

		friend std::ostream& operator <<(std::ostream& os, const JSON::iterator::RESULT& res);
	};

	RESULT& operator *();
	RESULT* operator ->();

	iterator& operator ++() { idx++; return *this; }
	iterator operator ++(int) { iterator tmp = *this; ++(*this); return tmp; }
	friend bool operator ==(const iterator& a, const iterator& b) { return a.idx == b.idx; };
	friend bool operator !=(const iterator& a, const iterator& b) { return a.idx != b.idx; };

	friend std::ostream& operator <<(std::ostream& os, const JSON::iterator& it);

	private:

	JSON* v;
	RESULT result;
	size_t idx;

	iterator(JSON* v, size_t index) : v(v), idx(index) {}
};

std::ostream& operator <<(std::ostream& os, const JSON::iterator& it);
std::ostream& operator <<(std::ostream& os, const JSON::iterator::RESULT& res);
