#include "pch.h"

#include <unordered_set>

#include "StudentsId.hpp"

TEST(StudentsIdTest, ConstructorAndGet)
{
	StudentId id1(123);
	EXPECT_EQ(id1.get(), 123);

	StudentId id2(456);
	EXPECT_EQ(id2.get(), 456);

	StudentId id3(0);
	EXPECT_EQ(id3.get(), 0);

	StudentId id4(-1);
	EXPECT_EQ(id4.get(), -1);
}

TEST(StudentIdTest, EqualityOperator)
{
	StudentId id1(123);
	StudentId id2(123);
	StudentId id3(456);
	StudentId id4(123);

	EXPECT_TRUE(id1 == id2);
	EXPECT_TRUE(id1 == id4);

	EXPECT_FALSE(id1 == id3);
	EXPECT_FALSE(id2 == id3);

	EXPECT_TRUE(id1 == id1);
}

TEST(StudentIdTest, InequalityWithDifferentValues)
{
	StudentId id1(100);
	StudentId id2(200);

	EXPECT_FALSE(id1 == id2);
}

TEST(StudentIdTest, NoexceptSpecification)
{
	StudentId id1(123);
	StudentId id2(456);

	EXPECT_NO_THROW(id1.get());
	EXPECT_NO_THROW(id1 == id2);
}

TEST(StudentIdTest, HashFunction)
{
	StudentId id1(123);
	StudentId id2(123);
	StudentId id3(456);

	std::hash<StudentId> hasher;


	EXPECT_EQ(hasher(id1), hasher(id2));

	EXPECT_NE(hasher(id1), hasher(id3));

	std::hash<int> intHasher;
	EXPECT_EQ(hasher(id1), intHasher(123));
	EXPECT_EQ(hasher(id3), intHasher(456));
}

TEST(StudentIdTest, CanBeUsedInUnorderedSet)
{
	std::unordered_set<StudentId> courseSet;

	StudentId id1(123);
	StudentId id2(456);
	StudentId id3(123);

	courseSet.insert(id1);
	courseSet.insert(id2);
	courseSet.insert(id3);

	EXPECT_EQ(courseSet.size(), 2);

	EXPECT_TRUE(courseSet.find(id1) != courseSet.end());
	EXPECT_TRUE(courseSet.find(id2) != courseSet.end());
	EXPECT_TRUE(courseSet.find(id3) != courseSet.end());
	EXPECT_TRUE(courseSet.find(StudentId(999)) == courseSet.end());
}

TEST(StudentIdTest, OutputStreamOperator)
{
	StudentId id1(123);
	StudentId id2(-456);
	StudentId id3(0);

	std::stringstream ss1;
	ss1 << id1;
	EXPECT_EQ(ss1.str(), "123");

	std::stringstream ss2;
	ss2 << id2;
	EXPECT_EQ(ss2.str(), "-456");

	std::stringstream ss3;
	ss3 << id3;
	EXPECT_EQ(ss3.str(), "0");

	std::stringstream ss4;
	ss4 << "Course ID: " << id1 << ", Another: " << id2;
	EXPECT_EQ(ss4.str(), "Course ID: 123, Another: -456");
}

TEST(StudentIdTest, ConstCorrectness)
{
	const StudentId id1(123);
	const StudentId id2(123);
	StudentId id3(456);

	EXPECT_EQ(id1.get(), 123);

	EXPECT_TRUE(id1 == id1);
	EXPECT_TRUE(id1 == id2);
	EXPECT_TRUE(id2 == id1);

	StudentId id4(123);
	EXPECT_TRUE(id1 == id4);
	EXPECT_TRUE(id4 == id1);

	EXPECT_FALSE(id1 == id3);
	EXPECT_FALSE(id3 == id1);

	const StudentId id5(999);
	EXPECT_FALSE(id1 == id5);
	EXPECT_FALSE(id5 == id1);
}

TEST(StudentIdTest, FinalClassCannotBeInherited)
{
	StudentId id(123);
	EXPECT_EQ(id.get(), 123);
}

TEST(StudentIdTest, HashIsNoexcept)
{
	StudentId id(123);
	std::hash<StudentId> hasher;

	EXPECT_NO_THROW(hasher(id));
}

TEST(StudentIdTest, MultipleInstancesSameValue)
{
	StudentId id1(100);
	StudentId id2(100);
	StudentId id3(100);

	EXPECT_TRUE(id1 == id2);
	EXPECT_TRUE(id2 == id3);
	EXPECT_TRUE(id1 == id3);

	std::hash<StudentId> hasher;
	EXPECT_EQ(hasher(id1), hasher(id2));
	EXPECT_EQ(hasher(id2), hasher(id3));
}
