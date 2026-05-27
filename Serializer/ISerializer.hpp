#pragma once
#include <ostream>

#include "CourseManager.hpp"
#include "ParserDTO.hpp"

class ISerializer
{
public:
	virtual ~ISerializer() = default;

	virtual void write(std::ostream&, const CourseManager&) const = 0;

	virtual void read(const ParserDTO&, CourseManager&) = 0;
};
