#pragma once

// #include <stdlib.h>
#include <deque>
#include <set>

#include "coord.h"

class Position
{
/* typedefs */
public:
	/* ������������ ����������� ����������� */
	enum Direct
	{
		UP
		, UP_RIGHT
		, RIGHT
		, DOWN_RIGHT
		, DOWN
		, DOWN_LEFT
		, LEFT
		, UP_LEFT
		, MAX
	};

/* Memmbers */
private:
	///* �������� ���������� (�� ������ ������ 'Play') */
	//Coord __previousX, __previousY
	//	, _previousX, _previousY;

	std::deque<Direct> _path;

	std::set<Direct> _setFail;

	int _summa;
	/* �������� ���������� ��� ������
	 * - ����������� ����� ������
	*/
	void throwed(const Direct &);

/* Members */
public:
	/* ������� ���������� */
	Coord X, Y;

/* Methods */
private:

	inline int summa() { return X.Summa() + Y.Summa(); }

	void initialize(const int &, const int &
		/*, const int & = std::numeric_limits<int>::signaling_NaN(), const int & = std::numeric_limits<int>::signaling_NaN()
		, const int & = std::numeric_limits<int>::signaling_NaN(), const int & = std::numeric_limits<int>::signaling_NaN()*/);

/* Methods */
public:
	/* ����������� - �� ��������� */
	Position();

	Position(const Position & );

	Position(const Position *);
	/* �����������
	 * - ���������� 'X'
	 * - ���������� 'Y'
	*/
	Position(const int & , const int & );

	Position(const Coord &, const Coord &);
	/* ���������� */
	virtual ~Position();
	/* ���������� ������ � ��������� ��������� �������� */
	std::string Print();
	/* ���������� ����� ���� ��������� �������� */
	inline int Summa() { return _summa; }
	/* ����������� ������ � ����������� � ������������ �� �����
	 * - �����������
	 * - �����������
	*/
	void Play(const Direct &, const int & );

	/* ���������� �������� ��������� ���������� ����� ��� �������� �������
	 * - �����������
	 * - ������� ������������� (�� ������������)
	*/
	void Next(const Direct &, bool = false) ;

	/* ���������� �������� ��������� ���������� ����� ��� �������� ������� */
	void Return();

	/* ���������� ���������� ����� */
	Position &Previous() const;

	/* ���������� ������� �������������� ������������� �� ����� �� �����������
	 * - �����������
	*/
	void Fail(const Direct & );

	/* ���������� ������� ���������� ����������� ������������� �� ����� �� ������ �����������
	 * - ���-�� ���������/��������� ����������� ��������
	*/
	bool IsFail(const int & );

	/* ���������� ������� ���������� ����������� ������������� �� ����� (�� ����������� ������� �����������) �� �����������
	 * - �����������
	*/
	bool IsFail(const Position::Direct &);

	/* ������� �� ����������� 'direct' ����� �������
	 * - ����� ��� ���������
	 * - �����������
	*/
	int Difference(const Position &, const Direct &);

	Position &operator=(const Position &) ;

	bool operator<(const Position &) const;

	bool operator==(const Position &) const;

	bool operator!=(const Position &) const;
};


