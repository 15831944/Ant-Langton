#include "stdafx.h"
// #include <stdlib.h>
// #include <vector>
// #include <set>
#include <iostream>
#include <deque>
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
	_track.insert(TrackItem(Position(pos), Position::Direct::UNKNOWN));

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
	Position curPosition = (*_track.begin())._position;

	std::cout << "\nTest to:\n" + curPosition.Print();

	if (isSuccess(curPosition, limit, false) == true) {
		_track.insert(TrackItem(Position(curPosition), Position::Direct::UNKNOWN));
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
	Track::iterator setTrackEnd = _track.end();

	std::function<bool()> validateVectorPossible = [this, &pos, &setTrackEnd]() -> bool {
		bool bRes = false;
		std::vector<Position::Direct>::const_iterator iterVecPossible;

		iterVecPossible = _vecPossible.begin();
		while ((!(iterVecPossible == _vecPossible.end()))
			&& (bRes == false)) {
			if (!(_track.find(TrackItem(Position(pos).Return(*iterVecPossible), *iterVecPossible)) == setTrackEnd))
				bRes = true;
			else
				;

			iterVecPossible++;
		}

		return bRes;
	};

	return !(pos.Summa() > limit)
		&& ((bContains == false)
			|| ((_track.find(TrackItem(new Position(pos), Position::Direct::UNKNOWN)) == setTrackEnd)
				&& (validateVectorPossible() == true))
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

	Track::iterator iterTrackItem = _track.begin();

	while (!(iterTrackItem == _track.end())) {
		if (((*iterTrackItem)._position.X.Value() == pos.X.Value())
			&& ((*iterTrackItem)._position.Y.Value() == pos.Y.Value()))
			break;
		else
			;

		iterTrackItem++;
	}

	return iterTrackItem == _track.end();
}

/* Деструктор */
Counter::~Counter()
{
	try {
		_mtx22.lock();
		_track.clear();
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
	Position::Direct failDirect;
	Position startPosition = (*_track.begin())._position
		, curPosition = startPosition; // текущая позиция точки
	TrackItem trackItem;	
	// итератор текущего направления движения точки
	std::vector<Position::Direct>::const_iterator iterCurDirect = _vecPossible.begin();
	std::deque<std::vector<Position::Direct>::const_iterator> dequeNotAvaliable;
	// итератор для поиска одной из уже возможно добавленных точек
	Track::const_iterator iterTrackFind;

	_setPassed.clear();
	_setPassed.insert(startPosition);
	_track.insert(TrackItem(curPosition, Position::Direct::UNKNOWN));

	do {
		failDirect = Position::Direct::UNKNOWN;
		iterTrackFind = _track.begin(); // признак отсутствия вставки
		curPosition = curPosition.Next(*iterCurDirect);
		_setPassed.insert(curPosition);

		if (!(curPosition.Summa() > limit)) {
			trackItem = TrackItem(curPosition, *iterCurDirect);
			iterTrackFind = _track.find(trackItem);

			if (iterTrackFind == _track.end()) {
				_track.insert(trackItem);
				std::cout << "Ok: " << curPosition.Print() << std::endl;

				if (dequeNotAvaliable.empty() == true)
					iterCurDirect = (iterCurDirect + 1) == _vecPossible.end() ? _vecPossible.begin() : iterCurDirect + 1;
				else {
					iterCurDirect = dequeNotAvaliable.back();
					dequeNotAvaliable.pop_back();
				}
			} else
				;
		} else
			;
			
		if (!(iterTrackFind == _track.end())) {
			curPosition = curPosition.Return(*iterCurDirect);
			iterTrackFind = _track.find(TrackItem(curPosition, Position::Direct::UNKNOWN));
			(*iterTrackFind)._setFail.insert(*iterCurDirect);

			if ((dequeNotAvaliable.size() + 1) < _vecPossible.size()) {
				dequeNotAvaliable.push_back(iterCurDirect);
				iterCurDirect = iterCurDirect == _vecPossible.begin() ? std::prev(_vecPossible.end()) : iterCurDirect - 1;
			}
			else {
				// TODO: разместить внутрь Track (в метод 'Return')
				failDirect = (*iterTrackFind)._direct;
				curPosition = _track.Return(curPosition);
				iterTrackFind = _track.find(TrackItem(curPosition, Position::Direct::UNKNOWN));
				(*iterTrackFind)._setFail.insert(failDirect);

				dequeNotAvaliable.clear();
				iterCurDirect = std::find(_vecPossible.begin(), _vecPossible.end(), failDirect);
				iterCurDirect = (iterCurDirect + 1) == _vecPossible.end() ? _vecPossible.begin() : iterCurDirect + 1;

				/*std::cout << "Result: <" << _setPassed.size() << ":" << _track.size() << ">" << std::endl;
				getch();*/
			}
		} else
			;
	} while ((!(curPosition == startPosition))
		|| ((curPosition == startPosition) && (dequeNotAvaliable.size() == 0) && ((*iterTrackFind)._setFail.size() < _vecPossible.size())));

	std::cout << "\nCounter::thread(ver.1) stopped...";
}

/* Потоковая функция 2-ой версии алгоритма подсчета точек-ячеек
 * limit - ограничение суммы
*/
void Counter::threadCounter2(const int &limit)
{
	int radius = 0;
	std::list<std::thread>::iterator iterThreads;

	do {
		radius++;

		if (VersionScoreThreaded == false) {
			threadCounter22(radius, limit);
		} else
			_threads22.push_back(std::thread ([this, radius, limit] { return this->threadCounter22(std::ref(radius), limit); }));
		
	} while (
		//!(radius - limit > 0) 
		_busy > 0
		);

	if (VersionScoreThreaded == true) {
		iterThreads = _threads22.begin();
	}

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
	int iRes = 0 // кол-во успешных проверок
		, iPass = 0
		, maxPass = maxPassed(radius);
	Position::Direct curDirect = _vecPossible[0];
	Position startPosition
		, curPosition
		, markPosition;
	std::string message;

	_mtx22.lock();
	startPosition = (*_track.begin())._position;
	curPosition = startPosition;
	markPosition = startPosition;
	_mtx22.unlock();

	// выход на исходную для обхода точку (из центра - вверх)
	do {
		markPosition.Move(Position::Direct::UP_LEFT);
	} while (markPosition.Difference(startPosition, curDirect) < radius);
	// присваиеваем текущей точке для выполнения движения
	curPosition = Position(markPosition);

	try {
		do {
			// в 1-ый проход будет установлен RIGHT(2)
			curDirect = nextDirect();

			do {
				curPosition.Move(curDirect);
			
				_mtx22.lock();

				_setPassed.insert(curPosition);
				if (isSuccess(curPosition, limit) == true) {
					_track.insert(TrackItem(Position(curPosition), Position::Direct::UNKNOWN));
					iRes++;

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
			} while ((startPosition.Difference(curPosition, curDirect) - radius) < 0);
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

	message = "radius= " + std::to_string(radius) + "(passed <" + std::to_string(iPass) + ":" + std::to_string(maxPass) + ">, success=" + std::to_string(iRes) + "), limit = " + std::to_string(limit) + "; stopped...";
	if (VersionScoreThreaded == false)
		std::cout << "\nCounter: " << message;
	else {
		_mtx22.lock();
		std::cout << "\nCounter(ver.2): " << message;
		_mtx22.unlock();
	}

	_busy = iRes;
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