#pragma once

#include <variant>
#include <string>

struct JSON::fe_iterator : public std::variant<std::string, size_t, std::nullptr_t> {

	public:

		using variant::variant;

		bool is_object() const;
		bool is_array() const;
		size_t index() const;
		std::string name() const;
		std::string key() const;
		bool named() const;
		bool indexed() const;
};
