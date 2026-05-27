#pragma once
#include <functional>

template <typename Id>
concept Hashable = requires(Id id)
{
	{ std::hash<Id>{}(id) } -> std::convertible_to<size_t>;
};
