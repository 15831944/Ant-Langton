#include "stdafx.h"
#include "Position.h"

/* ����������� - �� ��������� */
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

/* �����������
 * xx - ���������� 'X'
 * yy - ���������� 'Y'
*/
Position::Position(const int &xx, const int &yy)
	: Position()
{
	initialize(xx, yy);
}

Position::Position(const Position &pos)
	: Position(pos.X, pos.Y)
{
	_path = pos._path;
	_setFail = pos._setFail;
}

Position::Position(const Position *pos)
	: Position(*pos)
{
}

Position::Position(const Coord &xx, const Coord &yy)
	: Position(xx.Value(), yy.Value())
{
}

/* ���������� */
Position::~Position()
{
}

void Position::initialize(const int &xx, const int &yy
	/*, const int & _x, const int & _y
	, const int & __x, const int & __y*/)
{
	/*__previousX =  __x == std::numeric_limits<int>::signaling_NaN() ? xx : __x;
	_previousX = _x == std::numeric_limits<int>::signaling_NaN() ? xx : _x;*/
	X = xx;

	/*__previousY = __y == std::numeric_limits<int>::signaling_NaN() ? yy : __y;
	_previousY = _y == std::numeric_limits<int>::signaling_NaN() ? yy : _y;*/
	Y = yy;

	_summa = summa();
}

/* ���������� ������ � ��������� ��������� �������� */
std::string Position::Print()
{
	return "X = " + X.Print() + ";" + "\nY = " + Y.Print() + "; SUMMA = " + SSTR(Summa());
}

/* ����������� ������ � ����������� � ������������ �� �����
 * direct - �����������
 * limit - �����������
*/
void Position::Play(const Direct &direct, const int &limit)
{
	// ��� �������� ��� ������������ ������� �����������
	Coord xx = 0
		, yy = 0;

	while (Summa() < limit) {
		xx = X; yy = Y;

		Next(direct);
	}
	// ���������� ������� ��������, ��������������� ������� �����������
	X = xx; Y = yy;

	_summa = summa();
}

/* ���������� �������� ��������� ���������� ����� ��� �������� �������
 * - direct - �����������
 * - bReverse - ������� ������������� (�� ������������)
*/
void Position::Next(const Direct &direct, bool bReverse)
{
	// TODO: 'var' �� �����������? ����� ��������� � ����������� 'Coord::+=', 'Coord::-='
	int var = 1;

	/*__previousX = _previousX;
	__previousY = _previousY;

	_previousX = X;
	_previousY = Y;*/

	_path.push_back(direct);

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

/* ���������� �������� ��������� ���������� ����� ��� �������� ������� */
void Position::Return()
{
	Direct direct;

	if (_path.size() > 0) {
		direct = _path.back();
		_path.pop_back();

		switch (direct) {
			case Direct::UP:
				Y --;
				break;
			case Direct::UP_RIGHT:
				X --; Y --;
				break;
			case Direct::RIGHT:
				X --;
				break;
			case Direct::DOWN_RIGHT:
				X --; Y ++;
				break;
			case Direct::DOWN:
				Y ++;
				break;
			case Direct::DOWN_LEFT:
				X ++; Y ++;
				break;
			case Direct::LEFT:
				X ++;
				break;
			case Direct::UP_LEFT:
				X ++; Y --;
				break;
			default:
				throwed(direct);
				break;
		}

		_summa = summa();
	}
	else
		throwed(Direct::MAX);
}

/* ���������� ���������� ����� */
Position &Position::Previous() const
{
	Position *posRes = new Position(*this);
	posRes->Return();

	return *posRes;
}

/* ���������� ������� �������������� ������������� �� ����� �� �����������
 * - direct - �����������
*/
void Position::Fail(const Position::Direct &direct)
{
	_setFail.insert(direct);
}

/* ���������� ������� ���������� ����������� ������������� �� ����� �� ������ �����������
 * - sz - ���-�� ���������/��������� ����������� ��������
*/
bool Position::IsFail(const int&sz)
{
	return _setFail.size() == sz;
}

/* ���������� ������� ���������� ����������� ������������� �� ����� (�� ����������� ������� �����������) �� �����������
 * - direct - �����������
*/
bool Position::IsFail(const Position::Direct &direct)
{
	return !(_setFail.find(direct) == _setFail.end());
}

/* ������� �� ����������� 'direct' ����� �������
 * - ����� ��� ���������
 * - �����������
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

/* �������� ���������� ��� ������
 * direct - ����������� ����� ������
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
