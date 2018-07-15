#include "stdafx.h"
//#include <stdlib.h>
#include <stdexcept>
#include <math.h>
#include "Coord.h"

/* ����������� - �� ��������� */
Coord::Coord()
{
	_value = 0;
	// ���� ��������� �������� '0'
	initialize0();
}

/* �����������
* - value - �������� �������� ��� �������� �������� �������
*/
Coord::Coord(const int& value)
	: Coord()
{
	_value = value;

	initialize();
}

/* �����������
 * - src - �������� ������ ��� �������� �������� �������
*/
Coord::Coord(const Coord& src)
	: Coord(src._value)
{
}

/* ���������� */
Coord::~Coord()
{
}

/* ���������� �������� ��������� � '0' */
void Coord::initialize0()
{
	for (int order = Order::MAX - 1; !(order < Order::UNIT); order--)
		_values[order] = 0;
}

/* ���������� �������� ��������� ��������� ����� */
void Coord::initialize()
{
	// ���� ��������� �������� � ����������� � ����������
	initialize0();

	for (int order = Order::MAX - 1; !(order < Order::UNIT); order--) {
		_values[order] = (abs(_value) - summaAt(order)) / pow(10, (order));

		if (!(_values[order] < 10))
			throwed(static_cast<Order>(order));
		else
			;
	}
}

/* ���������� ����� ���� ��������� �������� */
int Coord::Summa()
{
	int iRes = 0;

	for (int order = Order::MAX - 1; !(order < Order::UNIT); order--)
		iRes += _values[order];

	return iRes;
}

/* ���������� ����� ��������� ��������, ������� � ������� 'order'
 * order - ����� �������
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

/* ���������� ������ � ��������� ��������� �������� */
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
	// ������� ������ ������� (��� �������� ��������)
	strRes.resize(strRes.length() - 2);

	strRes += "; Summa=" + SSTR(Summa());

	return strRes;
}

/* �������� ���������� ��� ������
 * order - ������� �������� ��� ��������� �������� ��������� ������
*/
void Coord::throwed(const Order &order)
{
	throw std::runtime_error(std::string("Failed: ") + "from <" + SSTR(_value) + "> Order=" + SSTR(order) + ", " + "Value=" + SSTR(_values[order]));
}

/* ������� ����� ������ �� ���� ���������� � ��������� ��������
* src - ������ ��� �������� �������� �������
*/
Coord &Coord::operator= (const Coord& src) 
{
	_value = src._value;
	initialize();

	return *this;
}

/* ������� ����� ������ �� ���� ���������� � ��������� ��������
 * src - �������� ��� �������� �������
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