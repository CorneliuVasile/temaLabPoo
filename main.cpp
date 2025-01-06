#include <iostream>
#include "ANGAJAT.h"
#include <map>
#include <algorithm>
#include "Produs.h"
#include "Comanda.h"
#include <vector>
#include "EvenimenteSpeciale.h"
#include "Raport.h"
#include <set>
using namespace std;

bool esteUnNumarValidDouble(const string& str) {
    try {
        stod(str); // încercăm să convertim la double
        return true;
    } catch (const invalid_argument&) {
        return false;
    }
}

bool esteUnNumarValidInt(const string& str) {
    try {
        stoi(str); // încercăm să convertim la int
        return true;
    } catch (const invalid_argument&) {
        return false;
    }
}

void adaugaProdus(const string& fileName) {
    string tipProdus, oras, pretStr, bucatiStr;
    double pret;
    int bucati;
    ///Polimorfism
    RAPORTZILNIC raport;

    cout << "Introduceti tipul produsului: ";
    cin >> tipProdus;

    cout << "Introduceti pretul produsului: ";
    cin >> pretStr;
    while (!esteUnNumarValidDouble(pretStr)) {
        cout << "Pret invalid! Introduceti un pret valid: ";
        cin >> pretStr;
    }
    pret = stod(pretStr);  // Conversie in double

    cout << "Introduceti orasul: ";
    cin >> oras;

    cout << "Introduceti numarul de bucati: ";
    cin >> bucatiStr;
    while (!esteUnNumarValidInt(bucatiStr)) {
        cout << "Numar de bucati invalid! Introduceti un numar valid: ";
        cin >> bucatiStr;
    }
    bucati = stoi(bucatiStr);  // Conversie in int

    // Creăm obiectul produs
    Produs produsNou(tipProdus, pret, oras, bucati);
    raport.modificaProfitulDinOras("RapoarteFinanciare.csv",oras,pret*bucati);

    // Calculăm profitul și îl afișăm
    cout << "Profitul pentru produsul " << tipProdus << " este: " << produsNou.getProfit() << " lei.\n";

    // Adăugăm produsul în fișier
    Produs::adaugaInFisier(fileName, produsNou);

    cout << "Produs adaugat cu succes!\n";
}

void stergeProdus(const string& fileName) {
    string tipProdus, oras;

    cout << "Introduceti tipul produsului de sters: ";
    cin >> tipProdus;

    cout << "Introduceti orasul unde produsul se afla: ";
    cin >> oras;

    Produs::stergeDinFisier(fileName, tipProdus, oras);
    cout << "Produs sters cu succes!\n";
}

void veziProduse(const string& fileName) {
    auto produse = Produs::citesteDinFisier(fileName);

    cout << "Produse in fisier:\n";
    for (const auto& produs : produse) {
        cout << produs.toCSV() << endl;
    }
}

void veziProdusePeOras(const string& fileName) {
    string oras;
    cout << "Introduceti orasul (Bucuresti, Cluj-Napoca, Timisoara, Iasi, Brasov): ";
    cin >> oras;

    auto produse = Produs::citesteDinFisier(fileName);

    cout << "Produse in orasul " << oras << ":\n";
    for (const auto& produs : produse) {
        if (produs.getOras() == oras) {
            cout << produs.toCSV() << endl;
        }
    }
}

void modificaBucati(const string& fileName) {
    string tipProdus, oras, bucatiStr;
    int bucati;

    cout << "Introduceti tipul produsului pentru modificare: ";
    cin >> tipProdus;

    cout << "Introduceti orasul: ";
    cin >> oras;

    cout << "Introduceti numarul de bucati de adaugat (pozitiv) sau de sters (negativ): ";
    cin >> bucatiStr;
    while (!esteUnNumarValidInt(bucatiStr)) {
        cout << "Numar de bucati invalid! Introduceti un numar valid: ";
        cin >> bucatiStr;
    }
    bucati = stoi(bucatiStr);  // Conversie in int

    Produs::modificaBucatiInFisier(fileName, tipProdus, oras, bucati);
    cout << "Numar de bucati modificat cu succes!\n";
}

