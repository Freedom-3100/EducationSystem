#pragma once
#include <functional>
#include <iostream>

class CourseId final
{
	int _id;

public:
	CourseId(int id) : _id(id)
	{
	}

	bool operator==(const CourseId& other) const noexcept
	{
		return _id == other._id;
	}

	int get() const noexcept { return _id; }
};

inline std::ostream& operator<<(std::ostream& os, const CourseId& id)
{
	os << id.get();
	return os;
}

namespace std
{
	template <>
	struct hash<CourseId>
	{
		size_t operator()(const CourseId& id) const noexcept
		{
			return std::hash<int>{}(id.get());
		}
	};
}
