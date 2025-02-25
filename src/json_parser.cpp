#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <cmath>

#include "json/json.hpp"
#include "json/json_error.hpp"

static const std::pair<size_t, size_t> make_coords(const std::string& s, const size_t pos) {

	size_t row = 1, column = 1;

	if ( pos == 0 || pos >= s.length())
		return std::make_pair(0, 0);
	else if ( s.find_first_of('\n') == std::string::npos )
		return std::make_pair(0, pos);

	for ( size_t i = 0; i < pos; i++ ) {

		if ( s[i] == '\n' ) {
			row++;
			column = 1;
		} else column++;
	}

	return std::make_pair(row, column);
}

static void consume_ws(const std::string& s, size_t& pos) {

	while ( std::isspace(s[pos]))
		pos++;

	if ( s[pos] == '/' && s[pos + 1] == '/' ) {

		pos += 2;
		while ( pos < s.length() && s[pos] != '\n' )
			pos++;

		consume_ws(s, pos);

	} else if ( s[pos] == '/' && s[pos + 1] == '*' ) {

		pos += 2;
		while ( pos < s.length() && s[pos] != '*' && s[pos + 1] != '/' )
			pos++;

		pos += 2;
		consume_ws(s, pos);
	}
}

JSON JSON::parse_array(const std::string& s, size_t& pos, JSON::ERROR& ec) {

	consume_ws(s, ++pos);

	if ( s[pos] == ']' ) {

		pos++;
		return JSON::Array();
	}

	JSON json = JSON::Array();

	while ( true ) {

		JSON value;
		if ( value = JSON(s, pos, ec); ec )
			return nullptr;

		json += value;

		consume_ws(s, pos);

		if ( s[pos] == ',' ) {

			pos++;

			size_t backup_pos = pos;
			consume_ws(s, pos);

			if ( s[pos] == ']' ) {

				pos++;
				break;

			} else pos = backup_pos;

			continue;

		} else if ( s[pos] == ']' ) {

			pos++;
			break;

		} else {
			ec = { .code = JSON::ERROR_CODE::MISSING_COMMA, .pos = pos, .coords = make_coords(s, pos) };
			return nullptr;
		}
	}

	if ( ec )
		return nullptr;

	return json;
}

JSON JSON::parse_object(const std::string& s, size_t& pos, JSON::ERROR& ec) {

	consume_ws(s, ++pos);

	if ( s[pos] == '}' ) {

		pos++;
		return JSON::Object();

	}

	JSON json = JSON::Object();

	while ( true ) {

		auto begin = pos;
		JSON key, value;

		if ( key = JSON(s, pos, ec); ec )
			return nullptr;

		if ( key != JSON::TYPE::STRING ) {

			ec = { .code = JSON::ERROR_CODE::INVALID_KEY, .pos = begin, .coords = make_coords(s, pos) };
			return nullptr;

		}

		consume_ws(s, pos);

		if ( s[pos] != ':' ) {

			ec = { .code = JSON::ERROR_CODE::MISSING_SEMICOLON, .pos = pos, .coords = make_coords(s, pos) };
			return nullptr;

		} else pos++;

		begin = pos;

		if ( value = JSON(s, pos, ec); ec )
			return nullptr;

		json[key.to_string()] = value;

		consume_ws(s, pos);

		if ( s[pos] == ',' ) {

			pos++;

			size_t backup_pos = pos;
			consume_ws(s, pos);

			if ( s[pos] == '}' ) {

				pos++;
				break;

			} else pos = backup_pos;

			continue;

		} else if ( s[pos] == '}' ) {

			pos++;
			break;

		} else {

			ec = { .code = JSON::ERROR_CODE::MISSING_COMMA, .pos = pos, .coords = make_coords(s, pos) };
			return nullptr;
		}
	}

	if ( ec )
		return nullptr;

	return json;
}

JSON JSON::parse_string(const std::string& s, size_t& pos, JSON::ERROR& ec) {

	auto tick = s[pos++];
	std::string value;

	while ( pos < s.length() && s[pos] != tick ) {

		if ( s[pos] == '\\' ) {

			if ( JSON::unescape_chars.find(s[pos + 1]) != JSON::unescape_chars.end()) {

				value += JSON::unescape_chars.at(s[pos + 1]);
				pos += 2;
				continue;

			} else if ( s[pos + 1] == 'u' ) {

				std::string unicode = "0x"; // = "\\u";
				for ( size_t i = 0; i < 4; i++ )
					if ( std::isdigit(s[pos + 2 + i ]) ||
					   ( s[pos + 2 + i] >= 'a' && s[pos + 2 + i] <= 'f' ) ||
					   ( s[pos + 2 + i] >= 'A' && s[pos + 2 + i] <= 'F' ))
						unicode += std::tolower(s[pos + 2 + i]);

				if ( unicode.length() == 6 ) {

					int unicode_value;

					try {
						unicode_value = std::stold(unicode);
					} catch ( const std::exception& e ) {
						ec = { .code = JSON::ERROR_CODE::INVALID_UNICODE, .pos = pos, .coords = make_coords(s, pos) };
						return nullptr;
					}

					value += std::string::value_type(unicode_value);
					pos += 6;
					continue;

				} else {

					ec = { .code = JSON::ERROR_CODE::INVALID_UNICODE, .pos = pos, .coords = make_coords(s, pos) };
					return nullptr;
				}

			} else {

				value += '\\';
				pos += 2;
				return nullptr;
			}
		}

		value += s[pos++];
	}

	pos++;

	if ( s[pos - 1] != tick ) {
		ec = { .code = JSON::ERROR_CODE::MISSING_TICK, .pos = pos < 1 ? 0 : ( pos - 1 ), .coords = make_coords(s, pos < 1 ? 0 : ( pos - 1 )) };
		return nullptr;
	}

	return value;
}