void afiseazaMeniu() {
    cout << "1. Angajati\n";
    cout << "2. Produse\n";
    cout << "3. Comenzi\n";
    cout << "4. EvenimeteSpeciale\n";
    cout << "5. Rapoarte\n";
    cout << "0. Iesire\n";
    cout << "Alege o optiune: ";
}
void afiseazaMeniuAngajati() {
    cout << "1. Adauga angajat\n";
    cout << "2. Sterge angajat\n";
    cout << "3. Afiseaza toti angajatii\n";
    cout << "4. Afiseaza angajatii dupa oras\n";
    cout << "0. Iesire\n";
}


void afiseazaMeniuProduse() {
    cout << "1. Adauga produs\n";
    cout << "2. Sterge produs\n";
    cout << "3. Vezi produse in fisier\n";
    cout << "4. Vezi produse pe oras\n";
    cout << "5. Modifica bucati produs\n";
    cout << "0. Iesire\n";
    cout << "Alege o optiune: ";
}

void afiseazaMeniuComenzi() {
    cout << "1. Plaseaza o comanda\n";
    cout << "0. Iesire\n";
    cout << "Alege o optiune: ";
}

void afiseazaMeniuRaport() {
    cout << "1. Raport zilnic\n";
    cout << "2. Raport Lunar\n";
    cout << "0. Iesire\n";
    cout << "Alege o optiune: ";
}

void plaseazaComanda(const string& fileNameProduse, const string& fileNameComenzi) {
    string numeClient;
    cout << "Introduceti numele clientului: ";
    cin >> numeClient;

    double pretTotalComanda = 0;
    bool comandaIncheiata = false;

    while (!comandaIncheiata) {
        auto produse = Produs::citesteDinFisier(fileNameProduse);

        cout << "Produse disponibile:\n";
        for (const auto& produs : produse) {
            cout << produs.toCSV() << endl;
        }

        string produsSelectat;
        cout << "Introduceti produsul dorit (sau 'exit' pentru a termina comanda): ";
        cin >> produsSelectat;

        if (produsSelectat == "exit") {
            comandaIncheiata = true;
            break;
        }
        string orasProdus;

        cout << "Introduceti orasul de unde doriti produsul: ";
        cin >> orasProdus;

        int numarBucati;
        cout << "Introduceti numarul de bucati dorit: ";
        string numarStr;
        cin >> numarStr;
        while (!esteUnNumarValidInt(numarStr)) {
            cout << "Numar invalid! Incercati din nou: ";
            cin >> numarStr;
        }
        numarBucati = stoi(numarStr);

        // Găsim produsul din stoc
        bool produsGasit = false;
        for (auto& produs : produse) {
            if (produs.getTipProdus() == produsSelectat) {
                if (produs.getBucati() >= numarBucati) {
                    double pretComanda = produs.getPret() * numarBucati;
                    cout << "Pret total pentru " << numarBucati << " bucati de " << produsSelectat << " este: " << pretComanda << endl;

                    // Se actualizează numărul de bucăți
                    produs.setBucati(produs.getBucati() - numarBucati);

                    // Salvăm comanda într-un fișier
                    Comanda comanda(numeClient, produsSelectat, numarBucati, pretComanda, orasProdus);
                    Comanda::adaugaComandaInFisier(fileNameComenzi, comanda);

                    // Se actualizează produsele în fișier
                    Produs::modificaBucatiInFisier(fileNameProduse,produsSelectat,orasProdus, -numarBucati);

                    pretTotalComanda += pretComanda;
                    produsGasit = true;
                    Produs::stergeProduseCuBucatiNegative(fileNameProduse);
                    break;

                } else {
                    cout << "Nu sunt suficiente produse pe stoc!\n";
                    produsGasit = true;
                    break;
                }
            }
        }

        if (!produsGasit) {
            cout << "Produsul nu a fost găsit. Încercați din nou.\n";
        }
    }

    cout << "Total comanda pentru " << numeClient << ": " << pretTotalComanda << " lei.\n";
}



