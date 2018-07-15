#pragma once

#include <stdlib.h>
#include <algorithm>

class Position;

/* ����� ��� ������� ��������� ������: ������� ������� � �����������
 * ������ ���������: [XXXX,YYYY];NN
*/
class Arg
{
public:

	enum Version { TEST= 0, FINDING, SCORE, CROSS };

/* Members */
private:

	bool _default;

	Version _version;

	Position *_position;

	int _limit;

	int _successPrinted
		, _failPrinted
		, _versionScoreThreaded;

/* Methods */
private:
	/* ����������� - �� ��������� */
	Arg();
	/* �������� ���������� ��� ������
	 * - ��������, ������� �� ������� ����������
	*/
	void throwed(const char *);

/* Methods */
public:
	/* ����������� �� ��������� ��� �������
	 * - �������� ��� �������
	*/
	Arg(char *);

	Arg(const Version &, const int &, const int &, const int &);
	/* ���������� */
	~Arg();

	inline Version ResultVersion() { return _version; }

	inline Position &ResultPositon() { return *_position; }

	inline int ResultLimit() { return _limit; }	

	inline bool ResultSuccessPrinted() { return _successPrinted == 1; }

	inline bool ResultFailPrinted() { return _failPrinted == 1; }

	inline bool ResultVersionScoreThreaded() { return _versionScoreThreaded == 1; }

	std::string Print();
};

