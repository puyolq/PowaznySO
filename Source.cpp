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
	PCB* kupka = zarzadzanieProcesami.dodajProces("proces1", "init");
	PCB* kupkaa = zarzadzanieProcesami.dodajProces("proces2", "init");
	PCB* kupkaaa = zarzadzanieProcesami.dodajProces("proces3", "init");
	bool x = false;

	//ustawia idle jako proces aktywny, do testów poni¿szego b³êdu
	//while (!x) {
	//	if (kolejkaGotowych.glowa->proces == &idle) {
	//		x = true;
	//		std::cout << "IDLE"<<std::endl;
	//	}
	//	kolejkaGotowych.wyswietlKolejke();
	//	kolejkaGotowych.uruchomEkspedytor(true);
	//}


	//NAPRAWIC ¯EBY INACZEJ SIE WYKONYWALY METODY GDY GLOWA->PROCES == &IDLE || JU¯ SIÊ USUN¥£ (sprawdzanie stanu == 4 nie dzia³a, trzeba by 1 2 3 sprawdzic)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	interpreter.WpiszDoRam(kupka, "prog.txt");	//TUTAJ PROBUJE WPISAC KUPKE, A ONA JEST JUZ USUNIÊTA, METODA WYPIERDALA B£ÊDY
	interpreter.WpiszDoRam(kupkaa, "prog1.txt");
	interpreter.WpiszDoRam(kupkaaa, "prog2.txt");
	std::cout << ".........................";
	ram.showRam();
	//dysk.wypiszDrzewo();
	//interpreter.WykonywanieProgramu();
	dysk.wypiszDrzewo();
	while (kolejkaGotowych.glowa->proces != &idle){
		interpreter.PobierzRozkaz(kolejkaGotowych.glowa->proces);
		std::cout << " ROZKAZ: " << interpreter.Rozkaz << std::endl;
		interpreter.WykonywanieProgramu();
		interpreter.StanRejestrow();
		kolejkaGotowych.wyswietlKolejke();
	}
	std::cout << std::endl << "KONIEC" <<std::endl << std::endl;;
	dysk.wypiszDrzewo();
	std::cout<<std::endl<<dysk.pobierzDane("p1","txt",kolejkaGotowych.glowa->proces);
	//cout << "2:" << endl;
	//ram->showRam();

	std::cin.ignore();
	getchar();

	return 0;
}
