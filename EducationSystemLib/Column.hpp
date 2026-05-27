#pragma once
#include <memory>
#include <optional>
#include <string>
#include <functional>
#include <unordered_set>


class ColumnName
{
	std::string _name;

public:
	ColumnName(std::string n) : _name(std::move(n))
	{
	}

	const std::string& get() const { return _name; }

	bool operator==(const ColumnName& other) const noexcept
	{
		return _name == other._name;
	}
};

namespace std
{
	template <>
	struct hash<ColumnName>
	{
		size_t operator()(const ColumnName& name) const noexcept
		{
			return std::hash<std::string>{}(name.get());
		}
	};
}
