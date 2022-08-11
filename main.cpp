#include "TrieStablo.h"

#include <string>

int main()
{
    TrieStablo* stablo = new TrieStablo();

	cout << "\n\tPREDVIDJANJE TEKSTA KORISCENJEM TRIE STABLA\n\n";
	cout << "  Izaberite stavku sa menija:\n";
	cout << "  1. Stvaranje recnika na osnovu zadate datoteke ili skupa datoteka.\n";
	cout << "  2. Umetanje nove reci u recnik.\n";
	cout << "  3. Predvidjanje reci na osnovu zadatog stringa.\n";
	cout << "  4. Unistavanje recnika.\n";
	cout << "  5. Ispisivanje statistike Trie stabla.\n";
	cout << "  0. Kraj programa\n";
	cout << "________________________________________________________________________________________________________________________\n\n";

	int x;
	string path, str;
	const char* c = nullptr;

	while (true)
	{
		cout << "Vas izbor: ";
		cin >> x;
		cout << endl;

		switch (x)
		{
		case 0: 
			delete stablo; 
			exit(0);

		case 1:
			path = "C:\\Users\\Teodor\\Desktop\\dz3_asp2\\dz3_asp2\\datoteke";
			stablo->readDirectory(path);
			stablo->ispisiStatistiku(cout);

			break;
		case 2:
			cout << "Unesite rec koju zelite da ukljucite u recnik: ";
			cin.ignore();
			getline(cin, str);
			c = str.c_str();
			if (lettersOnly(c)) stablo->umetni(c);
			else cout << "Nekorektan unos.";
			cout << endl;

			break;
		case 3:
			cout << "Unesite rec za predikciju: ";
			cin.ignore();
			getline(cin, str);
			c = str.c_str();
			if (lettersOnly(c)) stablo->predikcija(c);
			else cout << "Nekorektan unos.";
			cout << endl;

			break;
		case 4:
			if (stablo) delete stablo;
			stablo = nullptr;
			stablo = new TrieStablo();

			break;
		case 5:
			if (stablo) stablo->ispisiStatistiku(cout);

			break;
		default:
			cout << "Nekorektan unos. Pokusajte ponovo." << endl;
		}
	}

    return 0;
}

/*
int main() 
{
	TrieStablo stablo;

	stablo.predikcija("afte");

    string path = "C:\\Users\\Teodor\\Desktop\\dz3_asp2\\dz3_asp2\\datoteke";

    stablo.readDirectory(path);

    stablo.ispisiStatistiku(cout);

    stablo.predikcija("afte");
    stablo.predikcija("poga");
    stablo.predikcija("cact");
    stablo.predikcija("pigqkd");
    stablo.predikcija("fact");
    stablo.predikcija("aftermoom");

	return 0;
}
*/
