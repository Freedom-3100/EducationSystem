#pragma once
#include <string>
#include <vector>

struct ParserDTO
{
	std::string field_type;
	std::string field_id;
	std::vector<std::pair<std::string, std::string>> fields;

	void add_type(std::string field)
	{
		field_type = std::move(field);
	}

	void add_id(std::string field)
	{
		field_id = std::move(field);
	}

	void add_field(const std::pair<std::string, std::string>& fields_pair)
	{
		fields.push_back(fields_pair);
	}
};
