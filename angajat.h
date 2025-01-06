#ifndef ANGAJAT_H
#define ANGAJAT_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
///CLASE
// Clasa de bază Angajat
class Angajat {
///INCAPSULARE
protected:  // Facem protejat pentru acces de către clase derivate
    string nume;
    string prenume;
    double salariu;
    string functia;
    double oraInceperiiTurei;
    double oraTerminariiTurei;
    string orasulUndeMuncesc;

public:
    /// Constructor
    Angajat(string n, string p, double s, string f, double oStart, double oEnd, string oras)
            : nume(n), prenume(p), salariu(s), functia(f), oraInceperiiTurei(oStart),
              oraTerminariiTurei(oEnd), orasulUndeMuncesc(oras) {}

    // Getter virtual pentru orasulUndeMuncesc
     string getOrasulUndeMuncesc() const {
        return orasulUndeMuncesc;
    }

    // Getteri
    string getNume() const { return nume; }
    string getPrenume() const { return prenume; }
    double getSalariu() const {return salariu;}

    // Metodă virtuală pentru convertire în format CSV
    virtual string toCSV() const {
        stringstream ss;
        ss << "Angajat," << nume << "," << prenume << "," << salariu << "," << functia << ","
           << oraInceperiiTurei << "," << oraTerminariiTurei << "," << orasulUndeMuncesc;
        return ss.str();
    }
    ///DESTRUCTOR
    virtual ~Angajat() = default; // Destructor virtual

    // Metodă statică pentru ștergerea unei intrări pe baza numelui și prenumelui
    static void stergeDinFisier(const string& fileName, const string& nume, const string& prenume) {
        vector<Angajat*> angajati = citesteDinFisier(fileName);

        // Filtrare pentru eliminare
        angajati.erase(
                remove_if(
                        angajati.begin(),
                        angajati.end(),
                        [&](const Angajat* a) {
                            return a->getNume() == nume && a->getPrenume() == prenume;
                        }),
                angajati.end());

        // Rescrierea fișierului cu lista actualizată
        scrieInFisier(fileName, angajati);

        for (auto angajat : angajati) delete angajat; // Curățare memorie
    }

    static vector<Angajat*> citesteDinFisier(const string& fileName);
    static void scrieInFisier(const string& fileName, const vector<Angajat*>& angajati);
    static void adaugaInFisier(const string& fileName, const Angajat* angajat);

};

// Alte clase
///MOSTENIRE
class Barista : public Angajat {
private:
    int varsta;

public:
    ///Constructor
    Barista(string n, string p, double s, string f, double oStart, double oEnd, string oras, int v)
            : Angajat(n, p, s, f, oStart, oEnd, oras), varsta(v) {}

    string toCSV() const override {
        stringstream ss;
        ss << "Barista," << nume << "," << prenume << "," << salariu << "," << functia << ","
           << oraInceperiiTurei << "," << oraTerminariiTurei << "," << orasulUndeMuncesc << "," << varsta;
        return ss.str();
    }
};

class Manager : public Angajat {
private:
    int nrDeOameniInSubordine;

public:
    Manager(const string& nume, const string& prenume, double salariu, const string& functie,
            double oraInceput, double oraSfarsit, const string& oras, int nrDeOameni)
            : Angajat(nume, prenume, salariu, functie, oraInceput, oraSfarsit, oras), nrDeOameniInSubordine(nrDeOameni) {}

    int getNrDeOameniInSubordine() const {
        return nrDeOameniInSubordine;
    }

    void setNrDeOameniInSubordine(int nrDeOameni) {
        nrDeOameniInSubordine = nrDeOameni;
    }

    string obtineDetalii()  {
        return obtineDetalii() + ", Nr. de subordonati: " + to_string(nrDeOameniInSubordine);
    }
};
///MOSTENIRE
class Ospatar : public Angajat {
private:
    int aniDeExperienta;

public:
    Ospatar(string n, string p, double s, string f, double oStart, double oEnd, string oras, int aniExp)
            : Angajat(n, p, s, f, oStart, oEnd, oras), aniDeExperienta(aniExp) {}

    string toCSV() const override {
        stringstream ss;
        ss << "Ospatar," << nume << "," << prenume << "," << salariu << "," << functia << ","
           << oraInceperiiTurei << "," << oraTerminariiTurei << "," << orasulUndeMuncesc << "," << aniDeExperienta;
        return ss.str();
    }
};

// Funcții globale pentru citire/scriere fișier

vector<Angajat*> Angajat::citesteDinFisier(const string& fileName) {
    vector<Angajat*> angajati;
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Eroare la deschiderea fișierului: " << fileName << endl;
        return angajati;
    }

    string linie;
    while (getline(file, linie)) {
        stringstream ss(linie);
        string tip, nume, prenume, functia, oras;
        double salariu, oraStart, oraEnd;
        int valoareExtra;

        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        ss >> salariu; ss.ignore();
        getline(ss, functia, ',');
        ss >> oraStart; ss.ignore();
        ss >> oraEnd; ss.ignore();
        getline(ss, oras, ',');

        if (tip == "Barista") {
            ss >> valoareExtra;
            angajati.push_back(new Barista(nume, prenume, salariu, functia, oraStart, oraEnd, oras, valoareExtra));
        } else if (tip == "Manager") {
            ss >> valoareExtra;
            angajati.push_back(new Manager(nume, prenume, salariu, functia, oraStart, oraEnd, oras, valoareExtra));
        } else if (tip == "Ospatar") {
            ss >> valoareExtra;
            angajati.push_back(new Ospatar(nume, prenume, salariu, functia, oraStart, oraEnd, oras, valoareExtra));
        } else if (tip == "Angajat") {
            angajati.push_back(new Angajat(nume, prenume, salariu, functia, oraStart, oraEnd, oras));
        }
    }

    file.close();
    return angajati;
}

void Angajat::scrieInFisier(const string& fileName, const vector<Angajat*>& angajati) {
    ofstream file(fileName);

    if (!file.is_open()) {
        cerr << "Eroare la deschiderea fișierului: " << fileName << endl;
        return;
    }

    for (const auto& angajat : angajati) {
        file << angajat->toCSV() << endl;
    }

    file.close();
}
void Angajat::adaugaInFisier(const string& fileName, const Angajat* angajat) {
    ofstream file(fileName, ios::app); // Deschidere în modul "append"
    if (!file.is_open()) {
        cerr << "Eroare la deschiderea fișierului: " << fileName << endl;
        return;
    }

    file << angajat->toCSV() << endl;
    file.close();
}



#endif // ANGAJAT_H