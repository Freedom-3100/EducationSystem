#pragma once

template <typename T>
class Cell;

class ICell
{
public:
	virtual ~ICell() = default;

	virtual std::unique_ptr<ICell> clone() const = 0;
	virtual void print(std::ostream& os) const = 0;
	virtual const std::type_info& type() const = 0;

	template <typename T>
	T& get()
	{
		return dynamic_cast<Cell<T>&>(*this).value;
	}

	template <typename T>
	const T& get() const
	{
		return dynamic_cast<const Cell<T>&>(*this).value;
	}

	template <typename T>
	std::optional<std::reference_wrapper<T>> try_get()
	{
		if (this->type() == typeid(T))
		{
			return std::ref(static_cast<Cell<T>&>(*this).value);
		}
		return std::nullopt;
	}

	template <typename T>
	std::optional<std::reference_wrapper<const T>> try_get() const
	{
		if (this->type() == typeid(T))
		{
			return std::cref(static_cast<const Cell<T>&>(*this).value);
		}
		return std::nullopt;
	}
};

template <typename T>
class Cell : public ICell
{
public:
	T value;

	Cell(T v) : value(std::move(v))
	{
	}

	std::unique_ptr<ICell> clone() const override
	{
		return std::make_unique<Cell<T>>(value);
	}

	void print(std::ostream& os) const override
	{
		os << value;
	}

	const std::type_info& type() const override
	{
		return typeid(T);
	}
};
