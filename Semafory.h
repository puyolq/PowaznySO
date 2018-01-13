#pragma once
class PCB;
#include "ZarzadzanieProcesami.h"
#include <queue>
#include <exception>

class bledneWywolanieWait : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Nieaktywny Proces wykonał wait()";
	}
};
class bledneWywolanieSignal : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Nieaktywny Proces wykonał signal()";
	}


};

class Semafory
{
public:
	Semafory();
	Semafory(int x);
	~Semafory();

	void wait(PCB* proces);
	void signal(PCB* proces);
	int dlugosc();			//sprawdzanie czy otwarty za pomoca tej metody
							//<0 to otwarty >=0 zamknietyy
private:
	int wartosc;
	std::queue<PCB*> kolejka;

};


