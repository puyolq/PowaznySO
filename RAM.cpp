#include "RAM.hpp"
#include <stdlib.h>
#include <algorithm>
#include <iostream>
RAM ram;

void RAM::memMerge()
{
	for (int i = 0; i<freeBlocks.size() - 1; i++)
	{
		int koniec, poczatek;
		koniec = freeBlocks[i].base + freeBlocks[i].limit;
		poczatek = freeBlocks[i + 1].base;
		if (koniec == poczatek)
		{
			freeBlocks[i].limit += freeBlocks[i + 1].limit;
			freeBlocks.erase(freeBlocks.begin() + i + 1);
			memMerge();
		}
	}
}

void RAM::rewrite(int base1, int base2, int size)
{
	for (int i = 0; i<size; i++)
	{
		RAM_Content[base1 + i] = RAM_Content[base2 + i];
		RAM_Content[base2 + i] = '\0';
	}
}

void RAM::defragment()
{
	std::vector <PCB*> &lista = zarzadzanieProcesami.init->dajPotomkowRAM();
	int base1 = 0;
	if (claimedBlocks[0].base == 0) {// jezeli 1 zajety jest na poczatku
		for (int i = 0; i < claimedBlocks.size() - 1; i++)
		{
			base1 += claimedBlocks[i].limit;
			rewrite(base1, claimedBlocks[i + 1].base, claimedBlocks[i + 1].limit);
			for (auto e : lista) {
				if (e->dajRamLokalizacja() == claimedBlocks[i + 1].base)
				{
					e->ustawRamLokalizacja(claimedBlocks[i].base + claimedBlocks[i].limit);
				}
			}
			claimedBlocks[i + 1].base = claimedBlocks[i].base + claimedBlocks[i].limit;
		}
	}
	else
	{
		lista[0]->ustawRamLokalizacja(0);
		rewrite(0, claimedBlocks[0].base, claimedBlocks[0].limit);
		claimedBlocks[0].base = 0;
		defragment();
	}

	freeBlocks.clear();
	Block tmp;
	tmp.base = claimedBlocks.back().base + claimedBlocks.back().limit;
	tmp.limit = RAM_SIZE - tmp.base;
	freeBlocks.push_back(tmp);//wolny blok pamieci na koncu
}

bool RAM::isFreeBlock(int a)
{
	for (int i = 0; i<freeBlocks.size(); i++)
	{
		if (a <= freeBlocks[i].limit) { return true; }
	}
	return false;
}

void RAM::FBRemove(PCB* a)
{
	int i = 0;
	for (; i<freeBlocks.size(); ++i) { //while nie dzialalo dla 1 elementu bo i tak robil i++
		if (freeBlocks[i].limit >= a->ramRozmiar) {
			break;
		}
	}//rozmiar bloku nie mniejszy niz wymagane
	int base = freeBlocks[i].base;
	int limit = a->ramRozmiar;
	if (freeBlocks[i].limit == limit) { freeBlocks.erase(freeBlocks.begin() + i); }
	else {
		freeBlocks[i].base += limit;
		freeBlocks[i].limit -= limit;
	}
	a->ramLokalizacja = base;//zapisuje polozenie procesu
	Block claimed;
	claimed.base = base;
	claimed.limit = limit;
	claimedBlocks.push_back(claimed);
	std::sort(claimedBlocks.begin(), claimedBlocks.end(), [](const Block &b1, const Block &b2) {return b1.base < b2.base; });
}


void RAM::memWrite(PCB* a, std::string polecenie, int claimed)
{
	if (claimed) { for (int i = 0; i < claimed; i++) { polecenie += '\u5350 '; } }
	FBRemove(a);
	int j = 0;
	
	for (int i = a->ramLokalizacja; i < a->ramRozmiar + a->ramLokalizacja; i++, j++)
	{
		RAM_Content[i] = polecenie[j];
	}
}


int RAM::findStack(int base)
{
	int stack;
	for (int i = 0; i < RAM_SIZE; i++)
	{

		if (RAM_Content[base + i] == 'E' && RAM_Content[base + i + 1] == 'x' && RAM_Content[base + i + 2] == '\00')
		{
			stack = i + 3;
		}
	}
	return stack;
}

///////////////////////////////////////////////
// TU PUBLICZNE MACIE //
///////////////////////////////////////////////




