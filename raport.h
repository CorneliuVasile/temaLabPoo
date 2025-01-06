#ifndef RAPORT_H
#define RAPORT_H
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "EvenimenteSpeciale.h"
#include "ANGAJAT.h"
#include <string>


class RAPORTZILNIC {
protected:
    double profit;
    string oras;

public:
    double getProfit() { return profit; }

    void setProfit(double profitNou) { profit = profitNou; }

    void calculeazaSumaComenzilor(const string &fileName, const string &orasCautat) {
        ifstream file(fileName);

        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru citire." << endl;
            setProfit(0.0);
            return;
        }

        double sumaTotala = 0.0;
        string linie;

        // Citirea fiecărei linii din fișier
        while (getline(file, linie)) {
            stringstream ss(linie);
            string numeClient, produsComandat, numarBucati, pretFinal, oras;

            // Sparge linia CSV în câmpuri
            getline(ss, numeClient, ',');
            getline(ss, produsComandat, ',');
            getline(ss, numarBucati, ',');
            getline(ss, pretFinal, ',');
            getline(ss, oras, ',');

            // Verificăm dacă orașul comenzii corespunde
            if (oras == orasCautat) {
                try {
                    sumaTotala += stod(pretFinal);
                } catch (const invalid_argument &) {
                    cerr << "Eroare la conversia prețului în linia: " << linie << endl;
                }
            }
        }

        file.close();

        // Calculăm profitul (15% din suma totală)
        setProfit(sumaTotala * 0.15);
    }


    // Salvează raportul într-un fișier
    void salveazaInFisier(const string& raportFileName) const {
        ofstream file(raportFileName, ios::app);

        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru scriere." << endl;
            return;
        }

        // Scrie datele în format CSV
        file << oras << "," << profit << endl;
        file.close();
    }

    // Setează orașul pentru raport
    void setOras(const string& orasNou) {
        oras = orasNou;
    }

    // Get pentru oraș
    string getOras() const {
        return oras;
    }

    // Șterge rapoartele dintr-un anumit oraș
    void stergeRapoarteDinOras(const string& raportFileName, const string& orasDeSters) {
        ifstream file(raportFileName);
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru citire." << endl;
            return;
        }

        string linie;
        vector<string> liniiRamase;

        // Citim toate liniile care nu aparțin orașului specificat
        while (getline(file, linie)) {
            stringstream ss(linie);
            string orasCurent;
            getline(ss, orasCurent, ',');

            if (orasCurent != orasDeSters) {
                liniiRamase.push_back(linie);
            }
        }

        file.close();

        // Suprascriem fișierul original cu liniile rămase
        ofstream outFile(raportFileName, ios::trunc);
        if (!outFile.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru scriere." << endl;
            return;
        }

        for (const string& linieRamasa : liniiRamase) {
            outFile << linieRamasa << endl;
        }

        outFile.close();
    }

    // Adună profitul pentru toate rapoartele dintr-un anumit oraș
    double adunaProfitulDinOras(const string& raportFileName, const string& orasCautat) {
        ifstream file(raportFileName);
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru citire." << endl;
            return 0.0;
        }

        double profitTotal = 0.0;
        string linie;

        // Citirea fiecărei linii din fișier
        while (getline(file, linie)) {
            stringstream ss(linie);
            string orasCurent, profitCurent;

            getline(ss, orasCurent, ',');
            getline(ss, profitCurent, ',');
///Exceptions
            if (orasCurent == orasCautat) {
                try {
                    profitTotal += stod(profitCurent);
                } catch (const invalid_argument &) {
                    cerr << "Eroare la conversia profitului în linia: " << linie << endl;
                }
            }
        }

        file.close();
        return profitTotal;
    }
    static double SalariiAngajatiiDinOras(const string& fileName, const string& orasCautat) {
        // Citește lista de angajați din fișier
        vector<Angajat*> angajati = Angajat::citesteDinFisier(fileName);

        // Variabilă pentru numărarea angajaților din orașul căutat
        double salariiAngajati = 0;

        // Parcurge lista de angajați
        for (const auto& angajat : angajati) {
            if (angajat->getOrasulUndeMuncesc() == orasCautat) {
                salariiAngajati = salariiAngajati + angajat->getSalariu();
            }
        }

        // Curăță memoria angajaților
        for (auto angajat : angajati) {
            delete angajat;
        }

        return salariiAngajati;
    }
    void modificaProfitulDinOras(const string& raportFileName, const string& orasCautat, double nouProfit) {
        ifstream file(raportFileName);
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru citire." << endl;
            return;
        }

        string linie;
        vector<string> liniiActualizate;

        // Citim toate liniile din fișier
        while (getline(file, linie)) {
            stringstream ss(linie);
            string orasCurent, profitCurentString;
            double profitCurent;

            getline(ss, orasCurent, ',');
            getline(ss, profitCurentString, ',');
            profitCurent = stod(profitCurentString);
            // Modificăm profitul pentru orașul specificat
            if (orasCurent == orasCautat) {
                profitCurent = profitCurent - nouProfit;
            }
            profitCurentString = to_string(profitCurent);
            // Reconstruim linia actualizată
            liniiActualizate.push_back(orasCurent + "," + profitCurentString);
        }

        file.close();

        // Rescriem fișierul cu datele actualizate
        ofstream outFile(raportFileName, ios::trunc);
        if (!outFile.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru scriere." << endl;
            return;
        }

        for (const string& linieActualizata : liniiActualizate) {
            outFile << linieActualizata << endl;
        }

        outFile.close();
    }
};
#endif // RAPORT_H