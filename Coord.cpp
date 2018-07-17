#include "stdafx.h"
//#include <stdlib.h>
#include <stdexcept>
#include <math.h>
#include "Coord.h"

/* Конструктор - по умолчанию */
Coord::Coord()
{
	_value = 0;
	// всем элементам значение '0'
	initialize0();
}

/* Конструктор
* - value - исходное значение для создания текущего объекта
*/
Coord::Coord(const int& value)
	: Coord()
{
	_value = value;

	initialize();
}

/* Конструктор
 * - src - исходный объект для создания текущего объекта
*/
Coord::Coord(const Coord& src)
	: Coord(src._value)
{
}

/* Деструктор */
Coord::~Coord()
{
}

/* Установить значения элементов в '0' */
void Coord::initialize0()
{
	for (int order = Order::MAX - 1; !(order < Order::UNIT); order--)
		_values[order] = 0;
}

/* Установить значения элементов исходного числа */
void Coord::initialize()
{
	// всем элементам значения в соответсвии с аргументом
	initialize0();

	for (int order = Order::MAX - 1; !(order < Order::UNIT); order--) {
		_values[order] = (abs(_value) - summaAt(order)) / pow(10, (order));

		if (!(_values[order] < 10))
			throwed(static_cast<Order>(order));
		else
			;
	}
}

/* Возвратить сумму всех элементов значения */
int Coord::Summa()
{
	int iRes = 0;

	for (int order = Order::MAX - 1; !(order < Order::UNIT); order--)
		iRes += _values[order];

	return iRes;
}

/* Возвратить сумму элементов значения, начиная с позиции 'order'
 * order - номер позиции
*/
int Coord::summaAt(int order)
{
	int iRes = 0;

	iRes = _values[order] * pow (10, order);

	if (order < (Order::MAX - 1))
		iRes += summaAt(order + 1);
	else
		;

	return iRes;
}

/* Возвратить строку с подробным описанием значения */
std::string Coord::Print()
{
	std::string strRes = "Coord from <" + SSTR(_value) + ">="
		, orderPass = "";

	for (int order = Order::MAX - 1; !(order < Order::UNIT); order--) {
		orderPass = "[" + SSTR(order) + ", " + SSTR(_values[order]) + "], ";
		if (orderPass.length() < 2)
			throwed(static_cast<Order>(order));
		else
			;

		strRes += orderPass;
	}
	// удалить лишние символы (для крайнего элемента)
	strRes.resize(strRes.length() - 2);

	strRes += "; Summa=" + SSTR(Summa());

	return strRes;
}

Coord &Coord::Next() const
{
	return *new Coord(_value + 1);
}

Coord &Coord::Return() const
{
	return *new Coord(_value - 1);
}

/* Прервать выполнение при ошибке
 * order - позиция элемента при обработке которого произошла ошибка
*/
void Coord::throwed(const Order &order)
{
	throw std::runtime_error(std::string("Failed: ") + "from <" + SSTR(_value) + "> Order=" + SSTR(order) + ", " + "Value=" + SSTR(_values[order]));
}

/* Создать новый объект на базе указанного в аргументе значения
* src - объект для создания текущего объекта
*/
Coord &Coord::operator= (const Coord& src) 
{
	_value = src._value;
	initialize();

	return *this;
}

/* Создать новый объект на базе указанного в аргументе значения
 * src - значение для создания объекта
*/
Coord &Coord::operator= (const int& src) 
{
	_value = src;
	initialize();

	return *this;
}

Coord &Coord::operator++ ()
{
	_value++;
	initialize();

	return *this;
}

Coord Coord::operator++ (int)
{
	Coord coordRes(*this);

	++(*this);

	return coordRes;
}

Coord Coord::operator+= (int var)
{
	Coord coordRes(*this);
	int v = var;

	if (v < 0)
		throwed(Order::MAX);
	else
		;

	while (v-- > 0)
		++(*this);

	return coordRes;
}

Coord &Coord::operator-- ()
{
	_value--;
	initialize();

	return *this;
}

Coord &Coord::operator-- (int) 
{
	Coord coordRes(*this);

	--(*this);

	return coordRes;
}

Coord &Coord::operator-= (int var)
{
	Coord coordRes(*this);
	int v = var;

	if (v < 0)
		throwed(Order::MAX);
	else
		;

	while (v-- > 0)
		--(*this);

	return coordRes;
}