RAM::RAM()
{

	freeRAM = RAM_SIZE;
	processNum = 0;
	for (int i = 0; i < RAM_SIZE; ++i) { RAM_Content[i] = '\0'; } // "pusty" RAM
	Block start;
	start.base = 0;
	start.limit = RAM_SIZE;
	freeBlocks.push_back(start);//tworze blok wolnej pamieci o wielkosci RAM_SIZE
	RAM_Content[RAM_SIZE] = 'J';
	RAM_Content[RAM_SIZE + 1] = 'P';
	RAM_Content[RAM_SIZE + 2] = '\00';

}

void RAM::addToMem(PCB* a, std::string polecenie, int claimedSpace)
{
	if (a->ramRozmiar <= 0) { a->ramRozmiar = polecenie.size() + claimedSpace; }
	if (a->ramRozmiar <= freeRAM)
	{
		if (isFreeBlock(a->ramRozmiar))
		{
			memWrite(a, polecenie,claimedSpace);
			freeRAM -= a->ramRozmiar;
		}
		else
		{
			defragment();
			memWrite(a, polecenie,claimedSpace);
			freeRAM -= a->ramRozmiar;
		}
	}
	else { 
		// TODO SEMAFOR 
	}

}

void RAM::deleteFromMem(PCB* a)
{
	if (a->dajRamLokalizacja() != -1) {
		int i = 0;
		for (; i < claimedBlocks.size(); ++i) {
			if (claimedBlocks[i].base == a->ramLokalizacja) {
				break;
			}
		}
		int limit = claimedBlocks[i].limit;
		int base = claimedBlocks[i].base;
		for (int pos = base; pos < base + limit; pos++)
		{
			RAM_Content[pos] = '\0';
		}//czyszczenie ramu
		claimedBlocks.erase(claimedBlocks.begin() + i);
		Block free;
		free.base = base;
		free.limit = limit;
		freeBlocks.push_back(free);
		freeRAM += limit;

		std::sort(freeBlocks.begin(), freeBlocks.end(), [](const Block &b1, const Block &b2) {return b1.base < b2.base; });
		memMerge();

		// TODO SEMAFOR LECI
	}
}

void RAM::showRam()
{
	//system("cls");
	for (int i = 0; i < TRU_RAM_SIZE; i++)
	{
		if (i % 8 != 0) {
			std::cout << "|" << RAM_Content[i] << "|";
		}
		else { std::cout << std::endl << "|" << RAM_Content[i] << "|"; }
	}
	std::cout << std::endl;
}

void RAM::showRange(int start, int lenght)
{
	std::cout << "|";
	for (int i = start; i<start + lenght; i++)
	{
		std::cout << RAM_Content[i] << "|";
	}
	std::cout << std::endl;
}

std::string RAM::showProcess(int base)
{
	if (base == -1) {
		return "";
	}
	else {
		std::string process = "";

		while (RAM_Content[base] != '\00') { process += RAM_Content[base]; base++; }
		return process;
	}
}

void RAM::saveToRam(PCB* b, std::string a, int localisation)
{
	int stack = findStack(b->dajRamLokalizacja());
	a += '\00';
	if (localisation < b->dajRamLokalizacja() || localisation + a.size() > b->dajRamLokalizacja() + b->dajRamRozmiar())
	{
		// TODO FATAL ERROR !
		return;
	}
	int j = 0;
	for (int g = localisation + b->dajRamLokalizacja() + stack; g < localisation + a.size(); g++, j++)
	{
		RAM_Content[g] = a[j];
	}
}

std::string RAM::memRead(PCB* b, int localisation)
{
	
	if (localisation < b->dajRamLokalizacja() || localisation> b->dajRamLokalizacja() + b->dajRamRozmiar())
	{
		// TODO FATAL ERROR !
		return "";
	}
	int a = findStack(b->dajRamLokalizacja()) + localisation;
	std::string tekst;
	int i = 0;
	while(RAM_Content[i]!='\00')
	{
		tekst += RAM_Content[i];
		++i;
	}
	return tekst;
}

void RAM::printBLOCKS()
{
	std::cout << std::endl << "FREE: ";
	for (auto e : freeBlocks) { std::cout << e.base << " " << e.limit << std::endl; }
	std::cout << std::endl << "TAKEN: ";
	for (auto e : claimedBlocks) { std::cout << e.base << " " << e.limit << std::endl; }
	std::cout << std::endl << "TAKEN: ";
	for (auto e : writtenBlocks) { std::cout << e.base << " " << e.limit << std::endl << e.origin << std::endl; }
}
