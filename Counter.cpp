#include "stdafx.h"
// #include <stdlib.h>
// #include <vector>
// #include <set>
#include <iostream>
// #include <deque>
// #include <algorithm>

#include "Counter.h"

/* Признак вывода на отображение результатов успешных проверок */
bool Counter::SuccessPrinted = true;
/* Признак вывода на отображение результатов неудачных проверок */
bool Counter::FailPrinted = false;
/* Признак распараллеливания потока в режиме SCORE(22) */
bool Counter::VersionScoreThreaded = false;

bool static operator<(const std::auto_ptr<Position> &pos1, const std::auto_ptr<Position> &pos2)
{
	return (*pos1) < (*pos2);
}

/* Конструктор - по умолчанию */
Counter::Counter()
{
	_curIndex = 0;
}

/* Конструктор - с аргументом (для версии Arg::CROSS, без потока)
 * - arrayPos - массив реперных точек
*/
Counter::Counter(Position arrayPos [])
	: Counter()
{
	int orthMinX = std::numeric_limits<int>::signaling_NaN(), orthMinY = std::numeric_limits<int>::signaling_NaN(), orthMaxX = -1 * std::numeric_limits<int>::signaling_NaN(), orthMaxY = -1 * std::numeric_limits<int>::signaling_NaN()
		, crossMinX = std::numeric_limits<int>::signaling_NaN(), crossMinY = std::numeric_limits<int>::signaling_NaN(), crossMaxX = -1 * std::numeric_limits<int>::signaling_NaN(), crossMaxY = -1 * std::numeric_limits<int>::signaling_NaN();

	std::set<Position> setOrth
		, setCross;

	for (int direct = 0; direct < Position::Direct::MAX; direct++) {
		switch (static_cast<Position::Direct>(direct)) {
			case Position::Direct::UP:
			case Position::Direct::RIGHT:
			case Position::Direct::DOWN:
			case Position::Direct::LEFT:
				if (orthMinX > arrayPos[direct].X.Value())
					orthMinX = arrayPos[direct].X.Value();
				else
					;

				if (orthMinY > arrayPos[direct].Y.Value())
					orthMinY = arrayPos[direct].Y.Value();
				else
					;

				if (orthMaxX < arrayPos[direct].X.Value())
					orthMaxX = arrayPos[direct].X.Value();
				else
					;

				if (orthMaxY < arrayPos[direct].Y.Value())
					orthMaxY = arrayPos[direct].Y.Value();
				else
					;
				break;
			case Position::Direct::UP_RIGHT:
			case Position::Direct::DOWN_RIGHT:
			case Position::Direct::DOWN_LEFT:
			case Position::Direct::UP_LEFT:
				if (crossMinX > arrayPos[direct].X.Value())
					crossMinX = arrayPos[direct].X.Value();
				else
					;

				if (crossMinY > arrayPos[direct].Y.Value())
					crossMinY = arrayPos[direct].Y.Value();
				else
					;

				if (crossMaxX < arrayPos[direct].X.Value())
					crossMaxX = arrayPos[direct].X.Value();
				else
					;

				if (crossMaxY < arrayPos[direct].Y.Value())
					crossMaxY = arrayPos[direct].Y.Value();
				else
					;
				break;
			default:
				break;
		}
	}

	setOrth.insert(Position(orthMinX, orthMinY));
	setOrth.insert(Position(orthMinX, orthMaxY));
	setOrth.insert(Position(orthMaxX, orthMinY));
	setOrth.insert(Position(orthMaxX, orthMaxY));

	setCross.insert(Position(crossMinX, crossMinY));
	setCross.insert(Position(crossMinX, crossMaxY));
	setCross.insert(Position(crossMaxX, crossMinY));
	setCross.insert(Position(crossMaxX, crossMaxY));
}

/* Конструктор - с аргументами (для версий Arg::FINDING, Arg::SCORE , с потоком)
 * - версия алг.
 * - начальная позиция объекта
 * - ограничение по сумме
*/
Counter::Counter(const Arg::Version &ver, const Position &pos, const int &limit)
	: Counter()
{
	// TODO: требуется проверка на вполнение условий ограничения, иначе трудности, т.к. все построено на '_setBusy.begin()'
	_setBusy.insert(std::auto_ptr<Position>(new Position (pos)));

	switch (ver) {
		case Arg::Version::FINDING:
		case Arg::Version::SCORE:
			_thread =
				std::thread([this, ver, limit] {
				return ver == Arg::Version::FINDING ? this->threadCounter1(limit)
					: ver == Arg::Version::SCORE ? this->threadCounter2(limit)
					: throw std::runtime_error(std::string("Failed: Counter::ctor () - unreachable code...")); })
				;
			break;
		case Arg::Version::TEST:
			test(limit);
			break;
		default:
			throw std::runtime_error(std::string("Failed: Counter::ctor () - version undefined..."));
			break;
	}
}

