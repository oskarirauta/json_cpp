#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include "json.hpp"


class json_dumper {

	private:
		int level = 0;
		int tab_size = 2;

		const std::string tab(int level) {
			return level < 1 ? "" : std::string(this -> tab_size * level, ' ');
		}

	public:

	JSON& json;
	bool pretty = true;
	std::string out;

	void iterate(JSON::fe_iterator it, JSON& j) {

		if ( j == JSON::TYPE::OBJECT && j.empty()) { out += "{}"; return; }
		else if ( j == JSON::TYPE::ARRAY && j.empty()) { out += "[]"; return; }

		if ( out.back() != '[' && out.back() != '{' )
			out += ',';

		if ( this -> pretty ) {
			out += "\n";
			out += tab(this -> level + 1);
		}

		if ( it.named())
			out += "\"" + JSON::escape(it.name()) + "\":" + ( pretty ? " " : "" );

		if ( j == JSON::TYPE::OBJECT || j == JSON::TYPE::ARRAY ) {
			out += j == JSON::TYPE::OBJECT ? "{" : "[";
			level++;
			j.for_each([this](JSON::fe_iterator& it2, JSON& j2) {
				this -> iterate(it2, j2);
			});
			level--;
			out += ( pretty ? ( "\n" + tab(this -> level + 1)) : "" );
			out += j == JSON::TYPE::OBJECT ? "}" : "]";
		} else if ( j == JSON::TYPE::STRING )
			out += "\"" + JSON::escape(j.to_string()) + "\"";
		else out += j.to_string();
	}

	std::string dump() {

		this -> level = 0;

		if ( json == JSON::TYPE::OBJECT && json.empty()) out = "{}";
		else if ( json == JSON::TYPE::ARRAY && json.empty()) out = "[]";
		else if ( json == JSON::TYPE::OBJECT || json == JSON::TYPE::ARRAY ) {
			out = json == JSON::TYPE::OBJECT ? "{" : "[";
			json.for_each([this](JSON::fe_iterator& it, JSON& j) {
				this -> iterate(it, j);
			});
			if ( pretty ) out += json == JSON::TYPE::OBJECT ? "\n}" : "\n]";
			else out += json == JSON::TYPE::OBJECT ? "}" : "]";
		} else out = json.to_string();

		return out;
	}

	json_dumper(JSON& json, bool pretty = true, int tab_size = 2) : level(0), tab_size(tab_size), json(json), pretty(pretty), out("") {}

};

void dump(JSON& j) {

	std::string out;

	if ( j == JSON::TYPE::OBJECT && j.empty()) out = "{}";
	else if ( j == JSON::TYPE::ARRAY && j.empty()) out = "[]";
	else if ( j == JSON::TYPE::OBJECT || j == JSON::TYPE::ARRAY ) {
		json_dumper d(j);
		out = d.dump();
        } else out = j.to_string();

	std::cout << /*"dump:\n" <<*/ out << std::endl;
}

int main(int argc, char **argv) {

	std::ifstream ifd("./sample", std::ios::in);

	if ( !ifd.is_open() || !ifd.good() || ifd.bad()) {

		if ( ifd.is_open())
			ifd.close();

		std::cerr << "abort, failed to open file \"sample\"" << std::endl;
		return 1;
	}

	std::stringstream ss;
	std::string blob;

	ss << ifd.rdbuf();
	blob = ss.str();
	ifd.close();

        JSON::ERROR ec;

	try {
		JSON json = JSON::parse(blob);
		dump(json);

	} catch ( const JSON::exception& e ) {
		std::cout << "ERROR: " << e.what() << std::endl;
	}

	return 0;
}
