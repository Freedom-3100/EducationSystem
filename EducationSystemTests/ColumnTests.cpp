#include "pch.h"
#include "outputoperators.hpp"
#include <unordered_set>

#include "Cell.hpp"
#include "Column.hpp"


TEST(ColumnNameTest, ConstructorAndGet)
{
	ColumnName name1("id");
	EXPECT_EQ(name1.get(), "id");

	ColumnName name2("name");
	EXPECT_EQ(name2.get(), "name");

	ColumnName name3("");
	EXPECT_EQ(name3.get(), "");

	std::string original = "age";
	ColumnName name4(std::move(original));
	EXPECT_EQ(name4.get(), "age");
	EXPECT_TRUE(original.empty());
}


TEST(ColumnNameTest, EqualityOperator)
{
	ColumnName name1("id");
	ColumnName name2("id");
	ColumnName name3("name");
	ColumnName name4("ID");

	EXPECT_TRUE(name1 == name2);
	EXPECT_FALSE(name1 == name3);
	EXPECT_FALSE(name1 == name4);
	EXPECT_TRUE(name1 == name1);
}

TEST(ColumnNameTest, HashFunction)
{
	ColumnName name1("id");
	ColumnName name2("id");
	ColumnName name3("name");

	std::hash<ColumnName> hasher;

	EXPECT_EQ(hasher(name1), hasher(name2));
	EXPECT_NE(hasher(name1), hasher(name3));

	std::hash<std::string> stringHasher;
	EXPECT_EQ(hasher(name1), stringHasher("id"));
}

TEST(ColumnNameTest, UseInUnorderedSet)
{
	std::unordered_set<ColumnName> columnSet;

	ColumnName name1("id");
	ColumnName name2("name");
	ColumnName name3("id");

	columnSet.insert(name1);
	columnSet.insert(name2);
	columnSet.insert(name3);

	EXPECT_EQ(columnSet.size(), 2);
	EXPECT_TRUE(columnSet.find(ColumnName("id")) != columnSet.end());
	EXPECT_TRUE(columnSet.find(ColumnName("name")) != columnSet.end());
	EXPECT_TRUE(columnSet.find(ColumnName("age")) == columnSet.end());
}

TEST(ColumnTest, IntColumnCreation)
{
	Cell<int> intColumn(42);
	EXPECT_EQ(intColumn.value, 42);

	std::unique_ptr<ICell> column = std::make_unique<Cell<int>>(100);
	EXPECT_EQ(column->get<int>(), 100);
}

TEST(ColumnTest, StringColumnCreation)
{
	Cell<std::string> stringColumn("hello");
	EXPECT_EQ(stringColumn.value, "hello");

	std::unique_ptr<ICell> column = std::make_unique<Cell<std::string>>("world");
	EXPECT_EQ(column->get<std::string>(), "world");
}

TEST(ColumnTest, DoubleColumnCreation)
{
	Cell<double> doubleColumn(3.14);
	EXPECT_DOUBLE_EQ(doubleColumn.value, 3.14);

	std::unique_ptr<ICell> column = std::make_unique<Cell<double>>(2.71);
	EXPECT_DOUBLE_EQ(column->get<double>(), 2.71);
}

TEST(ColumnTest, BoolColumnCreation)
{
	Cell<bool> boolColumn(true);
	EXPECT_EQ(boolColumn.value, true);

	std::unique_ptr<ICell> column = std::make_unique<Cell<bool>>(false);
	EXPECT_EQ(column->get<bool>(), false);
}


TEST(ColumnTest, CloneMethod)
{
	auto original = std::make_unique<Cell<int>>(42);
	auto clone = original->clone();

	EXPECT_NE(original.get(), clone.get());
	EXPECT_EQ(original->get<int>(), clone->get<int>());
	EXPECT_EQ(typeid(original->type()), typeid(clone->type()));
}

TEST(ColumnTest, PrintMethod)
{
	{
		std::stringstream ss;
		Cell<int> intColumn(123);
		intColumn.print(ss);
		EXPECT_EQ(ss.str(), "123");
	}

	{
		std::stringstream ss;
		Cell<std::string> stringColumn("test");
		stringColumn.print(ss);
		EXPECT_EQ(ss.str(), "test");
	}

	{
		std::stringstream ss;
		Cell<double> doubleColumn(3.14);
		doubleColumn.print(ss);
		EXPECT_TRUE(ss.str().find("3.14") != std::string::npos);
	}
}

