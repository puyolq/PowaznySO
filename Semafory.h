#pragma once

#include "ZarzadzanieProcesami.h"
#include <queue>

class Semafory
{
private:
	int wartosc;	
	std::queue<PCB*> kolejka;

public:
	Semafory();
	~Semafory();
	
	void czekaj(PCB* proces);
	void rusz(PCB* proces);
	int dlugosc();
};


