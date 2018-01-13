#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

#include"Interpreter.h"
#include <algorithm>
#include <cctype>

Interpreter interpreter;

//Konstruktor
Interpreter::Interpreter()
{
	A = 0;
	B = 0;
	C = 0;
	D = 0;
	NumerRozkazu = 0;
	LicznikRozkazow = 0;
	LokalizacjaProgramu = -1;
	Program = "";
	Rozkaz = "";
}

//Destruktor
Interpreter::~Interpreter() {};

//Wpisywanie programu z TXT do RAM          
void Interpreter::WpiszDoRam(PCB *pcb, int PamNaDane) {
	std::string program = pcb->dajProgram();
	std::string pom;
	std::string pom2 = "";
	program = program + ".txt";
	std::fstream plik;
	plik.open(program, std::ios::in);
	if (plik.good() == false) {
		std::cout << "Nie ma takiego programu" << std::endl;
	}
	else {
		while (getline(plik, pom)) {
			pom2 = pom2 + pom + '\00';
		}

	}
	ram.addToMem(pcb, pom2, PamNaDane);
}

//Wypisywanie stanow rejestrow aktualnie zapisanych w interpreterze 
void Interpreter::StanRejestrow() {
	std::cout << "A: " << this->A << std::endl;
	std::cout << "B: " << this->B << std::endl;
	std::cout << "C: " << this->C << std::endl;
	std::cout << "D: " << this->D << std::endl;
	std::cout << "Licz: " << this->LicznikRozkazow << std::endl;
	std::cout << "Stan: " << this->StanProcesu << std::endl;
}

//metoda do pobierania pojedynczego rozkazu z ramu
void Interpreter::PobierzRozkaz(PCB *pcb) {
	if (pcb->ramLokalizacja != -1) {
		Rozkaz.clear();
		LicznikRozkazow = kolejkaGotowych.glowa->proces->dajLicznikRozkazow();
		LokalizacjaProgramu = kolejkaGotowych.glowa->proces->dajRamLokalizacja();
		int lokalizacja = pcb->ramLokalizacja + LicznikRozkazow;
		LokalizacjaProgramu = pcb->ramLokalizacja;
		// metoda mariana ktora odsyla std::stringa 
		Rozkaz = ram.showProcess(lokalizacja);
		LicznikRozkazow += Rozkaz.size();
		LicznikRozkazow += 1;
		pcb->ustawLicznikRozkazow(LicznikRozkazow);
	}
}

//Pobieranie Rejestrow od pcb i ustawianie w interpreterze
void Interpreter::PobierzRejestry() {
	A = kolejkaGotowych.glowa->proces->dajRej1();
	B = kolejkaGotowych.glowa->proces->dajRej2();
	C = kolejkaGotowych.glowa->proces->dajRej3();
	D = kolejkaGotowych.glowa->proces->dajRej4();
	LicznikRozkazow = kolejkaGotowych.glowa->proces->dajLicznikRozkazow();
	LokalizacjaProgramu = kolejkaGotowych.glowa->proces->dajRamLokalizacja();
	Program = kolejkaGotowych.glowa->proces->dajProgram();
	MiejsceNaDane = kolejkaGotowych.glowa->proces->dajMiejsceNaDane();
}

//Kopiowanie rejestrow z interpretera do PCB
void Interpreter::UstawRejestr() {
	kolejkaGotowych.glowa->proces->ustawRej1(A);
	kolejkaGotowych.glowa->proces->ustawRej2(B);
	kolejkaGotowych.glowa->proces->ustawRej3(C);
	kolejkaGotowych.glowa->proces->ustawRej4(D);
	kolejkaGotowych.glowa->proces->ustawLicznikRozkazow(LicznikRozkazow);
	kolejkaGotowych.glowa->proces->ustawRamLokalizacja(LokalizacjaProgramu);
	kolejkaGotowych.glowa->proces->ustawProgram(Program);
}

//sprawdzanie czy dany string to liczba
bool Interpreter::CzyStringLiczba(std::string& s)
{
	if (s[0] == '-') {
		s = s.substr(1, s.size() - 1);
	}
	return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
	return true;
}

