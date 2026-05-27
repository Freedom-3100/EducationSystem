#pragma once
#include "ISerializer.hpp"
#include <ranges>

class StudentSerializer final : public ISerializer
{
public:
	void read(const ParserDTO& dto, CourseManager& manager) override
	{
		if (dto.field_type != "student")
		{
			return;
		}

		auto student_id = create_id_field(dto.field_id);

		auto vec_it = dto.fields.begin();

		manager.add_student(student_id, create_student_name(vec_it->second));

		++vec_it;

		auto courses = create_courses_links(vec_it->second);

		for (const auto& course : courses)
		{
			manager.add_course(course, std::nullopt);
			manager.add_link(student_id, course);
		}
	}

	void write(std::ostream& output, const CourseManager& manager) const override
	{
		auto preview = manager.get_students_preview();

		for (const auto& val : preview)
		{
			output << "student ";
			output << val.id << " name " << val.name;
			auto& courses = manager.get_courses(val.id);
			output << " courses ";
			for (const auto& course : courses)
			{
				output << course << " ";
			}
			output << "\n";
		}
	}

private:
	static StudentName create_student_name(const std::string& field)
	{
		return StudentName{field};
	}


	static std::vector<CourseId> create_courses_links(const std::string& field)
	{
		std::vector<CourseId> out;

		for (auto token : field | std::views::split(' '))
		{
			if (!token.empty())
			{
				auto course = CourseId{std::stoi(std::string(token.begin(), token.end()))};
				out.push_back(course);
			}
		}
		return out;
	}

	static StudentId create_id_field(const std::string& field)
	{
		auto id = StudentId{std::stoi(field)};
		return id;
	}
};
