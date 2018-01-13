#include "Shell.h"

using namespace std;




Shell::Shell()
{
	clog << "Shell initialized" << endl;
	idle.ustawStatus(3);
	kolejkaGotowych.dodajDoKolejki(&idle);
}

void Shell::CP(std::string nazwa, std::string rodzic, std::string program, int rozmiar)
{
	if (zarzadzanieProcesami.znajdzProces(nazwa) == nullptr) {
		zarzadzanieProcesami.dodajProces(nazwa, rodzic);

		if (program == "") {
			;
		}
		else {
			zarzadzanieProcesami.znajdzProces(nazwa)->ustawProgram(program);
			zarzadzanieProcesami.znajdzProces(nazwa)->ustawMiejsceNaDane(rozmiar);
			//interpreter.WpiszDoRam(zarzadzanieProcesami.znajdzProces(nazwa), program, rozmiar);
		}
	}
	else std::clog << "Proces istnieje" << std::endl;
}

void Shell::DP(std::string nazwa)
{
	if (nazwa == "init") {
		clog << "Zakaz usuwania procesu init" << endl;
	}
	else if (zarzadzanieProcesami.znajdzProces(nazwa) == nullptr)
		clog << "Proces nie istnieje" << endl;
	else {
		if (zarzadzanieProcesami.znajdzProces(nazwa)->dajDeskryptorGniazda() != 0)
		{
			Gniazdo* wskaznikNaGniazdo = komunikacja.znajdzGniazdoWSpisie(zarzadzanieProcesami.znajdzProces(nazwa)->dajDeskryptorGniazda());
			if (wskaznikNaGniazdo->czyWlascicielZablokowany() == true)
				wskaznikNaGniazdo->odblokujWlasciciela();
			komunikacja.usunGniazdo(zarzadzanieProcesami.znajdzProces(nazwa)->dajId());
		}
		zarzadzanieProcesami.usunProces(nazwa);
	}
}

void Shell::BC(std::string nazwa)
{
	if (zarzadzanieProcesami.znajdzProces(nazwa) != nullptr)
		zarzadzanieProcesami.wyswietlProces(nazwa);
	else
		clog << "Proces o danej nazwie nie istnieje" << endl;
}

void Shell::MP(std::string co, std::string dokad)
{
	if (zarzadzanieProcesami.znajdzProces(co) != nullptr && zarzadzanieProcesami.znajdzProces(dokad) != nullptr) {
		zarzadzanieProcesami.przeniesProces(co, dokad);
	}
	else
		clog << "Jeden z procesow lub oba nie istnieja" << endl;
}

void Shell::GO()
{
	//if (zarzadzanieProcesami.iloscProcesow() != 1 && kolejkaGotowych.rozmiarKolejki()>0) {

	//interpreter.PobierzRozkaz(kolejkaGotowych.glowa->proces);
	//if (interpreter.Rozkaz != "") {
	//}
	interpreter.WykonywanieProgramu();

//	}
	//else std::cout << "Nie ma zaladowanego programu" << std::endl;
	//interpreter.StanRejestrow();
}

void Shell::PP()
{
	zarzadzanieProcesami.wyswietlWszystkieProcesy();
}

void Shell::MC(int adres, int rozmiar)
{
	ram.showRange(adres, rozmiar);
}

void Shell::MS()
{
	ram.showRam();
}

