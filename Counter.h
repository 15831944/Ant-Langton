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
	/* �������� ��������� �����(����������� ��� �����) */
	const std::vector<Position::Direct> _vecPossible = { Position::Direct::UP
		, Position::Direct::RIGHT
		, Position::Direct::DOWN
		, Position::Direct::LEFT
	};

	/* ������� ������ ��� ������ ���������� ����������� �������� */
	unsigned short _curIndex;
	/* ��������� ��� �����, �������������� �������� ����������� */
	std::set<std::auto_ptr<Position>> _setBusy;

	std::thread _thread;
	std::list<std::thread> _threads22;
	std::recursive_mutex _mtx22;

/* Members */
public:
	//* ������� ������ �� ����������� ����������� �������� �������� */
	static bool SuccessPrinted;
	/* ������� ������ �� ����������� ����������� ��������� �������� */
	static bool FailPrinted;
	/* ������� ����������������� ������ � ������ SCORE(22) */
	static bool VersionScoreThreaded;

/* Methods */
private:
	/* ���������� ������� ������/������ �������� ���������� ������� �����������
	 * - ������� ������ ��� ��������
	 * - ����������� �����
	 * - ������� ������������� ��������� ������� � �������� ���������� (�����, ��������� �������� �� ���������� ������� �����������)
	*/
	bool isSuccess(Position &, const int &, bool = true) const;
	/* ���������� ��������� ����������� �������� ������� */
	Position::Direct nextDirect();
	/* ��������� ������������ ��������� ������� (�������� ������ ��������� ���������� 'set' - ��� �������)
	 * - ���������� �������-������
	*/
	bool Counter::isUniquePosition(const Position &) ;

	int maxPassed(const int &) const;

	/* �������������� ���������� ������� ��� �����
	 * - ������� �����
	*/
	void Counter::test(const int & );

/* Methods */
public:
	/* ����������� - �� ��������� */
	Counter();

	/* ����������� - � �����������
	 * - ������ ���.
	 * - ��������� ������� �������
	 * - ����������� �� �����
	*/
	Counter(const Arg::Version &, const Position &, const int &);

	/* ����������� - � ���������� (��� ������ Arg::CROSS, ��� ������)
	 * - ������ �������� �����
	*/
	Counter(Position []);
	/* ���������� */
	virtual ~Counter();

	/* ��������� ������� 1-�� ������ ��������� �������� �����-�����
	 * - ����������� �����
	*/
	void threadCounter1(const int &);

	/* ��������� ������� 1-�� ������ ��������� �������� �����-�����
	 * - ����������� �����
	*/
	void threadCounter2(const int &);
	void threadCounter22(const int & , const int &);

	/* ���������� ��������� �������� */
	inline int Result() const { return _setBusy.size(); }

	inline static bool IsThread(const Arg::Version &ver)
	{
		return (ver == Arg::Version::FINDING)
			|| (ver == Arg::Version::SCORE);
	}

	/* ���������� ����� */
	void Stop();
};

