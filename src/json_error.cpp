#include "json/json.hpp"
#include "json/json_error.hpp"

static std::map<JSON::ERROR_CODE, std::string> error_descriptions = {
	{ JSON::ERROR_CODE::NO_ERROR, "no errors" },
	{ JSON::ERROR_CODE::END_OF_BLOB, "unexpected end of json blob" },
	{ JSON::ERROR_CODE::UNSUPPORTED_BEGIN_OF_BLOB, "json blob begins with unsupported character" },
	{ JSON::ERROR_CODE::NULLPTR_MISREAD, "null attribute expected, found garbage" },
	{ JSON::ERROR_CODE::BOOL_MISREAD, "bool attribute expected, found garbage" },
	{ JSON::ERROR_CODE::EXP_MISSING, "exponent defined, but exponent value is missing" },
	{ JSON::ERROR_CODE::ILLEGAL_NUMBER, "number value is garbage" },
	{ JSON::ERROR_CODE::FLOAT_OUT_OF_RANGE, "float conversion failed, out of range" },
	{ JSON::ERROR_CODE::INT_OUT_OF_RANGE, "int conversion failed, out of range" },
	{ JSON::ERROR_CODE::INVALID_UNICODE, "invalid unicode character" },
	{ JSON::ERROR_CODE::UNSUPPORTED_ESCAPE_CHARACTER, "unsupported escape character" },
	{ JSON::ERROR_CODE::INVALID_KEY, "object has invalid key, only string values are accepted as keys" },
	{ JSON::ERROR_CODE::MISSING_TICK, "unexpected end of blob, missing tick ending string" },
	{ JSON::ERROR_CODE::MISSING_SEMICOLON, "semicolon is missing in json blob" },
	{ JSON::ERROR_CODE::MISSING_COMMA, "json blob is missing a comma" },

	{ JSON::ERROR_CODE::ELEMENT_COPY_ERROR, "json element copy error" },
	{ JSON::ERROR_CODE::INVALID_ELEMENT_TYPE, "invalid json element type" },
	{ JSON::ERROR_CODE::ASSIGNMENT_ERROR, "json element assignment error" },
	{ JSON::ERROR_CODE::OBJECT_SUBSCRIPT_FAIL, "subscript failure, json element is not object" },
	{ JSON::ERROR_CODE::ARRAY_SUBSCRIPT_FAIL, "subscript failure, json element is not array" },
	{ JSON::ERROR_CODE::ARRAY_SUBSCRIPT_RANGE_ERROR, "subscript failure, out of json array bounds" },
	{ JSON::ERROR_CODE::OBJECT_KEY_DOES_NOT_EXIST, "subscript failure with .at(key), key does not exist in object" },
	{ JSON::ERROR_CODE::INVALID_OBJECT_SUBSCRIPT, "subscript failure with .at(key), function can be used only for object types" },
	{ JSON::ERROR_CODE::INVALID_ARRAY_SUBSCRIPT, "subscript failure with .at(index), function can be used only for array types" },
	{ JSON::ERROR_CODE::STRING_CONVERSION_FAILED, "json element string conversion failed" },
	{ JSON::ERROR_CODE::FLOAT_CONVERSION_INVALID, "json conversion to float failed, value is not convertible to number" },
	{ JSON::ERROR_CODE::FLOAT_CONVERSION_RANGE, "json conversion to float failed, value is out of range" },
	{ JSON::ERROR_CODE::FLOAT_CONVERSION_CAST, "json conversion to float failed, casting error" },
	{ JSON::ERROR_CODE::FLOAT_CONVERSION_ERROR, "json conversion to float failed" },
	{ JSON::ERROR_CODE::INT_CONVERSION_INVALID, "json conversion to int failed, value is not convertible to number" },
	{ JSON::ERROR_CODE::INT_CONVERSION_RANGE, "json conversion to int failed, value is out of range" },
	{ JSON::ERROR_CODE::INT_CONVERSION_CAST, "json conversion to int failed, casting error" },
	{ JSON::ERROR_CODE::INT_CONVERSION_ERROR, "json conversion to int failed" },
	{ JSON::ERROR_CODE::BOOL_CONVERSION_INVALID, "json conversion to bool failed, value is not true, false or convertible to number" },
	{ JSON::ERROR_CODE::BOOL_CONVERSION_RANGE, "json conversion to bool failed, number value is out of range" },
	{ JSON::ERROR_CODE::BOOL_CONVERSION_CAST, "json conversion to bool failed, casting error" },
	{ JSON::ERROR_CODE::BOOL_CONVERSION_ERROR, "json conversion to bool failed" },
	{ JSON::ERROR_CODE::ELEMENT_CANNOT_CLEAR, "this type of element does not have clear() available" },
	{ JSON::ERROR_CODE::ERASE_FAILED_NO_KEY, "erase failed, key does not exist in object" },
	{ JSON::ERROR_CODE::ELEMENT_CANNOT_ERASE_KEY, "erase failed, erase(key) is available only for object types" },
	{ JSON::ERROR_CODE::ERASE_FAILED_NO_INDEX, "erase failed, index is out of bounds" },
	{ JSON::ERROR_CODE::ELEMENT_CANNOT_ERASE_INDEX, "erase failed, erase(index) is available only for array types" },
	{ JSON::ERROR_CODE::APPEND_ARRAY_FAILED, "append failed, function is available only for array types" },
	{ JSON::ERROR_CODE::INSERT_ARRAY_FAILED, "insert failed, function is available only for array types" },
	{ JSON::ERROR_CODE::APPEND_OBJECT_FAILED, "append failed, function is available only for object types" },
	{ JSON::ERROR_CODE::INSERT_OBJECT_FAILED, "insert failed, function is available only for object types" },
	{ JSON::ERROR_CODE::FUNCTION_UNAVAILABLE_FOR_CONST, "this function is not available for const JSON" },
	{ JSON::ERROR_CODE::HEX_EXP, "exponent is not supported with hexadecimal values" },
	{ JSON::ERROR_CODE::HEX_FLOAT, "hexadecimal values do not support floats" },
	{ JSON::ERROR_CODE::ILLEGAL_HEX_NUMBER, "hexadecimal number value is garbage" },
	{ JSON::ERROR_CODE::HEX_OUT_OF_RANGE, "hexadecimal number conversion failed, out of range" },

	{ JSON::ERROR_CODE::FILE_NOT_OPEN, "failed to parse json from file, input file stream is not open" },
	{ JSON::ERROR_CODE::FILE_READ_EOF, "failed to parse json from file, unexpected end of file" },
	{ JSON::ERROR_CODE::FILE_READ_ERROR, "failed to parse json from file, file is not readable" },

	{ JSON::ERROR_CODE::PREDICATE_MISMATCH, "json cannot be validated to required format" },
	{ JSON::ERROR_CODE::PREDICATE_REQUIRED_MISSING, "json failed to validate, required key is missing" },

};

