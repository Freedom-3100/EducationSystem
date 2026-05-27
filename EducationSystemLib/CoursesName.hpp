#pragma once
#include <string>

class CoursesName
{
	std::string _name;

public:
	CoursesName(std::string n) : _name(std::move(n))
	{
	}

	const std::string& get() const { return _name; }

	bool operator==(const CoursesName& other) const noexcept
	{
		return _name == other._name;
	}
};
