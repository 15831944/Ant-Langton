#include "stdafx.h"
#include "Position.h"

/* Конструктор - по умолчанию */
Position::Position()
{
	/*__previousX =
	__previousY =
	_previousX =
	_previousY =*/
	X =
	Y =
		0;
}

/* Конструктор
 * xx - координата 'X'
 * yy - координата 'Y'
*/
Position::Position(const int &xx, const int &yy)
	: Position()
{
	initialize(xx, yy);
}

Position::Position(const Position &pos)
	: Position(pos.X, pos.Y)
{
}

Position::Position(const Position &pos, const Direct &direct)
	: Position (pos)
{
	Move(direct);
}

Position::Position(const Position *pos)
	: Position(*pos)
{
}

Position::Position(const Position *pos, const Direct &direct)
	: Position (*pos, direct)
{
}

Position::Position(const Coord &xx, const Coord &yy)
	: Position(xx.Value(), yy.Value())
{
}

/* Деструктор */
Position::~Position()
{
}

void Position::initialize(const int &xx, const int &yy)
{
	X = xx;
	Y = yy;

	_summa = summa();
}

/* Возвратить строку с подробным описанием значения */
std::string Position::Print()
{
	return "X = " + X.Print() + ";" + "\nY = " + Y.Print() + "; SUMMA = " + SSTR(Summa());
}

/* Переместить объект в направлении с ограничением по сумме
 * direct - направление
 * limit - ограничение
*/
void Position::Play(const Direct &direct, const int &limit)
{
	// для возврата при невыполнении условия ограничения
	Coord xx = 0
		, yy = 0;

	while (Summa() < limit) {
		xx = X; yy = Y;

		Move(direct);
	}
	// возвратить крайние значения, удовлетворяющие условию ограничения
	X = xx; Y = yy;

	_summa = summa();
}

/* Установить значения координат предыдущей точки для текущего объекта
 * - direct - направление
 * - bReverse - признак реверсивности (не используется)
*/
void Position::Move(const Direct &direct, bool bReverse)
{
	// TODO: 'var' не учитывается? Иначе сложности с операторами 'Coord::+=', 'Coord::-='
	int var = 1;

	switch (direct) {
		case Direct::UP:
			Y += var;
			break;
		case Direct::UP_RIGHT:
			X += var; Y += var;
			break;
		case Direct::RIGHT:
			X += var;
			break;
		case Direct::DOWN_RIGHT:
			X ++; Y --;
			break;
		case Direct::DOWN:
			Y --;
			break;
		case Direct::DOWN_LEFT:
			X --; Y --;
			break;
		case Direct::LEFT:
			X --;
			break;
		case Direct::UP_LEFT:
			X --; Y ++;
			break;
		default:
			throwed(direct);
			break;
	}

	_summa = summa();
}

/* Установить значения координат предыдущей точки для текущего объекта */
Position &Position::Return(const Direct &direct) const
{
	Coord x = X
		, y = Y;
	
	switch (direct)	{
		case UP:
			y = Y.Return();
			break;
		case RIGHT:
			x =X.Return();
			break;
		case DOWN:
			y = Y.Next();
			break;
		case LEFT:
			x = X.Next();
			break;
		default:
			break;
	}

	return *new Position(x, y);
}

/* Возвратить следующую точку */
Position &Position::Next(const Direct &direct) const
{
	Coord x = X
		, y = Y;

	switch (direct)
	{
		case UP:
			y = Y.Next();
			break;
		case RIGHT:
			x = X.Next();
			break;
		case DOWN:
			y = Y.Return();
			break;
		case LEFT:
			x = X.Return();
			break;
		default:
			break;
	}

	return *new Position(x, y);
}

/* Возвратить предыдущую точку */
Position &Position::Previous(const Position::Direct &direct) const
{
	Coord x = X
		, y = Y;

	switch (direct)
	{
		case UP:
			y = Y.Next();
			break;
		case RIGHT:
			x = X.Next();
			break;
		case DOWN:
			y = Y.Return();
			break;
		case LEFT:
			x = X.Return();
			break;
		default:
			break;
	}

	return *new Position(x, y);
}

/* Разница по направлению 'direct' между точками
 * - точка для сравнения
 * - направление
*/
int Position::Difference(const Position &pos, const Direct &direct)
{
	int iRes = std::numeric_limits<int>::signaling_NaN();

	switch (direct) {
		case Direct::UP:
		case Direct::DOWN:
			iRes = std::abs(Y.Value() - pos.Y.Value());
			break;
		case Direct::RIGHT:
		case Direct::LEFT:
			iRes = std::abs(X.Value() - pos.X.Value());
			break;
		case Direct::UP_RIGHT:
		case Direct::DOWN_RIGHT:
		case Direct::DOWN_LEFT:
		case Direct::UP_LEFT:
			// TODO:
			break;
		default:
			throwed(direct);
			break;
	}

	return iRes;
}

/* Прервать выполнение при ошибке
 * direct - направление перем ещения
*/
void Position::throwed(const Direct &direct)
{
	throw std::runtime_error(std::string("Failed: ") + "from <" + Print() + "> Direct=" + SSTR(static_cast<int>(direct)));
}

bool Position::operator<(const Position &pos) const
{
	return (X.Value() < pos.X.Value())
		|| ((X.Value() == pos.X.Value())
			&& (Y.Value() < pos.Y.Value()));
}

bool Position::operator==(const Position &pos) const
{
	return (X.Value() == pos.X.Value())
		&& (Y.Value() == pos.Y.Value())
		//&& (_path == pos._path)
		;
}

bool Position::operator!=(const Position &pos) const
{
	return !(X.Value() == pos.X.Value())
		|| !(Y.Value() == pos.Y.Value())
		//|| !(_path == pos._path)
		;
}

bool static operator==(const std::auto_ptr<Position> &pos1, const std::auto_ptr<Position>  &pos2)
{
	return ((*pos1).X.Value() == (*pos2).X.Value())
		&& ((*pos1).Y.Value() == (*pos2).Y.Value());
}

Position &Position::operator=(const Position &pos)
{
	initialize(pos.X.Value(), pos.Y.Value()
		/*, pos._previousX.Value(), pos._previousY.Value()
		, pos.__previousX.Value(), pos.__previousY.Value()*/);

	return *this;
}