JSON::ERROR& JSON::ERROR::operator =(const JSON::ERROR_CODE& ec) {

	this -> code = ec;
	return *this;
}

JSON::ERROR& JSON::ERROR::operator =(const JSON::ERROR& other) {

	this -> code = other.code;
	this -> pos = other.pos;
	this -> coords = other.coords;
	return *this;
}

bool JSON::ERROR::operator ==(const JSON::ERROR_CODE& ec) const {
	return this -> code == ec;
}

JSON::ERROR::operator bool() const {
	return this -> code != NO_ERROR && this -> code != EXP_MISSING &&
		this -> code != FLOAT_OUT_OF_RANGE && this -> code != INT_OUT_OF_RANGE;
}

const size_t JSON::ERROR::row() const {
	return coords.first;
}

const size_t JSON::ERROR::column() const {
	return coords.second;
}

const std::string JSON::ERROR::describe() const {
	if ( static_cast<int>(this -> code) <= 99 )
		return JSON::ERROR::describe(this -> code) + " at row " + std::to_string(this -> row()) + " column " + std::to_string(this -> column());
	else return JSON::ERROR::describe(this -> code);
}

const std::string JSON::ERROR::describe(const JSON::ERROR_CODE& ec) {

	if ( error_descriptions.find(ec) != error_descriptions.end())
		return error_descriptions.at(ec);
	else return "unknown error";
}

bool JSON::exception::operator ==(const JSON::ERROR_CODE& ec) const {
	return this -> _e.code == ec;
}

JSON::exception::operator bool() const {
	return this -> _e == true ? true : false;
}

const JSON::ERROR_CODE JSON::exception::code() const {
	return this -> _e.code;
}

const size_t JSON::exception::pos() const {
	return this -> _e.pos;
}

const std::pair<size_t, size_t> JSON::exception::coords() const {
	return this -> _e.coords;
}

const size_t JSON::exception::row() const {
	return this -> _e.coords.first;
}

const size_t JSON::exception::column() const {
	return this -> _e.coords.second;
}

const std::string JSON::exception::msg() const {
	return this -> _msg;
}

const std::string JSON::exception::describe() const {
	return this -> _e.describe();
}

const char* JSON::exception::what() const noexcept {
	return this -> _msg.empty() ? this -> _e.describe().c_str() : this -> _msg.c_str();
}

std::ostream& operator <<(std::ostream& os, const JSON::ERROR& e) {
	os << e.describe();
	return os;
}

std::ostream& operator <<(std::ostream& os, const JSON::exception& e) {
	os << e.what();
	return os;
}

std::ostream& operator <<(std::ostream& os, const JSON::ERROR_CODE& e) {
	os << JSON::ERROR::describe(e);
	return os;
}
