#include <charconv>
#include <cstdio>

#include "json/json.hpp"

const std::map<JSON::TYPE, std::string> JSON::type_descs = {
	{ JSON::TYPE::OBJECT, "object" },
	{ JSON::TYPE::ARRAY, "array" },
	{ JSON::TYPE::STRING, "string" },
	{ JSON::TYPE::FLOAT, "float" },
	{ JSON::TYPE::INT, "int" },
	{ JSON::TYPE::BOOL, "bool" },
	{ JSON::TYPE::NULLPTR, "null" }
};

const std::map<std::string::value_type, std::string> JSON::escape_chars = {
	{ '\"', "\\\"" },
	{ '\'', "\\\'" },
	{ '/', "\\/" },
	{ '\\', "\\\\" },
	{ '\b', "\\b" },
	{ '\f', "\\f" },
	{ '\n', "\\n" },
	{ '\r', "\\r" },
	{ '\t', "\\t" }
};

const std::map<std::string::value_type, std::string::value_type> JSON::unescape_chars = {
	{ '\"', '\"' },
	{ '\'', '\'' },
	{ '/', '/' },
	{ 'b', '\b' },
	{ 'f', '\f' },
	{ 'n', '\n' },
	{ 'r', '\r' },
	{ 't', '\t' },
	{ '\\', '\\' }
};

static std::string special_chars = "\b\f\n\r\t";

const std::string unicode_conv(const std::string::value_type& ch) {

	std::string out(6, '\\');
	sprintf(&out[1], "u%04x", (int)ch);
	return out;
}

const std::string JSON::escape(const std::string& s) {

	std::string out;
	for ( auto& ch : s ) {

		if ( JSON::escape_chars.find(ch) != JSON::escape_chars.end())
			out += JSON::escape_chars.at(ch);
		else if ( ch >= 0x00 && ch <= 0x1f )
			out += unicode_conv(ch);
		else out += ch;
	}

	return out;
}

const std::string JSON::unescape(const std::string& s) {

	bool escaped = false;
	bool unicode = false;
	std::string sequence;

	std::string out;
	for ( auto& ch : s ) {

		if ( unicode ) {

			if (( ch >= 'a' && ch <= 'f' ) ||
			    ( ch >= 'A' && ch <= 'F' ) ||
			    ( ch >= '0' && ch <= '9' )) {

				sequence += ch;

				if ( sequence.length() == 4 ) {

					int n;
					try {
						n = std::stoll(sequence, nullptr, 16);
					} catch ( const std::exception& e) {
						out += "\\u" + sequence;
						sequence = "";
						unicode = false;
						continue;
					}
					out += std::string::value_type(n);
					sequence = "";
					unicode = false;
				}
				continue;
			}

			out += "\\u" + sequence;
			out += ch;
			sequence = "";
			unicode = false;
			continue;
		}

		if ( !escaped && ch == '\\' ) {
			escaped = true;
			continue;
		} else if ( !escaped ) {
			out += ch;
			continue;
		}

		// escaped
		if ( JSON::unescape_chars.find(ch) != JSON::unescape_chars.end())
			out += JSON::unescape_chars.at(ch);
		else if ( ch == 'u' )
			unicode = true;
		else out += ch;
		escaped = false;
	}

	if ( !sequence.empty())
		out += "\\u" + sequence;

	return out;
}

const std::string JSON::trim_special_chars(const std::string& s) {

	std::string out;

	for ( const std::string::value_type& ch : s ) {

		if (( special_chars.find_first_of(ch) != std::string::npos ) || ( ch >= 0x00 && ch <= 0x1f ))
			continue;
		out += ch;
	}

	return out;
}

const std::string JSON::describe(const JSON::TYPE& type) {

	return JSON::type_descs.find(type) != JSON::type_descs.end() ? JSON::type_descs.at(type) : "unknown";
}

const bool JSON::is_convertible(const JSON::TYPE& to) const {

	if ( to == OBJECT )
		return *this == OBJECT;
	else if ( to == ARRAY )
		return *this == ARRAY;
	if ( to == STRING )
		return ( *this != OBJECT && *this != ARRAY ) ? true : false;
	else if ( to == FLOAT ) {

		if ( *this == FLOAT || *this == INT || *this == BOOL )
			return true;
		else if ( *this == OBJECT || *this == ARRAY || *this == NULLPTR )
			return false;
		else if ( *this == STRING ) {

			try {
				std::stold(std::get<std::string>(*this));
			} catch ( const std::exception& e ) {
				return false;
			}

			return true;
		}

	} else if ( to == INT ) {

		if ( *this == INT || *this == FLOAT || *this == BOOL )
			return true;
		else if ( *this == OBJECT || *this == ARRAY || *this == NULLPTR )
			return false;
		else if ( *this == STRING ) {

			try {
				std::stoll(std::get<std::string>(*this));
			} catch ( const std::exception& e ) {
				return false;
			}

			return true;
		}
	} else if ( to == BOOL ) {

		if ( *this == INT || *this == FLOAT || *this == BOOL )
			return true;
		else if ( *this == OBJECT || *this == ARRAY || *this == NULLPTR )
			return false;
		else if ( *this == STRING ) {

			std::string s = std::get<std::string>(*this);
			if ( JSON::to_lower(s) == "true" || JSON::to_lower(s) == "false" )
				return true;

			int result{};
			auto [ptrc, ec] = std::from_chars(s.data(), s.data() + s.size(), result);

			return ec == std::errc() ? true : false;
		}
	}

	return false;
}

const bool JSON::convertible_to(const JSON::TYPE& type) const {
	return this -> is_convertible(type);
}

std::ostream& operator <<(std::ostream& os, const JSON::TYPE& type) {
	os << JSON::describe(type);
	return os;
}

std::ostream& operator <<(std::ostream& os, const JSON::PREDICATE& predicate) {
	os << JSON::describe(predicate);
	return os;
}

std::ostream& operator <<(std::ostream& os, const std::vector<JSON::PREDICATE>& predicates) {
	os << JSON::describe(predicates);
	return os;
}

std::ostream& operator <<(std::ostream& os, const JSON& json) {
	os << json.to_string();
	return os;
}
