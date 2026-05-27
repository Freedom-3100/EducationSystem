#include "pch.h"
#include "outputoperators.hpp"
#include "DB.hpp"
#include "StudentsId.hpp"
#include "TeachersId.hpp"
#include "CoursesId.hpp"


using TestValueInt = int;
using TestValueStr = std::string;

static ColumnName ColAge{"age"};
static ColumnName ColName{"name"};


template <typename Id>
class DataBaseTest : public ::testing::Test
{
protected:
	DataBase<Id> db;

	Id make_id(int v)
	{
		return Id{v};
	}
};

using IdTypes = ::testing::Types<
	CourseId,
	StudentId,
	TeacherId
>;


TYPED_TEST_CASE(DataBaseTest, IdTypes);


TYPED_TEST(DataBaseTest, SetAndGetData)
{
	auto id = this->make_id(1);

	Cell<int> age{42};
	Cell<std::string> name{"Alex"};

	EXPECT_TRUE(this->db.set(id, ColAge, age));
	EXPECT_TRUE(this->db.set(id, ColName, name));

	const auto& age_cell = this->db.get_data(id, ColAge);
	const auto& name_cell = this->db.get_data(id, ColName);

	EXPECT_EQ(age_cell.get<int>(), 42);
	EXPECT_EQ(name_cell.get<std::string>(), "Alex");
}

TYPED_TEST(DataBaseTest, SetSameColumnTwiceReturnsFalse)
{
	auto id = this->make_id(1);

	Cell<int> v1{10};
	Cell<int> v2{20};

	EXPECT_TRUE(this->db.set(id, ColAge, v1));
	EXPECT_FALSE(this->db.set(id, ColAge, v2));
}

TYPED_TEST(DataBaseTest, GetListIds)
{
	auto id1 = this->make_id(1);
	auto id2 = this->make_id(2);

	Cell<int> value{100};

	this->db.set(id1, ColAge, value);
	this->db.set(id2, ColAge, value);

	auto ids = this->db.get_list_ids();

	EXPECT_EQ(ids.size(), 2);
	EXPECT_NE(std::find(ids.begin(), ids.end(), id1), ids.end());
	EXPECT_NE(std::find(ids.begin(), ids.end(), id2), ids.end());
}

TYPED_TEST(DataBaseTest, GetCellNames)
{
	auto id = this->make_id(1);

	Cell<int> age{30};
	Cell<std::string> name{"Bob"};

	this->db.set(id, ColAge, age);
	this->db.set(id, ColName, name);

	auto cols = this->db.get_cell_names(id);

	EXPECT_EQ(cols.size(), 2);
	EXPECT_NE(std::find(cols.begin(), cols.end(), ColAge), cols.end());
	EXPECT_NE(std::find(cols.begin(), cols.end(), ColName), cols.end());
}

TYPED_TEST(DataBaseTest, ThrowsOnMissingId)
{
	auto id = this->make_id(42);

	EXPECT_THROW(
		this->db.get_data(id, ColAge),
		std::out_of_range
	);
}
