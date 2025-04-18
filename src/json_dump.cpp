#include "json/json_const_iter.hpp"
#include "json/json_iter.hpp"
#include "json/json.hpp"

static const int tab_size = 2;

static const std::string tab(int level) {

	return level < 1 ? "" : std::string(tab_size * level, ' ');
}

static const std::string const_iterate(const JSON x, const bool pretty, int level) {

	if ( x == JSON::TYPE::OBJECT && x.empty()) return "{}";
	else if ( x == JSON::TYPE::ARRAY && x.empty()) return "[]";

	std::string out;

	for ( auto j = x.begin(); j != x.end(); j++ ) {

		if ( pretty ) {
			out += !out.empty() ? ",\n" : "";
			out += tab(level + 1);
		} else if ( !pretty && !out.empty()) out += ',';

		if ( j.named())
			out += "\"" + JSON::escape(j.name()) + ( pretty ? "\": " : "\":" );

		if ( j == JSON::TYPE::OBJECT || j == JSON::TYPE::ARRAY )
			out += const_iterate(j.value(), pretty, level + 1);
		else if ( j == JSON::TYPE::STRING )
			out += "\"" + JSON::escape(j.to_string()) + "\"";
		else out += j.to_string();
	}

	if ( x == JSON::TYPE::OBJECT ) {
		if ( pretty ) return "{\n" + out + "\n" + tab(level) + "}";
		else return "{" + out + "}";
	} else if ( x == JSON::TYPE::ARRAY ) {
		if ( pretty ) return "[\n" + out + "\n" + tab(level) + "]";
		else return "[" + out + "]";
	} else return out;
}

static const std::string iterate(JSON x, const bool pretty, int level) {

	if ( x == JSON::TYPE::OBJECT && x.empty()) return "{}";
	else if ( x == JSON::TYPE::ARRAY && x.empty()) return "[]";

	std::string out;

	for ( auto j = x.begin(); j != x.end(); j++ ) {

		if ( pretty ) {
			out += !out.empty() ? ",\n" : "";
			out += tab(level + 1);
		} else if ( !pretty && !out.empty()) out += ',';

		if ( j.named())
			out += "\"" + JSON::escape(j.name()) + ( pretty ? "\": " : "\":" );

		if ( j == JSON::TYPE::OBJECT || j == JSON::TYPE::ARRAY )
			out += iterate(*j.value(), pretty, level + 1);
		else if ( j == JSON::TYPE::STRING )
			out += "\"" + JSON::escape(j.to_string()) + "\"";
		else out += j.to_string();
	}

	if ( x == JSON::TYPE::OBJECT ) {
		if ( pretty ) return "{\n" + out + "\n" + tab(level) + "}" ;
		else return "{" + out + "}";
	} else if ( x == JSON::TYPE::ARRAY ) {
		if ( pretty  ) return "[\n" + out + "\n" + tab(level) + "]";
		else return "[" + out + "]";
	} else return out;
}

const std::string JSON::dump(const bool pretty) const {

	if ( *this == JSON::TYPE::OBJECT || *this == JSON::TYPE::ARRAY )
		return const_iterate(*this, pretty, 0);
	else return to_string();
}

const std::string JSON::dump(const bool pretty) {

	if ( *this == JSON::TYPE::OBJECT || *this == JSON::TYPE::ARRAY )
		return iterate(*this, pretty, 0);
	else return to_string();
}


const std::string JSON::dump_minified() const {
	return this -> dump(false);
}

const std::string JSON::dump_minified() {
	return this -> dump(false);
}

const std::string JSON::describe(const JSON::PREDICATE& predicate, int level) {

	std::string s = tab(level);

	s += "{ \"" + predicate._name + "\": { .type = " + JSON::describe(predicate._type);
	s += std::string(", .required = ") + ( predicate._required ? "true" : "false" );

	if ( predicate._type != JSON::OBJECT && predicate._type != JSON::ARRAY && !predicate._values.empty()) {

		std::string v;
		s += ", .values = {\n";

		for ( const JSON& j : predicate._values )
			v += ( !v.empty() ? ",\n" : "" ) + tab(level + 2) + j.to_string();

		s += v + "\n" + tab(level) + "}}";

	} else if ( predicate._type == JSON::OBJECT && !predicate._validate.empty()) {

		s += ", .validate = {\n";
		s += JSON::describe(predicate._validate, level + 2 ) + "\n" + tab(level) + "}}";

	} else s += " }";

	s += "}";
	return s;
}

const std::string JSON::describe(const std::vector<JSON::PREDICATE>& predicates, int level) {

	std::string s;
	for ( const JSON::PREDICATE& predicate : predicates )
		s += ( !s.empty() ? ",\n" : "" ) + describe(predicate, level);

	return s;
}


const std::string JSON::describe(const std::vector<JSON::PREDICATE>& predicates) {

	return JSON::describe(predicates, 0);
}

const std::string JSON::describe(const JSON::PREDICATE& predicate) {

	return JSON::describe(predicate, 0);
}