//poprawnosc komend asemblerowych
bool Interpreter::KOMENDA() {
	std::vector<std::string> komenda;
	komenda.clear();
	//Rozkaz += " ";
	std::string pom = "";
	std::string rozkaz = "";
	pom = Rozkaz;
	pom += " ";
	for (int i = 0; pom.size() > i; i++) {
		if (pom[i] != ' ') {
			rozkaz += pom[i];
		}
		if (pom[i] == ' ') {
			komenda.push_back(rozkaz);
			rozkaz = "";
		}
	}
	if (komenda.size() == 1) {
		if (komenda[0] == "SM") {
			if (CzyStringLiczba(komenda[1])) {
				return true;
			}
			else {
				return false;
			}
			return false;
		}
		else if (komenda[0] == "RM" || komenda[0] == "EX" || komenda[0] == "JP" || komenda[0] == "DS" || komenda[0] == "RP") {
			if (komenda[0] == "JP" && kolejkaGotowych.glowa->proces->dajId() == 1) {
				return true;
			}
			else {
				if (komenda[0] == "RM" || komenda[0] == "EX" || komenda[0] == "DS" || komenda[0] == "RP") {
					return true;
				}
				else {
					return false;
				}

			}
		}
		else {
			return false;
		}
	}
	else if (komenda.size() == 2) {
		if (komenda[0] == "DK") {
			if (komenda[1].size()>0) {
				return true;
			}
			return false;
		}
		if (komenda[0] == "SM") {
			if (CzyStringLiczba(komenda[1])) {
				return true;
			}
			else {
				return false;
			}
			return false;
		}
		else if (komenda[0] == "RM" || komenda[0] == "EX" || komenda[0] == "DS") {
			return false;
		}
		else {
			if (komenda[0] == "IN" || komenda[0] == "DC" || komenda[0] == "CF" || komenda[0] == "CS" || komenda[0] == "BS" || komenda[0] == "RP" || komenda[0] == "JP") {
				if (komenda[0] == "IN") {
					if (komenda[1].size() == 1) {
						if (komenda[1] == "A" || komenda[1] == "B" || komenda[1] == "C" || komenda[1] == "D") {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "DC") {
					if (komenda[1].size() == 1) {
						if (komenda[1] == "A" || komenda[1] == "B" || komenda[1] == "C" || komenda[1] == "D") {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "CF") {
					if (komenda[1].size() >= 3) {
						int kropka = 0;
						for (int i = 0; komenda[1].size() > i; i++) {
							if (komenda[1][i] == '.') {
								kropka++;
							}
						}
						if (kropka == 1) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "CS") {
					if (CzyStringLiczba(komenda[1])) {
						return true;
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "BS") {
					if (CzyStringLiczba(komenda[1])) {
						return true;
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "RP") {
					if (komenda[1].size() >= 3) {
						int kropka = 0;
						for (int i = 0; komenda[1].size() > i; i++) {
							if (komenda[1][i] == '.') {
								kropka++;
							}
						}
						if (kropka == 1) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "JP") {
					if (CzyStringLiczba(komenda[1])) {
						return true;
					}
					else {
						return false;
					}
				}
			}
			else {
				return false;
			}
		}
	}
	else if (komenda.size() == 3) {
		if (komenda[0] == "RR") {
			if (CzyStringLiczba(komenda[1]) && CzyStringLiczba(komenda[2])) {
				return true;
			}
			return false;
		}
		else if (komenda[0] == "RM" || komenda[0] == "EX" || komenda[0] == "JP" || komenda[0] == "DS" || komenda[0] == "IN" || komenda[0] == "DC" || komenda[0] == "CF" || komenda[0] == "CS" || komenda[0] == "BS" || komenda[0] == "RP" || komenda[0] == "JP") {
			return false;
		}
		else {
			if (komenda[0] == "AD" || komenda[0] == "MV" || komenda[0] == "SB" || komenda[0] == "ML" || komenda[0] == "WF" || komenda[0] == "CK" || komenda[0] == "SM" || komenda[0] == "SP") {
				if (komenda[0] == "AD") {
					if (komenda[1] == "A" || komenda[1] == "B" || komenda[1] == "C" || komenda[1] == "D") {
						if (komenda[2].size() == 1) {
							if (komenda[2] == "A" || komenda[2] == "B" || komenda[2] == "C" || komenda[2] == "D" || komenda[2] == "0" || komenda[2] == "1" || komenda[2] == "2" || komenda[2] == "3" || komenda[2] == "4" || komenda[2] == "5" || komenda[2] == "6" || komenda[2] == "7" || komenda[2] == "8" || komenda[2] == "9")
							{
								return true;
							}
							else {
								return false;
							}
						}
						else {
							if (komenda[2][0] == '[' && komenda[2][komenda[2].size() - 1] == ']') {
								if (komenda[2].size() == 2) {
									return false;
								}
								else {
									if (CzyStringLiczba(komenda[2].substr(1, komenda[2].size() - 2))) {
										return true;
									}
									else {
										return false;
									}
								}
							}
							else {
								if (CzyStringLiczba(komenda[2])) {
									return true;
								}
								else {
									return false;
								}
							}
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "MV") {
					if (komenda[1][0] == '[') {
						return true;
					}
					if (komenda[1] == "A" || komenda[1] == "B" || komenda[1] == "C" || komenda[1] == "D") {
						if (komenda[2].size() == 1) {
							if (komenda[2] == "A" || komenda[2] == "B" || komenda[2] == "C" || komenda[2] == "D" || komenda[2] == "0" || komenda[2] == "1" || komenda[2] == "2" || komenda[2] == "3" || komenda[2] == "4" || komenda[2] == "5" || komenda[2] == "6" || komenda[2] == "7" || komenda[2] == "8" || komenda[2] == "9")
							{
								return true;
							}
							else {
								return false;
							}
						}
						else {
							if (komenda[2][0] == '[' && komenda[2][komenda[2].size() - 1] == ']') {
								if (komenda[2].size() == 2) {
									return false;
								}
								else {
									if (CzyStringLiczba(komenda[2].substr(1, komenda[2].size() - 2))) {
										return true;
									}
									else {
										return false;
									}
								}
							}
							else {
								if (CzyStringLiczba(komenda[2])) {
									return true;
								}
								else {
									return false;
								}
							}
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "SB") {
					if (komenda[1] == "A" || komenda[1] == "B" || komenda[1] == "C" || komenda[1] == "D") {
						if (komenda[2].size() == 1) {
							if (komenda[2] == "A" || komenda[2] == "B" || komenda[2] == "C" || komenda[2] == "D" || komenda[2] == "0" || komenda[2] == "1" || komenda[2] == "2" || komenda[2] == "3" || komenda[2] == "4" || komenda[2] == "5" || komenda[2] == "6" || komenda[2] == "7" || komenda[2] == "8" || komenda[2] == "9")
							{
								return true;
							}
							else {
								return false;
							}
						}
						else {
							if (komenda[2][0] == '[' && komenda[2][komenda[2].size() - 1] == ']') {
								if (komenda[2].size() == 2) {
									return false;
								}
								else {
									if (CzyStringLiczba(komenda[2].substr(1, komenda[2].size() - 2))) {
										return true;
									}
									else {
										return false;
									}
								}
							}
							else {
								if (CzyStringLiczba(komenda[2])) {
									return true;
								}
								else {
									return false;
								}
							}
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "ML") {
					if (komenda[1] == "A" || komenda[1] == "B" || komenda[1] == "C" || komenda[1] == "D") {
						if (komenda[2].size() == 1) {
							if (komenda[2] == "A" || komenda[2] == "B" || komenda[2] == "C" || komenda[2] == "D" || komenda[2] == "0" || komenda[2] == "1" || komenda[2] == "2" || komenda[2] == "3" || komenda[2] == "4" || komenda[2] == "5" || komenda[2] == "6" || komenda[2] == "7" || komenda[2] == "8" || komenda[2] == "9")
							{
								return true;
							}
							else {
								return false;
							}
						}
						else {
							if (komenda[2][0] == '[' && komenda[2][komenda[2].size() - 1] == ']') {
								if (komenda[2].size() == 2) {
									return false;
								}
								else {
									if (CzyStringLiczba(komenda[2].substr(1, komenda[2].size() - 2))) {
										return true;
									}
									else {
										return false;
									}
								}
							}
							else {
								if (CzyStringLiczba(komenda[2])) {
									return true;
								}
								else {
									return false;
								}
							}
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "WF") {
					if (komenda[1].size() >= 3) {
						int kropka = 0;
						for (int i = 0; komenda[1].size() > i; i++) {
							if (komenda[1][i] == '.') {
								kropka++;
							}
						}
						if (kropka == 1) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "CK") {
					return true;
				}
				else if (komenda[0] == "SM") {
					if (CzyStringLiczba(komenda[1])) {
						return true;
					}
					else {
						return false;
					}
				}
				else if (komenda[0] == "SP") {
					if (CzyStringLiczba(komenda[1])) {
						if (komenda[2].size() >= 3) {
							int kropka = 0;
							for (int i = 0; komenda[1].size() > i; i++) {
								if (komenda[1][i] == '.') {
									kropka++;
								}
							}
							if (kropka == 1) {
								return true;
							}
							else {
								return false;
							}
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
			}
			else {
				return false;
			}
		}
	}
	else {
		if (komenda[0] == "CP") {
			if (komenda.size() == 4) {
				if (CzyStringLiczba(komenda[3])) {
					return true;
				}
				else {
					int a = std::count(komenda[2].begin(), komenda[2].end(), '.');
					if (a == 1) {
						return true;
					}
					return false;
				}
			}
			else {
				return false;
			}
		}
		else if (komenda[0] == "SM") {
			if (CzyStringLiczba(komenda[1])) {
				return true;
			}
			else {
				return false;
			}
			return false;
		}
	}
}

//Wykonywanie programu
void Interpreter::WykonywanieProgramu() {
	PobierzRejestry();
	if (Program != "" && LokalizacjaProgramu == -1) {
		WpiszDoRam(kolejkaGotowych.glowa->proces, kolejkaGotowych.glowa->proces->dajMiejsceNaDane());
	}
	PobierzRejestry();
	if (LokalizacjaProgramu != -10) {
		if (LokalizacjaProgramu != -1) {
			PobierzRozkaz(kolejkaGotowych.glowa->proces);
			std::cout << " ******************************" << std::endl;
			std::cout << " Proces aktywny: " << kolejkaGotowych.glowa->proces->dajNazwe() << std::endl;
			if (interpreter.LokalizacjaProgramu >= 0) {
				if (interpreter.KOMENDA()) {
					std::cout << " ROZKAZ: " << interpreter.Rozkaz << std::endl;
				}
				else {
					std::cout << " ROZKAZ: " << interpreter.Rozkaz << " -";
					std::cout << " Niepoprawny rozkaz, proces usuniety" << std::endl;
				}
			}
			if (KOMENDA()) {
				if (Rozkaz != "EX") {
					std::string Symbol, Dane;
					if (Rozkaz.size() == 2) {
						Symbol = Rozkaz;
						Dane = "";
					}
					else {
						Symbol = Rozkaz.substr(0, 2); // Rozkaz memoniczny
						Dane = Rozkaz.substr(3, Rozkaz.length() - 3); //Dane od danego Rozkazu
					}
					//Rozkazy Asemblerowe
#pragma region Rozkazy Asemblerowe

#pragma region RozkazyArytmetyczno logiczne
					//Dodawanie
					if (Symbol == "AD") {
						std::string rejestr = Dane.substr(0, 1);
						std::string Pom = Dane.substr(2, Dane.size() - 2);
						//Sumowanie Rejestr Rejestr
						if (rejestr == "A") {
							if (Pom == "B") {
								A += B;
							};
							if (Pom == "C") {
								A += C;
							};
							if (Pom == "D")
							{
								A += D;
							};
							//Sumowanie Rejestr RAM

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									A += stoi(ram.showProcess(Indeks));
								}
								else {
									//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
									A += 0;
								}
							}


							//Sumowanie Rejestr Liczba
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								A += Liczba;
							}
						};
						//Sumowanie Rejestr Rejestr
						if (rejestr == "B") {
							if (Pom == "A") {
								B += A;
							};
							if (Pom == "C") {
								B += C;
							};
							if (Pom == "D")
							{
								B += D;
							};
							//Sumowanie Rejestr RAM
							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									B += stoi(ram.showProcess(Indeks));
								}
								else {
									//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
									B += 0;
								}
							}

							//Sumowanie Rejestr Liczba
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								B += Liczba;
							}
						};
						//Sumowanie Rejestr Rejestr
						if (rejestr == "C") {
							if (Pom == "B") {
								C += B;
							};
							if (Pom == "A") {
								C += A;
							};
							if (Pom == "D")
							{
								C += D;
							};
							//Sumowanie Rejestr RAM
							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									C += stoi(ram.showProcess(Indeks));
								}
								else {
									//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
									C += 0;
								}
							}

							//Sumowanie Rejestr Liczba
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								C += Liczba;
							}
						};
						//Sumowanie Rejestr Rejestr
						if (rejestr == "D")
						{
							if (Pom == "B") {
								D += B;
							};
							if (Pom == "C") {
								D += C;
							};
							if (Pom == "A")
							{
								D += A;
							};
							//Sumowanie Rejestr RAM
							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									D += stoi(ram.showProcess(Indeks));
								}
								else {
									//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
									D += 0;
								}
							}

							//Sumowanie Rejestr Liczba
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								D += Liczba;
							}

						};
					}

					//Wpysiwanie
					else if (Symbol == "MV") {
						if (Dane[0] == 'A' || Dane[0] == 'B' || Dane[0] == 'C' || Dane[0] == 'D') {
							std::string rejestr = Dane.substr(0, 1);
							std::string Pom = Dane.substr(2, Dane.size() - 2);
							//Kopiowanie jednego rejestru do drugiego
							if (Pom[0] == 'A' || Pom[0] == 'B' || Pom[0] == 'C' || Pom[0] == 'D') {
								if (rejestr == "A") {
									if (Pom[0] == 'B') {
										A = B;
									}
									if (Pom[0] == 'C') {
										A = C;
									}
									if (Pom[0] == 'D') {
										A = D;
									}
								}
								if (rejestr == "B") {
									if (Pom[0] == 'A') {
										B = A;
									}
									if (Pom[0] == 'C') {
										B = C;
									}
									if (Pom[0] == 'D') {
										B = D;
									}
								}
								if (rejestr == "C") {
									if (Pom[0] == 'B') {
										C = B;
									}
									if (Pom[0] == 'A') {
										C = A;
									}
									if (Pom[0] == 'D') {
										C = D;
									}
								}
								if (rejestr == "D") {
									if (Pom[0] == 'B') {
										D = B;
									}
									if (Pom[0] == 'C') {
										D = C;
									}
									if (Pom[0] == 'A') {
										D = A;
									}
								}
							}
							else {
								//Wpisywanie liczby do danego rejestru, działa dla ujemnych 
								std::string liczba;
								liczba.clear();
								int Liczba;
								int i = 0;
								while (Pom[i] != '\0') {
									liczba += Pom[i];
									i++;
								}
								Liczba = stoi(liczba);
								if (rejestr == "A") {
									A = Liczba;
								}
								if (rejestr == "B") {
									B = Liczba;
								}
								if (rejestr == "C") {
									C = Liczba;
								}
								if (rejestr == "D") {
									D = Liczba;
								}
							}
						}
						else {
							int i = 1;
							std::string indeks;
							indeks.clear();
							std::string liczba; // CO WPISUJE 
							liczba.clear();
							int Indeks; // POD JAKI INDEKS
							int pom = 0;
							for (i = 1; Dane.size() > i; i++) {
								if (Dane[i] == ']') { pom = 1; i += 2; }
								if (pom == 0) indeks += Dane[i];
								if (pom == 1) liczba += Dane[i];
							}
							Indeks = stoi(indeks);
							if (liczba[0] == '~') {
								if (liczba[1] == 'A') { ram.saveToRam(kolejkaGotowych.glowa->proces, std::to_string(A), Indeks); }
								if (liczba[1] == 'B') { ram.saveToRam(kolejkaGotowych.glowa->proces, std::to_string(B), Indeks); }
								if (liczba[1] == 'C') { ram.saveToRam(kolejkaGotowych.glowa->proces, std::to_string(C), Indeks); }
								if (liczba[1] == 'D') { ram.saveToRam(kolejkaGotowych.glowa->proces, std::to_string(D), Indeks); }
							}
							else {
								ram.saveToRam(kolejkaGotowych.glowa->proces, liczba, Indeks);
							}
						}

					}

					//Zwiekszanie o 1
					else if (Symbol == "IN") {
						if (Dane == "A") {
							A += 1;
						}
						if (Dane == "B") {
							B += 1;
						}
						if (Dane == "C") {
							C += 1;
						}
						if (Dane == "D") {
							D += 1;
						}
					}

					//Zmniejszanie o 1
					else if (Symbol == "DC") {
						if (Dane == "A") {
							A -= 1;
						}
						if (Dane == "B") {
							B -= 1;
						}
						if (Dane == "C") {
							C -= 1;
						}
						if (Dane == "D") {
							D -= 1;
						}
					}

					//Skok warunkowy dopuki D>0
					else if (Symbol == "JP") {
						if (Dane.size() != 0) {
							int skok = stoi(Dane);
							if (D > 0) {
								LicznikRozkazow -= skok;
								D -= 1;
							}
						}
						else {
							LicznikRozkazow -= 3;
						}
					}

					//Odejmowanie
					else if (Symbol == "SB") {
						std::string rejestr = Dane.substr(0, 1);
						std::string Pom = Dane.substr(2, Dane.size() - 2);
						//Odejmowanie Rejestr Rejestr
						if (rejestr == "A") {
							if (Pom == "B") {
								A -= B;
							};
							if (Pom == "C") {
								A -= C;
							};
							if (Pom == "D")
							{
								A -= D;
							};
							//Odejmowanie z rejestru watosci z ramu

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									A -= stoi(ram.showProcess(Indeks));
								}
								else {
									A -= 0;
								}
							}

							//Odejmowanie od rejestru danej liczby 
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								A -= Liczba;
							}
						};
						//Odejmowanie Rejestr Rejestr
						if (rejestr == "B") {
							if (Pom == "A") {
								B -= A;;
							};
							if (Pom == "C") {
								B -= C;
							};
							if (Pom == "D")
							{
								B -= D;
							};
							//Odejmowanie z rejestru watosci z ramu

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									B -= stoi(ram.showProcess(Indeks));
								}
								else {
									B -= 0;
								}
							}

							//Odejmowanie od rejestru danej liczby 
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								B = B - Liczba;
							}

						};
						//Odejmowanie Rejestr Rejestr
						if (rejestr == "C") {
							if (Pom == "B") {
								C -= B;
							};
							if (Pom == "A") {
								C -= A;
							};
							if (Pom == "D")
							{
								C -= D;
							};

							//Odejmowanie z rejestru watosci z ramu

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									C -= stoi(ram.showProcess(Indeks));
								}
								else {
									C -= 0;
								}
							}

							//Odejmowanie od rejestru danej liczby 
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								C -= Liczba;
							}

						};
						//Odejmowanie Rejestr Rejestr
						if (rejestr == "D")
						{
							if (Pom == "B") {
								D -= B;
							};
							if (Pom == "C") {
								D -= C;
							};
							if (Pom == "A")
							{
								D -= A;
							};
							//Odejmowanie z rejestru watosci z ramu

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									D -= stoi(ram.showProcess(Indeks));
								}
								else {
									D -= 0;
								}
							}

							//Odejmowanie od rejestru danej liczby 
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								D -= Liczba;
							}

						};
					}

					//Mnożenie
					else if (Symbol == "ML") {
						std::string rejestr = Dane.substr(0, 1);
						std::string Pom = Dane.substr(2, Dane.size() - 2);
						//Mnozenie Rejestr Rejestr
						if (rejestr == "A") {
							if (Pom == "B") {
								A *= B;
							};
							if (Pom == "C") {
								A *= C;
							};
							if (Pom == "D")
							{
								A *= D;
							};
							//mnozenie rejetr ram

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									A *= stoi(ram.showProcess(Indeks));
								}
								else {
									A = A;
								}
							}

							//Mnozenie rejestr liczba 
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								A *= Liczba;
							}
						};
						//Mnozenie Rejestr Rejestr
						if (rejestr == "B") {
							if (Pom == "A") {
								B *= A;
							};
							if (Pom == "C") {
								B *= C;
							};
							if (Pom == "D")
							{
								B *= D;
							};
							//Mnozenie rejestr ram

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									B *= stoi(ram.showProcess(Indeks));
								}
								else {
									B = B;
								}
							}

							//Mnozenie rejestr liczba 
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								B *= Liczba;
							}
						};
						//Mnozenie Rejestr Rejestr
						if (rejestr == "C") {
							if (Pom == "B") {
								C *= B;
							};
							if (Pom == "A") {
								C *= A;
							};
							if (Pom == "D")
							{
								C *= D;
							};
							//Mnozenie rejestr ram

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									C *= stoi(ram.showProcess(Indeks));
								}
								else {
									C = C;
								}
							}

							//Mnozenie rejestr liczba 
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								C *= Liczba;
							}

						};
						//Mnozenie Rejestr Rejestr
						if (rejestr == "D")
						{
							if (Pom == "B") {
								D *= B;
							};
							if (Pom == "C") {
								D *= C;
							};
							if (Pom == "A")
							{
								D *= A;
							};
							//Mnozenie rejestr ram

							if (Pom[0] == '[') {
								std::string indeks;
								int Indeks;
								int i = 1;
								while (Pom[i] != ']') {
									indeks += Pom[i];
									i++;
								}
								Indeks = stoi(indeks);
								if (CzyStringLiczba(ram.showProcess(Indeks))) {
									D *= stoi(ram.showProcess(Indeks));
								}
								else {
									//////////////////////////////////////////////DO USTALENIA ///////////////////////////////////////////////////
									D = D;
								}
							}

							//Mnozenie rejestr liczba 
							if (Pom[0] == '0' || Pom[0] == '1' || Pom[0] == '2' || Pom[0] == '3' || Pom[0] == '4' || Pom[0] == '5' || Pom[0] == '6' || Pom[0] == '7' || Pom[0] == '8' || Pom[0] == '9') {
								std::string liczba = Dane.substr(2, Dane.size() - 2);
								int Liczba = stoi(liczba);
								D *= Liczba;
							}

						}
					}
