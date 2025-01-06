#ifndef COMANDA_H
#define COMANDA_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Comanda {
private:
    string numeClient;
    string produsComandat;
    int numarBucati;
    double pretFinal;
    string oras;

public:
    // Constructor
    Comanda(const string& numeClient, const string& produsComandat, int numarBucati, double pretFinal, const string& oras)
            : numeClient(numeClient), produsComandat(produsComandat), numarBucati(numarBucati), pretFinal(pretFinal), oras(oras) {}

    // Getteri
    string getNumeClient() const {
        return numeClient;
    }

    string getProdusComandat() const {
        return produsComandat;
    }

    int getNumarBucati() const {
        return numarBucati;
    }

    double getPretFinal() const {
        return pretFinal;
    }

    string getOras() const {
        return oras;
    }

    // Set pentru oras
    void setOras(const string& orasNou) {
        oras = orasNou;
    }

    // Serializarea comenzii în format CSV
    string toCSV() const {
        return numeClient + "," + produsComandat + "," + to_string(numarBucati) + "," + to_string(pretFinal) + "," + oras;
    }

    // Adăugarea comenzii în fișier
    static void adaugaComandaInFisier(const string& fileName, const Comanda& comanda) {
        ofstream file(fileName, ios::app);

        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fisierului pentru comenzi." << endl;
            return;
        }

        file << comanda.toCSV() << endl;
        file.close();
    }
    // Ștergerea comenzilor din fișier pe baza unui criteriu
    static void stergeComenziDinFisier(const string& fileName, const string& criteriuOras) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Eroare la deschiderea fisierului pentru citire." << endl;
            return;
        }

        vector<string> comenziRamase;
        string linie;

        // Citirea comenzilor și păstrarea celor care nu corespund criteriului
        while (getline(file, linie)) {
            stringstream ss(linie);
            string numeClient, produsComandat, numarBucati, pretFinal, oras;

            getline(ss, numeClient, ',');
            getline(ss, produsComandat, ',');
            getline(ss, numarBucati, ',');
            getline(ss, pretFinal, ',');
            getline(ss, oras, ',');

            if (oras != criteriuOras) {  // Păstrăm comenzile care nu sunt din criteriuOras
                comenziRamase.push_back(linie);
            }
        }

        file.close();

        // Rescrierea fișierului cu comenzile rămase
        ofstream fileOut(fileName);
        if (!fileOut.is_open()) {
            cerr << "Eroare la deschiderea fisierului pentru scriere." << endl;
            return;
        }

        for (const auto& comanda : comenziRamase) {
            fileOut << comanda << endl;
        }

        fileOut.close();
    }
};

#endif // COMANDA_H