#pragma once
#include <memory>
#include <unordered_set>

#include "DB.hpp"
#include "IRepository.hpp"
#include "TeacherPreview.hpp"


class TeacherRepository final : public ITeacherRepository
{
	std::shared_ptr<DataBase<TeacherId>> _db;

public:
	explicit TeacherRepository(std::shared_ptr<DataBase<TeacherId>> db)
		: _db(std::move(db))
	{
	}


	const std::optional<TeacherName>& add_name(const TeacherId& teacher_id,
	                                           const std::optional<TeacherName>& teacher_name) override
	{
		auto& name = _db->get_or_create<std::optional<TeacherName>>(teacher_id, ColumnName{"name"});
		if (!name)
		{
			name = teacher_name;
		}
		return name;
	}


	bool add_course(const TeacherId& teacher_id, const CourseId& course_id) override
	{
		auto& courses = _db->get_or_create<std::unordered_set<CourseId>>(teacher_id, ColumnName{"courses"});

		return courses.insert(course_id).second;
	}


	const std::optional<int>& add_experience(const TeacherId& teacher_id, std::optional<int> experience) override
	{
		auto& val = _db->get_or_create<std::optional<int>>(teacher_id, ColumnName{"experience"});

		if (!val)
		{
			val = experience;
		}

		return val;
	}


	const std::optional<TeacherName>& get_name(const TeacherId& teacher_id) const override
	{
		return _db->get_data(teacher_id, ColumnName{"name"})
		          .get<std::optional<TeacherName>>();
	}

	const std::optional<int>& get_experience(const TeacherId& teacher_id) const override
	{
		return _db->get_data(teacher_id, ColumnName{"experience"})
		          .get<std::optional<int>>();
	}

	const std::unordered_set<CourseId>& get_courses(const TeacherId& teacher_id) const override
	{
		return _db->get_data(teacher_id, ColumnName{"courses"})
		          .get<std::unordered_set<CourseId>>();
	}


	bool remove_course(const TeacherId& teacher_id, const CourseId& course_id) override
	{
		auto& courses =
			_db->get_data(teacher_id, ColumnName{"courses"})
			   .get<std::unordered_set<CourseId>>();

		return courses.erase(course_id) > 0;
	}


	std::vector<TeacherPreview> get_preview() const override
	{
		std::vector<TeacherPreview> out;

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
