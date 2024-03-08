#pragma once

#include <utility>
#include <ostream>
#include <variant>
#include <string>
#include <vector>
#include <map>

#include "json/json.hpp"

struct JSON::const_iterator {

	friend class JSON;

	class RESULT : public std::variant<std::pair<std::string, JSON>, std::pair<size_t, JSON>, JSON> {

		public: using variant::variant;

		const JSON::TYPE type() const;
		const bool operator ==(const JSON::TYPE type) const;
		const bool operator !=(const JSON::TYPE type) const;
		const bool indexed() const;
		const bool named() const;
		const size_t length() const;
		const size_t index() const;
		const std::string name() const;
		const JSON value() const;

		const std::size_t size() const;
		const bool empty() const;
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

		friend std::ostream& operator <<(std::ostream& os, const JSON::const_iterator::RESULT& c_res);
	};

	const RESULT& operator *() const;
	const RESULT* operator ->() const;

	const_iterator& operator ++() { idx++; return *this; }
	const_iterator operator ++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
	friend bool operator ==(const const_iterator& a, const const_iterator& b) { return a.idx == b.idx; };
	friend bool operator !=(const const_iterator& a, const const_iterator& b) { return a.idx != b.idx; };

	friend std::ostream& operator <<(std::ostream& os, const JSON::const_iterator& c_it);

	private:

	const JSON* v;
	mutable RESULT result;
	size_t idx;

	const_iterator(const JSON* v, size_t index) : v(v), idx(index) {}
};

std::ostream& operator <<(std::ostream& os, const JSON::const_iterator& c_it);
std::ostream& operator <<(std::ostream& os, const JSON::const_iterator::RESULT& c_res);
