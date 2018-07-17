#pragma once

// #include <stdlib.h>
#include <deque>
#include <tuple>
#include <vector>
#include <set>

#include "coord.h"

class Position
{
/* typedefs */
public:
	/* Перечисление направлений перемещения */
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

	const std::vector<Direct> VectorDirectPossible = { UP, RIGHT, DOWN, LEFT };

/* Memmbers */
private:

	int _summa;
	/* Прервать выполнение при ошибке
	 * - направление перем ещения
	*/
	void throwed(const Direct &);

/* Members */
public:
	/* Текущие координаты */
	Coord X, Y;

/* Methods */
private:

	inline int summa() { return X.Summa() + Y.Summa(); }

	void initialize(const int &, const int &
		/*, const int & = std::numeric_limits<int>::signaling_NaN(), const int & = std::numeric_limits<int>::signaling_NaN()
		, const int & = std::numeric_limits<int>::signaling_NaN(), const int & = std::numeric_limits<int>::signaling_NaN()*/);

/* Methods */
public:
	/* Конструктор - по умолчанию */
	Position();

	Position(const Position & );

	Position(const Position & , const Direct& );

	Position(const Position *);

	Position(const Position *, const Direct&);
	/* Конструктор
	 * - координата 'X'
	 * - координата 'Y'
	*/
	Position(const int & , const int & );

	Position(const Coord &, const Coord &);
	/* Деструктор */
	virtual ~Position();
	/* Возвратить строку с подробным описанием значения */
	std::string Print();
	/* Возвратить сумму всех элементов значения */
	inline int Summa() { return _summa; }
	/* Переместить объект в направлении с ограничением по сумме
	 * - направление
	 * - ограничение
	*/
	void Play(const Direct &, const int & );

	/* Установить значения координат предыдущей точки для текущего объекта
	 * - направление
	 * - признак реверсивности (не используется)
	*/
	void Move(const Direct &, bool = false) ;

	/* Установить значения координат предыдущей точки для текущего объекта */
	void Return(const Direct & );

	/* Возвратить следующую точку */
	Position &Next(const Direct & ) const;

	/* Возвратить предыдущую точку */
	Position &Previous(const Direct & ) const;

	/* Установить признак нневозможности пердвигаиться из точки по направлению
	 * - направление
	*/
	void Fail(const Direct & );

	/* Возвратить признак отсутствия возможности передвигаться из точки по любому направлению
	 * - кол-во возможных/доступных направлений движения
	*/
	bool IsFail(const int & );

	/* Возвратить признак отсутствия возможности передвигаться из точки (не выполняются условия ограничений) по направлению
	 * - направление
	*/
	bool IsFail(const Position::Direct &);

	/* Разница по направлению 'direct' между точками
	 * - точка для сравнения
	 * - направление
	*/
	int Difference(const Position &, const Direct &);

	Position &operator=(const Position &) ;

	bool operator<(const Position &) const;

	bool operator==(const Position &) const;

	bool operator!=(const Position &) const;
};


