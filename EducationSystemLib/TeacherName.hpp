#pragma once
#include <string>

class TeacherName
{
	std::string _name;

public:
	TeacherName(std::string n) : _name(std::move(n))
	{
	}

	const std::string& get() const { return _name; }

	bool operator==(const TeacherName& other) const noexcept
	{
		return _name == other._name;
	}
};
