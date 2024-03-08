#pragma once
#include <sstream>
#include <string>
#include <map>

#include "json/json.hpp"

struct JSON::ERROR {

	JSON::ERROR_CODE code = JSON::ERROR_CODE::NO_ERROR;
	size_t pos = 0;
	std::pair<size_t, size_t> coords;

	ERROR& operator =(const JSON::ERROR_CODE& ec);
	ERROR& operator =(const JSON::ERROR& other);

	bool operator ==(const JSON::ERROR_CODE& ec) const;
	operator bool() const;

	const size_t row() const;
	const size_t column() const;

        static const std::string describe(const JSON::ERROR_CODE& ec);
	const std::string describe() const;

	friend std::ostream& operator <<(std::ostream& os, const JSON::ERROR& e);
};

class JSON::exception : public std::runtime_error {

	private:
		JSON::ERROR _e;
		std::string _msg;

	public:
		const JSON::ERROR_CODE code() const;
		const size_t pos() const;
		const std::pair<size_t, size_t> coords() const;
		const size_t row() const;
		const size_t column() const;
		const std::string msg() const;
		const std::string describe() const;
		const char * what() const noexcept override;

		bool operator ==(const JSON::ERROR_CODE& ec) const;
		operator bool() const;

		exception(const JSON::ERROR& e, const std::string& msg = "") : std::runtime_error(""), _e(e), _msg(msg) {}
		exception(const JSON::ERROR_CODE& ec, const std::string& msg = "") : std::runtime_error(""), _e(JSON::ERROR { .code = ec }), _msg(msg) {}

		friend std::ostream& operator <<(std::ostream& os, const JSON::exception& e);
};

std::ostream& operator <<(std::ostream& os, const JSON::ERROR& e);
std::ostream& operator <<(std::ostream& os, const JSON::ERROR_CODE& e);
std::ostream& operator <<(std::ostream& os, const JSON::exception& e);