/* Протестировать выполнение условий для точки
 * - ограничение
*/
void Counter::test(const int &limit)
{
	// тестируемая позиция точки
	Position curPosition = *(*_setBusy.begin());

	std::cout << "\nTest to:\n" + curPosition.Print();

	if (isSuccess(curPosition, limit, false) == true) {
		_setBusy.insert(std::auto_ptr<Position>(new Position(curPosition)));
		std::cout << "\nOk\n";
	}
	else {
		std::cout << "\nFail...\n";
	}
}

/* Возвратить признак успеха/ошибки проверки выполнения условий ограничений
 * pos - позиция ячейки для проверки
 * limit - ограничение суммы
 * - признак необходимости проверять наличие в итоговом контейнере (точки, прошедшие проверку на выполнение условий ограничений)
*/
bool Counter::isSuccess(Position &pos, const int &limit, bool bContains) const
{
	return !((pos.Summa() > limit))
		&& ((bContains == false)
			|| (_setBusy.find(std::auto_ptr<Position> (new Position (pos))) == _setBusy.end())
			//isUniquePosition(curPosition) == true
			);
}

/* Возвратить следующее направление движения объекта */
Position::Direct Counter::nextDirect()
{
	if ((_curIndex + 1) < _vecPossible.size())
		_curIndex++;
	else
		_curIndex = 0;

	return _vecPossible[_curIndex];
}

bool Counter::isUniquePosition(const Position &pos)
{
	bool bRes = true;

	std::set<std::auto_ptr<Position>>::iterator iter = _setBusy.begin();

	while (!(iter == _setBusy.end())) {
		if (((*iter)->X.Value() == pos.X.Value())
			&& ((*iter)->Y.Value() == pos.Y.Value()))
			break;
		else
			;

		iter++;
	}

	return iter == _setBusy.end();
}

/* Деструктор */
Counter::~Counter()
{
	try {
		_mtx22.lock();
		_setBusy.clear();
		_mtx22.unlock();
	} catch (std::exception e) {
		// TODO:
		std::cout << "\n~Counter: " << e.what() << "\n";
	}
}

/* Потоковая функция 1-ой версии алгоритма подсчета точек-ячеек
 * limit - ограничение суммы
*/
void Counter::threadCounter1(const int &limit)
{
	// текущее направление движения точки
	Position::Direct curDirect = _vecPossible[0];
	// текущая позиция точки
	Position curPosition = *(*_setBusy.begin());
	// итератор для поиска одной из уже возможно добавленных точек
	std::set<std::auto_ptr<Position>>::iterator iterFind = _setBusy.end();

	do {
		if (curPosition.IsFail(curDirect) == false) {
			curPosition.Next(curDirect);
			std::cout << "\nPlay to:" + SSTR(curDirect) + "\n" + curPosition.Print();

			if (isSuccess(curPosition, limit) == true) {
				_setBusy.insert(std::auto_ptr<Position> (new Position(curPosition)));
				std::cout << "\nOk\n";
			} else {
				curPosition.Return();
				// TODO: повторный код
				iterFind = _setBusy.find(std::auto_ptr<Position>(new Position(curPosition)));
				if (!(iterFind == _setBusy.end()))
					(*iterFind)->Fail(curDirect);
				else
					;
				curPosition.Fail(curDirect);
				std::cout << "\nFail...\n";

				std::vector<Position::Direct>::const_iterator iterPossible = _vecPossible.begin();
				while (!(iterPossible == _vecPossible.end())) {
					if ((!(curDirect == *iterPossible)) // 'curDirect' - уже привело к 'Fail'
						&& (curPosition.IsFail(*iterPossible) == false)) { // проверить о таком направлении в прошлом
						curPosition.Next(*iterPossible);
						std::cout << "Play to:" + SSTR(*iterPossible) + "\n" + curPosition.Print();

						if (isSuccess(curPosition, limit) == true) {
							_setBusy.insert(std::auto_ptr<Position>(new Position(curPosition)));
							std::cout << "\nOk\n";

							break;
						} else {
							std::auto_ptr<Position> (new Position(curPosition))->Return();
							// TODO: повторный код
							iterFind = _setBusy.find(std::auto_ptr<Position>(new Position(curPosition)));
							if (!(iterFind == _setBusy.end()))
								(*iterFind)->Fail(*iterPossible);
							else
								;
							curPosition.Fail(*iterPossible);
							std::cout << "\nFail...\n";
						}
					} else
						;

					iterPossible++;
				}

				if ((iterPossible == _vecPossible.end())
					&& (!(curPosition == *(*_setBusy.cbegin())))) // возвращаться из исходной точки нельзя
					curPosition.Return();
				else
					;
			}
		} else
			;

		curDirect = nextDirect();

		iterFind = _setBusy.find(std::auto_ptr<Position>(new Position(curPosition)));
	} while (!(iterFind == _setBusy.begin())
		|| ((iterFind == _setBusy.begin())
			&& (curPosition.IsFail(_vecPossible.size()) == false)));

	std::cout << "\nCounter::thread(ver.1) stopped...";
}