TEST(ColumnTest, TypeMethod)
{
	Cell<int> intColumn(0);
	EXPECT_EQ(intColumn.type(), typeid(int));

	Cell<std::string> stringColumn("");
	EXPECT_EQ(stringColumn.type(), typeid(std::string));

	Cell<double> doubleColumn(0.0);
	EXPECT_EQ(doubleColumn.type(), typeid(double));

	Cell<bool> boolColumn(true);
	EXPECT_EQ(boolColumn.type(), typeid(bool));
}

TEST(ColumnTest, TemplateGetMethod)
{
	auto column = std::make_unique<Cell<int>>(42);

	EXPECT_EQ(column->get<int>(), 42);

	EXPECT_THROW({
	             try {
	             column->get<std::string>();
	             }
	             catch (const std::bad_cast&) {
	             throw;
	             }
	             }, std::bad_cast);
}

TEST(ColumnTest, TemplateTryGetMethod)
{
	auto intColumn = std::make_unique<Cell<int>>(42);
	auto stringColumn = std::make_unique<Cell<std::string>>("hello");

	auto intOpt = intColumn->try_get<int>();
	EXPECT_TRUE(intOpt.has_value());
	EXPECT_EQ(intOpt.value().get(), 42);

	auto stringOpt = stringColumn->try_get<std::string>();
	EXPECT_TRUE(stringOpt.has_value());
	EXPECT_EQ(stringOpt.value().get(), "hello");

	auto wrongOpt = intColumn->try_get<std::string>();
	EXPECT_FALSE(wrongOpt.has_value());

	auto wrongOpt2 = stringColumn->try_get<int>();
	EXPECT_FALSE(wrongOpt2.has_value());
}

TEST(ColumnTest, PolymorphicUsage)
{
	std::vector<std::unique_ptr<ICell>> columns;

	columns.push_back(std::make_unique<Cell<int>>(1));
	columns.push_back(std::make_unique<Cell<std::string>>("test"));
	columns.push_back(std::make_unique<Cell<double>>(3.14));

	EXPECT_EQ(columns.size(), 3);

	EXPECT_EQ(columns[0]->type(), typeid(int));
	EXPECT_EQ(columns[1]->type(), typeid(std::string));
	EXPECT_EQ(columns[2]->type(), typeid(double));

	auto intOpt = columns[0]->try_get<int>();
	EXPECT_TRUE(intOpt.has_value());
	EXPECT_EQ(intOpt.value().get(), 1);

	auto stringOpt = columns[1]->try_get<std::string>();
	EXPECT_TRUE(stringOpt.has_value());
	EXPECT_EQ(stringOpt.value().get(), "test");

	auto wrongOpt = columns[0]->try_get<std::string>();
	EXPECT_FALSE(wrongOpt.has_value());
}

TEST(ColumnTest, CopyAndMoveSemantics)
{
	auto original = std::make_unique<Cell<std::string>>("original");
	auto copy = original->clone();

	EXPECT_NE(original.get(), copy.get());
	EXPECT_EQ(original->get<std::string>(), copy->get<std::string>());

	const_cast<std::string&>(original->get<std::string>()) = "modified";
	EXPECT_EQ(original->get<std::string>(), "modified");
	EXPECT_EQ(copy->get<std::string>(), "original");
}


TEST(ColumnTest, EdgeCases)
{
	Cell<std::string> emptyStringColumn("");
	EXPECT_EQ(emptyStringColumn.value, "");

	Cell<int> negativeColumn(-100);
	EXPECT_EQ(negativeColumn.value, -100);

	Cell<int> zeroColumn(0);
	EXPECT_EQ(zeroColumn.value, 0);

	Cell<int64_t> largeColumn(std::numeric_limits<int64_t>::max());
	EXPECT_EQ(largeColumn.value, std::numeric_limits<int64_t>::max());
}

TEST(ColumnTest, ComparisonWithSameTypeDifferentValues)
{
	Cell<int> col1(10);
	Cell<int> col2(20);

	EXPECT_NE(&col1, &col2);
}


TEST(ColumnTest, TypeSafety)
{
	auto intColumn = std::make_unique<Cell<int>>(42);

	auto opt1 = intColumn->try_get<double>();
	EXPECT_FALSE(opt1.has_value());

	auto opt2 = intColumn->try_get<std::string>();
	EXPECT_FALSE(opt2.has_value());

	auto opt3 = intColumn->try_get<bool>();
	EXPECT_FALSE(opt3.has_value());

	EXPECT_THROW(intColumn->get<std::string>(), std::bad_cast);
	EXPECT_THROW(intColumn->get<double>(), std::bad_cast);
}