JSON JSON::parse_number(const std::string& s, size_t& pos, JSON::ERROR& ec) {

	JSON json = nullptr;
	size_t begin = pos, exp_pos;
	bool is_float = false;
	bool is_hex = false;
	std::string value, exp;

	if ( s[pos] == '-' ) {
		value += '-';
		pos++;
	}

	if ( s[pos] == '.' ) {
		value += "0.";
		pos++;
	}

	if ( s[pos] == '0' && s[pos + 1] == 'x' ) {
		is_hex = true;
		pos += 2;
	}

	if ( !is_hex ) {

		while ( std::isdigit(s[pos]) || ( s[pos] == '.'  && value.find_first_of('.') == std::string::npos ))
			value += s[pos++];

	} else {

		if ( value.substr(0, 2) == "0." || value.substr(0, 3) == "-0." ) {

			ec = { .code = JSON::ERROR_CODE::HEX_FLOAT, .pos = begin, .coords = make_coords(s, begin) };
			return nullptr;
		}

		while ( std::string("1234567890abcdef").find(std::tolower(s[pos])) != std::string::npos )
			value += std::tolower(s[pos++]);

		while ( std::isspace(s[pos]))
			pos++;

		if ( std::tolower(s[pos]) == 'e' ) {
			ec = { .code = JSON::ERROR_CODE::HEX_EXP, .pos = pos, .coords = make_coords(s, pos) };
			return nullptr;
		}

		if ( std::tolower(s[pos]) == '.' ) {
			ec = { .code = JSON::ERROR_CODE::HEX_FLOAT, .pos = pos, .coords = make_coords(s, pos) };
			return nullptr;
		}
	}

	if ( !is_hex ) {

		if ( value.back() == '.' )
			value.pop_back();
		else if ( value.find_first_of('.') != std::string::npos )
			is_float = true;

		while ( value.length() > 1 && value[0] == '0' && std::isdigit(value[1]))
			value = value.substr(1, value.length() - 1);

		if ( std::tolower(s[pos]) == 'e') {

			exp_pos = pos;
			pos++;
			while ( std::isdigit(s[pos]))
				exp += s[pos++];

			if ( exp.empty())
				ec = { .code = JSON::ERROR_CODE::EXP_MISSING, .pos = pos, .coords = make_coords(s, pos) };
		}

		if ( value.empty() && exp.empty()) {

			ec = { .code = JSON::ERROR_CODE::END_OF_BLOB, .pos = pos, .coords = make_coords(s, pos) };
			return nullptr;

		} else if ( value.empty() && std::string(" \t,]}").find_first_of(s[pos]) == std::string::npos ) {

			ec = { .code = JSON::ERROR_CODE::ILLEGAL_NUMBER, .pos = begin, .coords = make_coords(s, pos) };
			return nullptr;

		}
	}

	long long e;
	bool exp_error = false;

	if ( !exp.empty()) {

		try {
			if ( is_hex ) {
				long double ld = std::stold(value); // stoll does not support 0x for hex..
				e = (long long)ld;
			} else e = std::stoll(value);
		} catch ( const std::invalid_argument& e ) {
			ec = { .code = JSON::ERROR_CODE::ILLEGAL_NUMBER, .pos = exp_pos, .coords = make_coords(s, exp_pos) };
			return nullptr;
		} catch ( const std::out_of_range& e ) {
			ec = { .code = JSON::ERROR_CODE::INT_OUT_OF_RANGE, .pos = exp_pos, .coords = make_coords(s, exp_pos) };
			exp_error = true;
		}
	}

	if ( is_float ) {

		double d;
		try {
			d = std::stold(value);
		} catch ( const std::invalid_argument& e ) {
			ec = { .code = JSON::ERROR_CODE::ILLEGAL_NUMBER, .pos = begin, .coords = make_coords(s, begin) };
			return nullptr;
		} catch ( const std::out_of_range& e ) {
			ec = { .code = JSON::ERROR_CODE::FLOAT_OUT_OF_RANGE, .pos = begin, .coords = make_coords(s, begin) };
			d = 0;
		}

		if ( !exp.empty() && !exp_error )
			d *= std::pow(10, e);

		return d;

	} else {

		long long ll;

		try {
			ll = std::stoll(value, nullptr, is_hex ? 16 : 10);
		} catch ( const std::invalid_argument& e ) {

			ec = { .code = is_hex ? JSON::ERROR_CODE::ILLEGAL_HEX_NUMBER : JSON::ERROR_CODE::ILLEGAL_NUMBER,
				.pos = begin, .coords = make_coords(s, begin) };
			return nullptr;
		} catch ( const std::out_of_range& e ) {

			ec = { .code = is_hex ? JSON::ERROR_CODE::HEX_OUT_OF_RANGE : JSON::ERROR_CODE::INT_OUT_OF_RANGE,
				.pos = exp_pos, .coords = make_coords(s, exp_pos) };
			ll = 0;
		}

		if ( !exp.empty() && !exp_error )
			ll *= std::pow(10, e);

		return ll;
	}

	return nullptr;
}

