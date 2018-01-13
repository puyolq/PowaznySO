#pragma once
#include "ZarzadzanieProcesami.h" //Bartosz Ptak
#include <vector>
#define RAM_SIZE 125
#define TRU_RAM_SIZE RAM_SIZE + 3
class RAM {

	//PCB *pcb = new PCB();
	char RAM_Content[TRU_RAM_SIZE]; // caly ram 
	int freeRAM; //dostepna wolna pamiec
	void defragment(); //defragmentacja
	void rewrite(int base1, int base2, int size);
	int processNum;//numer procesu
	struct Block { int base, limit; }; // bloki pamieci
	struct writtenBlock { int base, limit, origin; }; //wpisane  bloki pamieci
	std::vector<Block> freeBlocks; //lista zajetych blokow
	std::vector<Block> claimedBlocks; //lista zajetych blokow(potrzebne to komu???)
	std::vector<writtenBlock> writtenBlocks;//bloki pamieci
	bool isFreeBlock(int a); // Sprawdza dostêpn¹ pamiêæ (blok)
	void FBRemove(PCB*a);//usuniecie wolnego bloku
	void memWrite(PCB* a, std::string polecenie, int claimed);//Zapisywanie w pamieci
	void memMerge();//laczy 2 wolne bloki w 1 jezeli sa obok siebie
	Semafory semaforRAM;
	int findStack(int base);
public:
	RAM(); // konstruktor

	void addToMem(PCB*a, std::string polecenie, int claimedSpace); //dodaj do pamieci
	void deleteFromMem(PCB*a); //usun z pamieci
	void showRam();//wypisz cala zawartosc
	void showRange(int start, int lenght);
	void showRangeProcess(int start, int lenght, PCB* b);
	std::string showProcess(int base);
	void saveToRam(PCB* b, std::string a, int localisation);// wpisuje wartosc poczawszy od komorki podanej przez assembler do pola przydzielonego (od 1)
	std::string memRead(PCB* b, int localisation); // zwraca komorke pamieci przydzielonej programowi( numerowanie od 0)


												   ///////////// DO WYJEBANIA //////////////////////
	void printBLOCKS();
};

extern RAM ram;
