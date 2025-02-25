#include "json/json.hpp"
#include "json/json_error.hpp"

JSON::PREDICATE& JSON::PREDICATE::operator =(const JSON::TYPE& type) {

	this -> type = type;
	return *this;
}

bool JSON::PREDICATE::operator ==(const JSON::TYPE& type) const {

	return this -> type == type;
}

bool JSON::PREDICATE::operator ==(const JSON& json) const {

	return this -> type == json.type();
}

bool JSON::PREDICATE::operator !=(const JSON::TYPE& type) const {

	return this -> type != type;
}

bool JSON::PREDICATE::operator !=(const JSON& json) const {

	return this -> type != json.type();
}

void JSON::validate(const std::map<std::string, JSON::PREDICATE>& reqs, const std::string& path) const {

	for ( const auto& p : reqs ) {

		JSON::PREDICATE pred = p.second;

		if ( this -> contains(p.first) && pred != this -> at(p.first))
			throw JSON::exception(JSON::ERROR_CODE::PREDICATE_MISMATCH, path + p.first + " value mismatch, expected type is " +
				JSON::describe(pred.type) + ", found " + JSON::describe(this -> at(p.first).type()));
		else if ( !pred.optional && !this -> contains(p.first))
			throw JSON::exception(JSON::ERROR_CODE::PREDICATE_REQUIRED_MISSING, "required " + path + p.first + " was not found");
	}
}
