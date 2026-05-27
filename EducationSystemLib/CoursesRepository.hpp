#pragma once
#include <memory>
#include <unordered_set>

#include "CoursePreview.hpp"
#include "DB.hpp"
#include "IRepository.hpp"


class CourseRepository final : public ICourseRepository
{
	std::shared_ptr<DataBase<CourseId>> _db;

public:
	explicit CourseRepository(std::shared_ptr<DataBase<CourseId>> db)
		: _db(std::move(db))
	{
	}


	const std::optional<CoursesName>& add_name(const CourseId& course_id,
	                                           const std::optional<CoursesName>& courses_name) override
	{
		auto& name = _db->get_or_create<std::optional<CoursesName>>(course_id, ColumnName{"name"});
		if (!name)
		{
			name = courses_name;
		}
		return name;
	}

	bool add_student(const CourseId& course_id,
	                 const StudentId& student_id) override
	{
		auto& students = _db->get_or_create<std::unordered_set<StudentId>>(course_id, ColumnName{"students"});

		return students.insert(student_id).second;
	}

	bool add_teacher(const CourseId& course_id, const TeacherId& teacher_id) override
	{
		auto& teacher = _db->get_or_create<std::optional<TeacherId>>(course_id, ColumnName{"teacher"});

		const bool changed = (!teacher || *teacher != teacher_id);
		teacher = teacher_id;
		return changed;
	}

	const std::optional<CoursesName>& get_name(const CourseId& course_id) const override
	{
		return _db->get_data(course_id, ColumnName{"name"})
		          .get<std::optional<CoursesName>>();
	}


	const std::unordered_set<StudentId>&
	get_students(const CourseId& course_id) const override
	{
		return _db->get_data(course_id, ColumnName{"students"})
		          .get<std::unordered_set<StudentId>>();
	}

	const std::optional<TeacherId>&
	get_teacher(const CourseId& course_id) const override
	{
		return _db->get_or_create<std::optional<TeacherId>>(course_id, ColumnName{"teacher"});
	}


	bool remove_student(const CourseId& course_id,
	                    const StudentId& student_id) override
	{
		auto& students =
			_db->get_data(course_id, ColumnName{"students"})
			   .get<std::unordered_set<StudentId>>();

		return students.erase(student_id) > 0;
	}


	bool remove_teacher(const CourseId& course_id,
	                    const TeacherId& teacher_id) override
	{
		auto& teacher =
			_db->get_data(course_id, ColumnName{"teacher"})
			   .get<std::optional<TeacherId>>();

		if (!teacher || *teacher != teacher_id)
			return false;

		teacher = std::nullopt;
		return true;
	}


	std::vector<CoursePreview> get_preview() const override
	{
		std::vector<CoursePreview> out;

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
