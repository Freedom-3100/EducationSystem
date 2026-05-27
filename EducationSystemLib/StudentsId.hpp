#pragma once


class StudentId final
{
	int _id;

public:
	StudentId(const int& id) : _id(id)
	{
	}

	bool operator==(const StudentId& other) const noexcept
	{
		return _id == other.get();
	}

	const int& get() const { return _id; }
};

inline std::ostream& operator<<(std::ostream& os, const StudentId& id)
{
	os << id.get();
	return os;
}

namespace std
{
	template <>
	struct hash<StudentId>
	{
		size_t operator()(const StudentId& id) const noexcept
		{
			return hash<int>{}(id.get());
		}
	};
}
