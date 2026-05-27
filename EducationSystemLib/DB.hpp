#pragma once
#include <unordered_map>

#include "Column.hpp"
#include "Cell.hpp"
#include "IdConcept.hpp"

template <Hashable Id>
class DataBase final
{
	std::unordered_map<Id, std::unordered_map<ColumnName, std::unique_ptr<ICell>>> _data;

public:
	bool set(const Id& id, const ColumnName& column_name, const ICell& data)
	{
		auto& raw = _data[id];
		return raw.emplace(column_name, data.clone()).second;
	}

	ICell& get_data(const Id& id, const ColumnName& column_name)
	{
		auto& raw = _data.at(id);
		return *raw.at(column_name);
	}

	const ICell& get_data(const Id& id, const ColumnName& column_name) const
	{
		const auto& raw = _data.at(id);
		return *raw.at(column_name);
	}

	std::vector<Id> get_list_ids() const
	{
		std::vector<Id> result;
		for (const auto& it : _data)
		{
			result.push_back(it.first);
		}
		return result;
	}

	std::vector<ColumnName> get_cell_names(const Id& course_id) const
	{
		std::vector<ColumnName> result;
		auto it = _data.find(course_id);
		for (const auto& name : it->second)
		{
			result.push_back(name.first);
		}
		return result;
	}


	template <typename T>
	T& get_or_create(const Id& id, const ColumnName& column_name, T default_value = T{})
	{
		auto& row = _data[id];

		auto it = row.find(column_name);
		if (it == row.end())
		{
			auto cell = std::make_unique<Cell<T>>(std::move(default_value));
			auto [newIt, _] = row.emplace(column_name, std::move(cell));
			return newIt->second->template get<T>();
		}

		return it->second->template get<T>();
	}
};
