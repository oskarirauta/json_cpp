#include "json/json.hpp"
#include "json/json_error.hpp"

JSON::PREDICATE& JSON::PREDICATE::operator =(const JSON::TYPE& type) {

	this -> _type = type;
	this -> _required = false;
	this -> _allowed_values.clear();
	return *this;
}

JSON::PREDICATE& JSON::PREDICATE::operator =(const JSON::PREDICATE::PROPERTIES& cfg) {

	this -> _type = cfg.type;
	this -> _required = cfg.required;
	this -> _allowed_values = cfg.allowed_values;;
	return *this;
}

bool JSON::PREDICATE::operator ==(const JSON::TYPE& type) const {

	return this -> _type == type;
}

bool JSON::PREDICATE::operator ==(const JSON& json) const {

	return this -> _type == json.type();
}

bool JSON::PREDICATE::operator !=(const JSON::TYPE& type) const {

	return this -> _type != type;
}

bool JSON::PREDICATE::operator !=(const JSON& json) const {

	return this -> _type != json.type();
}

JSON::PREDICATE& JSON::PREDICATE::require(bool state) {

	this -> _required = state;
	return *this;
}

std::string JSON::PREDICATE::name() const {

	return this -> _name;
}

JSON::TYPE JSON::PREDICATE::type() const {

	return this -> _type;
}

bool JSON::PREDICATE::is_required() const {

	return this -> _required;
}

bool JSON::PREDICATE::is_optional() const {

	return !this -> _required;
}

std::vector<JSON>& JSON::PREDICATE::allowed_values() {

	return this -> _allowed_values;
}

const std::vector<JSON>& JSON::PREDICATE::allowed_values() const {

	return this -> _allowed_values;
}

void JSON::validate(const std::vector<JSON::PREDICATE>& reqs, const std::string& path) const {

	if ( this -> type() == JSON::TYPE::ARRAY )
		throw JSON::exception(JSON::ERROR_CODE::PREDICATE_ARRAY_FAIL);

	for ( const JSON::PREDICATE& p : reqs ) {

		if ( this -> contains(p._name) && p != this -> at(p._name))
			throw JSON::exception(JSON::ERROR_CODE::PREDICATE_MISMATCH, "\"" + path + p._name + "\" value mismatch, expected type is " +
				JSON::describe(p._type) + ", found " + JSON::describe(this -> at(p._name).type()));
		else if ( p._required && !this -> contains(p._name))
			throw JSON::exception(JSON::ERROR_CODE::PREDICATE_REQUIRED_MISSING, "required \"" + path + p._name + "\" was not found");

		if ( this -> contains(p._name) && !p._allowed_values.empty() && ( p._type != JSON::OBJECT && p._type != JSON::ARRAY && p._type != JSON::NULLPTR )) {

			bool matched = false;
			std::string allowed_list;
			size_t idx = 0;

			for ( const JSON& value : p._allowed_values ) {

				if ( value.type() != p._type )
					throw JSON::exception(JSON::ERROR_CODE::PREDICATE_TYPE_FAILURE);

				idx++;
				if ( !allowed_list.empty())
					allowed_list += idx == p._allowed_values.size() ? " and " : ", ";
				allowed_list += "\"" + value.to_string() + "\"";

				if ( p._type == JSON::TYPE::STRING && value.to_string() == this -> at(p._name).to_string())
					matched = true;
				else if ( p._type == JSON::TYPE::FLOAT && value.to_float() == this -> at(p._name).to_float())
					matched = true;
				else if ( p._type == JSON::TYPE::INT && value.to_number() == this -> at(p._name).to_number())
					matched = true;
				else if ( p._type == JSON::TYPE::BOOL && value.to_bool() == this -> at(p._name).to_bool())
					matched = true;
			}

			if ( !matched )
				throw JSON::exception(JSON::ERROR_CODE::PREDICATE_UNALLOWED_VALUE, "\"" + path + p._name + "\" value \"" +
					this -> at(p._name).to_string() + "\" does not match with allowed values: " + allowed_list);
		}
	}
}
