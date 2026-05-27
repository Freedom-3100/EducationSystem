#pragma once
#include "ParserDTO.hpp"

#include <string>
#include <string_view>
#include <istream>
#include <optional>


class IParser
{
public:
	virtual ~IParser() = default;
	virtual std::optional<ParserDTO> parse_next(std::istream& input) const = 0;
};

class TSVParser final : public IParser
{
	static std::string_view next_token(std::string_view& sv)
	{
		size_t start = sv.find_first_not_of(' ');
		if (start == std::string_view::npos)
		{
			sv = {};
			return {};
		}
		sv.remove_prefix(start);

		size_t space = sv.find(' ');
		std::string_view tok = sv.substr(0, space);

		if (space == std::string_view::npos)
			sv = {};
		else
			sv.remove_prefix(space + 1);

		return tok;
	}

public:
	std::optional<ParserDTO> parse_next(std::istream& input) const override
	{
		std::string line;

		if (!std::getline(input, line))
			return std::nullopt;

		ParserDTO dto;
		std::string_view sv(line);

		auto next = [&]()
		{
			return next_token(sv);
		};

		auto peek = [&]() -> std::string_view
		{
			std::string_view temp = sv;
			return next_token(temp);
		};

		auto is_field_name = [&](std::string_view t)
		{
			return t == "name"
				|| t == "courses"
				|| t == "teacher"
				|| t == "students"
				|| t == "experience";
		};

		std::string_view t = next();
		if (t.empty()) return std::nullopt;
		dto.add_type(std::string(t));

		std::string_view id = next();
		if (id.empty()) return std::nullopt;
		dto.add_id(std::string(id));

		while (!sv.empty())
		{
			std::string_view field = next();
			if (field.empty()) break;

			std::string value_acc;

			while (!sv.empty())
			{
				std::string_view p = peek();
				if (p.empty()) break;

				if (is_field_name(p))
					break;

				next();

				if (!value_acc.empty())
					value_acc += ' ';
				value_acc += p;
			}

			dto.add_field({std::string(field), value_acc});
		}

		return dto;
	}
};
