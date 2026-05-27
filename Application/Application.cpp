#include "outputoperators.hpp"
#include "CourseManager.hpp"
#include "CoursesDBSerializer.hpp"
#include "TeachersDBSerializer.hpp"
#include "StudentsDBSerializer.hpp"

#include <iostream>
#include <fstream>

#include "SerialazerFabric.hpp"


int main(int argc, char* argv[])
{
	std::string inputFile;
	std::string outputFile;

	std::cout << "Enter input file: ";
	std::cin >> inputFile;

	std::cout << "Enter output file: ";
	std::cin >> outputFile;

	std::ifstream input(inputFile);
	if (!input.is_open())
	{
		std::cerr << "Error: cannot open input file\n";
		return 1;
	}

	std::ofstream output(outputFile);
	if (!output.is_open())
	{
		std::cerr << "Error: cannot open output file\n";
		return 1;
	}

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

	SerializerFabric::deserialize(output, serializers, manager);

	std::cout << "Done.\n";
	return 0;
}
