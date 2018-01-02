#include "Semafory.h"

Semafory::Semafory()
{
	wartosc = 1;
}

Semafory::~Semafory()
{
}

void Semafory::czekaj(PCB * proces)
{
	if(wartosc>0)
	{
		--wartosc;
	}
	else
	{
		kolejka.push(proces);
		proces->ustawStatus(2);
		--wartosc;
	}
}

void Semafory::rusz(PCB * proces)
{
	if(kolejka.empty())
	{
		++wartosc;
	}else
	{
		kolejka.front()->ustawStatus(1);
		kolejka.pop();
		++wartosc;
	}
}

int Semafory::dlugosc()
{
	return (-1)*(this->wartosc);
}