void adaugaAngajat(const string& fileName) {
    string tip, nume, prenume, oras;
    double salariu, oraStart, oraEnd;
    int valoareExtra = 0;

    // Listele permise
    vector<string> tipuriPermise = {"Barista", "Manager", "Ospatar"};
    vector<string> orasePermise = {"Bucuresti", "Iasi", "Cluj-Napoca", "Timisoara", "Brasov"};

    // Tip angajat
    cout << "Introduceti tipul angajatului (Barista/Manager/Ospatar): ";
    cin >> tip;
    if (find(tipuriPermise.begin(), tipuriPermise.end(), tip) == tipuriPermise.end()) {
        cerr << "Tipul angajatului este invalid! Adaugarea a fost anulata." << endl;
        return;
    }

    // Nume
    cout << "Introduceti numele: ";
    cin >> nume;
    if (nume.empty() || any_of(nume.begin(), nume.end(), ::isdigit)) {
        cerr << "Numele nu poate contine cifre sau sa fie gol! Adaugarea a fost anulata." << endl;
        return;
    }

    // Prenume
    cout << "Introduceti prenumele: ";
    cin >> prenume;
    if (prenume.empty() || any_of(prenume.begin(), prenume.end(), ::isdigit)) {
        cerr << "Prenumele nu poate contine cifre sau sa fie gol! Adaugarea a fost anulata." << endl;
        return;
    }

    // Salariu
    cout << "Introduceti salariul brut: ";
    cin >> salariu;
    if (salariu <= 0) {
        cerr << "Salariul brut trebuie sa fie mai mare decat zero! Adaugarea a fost anulata." << endl;
        return;
    }

    // Calcul salariu net
    double salariuNet = salariu - (0.1 * salariu);
    cout << "Salariul net este: " << salariuNet << endl;

    // Ore lucru
    cout << "Introduceti ora de incepere a turei (0-24): ";
    cin >> oraStart;
    if (oraStart < 0 || oraStart > 24) {
        cerr << "Ora de incepere a turei trebuie sa fie intre 0 si 24! Adaugarea a fost anulata." << endl;
        return;
    }

    cout << "Introduceti ora de terminare a turei (0-24): ";
    cin >> oraEnd;
    if (oraEnd < 0 || oraEnd > 24 || oraEnd <= oraStart) {
        cerr << "Ora de terminare trebuie sa fie intre 0 si 24 si mai mare decat ora de incepere! Adaugarea a fost anulata." << endl;
        return;
    }

    // Oras
    cout << "Introduceti orasul unde lucreaza (Bucuresti/Iasi/Cluj-Napoca/Timisoara/Brasov): ";
    cin >> oras;
    if (find(orasePermise.begin(), orasePermise.end(), oras) == orasePermise.end()) {
        cerr << "Oras invalid! Adaugarea a fost anulata." << endl;
        return;
    }

    // Campuri specifice
    if (tip == "Barista") {
        cout << "Introduceti varsta: ";
        cin >> valoareExtra;
        if (valoareExtra <= 0 || valoareExtra > 100) {
            cerr << "Varsta introdusa este invalida! Adaugarea a fost anulata." << endl;
            return;
        }
    } else if (tip == "Manager") {
        vector<Angajat*> angajati = Angajat::citesteDinFisier(fileName);

        int nrSubordonati = count_if(angajati.begin(), angajati.end(),
                                     [&](const Angajat* a) {
                                         return a->getOrasulUndeMuncesc() == oras &&
                                                dynamic_cast<const Manager*>(a) == nullptr;
                                     });

        valoareExtra = nrSubordonati;
        cout << "Numarul de angajati subordonati (non-manageri) din oras: " << valoareExtra << endl;

        for (auto angajat : angajati) delete angajat;
    } else if (tip == "Ospatar") {
        cout << "Introduceti anii de experienta: ";
        cin >> valoareExtra;
        if (valoareExtra < 0) {
            cerr << "Anii de experienta trebuie sa fie cel putin 0! Adaugarea a fost anulata." << endl;
            return;
        }
    }

    ///OBIECTE
    Angajat* nouAngajat = nullptr;

    if (tip == "Barista") {
        nouAngajat = new Barista(nume, prenume, salariu, tip, oraStart, oraEnd, oras, valoareExtra);
    } else if (tip == "Manager") {
        nouAngajat = new Manager(nume, prenume, salariu, tip, oraStart, oraEnd, oras, valoareExtra);
    } else if (tip == "Ospatar") {
        nouAngajat = new Ospatar(nume, prenume, salariu, tip, oraStart, oraEnd, oras, valoareExtra);
    }

    // Adăugare în fișier
    Angajat::adaugaInFisier(fileName, nouAngajat);

    cout << "Angajatul a fost adaugat cu succes!" << endl;
    delete nouAngajat;
}

