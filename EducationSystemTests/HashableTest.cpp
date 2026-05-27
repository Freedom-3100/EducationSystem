#include "pch.h"
#include "IdConcept.hpp"
#include "TeachersId.hpp"
#include "CoursesId.hpp"
#include "StudentsId.hpp"

TEST(HashableConcept, StandardTypes)
{
	EXPECT_TRUE(Hashable<int>);
	EXPECT_TRUE(Hashable<std::string>);

	EXPECT_FALSE(Hashable<void>);
}

TEST(HashableConcept, UserDefinedIds)
{
	EXPECT_TRUE(Hashable<CourseId>);
	EXPECT_TRUE(Hashable<StudentId>);
	EXPECT_TRUE(Hashable<TeacherId>);
}

struct NotHashable
{
	int value;
};

TEST(HashableConcept, TypeWithoutHash)
{
	EXPECT_FALSE(Hashable<NotHashable>);
}

struct BrokenHash
{
};

namespace std
{
	template <>
	struct hash<BrokenHash>
	{
	};
}

TEST(HashableConcept, HashWithoutCallOperator)
{
	EXPECT_FALSE(Hashable<BrokenHash>);
}

struct ConvertibleHash
{
};

namespace std
{
	template <>
	struct hash<ConvertibleHash>
	{
		int operator()(const ConvertibleHash&) const
		{
			return 42;
		}
	};
}

TEST(HashableConcept, ConvertibleReturnType)
{
	EXPECT_TRUE(Hashable<ConvertibleHash>);
}
