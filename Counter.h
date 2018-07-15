#pragma once

#include "Position.h"

#include <future>
#include <thread>
// #include <set>
#include <list>
#include <vector>

#include "Arg.h"

class Counter
{
/* Members */
private:
	/* Перечень возможных шагов(направлений для шагов) */
	const std::vector<Position::Direct> _vecPossible = { Position::Direct::UP
		, Position::Direct::RIGHT
		, Position::Direct::DOWN
		, Position::Direct::LEFT
	};

	/* Текущий индекс для выбора очередного направления движения */
	unsigned short _curIndex;
	/* Контейнер для ячеек, удовлетвряющих условиям ограничений */
	std::set<std::auto_ptr<Position>> _setBusy;

	/* Признак завершения алгоритма SCORE(2) */
	int _busy;

	std::thread _thread;
	std::list<std::thread> _threads22;
	std::recursive_mutex _mtx22;

/* Members */
public:
	//* Признак вывода на отображение результатов успешных проверок */
	static bool SuccessPrinted;
	/* Признак вывода на отображение результатов неудачных проверок */
	static bool FailPrinted;
	/* Признак распараллеливания потока в режиме SCORE(22) */
	static bool VersionScoreThreaded;

/* Methods */
private:
	/* Возвратить признак успеха/ошибки проверки выполнения условий ограничений
	 * - позиция ячейки для проверки
	 * - ограничение суммы
	 * - признак необходимости проверять наличие в итоговом контейнере (точки, прошедшие проверку на выполнение условий ограничений)
	*/
	bool isSuccess(Position &, const int &, bool = true) const;
	/* Возвратить следующее направление движения объекта */
	Position::Direct nextDirect();
	/* Проверить уникальность координат позиции (дубликат метода сравнения контейнера 'set' - для отладки)
	 * - координаты позиции-ячейки
	*/
	bool Counter::isUniquePosition(const Position &) ;

	int maxPassed(const int &) const;

	/* Протестировать выполнение условий для точки
	 * - позиция точки
	*/
	void Counter::test(const int & );

/* Methods */
public:
	/* Конструктор - по умолчанию */
	Counter();

	/* Конструктор - с аргументами
	 * - версия алг.
	 * - начальная позиция объекта
	 * - ограничение по сумме
	*/
	Counter(const Arg::Version &, const Position &, const int &);

	/* Конструктор - с аргументом (для версии Arg::CROSS, без потока)
	 * - массив реперных точек
	*/
	Counter(Position []);
	/* Деструктор */
	virtual ~Counter();

	/* Потоковая функция 1-ой версии алгоритма подсчета точек-ячеек
	 * - ограничение суммы
	*/
	void threadCounter1(const int &);

	/* Потоковая функция 1-ой версии алгоритма подсчета точек-ячеек
	 * - ограничение суммы
	*/
	void threadCounter2(const int &);
	void threadCounter22(const int & , const int &);

	/* Возвратить результат подсчета */
	inline int Result() const { return _setBusy.size(); }

	inline static bool IsThread(const Arg::Version &ver)
	{
		return (ver == Arg::Version::FINDING)
			|| (ver == Arg::Version::SCORE);
	}

	/* Остановить поток */
	void Stop();
};

