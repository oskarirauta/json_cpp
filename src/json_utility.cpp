#include "json/json.hpp"

long long JSON::to_number(const JSON::number_variant& v) {

	long long ll;
	std::visit([&ll](const auto& n) { ll = (long long)n; }, v);
	return ll;
}

long double JSON::to_float(const JSON::float_variant& v) {

	long double ld;
	std::visit([&ld](const auto& n) { ld = (long double)n; }, v);
	return ld;
}

std::string JSON::to_string(const JSON::string_variant& v) {

	std::string s;
	std::visit([&s](const auto& c) { s = std::string(c); }, v);
	return s;
}

