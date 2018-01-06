#include <iostream>
#include <string>
#include "Kolejka procesow.hpp"

KolejkaProcesow kolejkaGotowych("gotowych");
KolejkaProcesow kolejkaOczekujacych("oczekujacych");


Wezel* bezczynnosc = new Wezel(&idle);
	
Wezel::Wezel(PCB* proces) : proces(proces), nastepny(NULL){}

Wezel::Wezel(): proces(NULL), nastepny(NULL){}

#pragma region KolejkaProcesow - cia³a metod
KolejkaProcesow::KolejkaProcesow(std::string nazwa):nazwa(nazwa){}

KolejkaProcesow::~KolejkaProcesow(){
	if (glowa && ogon)
		delete glowa, ogon;
}

void KolejkaProcesow::dodajDoKolejki(PCB* proces){
	Wezel* temp1 = new Wezel(proces);
	Wezel* temp2 = glowa;
	if (!glowa || glowa == bezczynnosc){
		glowa = temp1;
		glowa->proces->ustawStatus(3);
		ogon = glowa;
		return;
	}
	ogon->nastepny = temp1;
	ogon = temp1;
}

void KolejkaProcesow::usunProces(const short& idProcesu){
	Wezel* temp1 = glowa;
	for (short i = 0; i < rozmiarKolejki(); i++){
		if (temp1->nastepny) {
			if (temp1->nastepny->proces->dajId() == idProcesu) {
				Wezel* temp2 = temp1->nastepny;
				temp1->nastepny = temp2->nastepny;
				if (temp2 == ogon) ogon = temp1;
				delete temp2;
				return;
			}
			temp1 = temp1->nastepny;
		}
		else {	//jesli jest ogonem
			if (temp1->proces->dajId() == idProcesu) {
				*temp1 = NULL;
				sprawdzBezczynnosc();
				return;
			}
		}
	}
}

void KolejkaProcesow::usunZPoczatku(){
	sprawdzBezczynnosc();
	Wezel* temp1 = glowa;
	if (temp1->nastepny) {
		glowa = temp1->nastepny;
	}
	
}

void KolejkaProcesow::usunZKonca(){
	sprawdzBezczynnosc();
	Wezel* temp1 = glowa;
	while (temp1->nastepny) {
		if(temp1->nastepny)
			temp1 = temp1->nastepny;
	}
	Wezel* temp2 = temp1;
	if(temp1->nastepny)
		temp1 = temp1->nastepny;
	if(temp2->nastepny)
		temp2->nastepny = NULL;
	ogon = temp2;
}

void KolejkaProcesow::sprawdzBezczynnosc(){
	if (!glowa->nastepny) {
		glowa = bezczynnosc;
		glowa->proces->ustawStatus(3);
		ogon = bezczynnosc;
		glowa->nastepny = NULL;
	}
}

void KolejkaProcesow::uruchomEkspedytor(const bool &skonczylSie) {
	if (!glowa->nastepny && glowa!=bezczynnosc) {
		glowa->nastepny = bezczynnosc;
		ogon = bezczynnosc;

	}
	if (skonczylSie) {
		if (glowa != bezczynnosc) {
			glowa->proces->ustawStatus(4);
			usunZPoczatku();
			glowa->proces->ustawStatus(3);
			//stan = zakonczooopooooooopopony
		}
	}
	else {
		if (glowa != ogon) {
			if (glowa->nastepny == bezczynnosc) {
				glowa->nastepny = NULL;
				ogon = glowa;
				return;
			}
			glowa->proces->ustawStatus(1);
			ogon->nastepny = glowa;
			if (glowa->nastepny)
				glowa = glowa->nastepny;
			ogon->nastepny->nastepny = NULL;
			ogon = ogon->nastepny;
			glowa->proces->ustawStatus(3);
			return;
		}
		else {
			glowa = bezczynnosc;
			ogon = bezczynnosc;
		}
	}
//
}

void KolejkaProcesow::wyswietlKolejke(){
	short lp = 1;
	Wezel* temp = glowa;
	std::clog << "Kolejka procesow "<<nazwa<<":" << std::endl;

	while (temp){
		std::clog <<lp<<". PID:"<<temp->proces->dajId() <<" Nazwa:"<< temp->proces->dajNazwe()<< " Stan: "<<temp->proces->dajStatus() <<std::endl;
		temp = temp->nastepny;
		lp++;
	}
	std::clog << std::endl;
}

short KolejkaProcesow::rozmiarKolejki(){
	short licznik = 0;
	Wezel* temp = glowa;
	if (temp == bezczynnosc)
		return 0;
	while (temp) {
		licznik++;
		temp = temp->nastepny;
	}
	return licznik;
}
#pragma endregion

#pragma region Inne funkcje
void zmienKolejke(PCB * proces){
	if (kolejkaGotowych.glowa->proces == proces) {
		kolejkaOczekujacych.dodajDoKolejki(proces);
		kolejkaGotowych.usunZPoczatku();
	}
	else if (kolejkaOczekujacych.glowa->proces == proces) {
		kolejkaGotowych.dodajDoKolejki(proces);
		kolejkaOczekujacych.usunZPoczatku();
	}
}

void zmienKolejke(Wezel * wezel){
	if (kolejkaGotowych.glowa->proces == wezel->proces) {
		kolejkaOczekujacych.dodajDoKolejki(wezel->proces);
		kolejkaGotowych.usunZPoczatku();
	}
	else if (kolejkaOczekujacych.glowa->proces == wezel->proces) {
		kolejkaGotowych.dodajDoKolejki(wezel->proces);
		kolejkaOczekujacych.usunZPoczatku();
	}
}

void uspijProces(PCB *proces) {
	kolejkaGotowych.usunProces(proces->dajId());
	kolejkaOczekujacych.dodajDoKolejki(proces);
	//zmien stan na oczekujacy
}

void uspijProces(Wezel *wezel) {
	kolejkaGotowych.usunProces(wezel->proces->dajId());
	kolejkaOczekujacych.dodajDoKolejki(wezel->proces);
	//zmien stan na oczekujacy
}

void obudzProces(PCB *proces) {
	kolejkaOczekujacych.usunProces(proces->dajId());
	kolejkaGotowych.dodajDoKolejki(proces);
	//zmien stan na gotowy
}

void obudzProces(Wezel *wezel) {
	kolejkaOczekujacych.usunProces(wezel->proces->dajId());
	kolejkaGotowych.dodajDoKolejki(wezel->proces);
	//zmien stan na gotowy
}
#pragma endregion