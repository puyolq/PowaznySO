
#include "Semafory.h"

Semafory::Semafory()
{
	wartosc = 1;
}

Semafory::Semafory(int x)
{
	wartosc = x;
}

Semafory::~Semafory()
{
}

void Semafory::wait(PCB * proces)
{
	if (proces->dajStatus() <= 4) {
		if (wartosc>0)
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
	else {

		throw bledneWywolanieWait();
	}

}

void Semafory::signal(PCB * proces)
{
	if (proces->dajStatus() <= 4) {
		if (kolejka.empty())
		{
			++wartosc;
		}
		else
		{
			kolejka.front()->ustawStatus(1);
			kolejka.front()->ustawBlad(0);
			kolejka.pop();
			++wartosc;
		}
	}
	else {
		throw bledneWywolanieSignal(); }
}
//WYJATKI WYRZUCIC
int Semafory::dlugosc()
{
	return (-1)*(this->wartosc);
}