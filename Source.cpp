#include <iostream>
#include <string>
#include <Windows.h>
#include "Interpreter.h"
#include "Kolejka procesow.hpp"
#include "RAM.hpp"
#include "ZarzadzanieProcesami.h"
#include "dysk.h"
#include "Semafory.h"


int main()
{
	Dysk *dysk = new Dysk;
	RAM *ram = new RAM;
	ZarzadzanieProcesami zarzadzanie;
	Interpreter interpreter(dysk, ram);
	PCB* kupka = zarzadzanie.dodajProces("proces1", "init");
	PCB* kupkaa = zarzadzanie.dodajProces("proces2", "init");
	PCB* kupkaaa = zarzadzanie.dodajProces("proces3", "init");
	interpreter.WpiszDoRam(kupka, "prog.txt");
	interpreter.WpiszDoRam(kupkaa, "prog1.txt");
	interpreter.WpiszDoRam(kupkaaa, "prog2.txt");
	std::cout << ".........................";
	ram->showRam();
	//dysk.wypiszDrzewo();
	//interpreter.WykonywanieProgramu();
	dysk->wypiszDrzewo();
	do {
		interpreter.PobierzRozkaz(kolejkaGotowych.glowa->proces);
		std::cout << " ROZKAZ: " << interpreter.Rozkaz << std::endl;
		interpreter.WykonywanieProgramu();
		interpreter.StanRejestrow();
		kolejkaGotowych.wyswietlKolejke();
	} while (kolejkaGotowych.glowa->proces->dajId() != 99);
	std::cout << std::endl << "KONIEC" <<std::endl << std::endl;;
	dysk->wypiszDrzewo();
	std::cout<<std::endl<<dysk->pobierzDane("p1","txt",kolejkaGotowych.glowa->proces);
	//cout << "2:" << endl;
	//ram->showRam();

	std::cin.ignore();
	getchar();

	return 0;
}
