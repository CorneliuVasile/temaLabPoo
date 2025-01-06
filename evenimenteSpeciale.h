#ifndef EVENIMENTE_H
#define EVENIMENTE_H
#include "ANGAJAT.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class EvenimenteSpeciale {
private:
    string tipDeEveniment;
    double buget;
    double profit;
    double numarDeAngajatiNecesari;
    string oras;

public:
    // Constructor
    EvenimenteSpeciale(const string& tipDeEveniment = "", double buget = 0.0, double profit = 0.0, double numarDeAngajatiNecesari = 0.0, const string& oras = "")
            : tipDeEveniment(tipDeEveniment), buget(buget), profit(profit), numarDeAngajatiNecesari(numarDeAngajatiNecesari), oras(oras) {}

    // Getters
    string getTipDeEveniment() const { return tipDeEveniment; }
    double getBuget() const { return buget; }
    double getProfit() const { return profit; }
    double getNumarDeAngajatiNecesari() const { return numarDeAngajatiNecesari; }
    string getOras() const {return oras;}

    // Setters
    void setTipDeEveniment(const string& tip) { tipDeEveniment = tip; }
    void setBuget(double nouBuget) { buget = nouBuget; }
    void setProfit(double nouProfit) { profit = nouProfit; }
    void setNumarDeAngajatiNecesari(double numar) { numarDeAngajatiNecesari = numar; }
    void setOras(string oras1) {oras = oras1;}
    string toCSV() const {
        return tipDeEveniment + "," + to_string(buget) + "," + to_string(profit) + "," + to_string(numarDeAngajatiNecesari) + ","  + oras;
    }

    // Add event to file
    static void adaugaInFisier(const string& fileName, const EvenimenteSpeciale& evenimentNou) {
        ofstream file(fileName, ios::app);

        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului: " << fileName << endl;
            return;
        }

        file << evenimentNou.toCSV() << endl;
        file.close();
    }

    static map<string, int> numaraAngajatiiPeOrase(const string& fileName) {
        // Citește lista de angajați din fișier
        vector<Angajat*> angajati = Angajat::citesteDinFisier(fileName);

        // Creează un map pentru a ține evidența angajaților pe orașe
        map<string, int> numarAngajatiPeOrase;

        // Parcurge lista de angajați și actualizează numărătoarea pe orașe
        for (const auto& angajat : angajati) {
            string oras = angajat->getOrasulUndeMuncesc();
            numarAngajatiPeOrase[oras]++;
        }

        // Curăță memoria angajaților
        for (auto angajat : angajati) {
            delete angajat;
        }

        return numarAngajatiPeOrase;
    }

    // Funcție pentru numărarea angajaților dintr-un anumit oraș
    static int numaraAngajatiiDinOras(const string& fileName, const string& orasCautat) {
        // Citește lista de angajați din fișier
        vector<Angajat*> angajati = Angajat::citesteDinFisier(fileName);

        // Variabilă pentru numărarea angajaților din orașul căutat
        int numarAngajati = 0;

        // Parcurge lista de angajați
        for (const auto& angajat : angajati) {
            if (angajat->getOrasulUndeMuncesc() == orasCautat) {
                numarAngajati++;
            }
        }

        // Curăță memoria angajaților
        for (auto angajat : angajati) {
            delete angajat;
        }

        return numarAngajati;
    }

    static void interfataEvenimente() {
        while (true) {
            cout << "Selectati optiunea: " << endl;
            cout << "1. Adauga un eveniment" << endl;
            cout << "2. Iesire" << endl;

            int optiune;
            cin >> optiune;

            if (optiune == 2) break;

            switch (optiune) {
                case 1: {
                    string tipDeEveniment;
                    double buget = 0.0;
                    double profit = 0.0;
                    double numarDeAngajatiNecesari = 0.0;
                    string oras;

                    cout << "Introduceti tipul de eveniment (Muzica Live / Degustare de cafea / Unlimited coffee): ";
                    cin.ignore(); // Clear buffer
                    getline(cin, tipDeEveniment);

                    if (tipDeEveniment == "Muzica Live") {
                        buget = 1000.0;
                    } else if (tipDeEveniment == "Degustare de cafea") {
                        buget = 1300.0;
                    } else if (tipDeEveniment == "Unlimited coffee") {
                        buget = 800.0;
                    } else {
                        cerr << "Tip de eveniment invalid." << endl;
                        break;
                    }

                    profit = buget * 0.25;

                    cout << "Introduceti orasul evenimentului:(Pune un space inaintea numelui orasului)";
                    cin.ignore();
                    getline(cin, oras);

                    int numarAngajatiDisponibili = EvenimenteSpeciale::numaraAngajatiiDinOras("AngajatiSiFunctii.csv", oras);

                    if (numarAngajatiDisponibili <= 3) {
                        cerr << "Numar insuficient de angajati in orasul selectat." << endl;
                        break;
                    }

                    EvenimenteSpeciale eveniment(tipDeEveniment, buget, profit, 3, oras);
                    EvenimenteSpeciale::adaugaInFisier("EvenimenteSpeciale.csv", eveniment);
                    cout << "Eveniment adaugat cu succes." << endl;
                    break;
                }

                default:
                    cerr << "Optiune invalida." << endl;
            }
        }
    }
    // Calculează profitul tuturor evenimentelor dintr-un anumit oraș
    static double calculeazaProfitDinOras(const string& fileName, const string& orasCautat) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru citire." << endl;
            return 0.0;
        }

        double profitTotal = 0.0;
        string linie;

        // Parcurge fiecare linie a fișierului
        while (getline(file, linie)) {
            stringstream ss(linie);
            string tipEveniment, bugetStr, profitStr, angajatiStr, orasCurent;

            // Sparge linia în câmpuri CSV
            getline(ss, tipEveniment, ',');
            getline(ss, bugetStr, ',');
            getline(ss, profitStr, ',');
            getline(ss, angajatiStr, ',');
            getline(ss, orasCurent, ',');

            // Adaugă profitul dacă orașul coincide
            if (orasCurent == orasCautat) {
                try {
                    profitTotal += stod(profitStr); // Conversie profit la double
                } catch (const invalid_argument&) {
                    cerr << "Eroare la conversia profitului pentru linia: " << linie << endl;
                }
            }
        }

        file.close();
        return profitTotal;
    }

    // Șterge toate evenimentele dintr-un anumit oraș
    static void stergeEvenimenteDinOras(const string& fileName, const string& orasDeSters) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru citire." << endl;
            return;
        }

        vector<string> liniiRamase;
        string linie;

        // Citește toate liniile din fișier
        while (getline(file, linie)) {
            stringstream ss(linie);
            string orasCurent;
            string temp;
            for (int i = 0; i < 4; ++i) {
                getline(ss, temp, ','); // Treci peste câmpurile anterioare
            }
            getline(ss, orasCurent, ','); // Citește al cincilea câmp


            // Păstrează liniile care nu aparțin orașului de șters
            if (orasCurent != orasDeSters) {
                liniiRamase.push_back(linie);
            }
        }

        file.close();

        // Rescrie fișierul fără evenimentele din orașul de șters
        ofstream outFile(fileName, ios::trunc);
        if (!outFile.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru scriere." << endl;
            return;
        }

        for (const string& linie : liniiRamase) {
            outFile << linie << endl;
        }

        outFile.close();
    }
};




#endif // EVENIMENTE_H
