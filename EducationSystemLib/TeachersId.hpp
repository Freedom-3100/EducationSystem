#pragma once


class TeacherId final
{
	int _id;

public:
	TeacherId(const int& id) : _id(id)
	{
	}

	bool operator==(const TeacherId& other) const noexcept
	{
		return _id == other.get();
	}

	const int& get() const { return _id; }
};

inline std::ostream& operator<<(std::ostream& os, const TeacherId& id)
{
	os << id.get();
	return os;
}

namespace std
{
	template <>
	struct hash<TeacherId>
	{
		size_t operator()(const TeacherId& id) const noexcept
		{
			return hash<int>{}(id.get());
		}
	};
}