/* Потоковая функция 2-ой версии алгоритма подсчета точек-ячеек
 * limit - ограничение суммы
*/
void Counter::threadCounter2(const int &limit)
{
	int radius = 0;

	do {
		radius++;

		if (VersionScoreThreaded == false)
			threadCounter22(radius, limit);
		else
			_threads22.push_back(std::thread ([this, radius, limit] { return this->threadCounter22(std::ref(radius), limit); }));
		
	} while (!(radius - limit > 0));

	std::cout << "\nCounter::thread(ver.2) stopped...";
}

int Counter::maxPassed(const int &radius) const
{
	int iRes = 0
		, length = 0;

	length = 2 * radius + 1;
	iRes = 2 * length + 2 * (length - 2);

	return iRes;
}

/* Потоковая функция 2-ой версии (внутренняя) алгоритма подсчета точек-ячеек
* limit - ограничение суммы
*/
void Counter::threadCounter22(const int & radius, const int &limit)
{
	int iPass = 0
		, maxPass = maxPassed(radius);
	Position::Direct curDirect = _vecPossible[0];
	Position centrePosition
		, curPosition
		, markPosition;
	std::string message;

	_mtx22.lock();
	centrePosition = *(*_setBusy.begin());
	curPosition = *(*_setBusy.begin());
	markPosition = *(*_setBusy.begin());
	_mtx22.unlock();

	// выход на исходную для обхода точку (из центра - вверх)
	do {
		markPosition.Next(Position::Direct::UP_LEFT);
	} while (markPosition.Difference(centrePosition, curDirect) < radius);
	// присваиеваем текущей точке для выполнения движения
	curPosition = Position(markPosition);

	try {
		do {
			// в 1-ый проход будет установлен RIGHT(2)
			curDirect = nextDirect();

			do {
				curPosition.Next(curDirect);
			
				_mtx22.lock();

				if (isSuccess(curPosition, limit) == true) {
					_setBusy.insert(std::auto_ptr<Position>(new Position(curPosition)));

					if (SuccessPrinted == true)
						std::cout << "\nPlay to:" + SSTR(curDirect) + "\n" + curPosition.Print() + "\nOk\n";
					else
						;
				} else
					if (FailPrinted == true)
						std::cout << "\nPlay to:" + SSTR(curDirect) + "\n" + curPosition.Print() + "\nFail...\n";
					else
						;

				_mtx22.unlock();

				iPass++;
				if (iPass > maxPass) {
					message = std::string( "\n\n::threadCounter22() - BREAK; out of range passed a radius=") + std::to_string(radius) + "...\n";
					if (VersionScoreThreaded == false)
						std::cout << message;
					else {
						_mtx22.lock();
						std::cout << message;
						_mtx22.unlock();
					}

					break;
				} else
					;
			} while ((centrePosition.Difference(curPosition, curDirect) - radius) < 0);
		} while ((!(curPosition == markPosition))
			&& (!(iPass > maxPass)));
	} catch (std::exception e) {
		message = std::string("\n\n::threadCounter22() - ERROR; ") + std::string(e.what());
		if (VersionScoreThreaded == false)
			std::cout << message;
		else {
			_mtx22.lock();
			std::cout << message;
			_mtx22.unlock();
		}
	}

	message = "radius= " + std::to_string(radius) + "(passed <" + std::to_string(iPass) + ":" + std::to_string(maxPass) + ">), limit = " + std::to_string(limit) + "; stopped...";
	if (VersionScoreThreaded == false)
		std::cout << "\nCounter: " << message;
	else {
		_mtx22.lock();
		std::cout << "\nCounter(ver.2): " << message;
		_mtx22.unlock();
	}
}

/* Остановить поток */
void Counter::Stop()
{
	try {
		/* _thread.joinable() == ??? */
		if (_thread.joinable() == true) {
			std::cout << "\nCounter::thread stoping...";

			/*std::list<std::thread>::iterator iterThread22 = _threads22.begin();
			while (!(iterThread22 == _threads22.end()))
				(*iterThread22).~thread();*/

			_thread.detach();
			//_thread.~thread();
			//_thread.join();
		} else
			_thread.~thread();
	} catch (std::exception e) {
		std::cout << "\nCounter::Stop () - " << e.what() << "...";
	}
}