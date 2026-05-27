#pragma once
#include <unordered_set>

#include "IRepository.hpp"
#include "DB.hpp"
#include "StudentPreview.hpp"


class StudentRepository final : public IStudentRepository
{
	std::shared_ptr<DataBase<StudentId>> _db;

public:
	explicit StudentRepository(std::shared_ptr<DataBase<StudentId>> db)
		: _db(std::move(db))
	{
	}


	const std::optional<StudentName>& add_name(const StudentId& student_id,
	                                           const std::optional<StudentName>& student_name) override
	{
		auto& name = _db->get_or_create<std::optional<StudentName>>(
			student_id,
			ColumnName{"name"}
		);

		if (!name)
		{
			name = student_name;
		}
		return name;
	}

	const std::optional<StudentName>& get_name(const StudentId& student_id) const override
	{
		return _db->get_data(student_id, ColumnName{"name"})
		          .get<std::optional<StudentName>>();
	}


	const std::unordered_set<CourseId>& get_courses(const StudentId& student_id) const override
	{
		return _db->get_data(student_id, ColumnName{"courses"})
		          .get<std::unordered_set<CourseId>>();
	}

	bool add_course(const StudentId& student, const CourseId& course) override
	{
		auto& courses = _db->get_or_create<std::unordered_set<CourseId>>(student, ColumnName{"courses"});
		return courses.insert(course).second;
	}

	bool remove_course(const StudentId& student, const CourseId& course) override
	{
		auto& courses =
			_db->get_data(student, ColumnName{"courses"})
			   .get<std::unordered_set<CourseId>>();

		return courses.erase(course);
	}


	std::vector<StudentPreview> get_preview() const override
	{
		std::vector<StudentPreview> out;

		for (const auto& id : _db->get_list_ids())
		{
			auto& name = get_name(id);
			if (name.has_value())
			{
				out.push_back({id, name.value()});
			}
		}

		return out;
	}
};
