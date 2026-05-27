#pragma once
#include "ISerializer.hpp"
#include "Parser.hpp"

class SerializerFabric final
{
public:
	static void serialize(std::istream& input, const std::vector<std::unique_ptr<ISerializer>>& serializers,
	                      const IParser& parser, CourseManager& manager)
	{
		auto dto = parser.parse_next(input);
		while (dto.has_value())
		{
			for (const auto& serializer : serializers)
			{
				serializer->read(dto.value(), manager);
			}
			dto = parser.parse_next(input);
		}
	}

	static void deserialize(std::ostream& output, const std::vector<std::unique_ptr<ISerializer>>& serializers,
	                        const CourseManager& manager)
	{
		for (const auto& serializer : serializers)
		{
			serializer->write(output, manager);
		}
	}
};
