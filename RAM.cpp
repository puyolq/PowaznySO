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
		}
	}
}

void RAM::rewrite(int base1, int base2, int size)
{
	for (int i = 0; i<size; i++)
	{
		RAM_Content[base1 + i] = RAM_Content[base2 + i];
		RAM_Content[base2 + i] = 'w';
	}
}

void RAM::defragment()
{
	int base1 = 0;
	if (claimedBlocks[0].base == 0) {// jezeli 1 zajety jest na poczatku
		for (int i = 0; i < claimedBlocks.size() - 1; i++)
		{
			base1 += claimedBlocks[i].limit;
			rewrite(base1, claimedBlocks[i + 1].base, claimedBlocks[i + 1].limit);
			claimedBlocks[i + 1].base = claimedBlocks[i].base + claimedBlocks[i].limit;
		}
	}
	else
	{
		rewrite(0, claimedBlocks[0].base, claimedBlocks[0].limit);
		claimedBlocks[0].base = 0;
		defragment();
	}

	freeBlocks.clear();
	Block tmp;
	tmp.base = claimedBlocks.back().base + claimedBlocks.back().limit;
	tmp.limit = 256 - tmp.base;
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


void RAM::memWrite(PCB* a, std::string polecenie)
{
	FBRemove(a);
	int j = 0;
	for (int i = a->ramLokalizacja; i < a->ramRozmiar + a->ramLokalizacja; i++, j++)
	{
		RAM_Content[i] = polecenie[j];
	}
}

void RAM::WriteToRam(std::string a, writtenBlock &writtenTo, int localisation)
{

	a += ',';
	int j = 0;
	for (int g = localisation; g < localisation + a.size(); g++, j++)
	{
		RAM_Content[g] = a[j];
	}
}






///////////////////////////////////////////////
// TU PUBLICZNE MACIE //
///////////////////////////////////////////////


RAM::RAM()
{

	freeRAM = 256;
	processNum = 0;
	for (int i = 0; i < 256; ++i) { RAM_Content[i] = 'w'; } // "pusty" RAM
	Block start;
	start.base = 0;
	start.limit = 256;
	freeBlocks.push_back(start);//tworze blok wolnej pamieci o wielkosci 256
}

void RAM::addToMem(PCB* a, std::string polecenie)
{
	if (a->ramRozmiar <= 0) { a->ramRozmiar = polecenie.size(); }
	if (a->ramRozmiar <= freeRAM)
	{
		if (isFreeBlock(a->ramRozmiar))
		{
			memWrite(a, polecenie);
			freeRAM -= a->ramRozmiar;
		}
		else
		{
			defragment();
			memWrite(a, polecenie);
			freeRAM -= a->ramRozmiar;
		}
	}
	else { std::cout << "Brak pamieci"; }

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
			RAM_Content[pos] = 'w';
		}//czyszczenie ramu
		claimedBlocks.erase(claimedBlocks.begin() + i);
		Block free;
		free.base = base;
		free.limit = limit;
		freeBlocks.push_back(free);
		freeRAM += limit;
		for (auto e : writtenBlocks)
		{
			if (e.origin == base)
			{
				base = e.base;
				limit = e.limit;
				for (int pos = base; pos < base + limit; pos++)
				{
					RAM_Content[pos] = 'w';
				}//czyszczenie ramu
				for (i = 0; i < claimedBlocks.size(); i++) { if (claimedBlocks[i].base == base)  break; }
				claimedBlocks.erase(claimedBlocks.begin() + i);
				free.base = base;
				free.limit = limit;
				freeBlocks.push_back(free);
				freeRAM += limit;
				break;
			}
		}

		std::sort(freeBlocks.begin(), freeBlocks.end(), [](const Block &b1, const Block &b2) {return b1.base < b2.base; });
		memMerge();
	}
}

void RAM::showRam()
{
	system("cls");
	for (int i = 0; i < 256; i++)
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

	std::string process = "";

	while (RAM_Content[base] != ',') { process += RAM_Content[base]; base++; }
	return process;
}

void RAM::saveToRam(int a, int localisation, std::string value)
{
	int memClaimed = value.size() + 1;
	int freeBase, freeLimit;
	int freePosition = -1;
	Block freeBlock, claimedBlock;
	for (int i = 0; i< memClaimed; i++)
	{
		if (RAM_Content[localisation + i] != 'w') std::cout << "Pamiec zajeta" << std::endl; return;
	}//sprawdzanie czy mozna zapisac

	 //jesli mozna

	for (auto e : freeBlocks)
	{
		freePosition++;
		if (e.base <= localisation && e.base + e.limit >= localisation + memClaimed)
		{
			freeBase = e.base;
			freeLimit = e.limit;
		}
	}//szukanie w ktorym wolnym bloku to zapiszemy

	freeBlocks.erase(freeBlocks.begin() + freePosition);
	if (freeBase != localisation)
	{
		freeBlock.base = freeBase;
		freeBlock.limit = localisation - freeBase;
		freeBlocks.push_back(freeBlock);
	}

	if (freeBase + freeLimit != localisation + memClaimed)
	{
		freeBlock.base = localisation + memClaimed;
		freeBlock.limit = freeLimit - freeBlock.base;
		freeBlocks.push_back(freeBlock);
	}
	std::sort(freeBlocks.begin(), freeBlocks.end(), [](const Block &b1, const Block &b2) {return b1.base < b2.base; });
	memMerge();
	//dodanie nowych wolnych blokow

	claimedBlock.base = localisation;
	claimedBlock.limit = memClaimed;
	claimedBlocks.push_back(claimedBlock);
	std::sort(claimedBlocks.begin(), claimedBlocks.end(), [](const Block &b1, const Block &b2) {return b1.base < b2.base; });

	writtenBlock writtenTo;
	writtenTo.origin = a;
	writtenTo.limit = value.size() + 1;
	writtenTo.base = localisation;
	writtenBlocks.push_back(writtenTo);
	WriteToRam(value, writtenTo, localisation);

	freeRAM -= memClaimed;
}