JSON JSON::parse_bool(const std::string& s, size_t& pos, JSON::ERROR& ec) {

	if ( JSON::to_lower(s.substr(pos, 4)) == "true" ) {
		pos += 4;
		return true;
	} else if ( JSON::to_lower(s.substr(pos, 5)) == "false" ) {
		pos += 5;
		return false;
	} else ec = { .code = JSON::ERROR_CODE::BOOL_MISREAD, .pos = pos, .coords = make_coords(s, pos) };

	return nullptr;
}

JSON JSON::parse_nullptr(const std::string& s, size_t& pos, JSON::ERROR& ec) {

	if ( JSON::to_lower(s.substr(pos, 4)) != "null" )
		ec = { .code = JSON::ERROR_CODE::NULLPTR_MISREAD, .pos = pos, .coords = make_coords(s, pos) };
	else pos += JSON::to_lower(s.substr(pos, 7)) == "nullptr" ? 7 : 4;

	return nullptr;
}

JSON::JSON(const std::string& s, size_t& pos, JSON::ERROR& ec) {

	this -> emplace<std::nullptr_t>(std::forward<decltype(nullptr)>(nullptr));

	consume_ws(s, pos);
	if ( pos >= s.length()) {

		ec = { .code = JSON::ERROR_CODE::END_OF_BLOB, .pos = pos < 1 ? 0 : ( pos - 1 ), .coords = make_coords(s, pos < 1 ? 0 : ( pos - 1 )) };

	} else {

		JSON json;

		switch ( s[pos] ) {
			case '[':
				  json = JSON::parse_array(s, pos, ec);
				  break;
			case '{':
				  json = JSON::parse_object(s, pos, ec);
				  break;
			case '\"':
			case '\'':
				  json = JSON::parse_string(s, pos, ec);
				  break;
			case 't':
			case 'f':
				  json = JSON::parse_bool(s, pos, ec);
				  break;
			case 'n':
				  json = JSON::parse_nullptr(s, pos, ec);
				  break;
			default:
				if ( std::isdigit(s[pos]) || s[pos] == '-' || s[pos] == '.' )
					json = JSON::parse_number(s, pos, ec);
				else
					ec = { .code = JSON::ERROR_CODE::UNSUPPORTED_BEGIN_OF_BLOB, .pos = pos, .coords = make_coords(s, pos) };
				break;
		}

		if ( !ec )
			*this = json;
	}
}

JSON JSON::parse(const std::string& s) {

	JSON::ERROR ec;
	size_t pos = 0;
	JSON json(s, pos, ec);

	if ( ec )
		throw JSON::exception(ec);

	return json;
}

JSON JSON::parse(const char* s) {

	std::string _s(s);
	JSON j;

	try {
		j = JSON::parse(_s);
	} catch ( const JSON::exception& e ) {
		throw e;
	}

	return j;
}

JSON JSON::load(std::ifstream& ifd) {

	if ( !ifd.is_open())
		throw JSON::exception(JSON::ERROR_CODE::FILE_NOT_OPEN);

	else if ( !ifd.good() || ifd.bad() || ifd.fail()) {

		if ( ifd.is_open())
			ifd.close();

		throw JSON::exception(JSON::ERROR_CODE::FILE_READ_ERROR);

	} else if ( ifd.eof()) {

		if ( ifd.is_open())
			ifd.close();

		throw JSON::exception(JSON::ERROR_CODE::FILE_READ_EOF);
	}

	std::stringstream ss;
	std::string blob;

	ss << ifd.rdbuf();
	blob = ss.str();
	ifd.close();

	try {
		JSON json = JSON::parse(blob);
		return json;
	} catch ( const JSON::exception& e ) {
		throw e;
	}
}
