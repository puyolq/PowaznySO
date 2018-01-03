#include "Shell.h"

using namespace std;




Shell::Shell()
{
	clog << "Shell initialized" << endl;

}

void Shell::CP(std::string nazwa, std::string rodzic)
{
	zarzadzanieProcesami->dodajProces(nazwa, rodzic);
}

void Shell::DP(std::string nazwa)
{
	zarzadzanieProcesami->usunProces(nazwa);
}


void Shell::BC(std::string nazwa)
{
	zarzadzanieProcesami->wyswietlProces(nazwa);
}

void Shell::GO()
{
	cout << "robimy GO :)" << endl;
}

void Shell::PP()
{
	zarzadzanieProcesami->wyswietlWszystkieProcesy();
}

void Shell::MC(int adres, int rozmiar)
{
	cout << "do implementacji" << endl;
}

void Shell::MS()
{
	ram.showRam();
}



void Shell::MF(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu)
{
	Dysk.utworzPlik(nazwa, rozszerzenie, nazwaFolderu);
}

void Shell::DF(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu)
{
	Dysk.usunPlik(nazwa, rozszerzenie, nazwaFolderu);
}

void Shell::RF(std::string nazwa, std::string rozszerzenie, std::string nowaNazwa, std::string nazwaFolderu)
{
	Dysk.zmienNazwePliku(nazwa, rozszerzenie, nowaNazwa, nazwaFolderu);
}




void Shell::MD(std::string nazwa, std::string nazwaNadrzednego)
{
	Dysk.utworzFolder(nazwa, nazwaNadrzednego);
}

void Shell::AD(std::string nazwaDocelowego, std::string nazwaPliku, std::string rozszerzenie, std::string nazwaFolderuZPlikiem)
{
	Dysk.dodajPlikDoKatalogu(nazwaDocelowego, nazwaPliku, rozszerzenie, nazwaFolderuZPlikiem);
}

void Shell::DD(std::string nazwa)
{
	int pozycja = Dysk.znajdzFolder(nazwa);
	Dysk.usunFolder(pozycja);
}

void Shell::FD(std::string nazwa)
{
	Dysk.znajdzFolder(nazwa);
}

void Shell::PT()
{
	Dysk.wypiszDrzewo();
}

void Shell::PD()
{
	Dysk.wypiszDysk();
}

void Shell::PV()
{
	Dysk.wypiszWektorBitowy();
}

void Shell::PN()
{
	Dysk.wypiszTabliceIwezelow();
}

void Shell::PX()
{
	Dysk.wypiszTabliceWpisow();
}


void Shell::exit() {
	cout << "wylaczanka";
	wlaczone = false;
}


void Shell::czytajWejscie(std::string wejscie)
{
	istringstream buffer(wejscie);
	istream_iterator<string> beg(buffer), end;
	vector<string>args(beg, end);

	string komenda = args[0];
	//cout << args[0] << endl << args[1] << endl;
	if (komenda == "CP") {
		if (args.size()!=3)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			CP(args[1], args[2]);
	}
	else if (komenda == "DP") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			DP(args[1]);
	}

	else if (komenda == "BC") {
		if (args.size() != 3)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			BC(args[1]);
	}
	else if (komenda == "PP") {
		PP();
	}
	else if (komenda == "MF") {
		if (args.size() <3)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size()!=4)
				args.push_back("Dysk");
			MF(args[1], args[2], args[3]);
		}
	}
	else if (komenda == "DF") {
		if (args.size() <3)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			DF(args[1], args[2], args[3]);
		}
	}
	else if (komenda == "RF") {
		if (args.size() < 4)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			RF(args[1], args[2], args[3], args[4]);
		}
	}

	else if (komenda == "MD") {
		if (args.size() <2)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 3)
				args.push_back("Dysk");
			MD(args[1], args[2]);
		}
	}
	else if (komenda == "AD") {
		if (args.size() <4)
			cout << "niepoprawne uzycie komendy" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			AD(args[1], args[2], args[3], args[4]);
		}
	}
	else if (komenda == "DD") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			DD(args[1]);
	}
	else if (komenda == "FD") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			FD(args[1]);
	}
	else if (komenda == "PT") {
		PT();
	}
	else if (komenda == "PD") {
		PD();
	}
	else if (komenda == "PV") {
		PV();
	}
	else if (komenda == "PN") {
		PN();
	}
	else if (komenda == "PX") {
		PX();
	}
	else if (komenda == "MS") {
		MS();
	}
	else if (komenda == "MC") {
		if (args.size() != 3)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			MC(stoi(args[1]), stoi(args[2]));
	}

	else if (komenda == "open") {
		if (args.size() != 2)
			cout << "niepoprawne uzycie komendy" << endl;
		else
			ladujSkrypt(args[1]);
	}
	else if (komenda == "exit") {
		exit();
	}

	args.clear();

}

void Shell::ladujSkrypt(string nazwa)
{
	string komenda = "";
	skrypt.open(nazwa + ".txt");
	if (!skrypt.good()) {
		cout << "Brak pliku" << endl;
	}
	string linia;
	vector<vector<string>>polecenia;
	vector<string>polecenie;

	while (getline(skrypt, linia)) {
		istringstream buffer(linia);
		istream_iterator<string> beg(buffer), end;
		vector<string>polecenie(beg, end);
		polecenia.push_back(polecenie);
		polecenie.clear();

	}


	for (int i = 0; i < polecenia.size(); i++) {
		for (int j = 0; j < polecenia[i].size(); j++) {

			//cout << polecenia[i][j];
			if (j != polecenia[i].size())
				komenda = komenda + polecenia[i][j] + " ";
			else
				komenda = komenda + polecenia[i][j];
		}
		czytajWejscie(komenda);
		komenda = "";
		//cout << endl;
	}

	polecenia.clear();
	skrypt.close();
}

