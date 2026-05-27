#pragma once

#include <optional>

#include "CoursesId.hpp"
#include "CoursesName.hpp"
#include "StudentName.hpp"
#include "StudentPreview.hpp"

#include "StudentsId.hpp"
#include "TeacherName.hpp"
#include "TeacherPreview.hpp"
#include "TeachersId.hpp"

class IStudentRepository
{
public:
	virtual ~IStudentRepository() = default;

	virtual const std::optional<StudentName>& add_name(const StudentId&, const std::optional<StudentName>&) = 0;
	virtual const std::optional<StudentName>& get_name(const StudentId&) const = 0;

	virtual bool add_course(const StudentId&, const CourseId&) = 0;
	virtual bool remove_course(const StudentId&, const CourseId&) = 0;
	virtual const std::unordered_set<CourseId>& get_courses(const StudentId&) const = 0;

	virtual std::vector<StudentPreview> get_preview() const = 0;
};


class ICourseRepository
{
public:
	virtual ~ICourseRepository() = default;

	virtual const std::optional<CoursesName>& add_name(const CourseId&, const std::optional<CoursesName>&) = 0;
	virtual const std::optional<CoursesName>& get_name(const CourseId&) const = 0;

	virtual bool add_student(const CourseId&, const StudentId&) = 0;
	virtual bool remove_student(const CourseId&, const StudentId&) = 0;
	virtual const std::unordered_set<StudentId>& get_students(const CourseId&) const = 0;

	virtual bool add_teacher(const CourseId&, const TeacherId&) = 0;
	virtual bool remove_teacher(const CourseId&, const TeacherId&) = 0;
	virtual const std::optional<TeacherId>& get_teacher(const CourseId&) const = 0;

	virtual std::vector<CoursePreview> get_preview() const = 0;
};

class ITeacherRepository
{
public:
	virtual ~ITeacherRepository() = default;

	virtual const std::optional<TeacherName>& add_name(const TeacherId&, const std::optional<TeacherName>&) = 0;
	virtual const std::optional<TeacherName>& get_name(const TeacherId&) const = 0;

	virtual const std::optional<int>& add_experience(const TeacherId&, std::optional<int>) = 0;
	virtual const std::optional<int>& get_experience(const TeacherId&) const = 0;

	virtual const std::unordered_set<CourseId>& get_courses(const TeacherId&) const = 0;
	virtual bool add_course(const TeacherId&, const CourseId&) = 0;
	virtual bool remove_course(const TeacherId&, const CourseId&) = 0;

	virtual std::vector<TeacherPreview> get_preview() const = 0;
};