void stergeAngajat1(const string& fileName) {
    string nume, prenume;
    cout << "Introduceti numele angajatului pe care doriti sa il stergeti: ";
    cin >> nume;
    cout << "Introduceti prenumele angajatului pe care doriti sa il stergeti: ";
    cin >> prenume;

    Angajat::stergeDinFisier(fileName, nume, prenume);

    cout << "Angajatul a fost sters din fisier." << endl;
}

void afiseazaAngajati(const string& fileName) {
    vector<Angajat*> angajati = Angajat::citesteDinFisier(fileName);

    cout << "Lista tuturor angajatilor: " << endl;
    for (const auto& angajat : angajati) {
        cout << angajat->toCSV() << endl;
    }

    // Curățare memorie
    for (auto angajat : angajati) delete angajat;
}

void afiseazaAngajatiDupaOras(const string& fileName, const string& oras) {
    vector<Angajat*> angajati = Angajat::citesteDinFisier(fileName);

    cout << "Lista angajatilor din orasul " << oras << ": " << endl;
    for (const auto& angajat : angajati) {
        if (angajat->getOrasulUndeMuncesc() == oras) {
            cout << angajat->toCSV() << endl;
        }
    }

    // Curățare memorie
    for (auto angajat : angajati) delete angajat;
}