#pragma endregion RozkazyArytmetyczno logiczne					

#pragma region TworzeniePlikowNaDysku

					//Tworzenie pliku zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
					else if (Symbol == "CF") {
						std::string nazwa = "";
						std::string rozszerzenie = "";
						int pom = 0;
						for (int i = 0; Dane.size() > i; i++) {
							if (Dane[i] == '.') {
								pom = 1; i++;
							}
							if (pom == 0) {
								nazwa += Dane[i];
							}
							else {
								rozszerzenie += Dane[i];
							}
						}
						dysk.utworzPlik(nazwa, rozszerzenie, kolejkaGotowych.glowa->proces);
						//if (kolejkaGotowych.glowa->proces->dajBlad()) {
						//kolejkaGotowych.usunProces(kolejkaGotowych.glowa->proces->dajId());
						//		}

					}

					//Wpisywanie danych do pliku zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
					else if (Symbol == "WF") {
						std::string nazwa = "";
						std::string rozszerzenie = "";
						std::string dane = "";
						int i = 0;
						int pom = 0;
						for (i = 0; Dane.size() > i; i++) {
							if (Dane[i] == '.') {
								pom = 1; i++;
							}
							if (Dane[i] == ' ') {
								pom = 2; i++;
							}
							if (pom == 0) {
								nazwa += Dane[i];
							}
							if (pom == 1) {
								rozszerzenie += Dane[i];
							}
							if (pom == 2) {
								dane += Dane[i];
							}
						}
						if (dysk.znajdzPlik(nazwa, rozszerzenie) != -1) {
							if (dane[0] == '~') {
								if (dane[1] == 'A') {
									dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(A), kolejkaGotowych.glowa->proces);
								}
								if (dane[1] == 'B') {
									dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(B), kolejkaGotowych.glowa->proces);
								}
								if (dane[1] == 'C') {
									dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(C), kolejkaGotowych.glowa->proces);
								}
								if (dane[1] == 'D') {
									dysk.zapiszDoPliku(nazwa, rozszerzenie, std::to_string(D), kolejkaGotowych.glowa->proces);
								}
							}
							else {
								dysk.zapiszDoPliku(nazwa, rozszerzenie, dane, kolejkaGotowych.glowa->proces);
							}
							if (kolejkaGotowych.glowa->proces->dajBlad() == false) {
								dysk.zamknijPlik(nazwa, rozszerzenie, kolejkaGotowych.glowa->proces);
							}
						}
						else {
							kolejkaGotowych.glowa->proces->ustawBlad(true);
						}
					}

					//usuwanie pliku  zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
					else if (Symbol == "DF") {
						std::string nazwa = "";
						std::string rozszerzenie = "";
						int pom = 0;
						for (int i = 0; Dane.size() > i; i++) {
							if (Dane[i] == '.') {
								pom = 1; i++;
							}
							if (pom == 0) {
								nazwa += Dane[i];
							}
							else {
								rozszerzenie += Dane[i];
							}
						}
						dysk.usunPlik(nazwa, rozszerzenie, kolejkaGotowych.glowa->proces);

					}

					// Tworzenie folderow zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
					else if (Symbol == "CK") {
						std::string folder1 = "";
						std::string folder2 = "Dysk";
						for (int i = 0; Dane.size() > i; i++) {
							if (Dane[i] != ' ') {
								folder1 += Dane[i];
							}
							else {
								folder2 = Dane.substr(i + 1, Dane.size() - i - 1);
								break;
							}
						}
						dysk.utworzFolder(folder1, kolejkaGotowych.glowa->proces, folder2);
					}

					//usuwanie folderow zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
					else if (Symbol == "DK")
					{
						dysk.usunFolder(dysk.znajdzFolder(Dane), kolejkaGotowych.glowa->proces);
					}

