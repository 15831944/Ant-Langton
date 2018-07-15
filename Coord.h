#pragma once
/* ����� �������� ���������� [ X | Y ] */
class Coord
{
/* typedefs */
private:
	/* ������������ ��� �������� ������� ��������� � �������� �������� */
	enum Order
	{
		UNIT
		, DEC
		, HUNDERT
		, TAUSEND
		, MAX
	};

/* Members */
private:
	/* �������� �������� */
	int _value;
	/* ������ �������� �� �������� */
	short _values[Order::MAX];

/* Members */
public:

/* Methods */
private:
	/* ���������� �������� ��������� '0' */
	void initialize0();
	/* ���������� �������� ��������� ��������� ����� */
	void initialize();
	/* ���������� ����� ��������� ��������, ������� � ������� 'order'
	 * - ����� �������
	*/
	int summaAt(int );
	/* �������� ���������� ��� ������
	 * - ������� �������� ��� ��������� �������� ��������� ������
	*/
	void throwed(const Order& );

/* Methods */
public:
	/* ����������� - �� ��������� */
	Coord();
	/* �����������
	 * - �������� �������� ��� �������� �������� �������
	*/
	Coord(const int& );
	/* �����������
	 * - �������� ������ ��� �������� �������� �������
	*/
	Coord(const Coord&);
	/* ���������� */
	virtual ~Coord();
	/* ���������� ����� ���� ��������� �������� */
	int Summa();
	/* ���������� ������ � ��������� ��������� �������� */
	std::string Print();
	/* ������� ����� ������ �� ���� ���������� � ��������� ��������
	 * - ������ ��� �������� �������� �������
	*/
	Coord &operator= (const Coord&) ;
	/* ������� ����� ������ �� ���� ���������� � ��������� ��������
	 * - �������� ��� �������� �������
	*/
	Coord &operator= (const int&) ;
	/* �������� - ��������� - prefix */
	Coord &operator++ () ;
	/* �������� - ��������� - postfix */
	Coord operator++ (int) ;
	/* �������� - ��������� - � ����� */
	Coord operator+= (int);
	/* �������� - ��������� - prefix */
	Coord &operator-- ();
	/* �������� - ��������� - postfix */
	Coord &operator-- (int) ;
	/* �������� - ��������� - � ����� */
	Coord &operator-= (int);
	/* ���������� �������� �������� */
	inline int Value() const { return _value; }
};

