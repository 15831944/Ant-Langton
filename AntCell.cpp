// AntCell.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <thread>

#include "Counter.h"
#include "Arg.h"

int main(int argc, char* argv[])
{
	Position arrayPos[Position::Direct::MAX];
	// TODO: std::auto_ptr<Counter>
	std::unique_ptr<Counter>counter;
	// TODO: std::auto_ptr<Arg>
	Arg *arg = 0x0;

	std::thread _thread;

	try {
		if (argc > 1) {
			//std::cout << argv[1] << std::endl;
			arg = new Arg(argv[1]);
			std::cout << "Argument command line:\n" + arg->Print() + "\n";
		} else {
			arg = new Arg(Arg::Version::SCORE, 1000, 1000, 25);
		}

		Counter::SuccessPrinted = arg->ResultSuccessPrinted();
		Counter::FailPrinted = arg->ResultFailPrinted();

		if (Counter::IsThread(arg->ResultVersion()) == true) {
			// для режима 'FINDING' не применяется
			Counter::VersionScoreThreaded = arg->ResultVersionScoreThreaded();

			// отображаем раньше потока(Counter::ctor), чтобы запись не появилась в середине отладочного вывода
			std::cout << "\nPress any key to exit...\n";

			counter = std::unique_ptr<Counter> (new Counter(arg->ResultVersion(), arg->ResultPositon(), arg->ResultLimit()));
		} else {
			switch (arg->ResultVersion()) {
				case Arg::Version::CROSS:
					for (int direct = Position::Direct::MAX - 1; !(direct < 0); direct--) {
						arrayPos[direct] = arg->ResultPositon();

						std::cout << "\nPlay to <" + SSTR(direct) + ":" + SSTR(arg->ResultLimit()) + ">...\n";
						arrayPos[direct].Play(static_cast<Position::Direct>(direct), arg->ResultLimit());
						std::cout << arrayPos[direct].Print() << "\n";
					}

					counter = std::unique_ptr<Counter>(new Counter(arrayPos));
					break;
				case Arg::Version::TEST:
					counter = std::unique_ptr<Counter>(new Counter(arg->ResultVersion(), arg->ResultPositon(), arg->ResultLimit()));
					break;
				default:
					break;
			}
		}

		if (Counter::IsThread(arg->ResultVersion()) == false)
			// для этой версии приглашение еще не отображается
			std::cout << "\nPress any key to exit...\n";
		else
			;
		_getch();

		if (Counter::IsThread(arg->ResultVersion()) == true)
			counter->Stop();
		else
			;
		std::cout << "\nResult <" << SSTR(counter->ResultPassed()) << ":" << SSTR(counter->ResultTrack()) << ">...\n";

		// TODO: auto_ptr<Arg>
		if (!(arg == 0x0))
			delete arg;
		else
			;
		//// TODO: unique_ptr<Counter>
		//if (!(counter == 0x0))
		//	delete counter;
		//else
		//	;
	}
	catch (std::exception e) {
		std::cout << e.what() << "\n";
	}

    return 0;
}
