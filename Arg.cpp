#include "stdafx.h"
#include <iostream>
#include <string.h>
#include "Arg.h"

#include "Position.h"

/* Конструктор - по умолчанию */
Arg::Arg()
{
	_default = true;

	_limit = 0;
	_position = 0x0;
}

/* Конструктор со значением для разбора
* value - значение для разбора
*/
Arg::Arg(char *value)
	: Arg()
{
	Coord X, Y;
	std::string cc_value;
	char *part;

	_default = false;

	cc_value =
	part =
		value;

	if (std::strlen(value) > 4) {
		// разделитель версии и остальных настроек
		std::strtok(part, ";");
		if (strlen(part) > 0) {
			//std::cout << "\Version:" + std::string(part) + "\n";
			// TODO: проверить на возможность преобразования
			try {
				_version = static_cast<Version>(std::stoi(part));
			} catch (std::exception e) {
				throwed(std::strcat("Version parse: ", part));
			}
			cc_value.erase(0, strlen(part) + 1);
			part = (char *)cc_value.c_str();
			// разделитель позиции X и Y
			std::strtok(part, ","); 
			if (strlen(part) > 0) {
				//std::cout << "\nPositionX:" + std::string(part) + "\n";
				// TODO: проверить на возможность преобразования
				try {
					X = std::stoi(part);
				} catch (std::exception e) {
					throwed(std::strcat("PositionX parse: ", part));
				}
				cc_value.erase(0, strlen(part) + 1);
				part = (char *)cc_value.c_str();

				if (strlen(part) > 0) {
					// разделитель для позиции и ограничения
					std::strtok(part, ";");
					//std::cout << "\nPositionY:" + std::string(part) + "\n";
					// TODO: проверить на возможность преобразования
					try {
						Y = std::stoi(part);
					} catch (std::exception e) {
						throwed(std::strcat("PositionY parse: ", part));
					}
					cc_value.erase(0, strlen(part) + 1);
					part = (char *)cc_value.c_str();
					// ограничение
					if (strlen(part) > 0) {
						std::strtok(part, ";");
						//std::cout << "\nLimit:" + std::string(part) + "\n";
						// TODO: проверить на возможность преобразования
						try {
							_limit = std::stoi(part);

							_position = new Position(X.Value(), Y.Value());
						}
						catch (std::exception e) {
							throwed(std::strcat("Limit parse: ", part));
						}

						cc_value.erase(0, strlen(part) + 1);
						part = (char *)cc_value.c_str();
						// разделитель для отладочных сообщений успеха
						if (strlen(part) > 0) {
							std::strtok(part, ";");
							//std::cout << "\nSuccesPrinted:" + std::string(part) + "\n";
							// TODO: проверить на возможность преобразования
							try {
								_successPrinted = std::stoi(part);
							}
							catch (std::exception e) {
								throwed(std::strcat("SuccessPrinted parse: ", part));
							}
							
							cc_value.erase(0, strlen(part) + 1);
							part = (char *)cc_value.c_str();
							// разделитель для отладочных сообщений ошибок
							if (strlen(part) > 0) {
								std::strtok(part, ";");
								//std::cout << "\nFailPrinted:" + std::string(part) + "\n";
								// TODO: проверить на возможность преобразования
								try {
									_failPrinted = std::stoi(part);
								}
								catch (std::exception e) {
									throwed(std::strcat("FailPrinted parse: ", part));
								}
								
								cc_value.erase(0, strlen(part) + 1);
								part = (char *)cc_value.c_str();
								// разделитель для отладочных сообщений ошибок
								if (strlen(part) == 1) {
									// обработан крайний элемент ком./строки
									//std::strtok(part, ";");
									//std::cout << "\nVersionScoreThreaded:" + std::string(part) + "\n";
									// TODO: проверить на возможность преобразования
									try {
										_versionScoreThreaded = std::stoi(part);
									}
									catch (std::exception e) {
										throwed(std::strcat("VersionScoreThreaded parse: ", part));
									}
									
									// обработан крайний элемент ком./строки
									//cc_value.erase(0, strlen(part) + 1);
									//part = (char *)cc_value.c_str();
								} else
									throwed("VersionScoreThreaded undefined (length not is 1)...");
							} else
								throwed("FailPrinted undefined (length not is 1)...");
						} else
							throwed("SuccessPrinted undefined (length not is 1)...");
					} else
						throwed("Limit undefined (length is 0)...");
				} else
					throwed("PositionY undefined (length is 0)...");
			} else
				throwed("PositionX undefined (length is 0)...");
		} else
			throwed("Version undefined (length is 0)...");
	} else
		throwed(std::strcat("Count delimeters: ", value));
}

Arg::Arg(const Version &ver, const int &xx, const int &yy, const int &limit)
	: Arg ()
{
	_version = ver;
	_limit = limit;
	_position = new Position(xx, yy);
}

/* Деструктор */
Arg::~Arg()
{
	if (!(_position == 0x0))
		delete _position;
	else
		;
}

std::string Arg::toBoolean(const int &value)
{
	return value == 1 ? "true" : "false";
}

std::string Arg::Print()
{
	return _position->Print()
		+ ";\nLimit = " + SSTR(_limit)
		+ ";\nmessages = <success=" + toBoolean(_successPrinted) + ":fail=" + toBoolean(_failPrinted) + ">"
		+ ";\nver_score_thread = " + toBoolean(_versionScoreThreaded);
}

/* Прервать выполнение при ошибке
 * value - значение, которое не удалось распознать
*/
void Arg::throwed(const char *value)
{
	return throw std::runtime_error(std::string("Failed: ") + "from <" + value + ">");
}
