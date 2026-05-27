#pragma once
#include <ranges>

#include "ISerializer.hpp"

class CoursesSerializer final : public ISerializer
{
public:
	void read(const ParserDTO& dto, CourseManager& manager) override
	{
		if (dto.field_type != "course")
		{
			return;
		}

		auto id = create_id_field(dto.field_id);

		auto fields_pair = dto.fields.begin();
		auto name = create_course_name(fields_pair->second);

		manager.add_course(id, name);

		++fields_pair;

		auto teacher = create_teacher_id_field(fields_pair->second);

		manager.add_teacher(teacher, std::nullopt, std::nullopt);
		manager.add_link(teacher, id);

		++fields_pair;

		auto links = create_courses_links(fields_pair->second);

		for (const auto& student : links)
		{
			manager.add_student(student, std::nullopt);
			manager.add_link(student, id);
		}
	}

	void write(std::ostream& output, const CourseManager& manager) const override
	{
		auto preview = manager.get_course_preview();

		for (const auto& val : preview)
		{
			output << "course ";
			output << val.id << " name " << val.name;
			auto& students = manager.get_students(val.id);
			auto& teacher = manager.get_teacher(val.id);
			output << " teacher " << teacher.value();
			output << " students ";
			for (const auto& student : students)
			{
				output << student << " ";
			}
			output << "\n";
		}
	}

private:
	static CoursesName create_course_name(const std::string& field)
	{
		return CoursesName{field};
	}


	static std::vector<StudentId> create_courses_links(const std::string& field)
	{
		std::vector<StudentId> out;

		for (auto token : field | std::views::split(' '))
		{
			if (!token.empty())
			{
				auto course = StudentId{std::stoi(std::string(token.begin(), token.end()))};
				out.push_back(course);
			}
		}
		return out;
	}

	static CourseId create_id_field(const std::string& field)
	{
		return CourseId{std::stoi(field)};
	}

	static TeacherId create_teacher_id_field(const std::string& field)
	{
		return TeacherId{std::stoi(field)};
	}
};
