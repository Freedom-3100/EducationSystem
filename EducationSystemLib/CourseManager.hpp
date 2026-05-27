#pragma once
#include <memory>

#include "CoursesRepository.hpp"
#include "StudentRepository.hpp"
#include "TeacherRepository.hpp"


class CourseManager final
{
	std::shared_ptr<CourseRepository> _cRep;
	std::shared_ptr<StudentRepository> _sRep;
	std::shared_ptr<TeacherRepository> _tRep;

public:
	CourseManager(std::shared_ptr<CourseRepository> cRep, std::shared_ptr<StudentRepository> sRep,
	              std::shared_ptr<TeacherRepository> tRep): _cRep(std::move(cRep)),
	                                                        _sRep(std::move(sRep)), _tRep(std::move(tRep))
	{
	}


	void add_student(const StudentId& student_id, const std::optional<StudentName>& student_name) const
	{
		_sRep->add_name(student_id, student_name);
	}

	void add_teacher(const TeacherId& teacher_id, const std::optional<TeacherName>& teacher_name,
	                 const std::optional<int>& experience) const
	{
		_tRep->add_name(teacher_id, teacher_name);
		_tRep->add_experience(teacher_id, experience);
	}

	void add_course(const CourseId& course_id, const std::optional<CoursesName>& column_name) const
	{
		_cRep->add_name(course_id, column_name);
	}

	void add_link(const StudentId& student_id, const CourseId& course_id) const
	{
		_cRep->add_student(course_id, student_id);
		_sRep->add_course(student_id, course_id);
	}

	void add_link(const TeacherId& teacher_id, const CourseId& course_id) const
	{
		auto teacher = _cRep->get_teacher(course_id);

		if (teacher.has_value())
		{
			_cRep->remove_teacher(course_id, teacher_id);
			_tRep->remove_course(teacher_id, course_id);
		}

		_cRep->add_teacher(course_id, teacher_id);
		_tRep->add_course(teacher_id, course_id);
	}


	const std::optional<StudentName>& get_name(const StudentId& student_id) const
	{
		return _sRep->get_name(student_id);
	}

	const std::optional<TeacherName>& get_name(const TeacherId& teacher_id) const
	{
		return _tRep->get_name(teacher_id);
	}

	const std::optional<CoursesName>& get_name(const CourseId& course_id) const
	{
		return _cRep->get_name(course_id);
	}

	const std::optional<int>& get_experience(const TeacherId& teacher_id) const
	{
		return _tRep->get_experience(teacher_id);
	}

	std::vector<StudentPreview> get_students_preview() const
	{
		return _sRep->get_preview();
	}

	std::vector<TeacherPreview> get_teacher_preview() const
	{
		return _tRep->get_preview();
	}

	std::vector<CoursePreview> get_course_preview() const
	{
		return _cRep->get_preview();
	}


	const std::unordered_set<CourseId>& get_courses(const StudentId& student_id) const
	{
		return _sRep->get_courses(student_id);
	}

	const std::unordered_set<StudentId>& get_students(const CourseId& course_id) const
	{
		return _cRep->get_students(course_id);
	}

	const std::unordered_set<CourseId>& get_courses(const TeacherId& teacher_id) const
	{
		return _tRep->get_courses(teacher_id);
	}

	const std::optional<TeacherId>& get_teacher(const CourseId& course_id) const
	{
		return _cRep->get_teacher(course_id);
	}
};
