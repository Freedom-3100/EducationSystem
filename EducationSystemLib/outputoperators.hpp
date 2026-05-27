#pragma once
#include <iostream>
#include <optional>
#include <unordered_set>

#include "CoursesName.hpp"
#include "StudentName.hpp"
#include "TeacherName.hpp"

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
	for (const auto& val : vec)
	{
		os << val;
	}
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& opt)
{
	if (opt.has_value())
	{
		os << *opt;
	}
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& set)
{
	bool first = true;
	for (const auto& val : set)
	{
		if (!first) os << " ";
		first = false;
		os << val;
	}
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const TeacherName& name)
{
	os << name.get();
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const StudentName& name)
{
	os << name.get();
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const CoursesName& name)
{
	os << name.get();
	return os;
}