#pragma endregion TworzeniePlikowNaDysku

#pragma region KomunikacjaProcesowa
					//Rozkazy do procesow

					//Usuniecie gniazda - wywolywane wraz z zamknieciem procesu
					else if (Symbol == "DS") {
						komunikacja.usunGniazdo(kolejkaGotowych.glowa->proces->dajId());
					}

					//Funkcja przyporządkowuje nowe gniazdo procesowi o podanym ID
					else if (Symbol == "CS") {
						if (CzyStringLiczba(Dane)) {
							int deskryptor = stoi(Dane);
							komunikacja.stworzNoweGniazdo(kolejkaGotowych.glowa->proces->dajId(), deskryptor);
						}
						else {
							kolejkaGotowych.usunProces(kolejkaGotowych.glowa->proces->dajId());
						}
					}

					//Funkcja wiążąca gniazdo o podanym deskryptorze z procesem o podanym ID
					else if (Symbol == "BS") {
						if (CzyStringLiczba(Dane)) {
							int deskryptor = stoi(Dane);
							komunikacja.powiazanieGniazdaZProcesem(kolejkaGotowych.glowa->proces->dajId(), deskryptor);
						}
						else {
							kolejkaGotowych.usunProces(kolejkaGotowych.glowa->proces->dajId());
						}
					}

					//wysylanie wiadomosci
					else if (Symbol == "SM") {
						PCB* procesTymczasowy = kolejkaGotowych.glowa->proces;
						std::string odbiorca = "";
						int a = 0;
						for (int i = 0; i < Dane.size(); i++)
						{
							if (Dane[i] != ' ') {
								odbiorca += Dane[i];
								a++;
							}
							else
								break;
						}
						std::string wys = Dane.substr(a + 1, Dane.size() - a - 1);
						if (CzyStringLiczba(odbiorca)) {
							int deskryptor = stoi(odbiorca);
							komunikacja.rozkazWyslaniaKomunikatu(kolejkaGotowych.glowa->proces->dajId(), deskryptor, wys);
						}
						else {
							kolejkaGotowych.glowa->proces->ustawStatus(4);
							//kolejkaGotowych.usunProces(kolejkaGotowych.glowa->proces->dajId());
						}
					}

					//czytanie wiadomosci 
					else if (Symbol == "RM") {
						PCB* procesTymczasowy = kolejkaGotowych.glowa->proces;
						Wiadomosc odebranaWiadomosc = komunikacja.rozkazOdebraniaKomunikatu(kolejkaGotowych.glowa->proces->dajId());
						//Tutaj trzeba uwzglednic, ze proces moze sie zamknac w wyniku bledu
						//Czyli jezeli nie ma bledu, to:
						if (kolejkaGotowych.glowa->proces->dajBlad() == false) {
							//jesli zwrocona wiadomosc ma nadawce 0
							if (odebranaWiadomosc.pobierzNumerPIDNadawcy() == 0)
							{
								LicznikRozkazow -= 3;
								procesTymczasowy->ustawLicznikRozkazow(LicznikRozkazow);
								NumerRozkazu = 0;
								return;
							}
							//jesli nie ma bledu i nadawca jest rozny od 0 - wyswietl wiadomosc
							else
								std::cout << "Komunikat od procesu o ID: " << odebranaWiadomosc.pobierzNumerPIDNadawcy() << " o tresci: " << odebranaWiadomosc.pobierzKomunikat() << std::endl;
						}
						//Jesli jest blad, to przejdz dalej (jest blad, wiec proces zostanie zamkniety)
					}

					//zapis odczytanej wiadomosci do ram 
					else if (Symbol == "RP") {
						PCB* procesTymczasowy = kolejkaGotowych.glowa->proces;
						Wiadomosc odebranaWiadomosc = komunikacja.rozkazOdebraniaKomunikatu(kolejkaGotowych.glowa->proces->dajId());

						//Tutaj trzeba uwzglednic, ze proces moze sie zamknac w wyniku bledu
						//Czyli jezeli nie ma bledu, to:
						if (kolejkaGotowych.glowa->proces->dajBlad() == false) {
							//jesli zwrocona wiadomosc ma nadawce 0
							if (odebranaWiadomosc.pobierzNumerPIDNadawcy() == 0)
							{
								LicznikRozkazow -= 3;
								procesTymczasowy->ustawLicznikRozkazow(LicznikRozkazow);
								NumerRozkazu = 0;
								return;
							}
							//jesli nie ma bledu i nadawca jest rozny od 0 - wyswietl wiadomosc
							else
							{
								//std::cout << "Komunikat od procesu o ID: " << odebranaWiadomosc.pobierzNumerPIDNadawcy() << " o tresci: " << odebranaWiadomosc.pobierzKomunikat() << std::endl;
								ram.saveToRam(kolejkaGotowych.glowa->proces, odebranaWiadomosc.pobierzKomunikat(), 0);
							}
						}
					}
					//czytanie od danej komorki odpowiedniej liczby komurek
					else if (Symbol == "RR") {
						std::string str, dlu;
						int pom = 0;
						for (int i = 0; Dane.size() > i; i++) {
							if (Dane[i] != ' ') {
								if (pom == 0) {
									str += Dane[i];
								}
								if (pom == 1) {
									dlu += Dane[i];
								}
							}
							else {
								pom++;
							}
						}
						ram.showRangeProcess(stoi(str), stoi(dlu), kolejkaGotowych.glowa->proces);
					}
					//Tworzenie procesu z ptogramem 
					else if (Symbol == "CP") {
						std::string id, prog, dane;
						int pom = 0;
						for (int i = 0; Dane.size() > i; i++) {
							if (Dane[i] != ' ') {
								if (pom == 0) {
									id += Dane[i];
								}
								else if (pom == 1) {
									prog += Dane[i];
								}
								else if (pom == 2) {
									dane += Dane[i];
								}
							}
							else {
								pom++;
							}

						}
						if (zarzadzanieProcesami.znajdzProces(id) == nullptr) {
							zarzadzanieProcesami.dodajProces(id, kolejkaGotowych.glowa->proces->dajNazwe());

							zarzadzanieProcesami.znajdzProces(id)->ustawProgram(prog);
							zarzadzanieProcesami.znajdzProces(id)->ustawMiejsceNaDane(stoi(dane));
							//interpreter.WpiszDoRam(zarzadzanieProcesami.znajdzProces(nazwa), program, rozmiar);
						}
						else {
							kolejkaGotowych.glowa->proces->ustawBlad(true);
						}

					}
#pragma endregion KomunikacjaProcesowa

					else {
						std::clog << "Nie ma takiej komendy." << std::endl;
						kolejkaGotowych.glowa->proces->ustawStatus(4);
						return;
					}
#pragma endregion Rozkazy Asemblerowe

					UstawRejestr();
				}
				else {
					kolejkaGotowych.uruchomEkspedytor(true);
					return;
				}
			}
			else {
				kolejkaGotowych.glowa->proces->ustawStatus(4);
			}
			NumerRozkazu++;
			if (NumerRozkazu == 5) {
				NumerRozkazu = 0;
				kolejkaGotowych.uruchomEkspedytor(false);
			}
			if (kolejkaGotowych.glowa->proces->dajBlad()) {
				kolejkaGotowych.glowa->proces->ustawStatus(4);
			}
			std::clog << std::endl;
		}
		else {
			kolejkaGotowych.glowa->proces->ustawStatus(4);
		}
	}
	else {
		kolejkaGotowych.uruchomEkspedytor(false);
	}
}

