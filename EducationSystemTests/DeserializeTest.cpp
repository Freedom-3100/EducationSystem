#include "pch.h"
#include "outputoperators.hpp"
#include "SerialazerFabric.hpp"
#include "CoursesDBSerializer.hpp"
#include "StudentsDBSerializer.hpp"
#include "TeachersDBSerializer.hpp"
#include "Parser.hpp"
#include <fstream>

TEST(SerializerFabric, LoadFromFile)
{
	std::ofstream file("test_data.txt");
	ASSERT_TRUE(file.is_open());

	file << "student 1 name Misha courses 100 101 102\n";
	file << "teacher 10 name Petrov experience 10 courses 100 101 102\n";
	file << "course 100 name Math teacher 10 students 1\n";
	file << "course 101 name Russian teacher 10 students 1\n";
	file << "course 102 name Geometry teacher 10 students 1\n";

	file.close();

	std::ifstream input("test_data.txt");
	ASSERT_TRUE(input.is_open());

	auto students = std::make_shared<DataBase<StudentId>>();
	auto teachers = std::make_shared<DataBase<TeacherId>>();
	auto courses = std::make_shared<DataBase<CourseId>>();


	auto coursesRep = std::make_shared<CourseRepository>(courses);
	auto teachersRep = std::make_shared<TeacherRepository>(teachers);
	auto studentsRep = std::make_shared<StudentRepository>(students);


	CourseManager manager(coursesRep, studentsRep, teachersRep);

	std::vector<std::unique_ptr<ISerializer>> serializers;
	serializers.push_back(std::make_unique<StudentSerializer>());
	serializers.push_back(std::make_unique<TeachersSerializer>());
	serializers.push_back(std::make_unique<CoursesSerializer>());

	TSVParser parser;

	SerializerFabric::serialize(input, serializers, parser, manager);

	{
		const auto& name = manager.get_name(StudentId{1});
		ASSERT_TRUE(name.has_value());
		EXPECT_EQ(name.value().get(), "Misha");

		const auto& coursesSet = studentsRep->get_courses(StudentId{1});
		EXPECT_EQ(coursesSet.size(), 3u);
		EXPECT_TRUE(coursesSet.contains(CourseId{ 100 }));
		EXPECT_TRUE(coursesSet.contains(CourseId{ 101 }));
		EXPECT_TRUE(coursesSet.contains(CourseId{ 102 }));
	}
	{
		const auto& name = manager.get_name(TeacherId{10});
		ASSERT_TRUE(name.has_value());
		EXPECT_EQ(name.value().get(), "Petrov");

		const auto& experience = manager.get_experience(TeacherId{10});
		ASSERT_TRUE(experience.has_value());
		EXPECT_EQ(*experience, 10);

		const auto& coursesSet = teachersRep->get_courses(TeacherId{10});
		EXPECT_EQ(coursesSet.size(), 3u);
		EXPECT_TRUE(coursesSet.contains(CourseId{ 100 }));
		EXPECT_TRUE(coursesSet.contains(CourseId{ 101 }));
		EXPECT_TRUE(coursesSet.contains(CourseId{ 102 }));
	}

	{
		const auto& name100 = manager.get_name(CourseId{100});
		ASSERT_TRUE(name100.has_value());
		EXPECT_EQ(name100.value().get(), "Math");

		const auto& name101 = manager.get_name(CourseId{101});
		ASSERT_TRUE(name101.has_value());
		EXPECT_EQ(name101.value().get(), "Russian");

		const auto& name102 = manager.get_name(CourseId{102});
		ASSERT_TRUE(name102.has_value());
		EXPECT_EQ(name102.value().get(), "Geometry");

		for (CourseId cid : {CourseId{100}, CourseId{101}, CourseId{102}})
		{
			const auto& teacher = coursesRep->get_teacher(cid);
			ASSERT_TRUE(teacher.has_value());
			EXPECT_EQ(teacher.value().get(), TeacherId{ 10 });

			const auto& studentsSet = coursesRep->get_students(cid);
			EXPECT_EQ(studentsSet.size(), 1u);
			EXPECT_TRUE(studentsSet.contains(StudentId{ 1 }));
		}
	}

	std::ofstream write_file("data.txt", std::ios::out | std::ios::trunc);
	SerializerFabric::deserialize(write_file, serializers, manager);
}