int main() {
    string fileNameAngajati = "AngajatiSiFunctii.csv";
    string fileNameProduse = "ProduseSiPreturi.csv";
    string fileNameComenzi = "Comenzi.csv";
    string fileNameRapoarte = "RapoarteFinanciare.csv";
    string fileNameEveniment = "EvenimenteSpeciale.csv";
    int optiune = -1;

    while (optiune != 0) {
        afiseazaMeniu();
        cin >> optiune;

        switch (optiune) {
            case 1:
            {

                int optiune1 = -1;
                while (optiune1 != 0) {
                    afiseazaMeniuAngajati();
                    cin >> optiune1;
                switch (optiune1) {
                    case 1: {
                        cout << "\n--- Adauga un angajat nou ---\n";
                        adaugaAngajat(fileNameAngajati);
                        break;
                    }
                    case 2: {
                        cout << "\n--- Sterge un angajat ---\n";
                        stergeAngajat1(fileNameAngajati);
                        break;
                    }
                    case 3: {
                        cout << "\n--- Lista tuturor angajatilor ---\n";
                        afiseazaAngajati(fileNameAngajati);
                        break;
                    }
                    case 4: {
                        string oras;
                        cout << "\n--- Lista angajatilor dintr-un oras ---\n";
                        cout << "Introduceti numele orasului: ";
                        cin >> oras;
                        afiseazaAngajatiDupaOras(fileNameAngajati, oras);
                        break;
                    }
                    case 0: {
                        cout << "Iesire din aplicatie. La revedere!\n";
                        break;
                    }
                    default:
                        cout << "Optiune invalida. Te rog sa alegi din meniul de mai sus.\n";
                }
                }
            }

            case 2:
            {
                int optiune2 = -1;
                while (optiune2 != 0) {

                    afiseazaMeniuProduse();
                    cin>>optiune2;

                    switch(optiune2){
                        case 1:{
                            adaugaProdus(fileNameProduse);
                            break;
                        }
                        case 2:
                            stergeProdus(fileNameProduse);
                            break;
                        case 3:
                            veziProduse(fileNameProduse);
                            break;
                        case 4:
                            veziProdusePeOras(fileNameProduse);
                            break;
                        case 5:
                            modificaBucati(fileNameProduse);
                            break;
                        case 0:
                            cout << "La revedere!\n",optiune = -1;
                        default:
                            cout << "Optiune invalida! Incercati din nou.\n";
                            break;
                    }

                }
            }
            case 3:
            {
                int optiune3 = -1;
                while (optiune3 != 0) {

                    afiseazaMeniuComenzi();
                    cin >> optiune3;
                    switch (optiune3) {
                        case 1:
                            plaseazaComanda(fileNameProduse, fileNameComenzi);
                        case 0:
                            cout << "La revedere!\n", optiune = -1;
                            break;
                    }
                }
            }
            case 4:
            {
                EvenimenteSpeciale::interfataEvenimente();
            }
            case 5:
            {
                int optiune5 = -1;
                while(optiune5 != 0) {
                    afiseazaMeniuRaport();
                    cin >> optiune5;
                    switch (optiune5) {
                        case 1: {
                            string orasCautat;
                            set<string> oraseValabile = {"Bucuresti", "Iasi", "Cluj-Napoca", "Timisoara", "Brasov"};

                            cout<< "Introduceti numele orasului pentru care doriti sa generati raportul:(Bucuresti/Iasi/Cluj-Napoca/Timisoara/Brasov) ";
                            cin >> orasCautat;
                            if (oraseValabile.find(orasCautat) != oraseValabile.end()) {
                                    // Creăm un obiect RAPORTZILNIC
                                    RAPORTZILNIC raport;
                                    raport.setOras(orasCautat);

                                    // Calculăm suma comenzilor și generăm profitul pentru orașul specificat
                                    raport.calculeazaSumaComenzilor(fileNameComenzi, orasCautat);

                                    // Afisăm profitul pentru ziua respectivă
                                    double profitGenerat = raport.getProfit();
                                    Comanda::stergeComenziDinFisier(fileNameComenzi, orasCautat);
                                    //Comanda::stergeComenziDinFisier(fileNameComenzi,orasCautat);
                                    cout << "Profitul pentru orasul " << orasCautat << " este: " << profitGenerat << " RON"<< endl;

                                    // Salvăm raportul în fișier
                                    raport.salveazaInFisier(fileNameRapoarte);

                                    cout << "Raportul a fost salvat cu succes " << endl;
                                    break;
                                }
                            else
                            {
                                cout << "Orasul introdus nu este valid. Va rugam sa introduceti unul dintre urmatoarele orase: Bucuresti, Iasi, Cluj-Napoca, Timisoara, Brasov." << endl;
                            }

                        }
                        case 2:
                        {
                            string orasCautatLunar;
                            set<string> oraseValabile = {"Bucuresti", "Iasi", "Cluj-Napoca", "Timisoara", "Brasov"};

                            cout<< "Introduceti numele orasului pentru care doriti sa generati raportul LUNAR:(Bucuresti/Iasi/Cluj-Napoca/Timisoara/Brasov) ";
                            cin >> orasCautatLunar;
                            if (oraseValabile.find(orasCautatLunar) != oraseValabile.end()) {
                                RAPORTZILNIC raportLunar;
                                raportLunar.setOras(orasCautatLunar);

                                double profitLunar = raportLunar.adunaProfitulDinOras(fileNameRapoarte,orasCautatLunar);
                                profitLunar = profitLunar - raportLunar.SalariiAngajatiiDinOras(fileNameAngajati,orasCautatLunar);
                                profitLunar = profitLunar + EvenimenteSpeciale::calculeazaProfitDinOras(fileNameEveniment,orasCautatLunar);
                                raportLunar.setProfit(profitLunar);
                                EvenimenteSpeciale::stergeEvenimenteDinOras(fileNameEveniment,orasCautatLunar);
                                raportLunar.stergeRapoarteDinOras(fileNameRapoarte,orasCautatLunar);
                                cout << "Profitul lunar pentru orasul " << orasCautatLunar << " este: " << profitLunar << " RON"<< endl;
                                raportLunar.salveazaInFisier(fileNameRapoarte);
                                cout << "Raportul a fost salvat cu succes " << endl;
                                break;
                            }
                            else {
                                cout << "Orasul introdus nu este valid. Va rugam sa introduceti unul dintre urmatoarele orase: Bucuresti, Iasi, Cluj-Napoca, Timisoara, Brasov." << endl;
                            }

                        }
                        case 0:
                            cout << "La revedere!\n", optiune = -1;
                            break;
                    }
                }
            }

            case 0:
                cout << "La revedere!\n";
                break;
            default:
                cout << "Optiune invalida! Incercati din nou.\n";
                break;
        }
    }

    return 0;
}
