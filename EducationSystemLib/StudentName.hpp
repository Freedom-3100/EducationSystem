#pragma once
#include <string>

class StudentName
{
	std::string _name;

public:
	StudentName(std::string n) : _name(std::move(n))
	{
	}

	const std::string& get() const { return _name; }

	bool operator==(const StudentName& other) const noexcept
	{
		return _name == other._name;
	}
};
