#ifndef PRODUS_H
#define PRODUS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Produs {
private:
    string tipProdus;
    double pret;
    string oras;
    int bucati;  // Câmp pentru numărul de bucăți
    double profit; // Câmp pentru profit

    // Funcție privată pentru a calcula profitul pe baza prețului
    void calculeazaProfit() {
        profit = pret * 0.15;  // Profit de 15% din preț
    }

public:
    // Constructor
    Produs(const string& tipProdus, double pret, const string& oras, int bucati = 0)
            : tipProdus(tipProdus), pret(pret), oras(oras), bucati(bucati) {
        calculeazaProfit();  // Calculăm profitul la crearea produsului
    }

    // Getteri
    string getTipProdus() const { return tipProdus; }
    double getPret() const { return pret; }
    string getOras() const { return oras; }
    int getBucati() const { return bucati; }
    double getProfit() const { return profit; }  // Returnează profitul


    // Setteri
    void setTipProdus(const string& tip) { tipProdus = tip; }
    void setPret(double pretNou) {
        pret = pretNou;
        calculeazaProfit(); // Recalculăm profitul când prețul se modifică
    }
    void setOras(const string& orasNou) { oras = orasNou; }
    void setBucati(int numarBucati) { bucati = numarBucati; }

    // Citirea produselor din fișier
    static vector<Produs> citesteDinFisier(const string& fileName) {
        vector<Produs> produse;
        ifstream file(fileName);

        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fisierului " << fileName << endl;
            return produse;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string tipProdus, pretStr, oras, bucatiStr;

            if (getline(ss, tipProdus, ',') &&
                getline(ss, pretStr, ',') &&
                getline(ss, oras, ',') &&
                getline(ss, bucatiStr, ',')) {

                double pret = stod(pretStr); // Conversie string -> double
                int bucati = stoi(bucatiStr); // Conversie string -> int
                produse.emplace_back(tipProdus, pret, oras, bucati);
            }
        }

        file.close();
        return produse;
    }

    // Adăugarea unui produs în fișier
    static void adaugaInFisier(const string& fileName, const Produs& produsNou) {
        ofstream file(fileName, ios::app);

        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fisierului " << fileName << endl;
            return;
        }

        file << produsNou.toCSV() << endl;
        file.close();
    }

    // Ștergerea unui produs din fișier
    static void stergeDinFisier(const string& fileName, const string& tipProdus, const string& oras) {
        vector<Produs> produse = citesteDinFisier(fileName);

        ofstream file(fileName, ios::trunc); // Rescriem fișierul
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fisierului pentru ștergere: " << fileName << endl;
            return;
        }

        for (const auto& produs : produse) {
            if (produs.getTipProdus() == tipProdus && produs.getOras() == oras) {
                continue; // Sărim peste produsul de șters
            }
            file << produs.toCSV() << endl;
        }

        file.close();
    }

    // Modificarea numărului de bucăți pentru un produs într-un oraș
    static void modificaBucatiInFisier(const string& fileName, const string& tipProdus, const string& oras, int numarBucati) {
        vector<Produs> produse = citesteDinFisier(fileName);

        ofstream file(fileName, ios::trunc); // Rescriem fișierul
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fisierului pentru modificare: " << fileName << endl;
            return;
        }

        for (auto& produs : produse) {
            if (produs.getTipProdus() == tipProdus && produs.getOras() == oras) {
                produs.setBucati(produs.getBucati() + numarBucati); // Adaugă sau scade bucăți
            }
            file << produs.toCSV() << endl;
        }

        file.close();
    }
    static void stergeProduseCuBucatiNegative(const string& fileName) {
        vector<Produs> produse = citesteDinFisier(fileName);

        ofstream file(fileName, ios::trunc); // Rescriem complet fișierul
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fișierului pentru curățare: " << fileName << endl;
            return;
        }

        for (const auto& produs : produse) {
            if (produs.getBucati() >= 0) {
                file << produs.toCSV() << endl; // Scriem în fișier doar produsele valide
            }
        }

        file.close();
    }

    // Serializarea produsului în format CSV
    string toCSV() const {
        return tipProdus + "," + to_string(pret) + "," + oras + "," + to_string(bucati) + "," + to_string(profit);
    }


};

#endif // PRODUS_H
