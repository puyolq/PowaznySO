#pragma once
class PCB;
#include "ZarzadzanieProcesami.h"
#include <queue>

class bledneWywolanieWait : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Nieaktywny Proces wykona³ wait().";
	}
};
class bledneWywolanieSignal : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Nieaktywny Proces wykona³ signal().";
	}


};


class Semafory
{
public:
	Semafory();
	~Semafory();
	
	void wait(PCB* proces);
	void signal(PCB* proces);
	int dlugosc();			//sprawdzanie czy otwarty za pomoca tej metody
	 				//<0 to otwarty >=0 zamkniety
private:
	int wartosc;
	std::queue<PCB*> kolejka;

};


