#pragma once
/* Класс описания координаты [ X | Y ] */
class Coord
{
/* typedefs */
private:
	/* Перечисление для описания позиций элементов в исходном значении */
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
	/* Исходное значение */
	int _value;
	/* Массив значений по позициям */
	short _values[Order::MAX];

/* Members */
public:

/* Methods */
private:
	/* Установить значения элементов '0' */
	void initialize0();
	/* Установить значения элементов исходного числа */
	void initialize();
	/* Возвратить сумму элементов значения, начиная с позиции 'order'
	 * - номер позиции
	*/
	int summaAt(int );
	/* Прервать выполнение при ошибке
	 * - позиция элемента при обработке которого произошла ошибка
	*/
	void throwed(const Order& );

/* Methods */
public:
	/* Конструктор - по умолчанию */
	Coord();
	/* Конструктор
	 * - исходное значение для создания текущего объекта
	*/
	Coord(const int& );
	/* Конструктор
	 * - исходный объект для создания текущего объекта
	*/
	Coord(const Coord&);
	/* Деструктор */
	virtual ~Coord();
	/* Возвратить сумму всех элементов значения */
	int Summa();
	/* Возвратить строку с подробным описанием значения */
	std::string Print();

	Coord &Return() const;

	Coord &Next() const;
	/* Создать новый объект на базе указанного в аргументе значения
	 * - объект для создания текущего объекта
	*/
	Coord &operator= (const Coord&) ;
	/* Создать новый объект на базе указанного в аргументе значения
	 * - значение для создания объекта
	*/
	Coord &operator= (const int&) ;
	/* Оператор - инкремент - prefix */
	Coord &operator++ () ;
	/* Оператор - инкремент - postfix */
	Coord operator++ (int) ;
	/* Оператор - инкремент - с шагом */
	Coord operator+= (int);
	/* Оператор - декремент - prefix */
	Coord &operator-- ();
	/* Оператор - декремент - postfix */
	Coord &operator-- (int) ;
	/* Оператор - декремент - с шагом */
	Coord &operator-= (int);
	/* Возвратить исходное значение */
	inline int Value() const { return _value; }
};