void Shell::MF(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu, PCB* proces)
{
	dysk.utworzPlik(nazwa, rozszerzenie, proces, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
}

void Shell::DF(std::string nazwa, std::string rozszerzenie, std::string nazwaFolderu, PCB* proces)
{
	dysk.usunPlik(nazwa, rozszerzenie, proces, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
}

void Shell::RF(std::string nazwa, std::string rozszerzenie, std::string nowaNazwa, std::string nazwaFolderu, PCB* proces)
{
	dysk.zmienNazwePliku(nazwa, rozszerzenie, proces, nowaNazwa, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
}

void Shell::SF(std::string nazwa, std::string rozszerzenie, std::string dane, PCB * proces, std::string nazwaFolderu)
{
	dysk.zapiszDoPliku(nazwa, rozszerzenie, dane, proces, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
	dysk.zamknijPlik(nazwa, rozszerzenie, proces, nazwaFolderu);
}

void Shell::PF(std::string nazwa, std::string rozszerzenie, PCB * proces, std::string nazwaFolderu)
{

	string wyj = dysk.pobierzDane(nazwa, rozszerzenie, proces, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
	cout << wyj << endl;
	dysk.zamknijPlik(nazwa, rozszerzenie, proces, nazwaFolderu);
}

void Shell::PE(std::string nazwa, std::string rozszerzenie, PCB* proces, int pozycjaStartowa, int ileZnakow, std::string nazwaFolderu)
{
	string wyj = dysk.pobierzWycinekDanych(nazwa, rozszerzenie, proces, pozycjaStartowa, ileZnakow, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
	cout << wyj << endl;
	dysk.zamknijPlik(nazwa, rozszerzenie, proces, nazwaFolderu);
}

void Shell::XF(std::string nazwa, std::string rozszerzenie, PCB * proces, std::string nazwaFolderu)
{
	dysk.otworzStratnie(nazwa, rozszerzenie, proces, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
	dysk.zamknijPlik(nazwa, rozszerzenie, proces, nazwaFolderu);
}

void Shell::OF(std::string nazwa, std::string rozszerzenie, PCB * proces, std::string nazwaFolderu)
{

	string wyj = dysk.pobierzDane(nazwa, rozszerzenie, proces, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
	cout << wyj << endl;
}

void Shell::CF(std::string nazwa, std::string rozszerzenie, PCB * proces, std::string nazwaFolderu)
{
	dysk.zamknijPlik(nazwa, rozszerzenie, proces, nazwaFolderu);
	obsluzBledy(proces->dajBlad());
}

void Shell::MD(std::string nazwa, std::string nazwaNadrzednego, PCB* proces)
{
	dysk.utworzFolder(nazwa, proces, nazwaNadrzednego);
	obsluzBledy(proces->dajBlad());
}

void Shell::AD(std::string nazwaDocelowego, std::string nazwaPliku, std::string rozszerzenie, std::string nazwaFolderuZPlikiem, PCB* proces)
{
	dysk.dodajPlikDoKatalogu(nazwaDocelowego, nazwaPliku, proces, rozszerzenie, nazwaFolderuZPlikiem);
	obsluzBledy(proces->dajBlad());
}

void Shell::DD(std::string nazwa, PCB* proces)
{
	int pozycja = dysk.znajdzFolder(nazwa);
	if (pozycja == -1) {
		clog << "Folder nie istnieje" << endl;
	}
	else {
		dysk.usunFolder(pozycja, proces);
		obsluzBledy(proces->dajBlad());
	}
}

void Shell::PT()
{
	dysk.wypiszDrzewo();
}

void Shell::PD()
{
	dysk.wypiszDysk();
}

void Shell::PV()
{
	dysk.wypiszWektorBitowy();
}

void Shell::PN()
{
	dysk.wypiszTabliceIwezelow();
}

void Shell::PX()
{
	dysk.wypiszTabliceWpisow();
}

void Shell::RQ()
{
	kolejkaGotowych.wyswietlKolejke();
}

void Shell::WQ()
{
	kolejkaOczekujacych.wyswietlKolejke();
}

void Shell::RB()
{
	ram.printBLOCKS();
}

void Shell::PS()
{
	komunikacja.wyswietlSpisWszystkichGniazd();
}

void Shell::help()
{
	komendy.open("help.txt");
	if (!komendy.good()) {
		cout << "Brak pliku z komendami" << endl;
	}
	string linia;
	clog << "______________________SPIS KOMEND______________________" << endl << endl;
	while (getline(komendy, linia)) {
		clog << linia << endl;
	}

	komendy.close();
}

void Shell::exit() {
	cout << "Wylaczanie.";
	wlaczone = false;
}

void Shell::czytajWejscie(std::string wejscie)
{
	istringstream buffer(wejscie);
	istream_iterator<string> beg(buffer), end;
	vector<string>args(beg, end);
	if (wejscie.size() == 0) {
		return;
	}
	//string komenda = args[0];
	string komenda;
	int i = 0;
	while (args[0][i])
	{
		komenda += toupper(args[0][i]);
		i++;
	}

	if (komenda == "CP") {
		if (args.size() < 2 || args.size() > 5)
			cout << "Niepoprawne uzycie komendy! CP nazwa *rodzic **program ***rozmiarNaDane" << endl;
		else {
			//ustalamy rodzica
			if (args.size() <3) {
				args.push_back("init");
			}
			//program ""
			if (args.size() < 4)
				args.push_back("");
			if (args.size() < 5) {
				args.push_back("0");
			}
			if (args.size() == 5 && !czyStringLiczba(args[4]))
			{
				clog << "Niepoprawne uzycie komendy! CP nazwa *rodzic **program ***rozmiarNaDane" << endl;
			}
			else {
				if (stoi(args[4])<RAM_SIZE - 2)
					CP(args[1], args[2], args[3], stoi(args[4]));
				else
					clog << "Niepoprawne uzycie komendy! CP nazwa *rodzic **program ***rozmiarNaDane" << endl;
			}
		}
	}
	else if (komenda == "DP") {
		if (args.size() != 2)
			cout << "Niepoprawne uzycie komendy! DP nazwa" << endl;
		else
			DP(args[1]);
	}
	else if (komenda == "BC") {
		if (args.size() != 2)
			cout << "Niepoprawne uzycie komendy! BC nazwa" << endl;
		else
			BC(args[1]);
	}
	else if (komenda == "PP") {
		if (args.size() != 1)
			cout << "Niepoprawne uzycie komendy! PP" << endl;
		else
			PP();
	}
	else if (komenda == "MF") {
		if (args.size() < 3 || args.size() > 4)
			cout << "Niepoprawne uzycie komendy! MF nazwa rozszerzenie *folder" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			CP("obslugaPliku", "init", "", 0);
			MF(args[1], args[2], args[3], zarzadzanieProcesami.znajdzProces("obslugaPliku"));
			DP("obslugaPliku");
		}
	}
	else if (komenda == "DF") {
		if (args.size() < 3 || args.size()>4)
			cout << "Niepoprawne uzycie komendy! DF nazwa rozszerzenie *folder" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			if (dysk.znajdzPlik(args[1], args[2]) == -1) {
				clog << "Nie znaleziono pliku." << endl;
				return;
			}
			else {
				CP("obslugaPliku", "init", "", 0);
				DF(args[1], args[2], args[3], zarzadzanieProcesami.znajdzProces("obslugaPliku"));
				DP("obslugaPliku");
			}
		}
	}
	else if (komenda == "RF") {
		if (args.size() < 4 || args.size()>5)
			cout << "Niepoprawne uzycie komendy! RF nazwa rozszerzenie nowaNazwa *folder" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			if (dysk.znajdzPlik(args[1], args[2]) == -1) {
				clog << "Nie znaleziono pliku." << endl;
				return;
			}
			else {
				CP("obslugaPliku", "init", "", 0);
				RF(args[1], args[2], args[3], args[4], zarzadzanieProcesami.znajdzProces("obslugaPliku"));
				DP("obslugaPliku");
			}
		}
	}
	else if (komenda == "MD") {
		if (args.size() < 2 || args.size()>3)
			cout << "Niepoprawne uzycie komendy! MD nazwa *nazwaNadrzednego" << endl;
		else {
			if (args.size() != 3)
				args.push_back("Dysk");
			CP("obslugaPliku", "init", "", 0);
			MD(args[1], args[2], zarzadzanieProcesami.znajdzProces("obslugaPliku"));
			DP("obslugaPliku");
		}
	}
	else if (komenda == "AD") {
		if (args.size() < 4 || args.size()>5)
			cout << "Niepoprawne uzycie komendy! AD nazwaDocelowego nazwaPliku rozszerzenie *nazwaFolderuZPlikiem" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			CP("obslugaPliku", "init", "", 0);
			AD(args[1], args[2], args[3], args[4], zarzadzanieProcesami.znajdzProces("obslugaPliku"));
			DP("obslugaPliku");
		}
	}
	else if (komenda == "DD" || komenda == "RD") {
		if (args.size() != 2)
			cout << "Niepoprawne uzycie komendy! DD nazwa" << endl;
		else if (args[1] == "Dysk") {
			clog << "Nie mozna usunac katalogu glownego" << endl;
		}
		else {
			CP("obslugaPliku", "init", "", 0);
			DD(args[1], zarzadzanieProcesami.znajdzProces("obslugaPliku"));
			DP("obslugaPliku");
		}
	}
	else if (komenda == "PT" || komenda == "DIR") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! PT" << endl;
		}
		else
			PT();
	}
	else if (komenda == "ECHO")
	{
		if (args.size() == 1)
		{
			clog << "ECHO is on." << endl;
		}
		else if (args.size() >= 2)
		{
			for (int z = 1; z < args.size(); z++)
				if (z == args.size() - 1)
					clog << args[z] << endl;
				else
					clog << args[z] << " ";
		}
	}
	else if (komenda == "PD") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! PD" << endl;
		}
		else
			PD();
	}
	else if (komenda == "PV") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! PV" << endl;
		}
		else
			PV();
	}
	else if (komenda == "PN") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! PN" << endl;
		}
		else
			PN();
	}
	else if (komenda == "PX") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! PX" << endl;
		}
		else
			PX();
	}
	else if (komenda == "MS") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! MS" << endl;
		}
		else
			MS();
	}
	else if (komenda == "MC") {
		if (args.size() != 3)
			cout << "Niepoprawne uzycie komendy! MC adres rozmiar" << endl;
		else {
			if (czyStringLiczba(args[1]) && czyStringLiczba(args[2]))
				MC(stoi(args[1]), stoi(args[2]));
			else
			{
				cout << "Niepoprawne uzycie komendy! MC adres rozmiar" << endl;
			}
		}
	}
	else if (komenda == "LS") {
		if (args.size() != 2)
			cout << "Niepoprawne uzycie komendy! LS nazwaSkryptu" << endl;
		else
			ladujSkrypt(args[1]);
	}
	else if (komenda == "EXIT") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! exit" << endl;
		}
		else
			exit();
	}
	else if (komenda == "SF") {
		if (args.size() < 4 || args.size()>5)
			cout << "Niepoprawne uzycie komendy! SF nazwa rozszerzenie dane *folder" << endl;
		else {
			if (args.size() != 5)
				args.push_back("Dysk");
			if (dysk.znajdzPlik(args[1], args[2]) == -1) {
				clog << "Nie znaleziono pliku." << endl;
				return;
			}
			else {
				CP("obslugaPliku", "init", "", 0);
				SF(args[1], args[2], args[3], zarzadzanieProcesami.znajdzProces("obslugaPliku"), args[4]);

				DP("obslugaPliku");
			}

		}
	}
	else if (komenda == "PF") {
		if (args.size() < 3 || args.size()>4)
			cout << "Niepoprawne uzycie komendy! PF nazwa rozszerzenie *folder" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			if (dysk.znajdzPlik(args[1], args[2]) == -1) {
				clog << "Nie znaleziono pliku." << endl;
				return;
			}
			else {
				CP("obslugaPliku", "init", "", 0);
				PF(args[1], args[2], zarzadzanieProcesami.znajdzProces("obslugaPliku"), args[3]);

				DP("obslugaPliku");
			}
		}
	}
	else if (komenda == "PE")
	{
		if (args.size() < 5 || args.size()>6)
			cout << "Niepoprawne uzycie komendy! PE nazwa rozszerzenie pozycjaStartowa ileZnakow *folder" << endl;
		else {
			if (args.size() != 6)
				args.push_back("Dysk");
			if (dysk.znajdzPlik(args[1], args[2]) == -1) {
				clog << "Nie znaleziono pliku." << endl;
				return;
			}
			else {
				if (czyStringLiczba(args[3]) && czyStringLiczba(args[4])) {
					CP("obslugaPliku", "init", "", 0);
					PE(args[1], args[2], zarzadzanieProcesami.znajdzProces("obslugaPliku"), stoi(args[3]), stoi(args[4]), args[5]);

					DP("obslugaPliku");
				}
				else
					cout << "Niepoprawne uzycie komendy! PE nazwa rozszerzenie pozycjaStartowa ileZnakow *folder" << endl;
			}
		}
	}
	else if (komenda == "XF") {
		if (args.size() < 3 || args.size()>4)
			cout << "Niepoprawne uzycie komendy! XF nazwa rozszerzenie *folder" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			if (dysk.znajdzPlik(args[1], args[2]) == -1) {
				clog << "Nie znaleziono pliku." << endl;
				return;
			}
			else {
				CP("obslugaPliku", "init", "", 0);
				XF(args[1], args[2], zarzadzanieProcesami.znajdzProces("obslugaPliku"), args[3]);
				DP("obslugaPliku");
			}
		}
	}
	else if (komenda == "OF") {
		if (args.size() < 3 || args.size()>4)
			cout << "Niepoprawne uzycie komendy! OF nazwa rozszerzenie *folder" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			if (dysk.znajdzPlik(args[1], args[2]) == -1) {
				clog << "Nie znaleziono pliku." << endl;
				return;
			}
			else {

				bool nazwaZajeta = true;
				std::string temp = random_string(8);
				PlikProces temp2 = { temp, args[1], args[2] };
				while (nazwaZajeta) {
					for (auto a : PlikiProcesy) {
						if (a.nazwaProcesu == temp) {
							temp = random_string(8);
						}
					}
					nazwaZajeta = false;
				}
				temp2.nazwaProcesu = temp;
				PlikiProcesy.push_back(temp2);

				CP(temp, "init", "", 0);
				zarzadzanieProcesami.znajdzProces(temp)->ustawRamLokalizacja(-10);

				OF(args[1], args[2], zarzadzanieProcesami.znajdzProces(temp), args[3]);
			}
		}
	}
	else if (komenda == "CF") {
		if (args.size() < 3 || args.size()>4)
			cout << "Niepoprawne uzycie komendy! CF nazwa rozszerzenie *folder" << endl;
		else {
			if (args.size() != 4)
				args.push_back("Dysk");
			if (dysk.znajdzPlik(args[1], args[2]) == -1) {
				clog << "Nie znaleziono pliku." << endl;
				return;
			}
			else {
				std::string temp = "";

				for (auto e : PlikiProcesy) {
					if (e.nazwaPliku == args[1] && e.rozszerzeniePliku == args[2]) {
						temp = e.nazwaProcesu;
						break;
					}
				}
				if (zarzadzanieProcesami.znajdzProces(temp) == nullptr) {
					clog << "Plik nie jest otwarty" << endl;
					return;
				}
				if (zarzadzanieProcesami.znajdzProces(temp)->dajBlad() == 0) {
					CF(args[1], args[2], zarzadzanieProcesami.znajdzProces(temp), args[3]);
					DP(temp);
					short x = -1;
					//	this->numeryIwezlow.erase(std::find(this->numeryIwezlow.begin(), this->numeryIwezlow.end(), numer) + 0);
					for (auto e : PlikiProcesy) {
						x++;
						if (e.nazwaProcesu == temp)
							break;
					}
					PlikiProcesy.erase(PlikiProcesy.begin() + x);
				}
				else {
					std::clog << "Blad procesu: " << temp << " = " << zarzadzanieProcesami.znajdzProces(temp)->dajBlad() << std::endl;
					zarzadzanieProcesami.znajdzProces(temp)->ustawBlad(0);
				}


			}
		}
	}
	else if (komenda == "GO") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! GO" << endl;
		}
		else
			GO();
	}
	else if (komenda == "RQ") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! RQ" << endl;
		}
		else
			RQ();
	}
	else if (komenda == "WQ") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! WQ" << endl;
		}
		else
			WQ();
	}
	else if (komenda == "PS")
	{
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! PS" << endl;
		}
		else
			PS();
	}
	else if (komenda == "RB") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! RB" << endl;
		}
		else
			RB();
	}
	else if (komenda == "MP") {
		if (args.size() != 3)
			cout << "Niepoprawne uzycie komendy! MP nazwaKtorego nazwaDoKtorego" << endl;
		else
			MP(args[1], args[2]);
	}
	else if (komenda == "CLS")
	{
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! cls" << endl;
		}
		else
			system("cls");
	}
	else if (komenda == "HELP") {
		if (args.size() != 1)
		{
			cout << "Niepoprawne uzycie komendy! help" << endl;
		}
		else
			help();
	}
	else {
		clog << "Niepoprawna komenda. Sprawdz 'help'" << endl;
	}
	args.clear();
	komenda.clear();
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
	//wczytywanie linii z pliku, rozbicie na elementy na podstawie spacji i wpisanie do wektora
	//polecenie, nastepnie polecenie do wektora polecen
	while (getline(skrypt, linia)) {
		istringstream buffer(linia);
		istream_iterator<string> beg(buffer), end;
		vector<string>polecenie(beg, end);
		polecenia.push_back(polecenie);
		polecenie.clear();

	}
	//odczytywanie komend i wykonywanie
	for (int i = 0; i < polecenia.size(); i++) {
		for (int j = 0; j < polecenia[i].size(); j++) {
			if (j != polecenia[i].size())
				komenda = komenda + polecenia[i][j] + " ";
			else
				komenda = komenda + polecenia[i][j];
		}
		czytajWejscie(komenda);
		komenda = "";
	}
	polecenia.clear();
	skrypt.close();
}

void Shell::obsluzBledy(short blad)
{
	if (blad)
	{
		std::cout << "Wystapil blad.\n";
	}
}

bool Shell::czyStringLiczba(std::string& s)
{

	return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
	return true;

}

std::string Shell::random_string(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}