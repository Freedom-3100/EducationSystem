#include "pch.h"

#include <unordered_set>

#include "TeachersId.hpp"

TEST(TeacherIdTest, ConstructorAndGet)
{
	TeacherId id1(123);
	EXPECT_EQ(id1.get(), 123);

	TeacherId id2(456);
	EXPECT_EQ(id2.get(), 456);

	TeacherId id3(0);
	EXPECT_EQ(id3.get(), 0);

	TeacherId id4(-1);
	EXPECT_EQ(id4.get(), -1);
}

TEST(TeacherIdTest, EqualityOperator)
{
	TeacherId id1(123);
	TeacherId id2(123);
	TeacherId id3(456);
	TeacherId id4(123);

	EXPECT_TRUE(id1 == id2);
	EXPECT_TRUE(id1 == id4);

	EXPECT_FALSE(id1 == id3);
	EXPECT_FALSE(id2 == id3);

	EXPECT_TRUE(id1 == id1);
}

TEST(TeacherIdTest, InequalityWithDifferentValues)
{
	TeacherId id1(100);
	TeacherId id2(200);

	EXPECT_FALSE(id1 == id2);
}

TEST(TeacherIdTest, NoexceptSpecification)
{
	TeacherId id1(123);
	TeacherId id2(456);

	EXPECT_NO_THROW(id1.get());
	EXPECT_NO_THROW(id1 == id2);
}

TEST(TeacherIdTest, HashFunction)
{
	TeacherId id1(123);
	TeacherId id2(123);
	TeacherId id3(456);

	std::hash<TeacherId> hasher;


	EXPECT_EQ(hasher(id1), hasher(id2));

	EXPECT_NE(hasher(id1), hasher(id3));

	std::hash<int> intHasher;
	EXPECT_EQ(hasher(id1), intHasher(123));
	EXPECT_EQ(hasher(id3), intHasher(456));
}

TEST(TeacherIdTest, CanBeUsedInUnorderedSet)
{
	std::unordered_set<TeacherId> courseSet;

	TeacherId id1(123);
	TeacherId id2(456);
	TeacherId id3(123);

	courseSet.insert(id1);
	courseSet.insert(id2);
	courseSet.insert(id3);

	EXPECT_EQ(courseSet.size(), 2);

	EXPECT_TRUE(courseSet.find(id1) != courseSet.end());
	EXPECT_TRUE(courseSet.find(id2) != courseSet.end());
	EXPECT_TRUE(courseSet.find(id3) != courseSet.end());
	EXPECT_TRUE(courseSet.find(TeacherId(999)) == courseSet.end());
}

TEST(TeacherIdTest, OutputStreamOperator)
{
	TeacherId id1(123);
	TeacherId id2(-456);
	TeacherId id3(0);

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

TEST(TeacherIdTest, ConstCorrectness)
{
	const TeacherId id1(123);
	const TeacherId id2(123);
	TeacherId id3(456);

	EXPECT_EQ(id1.get(), 123);

	EXPECT_TRUE(id1 == id1);
	EXPECT_TRUE(id1 == id2);
	EXPECT_TRUE(id2 == id1);

	TeacherId id4(123);
	EXPECT_TRUE(id1 == id4);
	EXPECT_TRUE(id4 == id1);

	EXPECT_FALSE(id1 == id3);
	EXPECT_FALSE(id3 == id1);

	const TeacherId id5(999);
	EXPECT_FALSE(id1 == id5);
	EXPECT_FALSE(id5 == id1);
}

TEST(TeacherIdTest, FinalClassCannotBeInherited)
{
	TeacherId id(123);
	EXPECT_EQ(id.get(), 123);
}

TEST(TeacherIdTest, HashIsNoexcept)
{
	TeacherId id(123);
	std::hash<TeacherId> hasher;

	EXPECT_NO_THROW(hasher(id));
}

TEST(TeacherIdTest, MultipleInstancesSameValue)
{
	TeacherId id1(100);
	TeacherId id2(100);
	TeacherId id3(100);

	EXPECT_TRUE(id1 == id2);
	EXPECT_TRUE(id2 == id3);
	EXPECT_TRUE(id1 == id3);

	std::hash<TeacherId> hasher;
	EXPECT_EQ(hasher(id1), hasher(id2));
	EXPECT_EQ(hasher(id2), hasher(id3));
}
