#pragma once

#include "ZarzadzanieProcesami.h"
#include <queue>

class Semafory
{
public:
	Semafory();
	~Semafory();
	
	void czekaj(PCB* proces);
	void rusz(PCB* proces);
	int dlugosc();			//sprawdzanie czy otwarty za pomoca tej metody
	 				//<0 to otwarty >=0 zamkniety
private:
	int wartosc;
	std::queue<PCB*> kolejka;

};


