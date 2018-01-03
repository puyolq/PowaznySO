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
	int dlugosc();
	bool czyPodniesiony(); // Zwraca true gdy semafor jest podniesiony, false gdy opuszczony;
private:
	int wartosc;
	std::queue<PCB*> kolejka;

};


