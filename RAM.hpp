#pragma once
#include "ZarzadzanieProcesami.h" //Bartosz Ptak
#include <vector>
class RAM {
	char RAM_Content[64]; // caly ram 
	int freeRAM; //dostepna wolna pamiec
	void defragment(); //defragmentacja
	void rewrite(int base1, int base2, int size);
	int processNum;//numer procesu
	struct Block { int base, limit;}; // bloki pamieci
	struct writtenBlock { int base, limit,  origin; }; //wpisane  bloki pamieci
	std::vector<Block> freeBlocks; //lista zajetych blokow
	std::vector<Block> claimedBlocks; //lista zajetych blokow(potrzebne to komu???)
	std::vector<writtenBlock> writtenBlocks;//bloki pamieci
	bool isFreeBlock(int a); // Sprawdza dost�pn� pami�� (blok)
	void FBRemove(PCB*a);//usuniecie wolnego bloku
	void memWrite(PCB* a, std::string polecenie);//Zapisywanie w pamieci
	void RAM::WriteToRam(std::string a, writtenBlock &writtenTo);//wpisywanie do ramu
	void memMerge();//laczy 2 wolne bloki w 1 jezeli sa obok siebie
public:
	RAM(); // konstruktor
	
	void addToMem(PCB*a, std::string polecenie); //dodaj do pamieci
	void deleteFromMem(PCB*a); //usun z pamieci
	void showRam();//wypisz cala zawartosc
	void showRange(int start, int lenght);
	std::string showProcess(int base);
	void saveToRam(PCB*a, int localisation, std::string value);// wpisuje wartosc poczawszy od komorki podanej przez assembler



	///////////// DO WYJEBANIA //////////////////////
	void printBLOCKS();
};