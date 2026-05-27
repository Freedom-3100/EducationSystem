#pragma once
#include "ISerializer.hpp"
#include <ranges>

class TeachersSerializer final : public ISerializer
{
public:
	void read(const ParserDTO& dto, CourseManager& manager) override
	{
		if (dto.field_type != "teacher")
		{
			return;
		}

		auto id = create_id_field(dto.field_id);

		auto fields_pair = dto.fields.begin();
		auto name = create_teacher_name(fields_pair->second);


		++fields_pair;
		auto experience = create_experience(fields_pair->second);

		manager.add_teacher(id, name, experience);

		++fields_pair;

		auto links = create_courses_links(fields_pair->second);

		for (auto& course : links)
		{
			manager.add_course(course, std::nullopt);
			manager.add_link(id, course);
		}
	}

	void write(std::ostream& output, const CourseManager& manager) const override
	{
		auto preview = manager.get_teacher_preview();

		for (const auto& val : preview)
		{
			output << "teacher ";
			output << val.id << " name " << val.name;
			auto& exp = manager.get_experience(val.id);

			output << " experience " << exp.value();
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
	static TeacherName create_teacher_name(const std::string& field)
	{
		return TeacherName{field};
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

	static TeacherId create_id_field(const std::string& field)
	{
		return TeacherId{std::stoi(field)};
	}

	static int create_experience(const std::string& field)
	{
		return std::stoi(field);
	}
};
