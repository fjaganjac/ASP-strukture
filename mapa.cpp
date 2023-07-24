#include <iostream>
#include <string>

using namespace std;

template <typename TipKljuc, typename TipVrijednost>
class Mapa
{
public:
    Mapa(){};
    virtual ~Mapa(){};
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuc &kljuc) = 0;
    virtual TipVrijednost operator[](const TipKljuc kljuc) const = 0;
    virtual TipVrijednost &operator[](TipKljuc kljuc) = 0;
};

template <typename TipKljuc, typename TipVrijednost>
class NizMapa : public Mapa<TipKljuc, TipVrijednost>
{
private:
    int kapacitet;
    pair<TipKljuc, TipVrijednost> *niz;
    int duzina;
    void prosiri()
    {
        if (kapacitet == duzina)
        {
            pair<TipKljuc, TipVrijednost> *noviNiz =
                new pair<TipKljuc, TipVrijednost>[kapacitet * 2];
            for (int i = 0; i < kapacitet; i++)
            {
                noviNiz[i] = niz[i];
            }
            kapacitet *= 2;
            delete[] niz;
            niz = noviNiz;
        }
    }

public:
    int brojElemenata() const { return duzina; }

    NizMapa(int size = 5)
    {
        kapacitet = size;
        duzina = 0;
        niz = new pair<TipKljuc, TipVrijednost>[kapacitet];
    }

    NizMapa(const NizMapa<TipKljuc, TipVrijednost> &m)
    {
        niz = new pair<TipKljuc, TipVrijednost>[m.duzina];
        for (int i = 0; i < m.duzina; i++)
            niz[i] = m.niz[i];
        duzina = m.duzina;
        kapacitet = m.kapacitet;
    }

    ~NizMapa() { delete[] niz; }

    void obrisi()
    {
        duzina = 0;
        kapacitet = 5;
        delete[] niz;
        niz = new pair<TipKljuc, TipVrijednost>[kapacitet];
    }

    void obrisi(const TipKljuc &kljuc)
    {
        int index = 0;
        for (int i = 0; i < duzina; i++)
        {
            if (niz[i].first == kljuc)
            {
                index = i;
                break;
            }
            if (i == duzina - 1)
                throw "ne postoji element sa datim kljucem";
        }

        for (int i = index; i < duzina - 1; i++)
            niz[i] = niz[i + 1];
        duzina--;
    }

    TipVrijednost operator[](const TipKljuc kljuc) const
    {
        int index;
        for (int i = 0; i < duzina; i++)
        {
            if (niz[i].first == kljuc)
            {
                index = i;
                break;
            }
            if (i == duzina - 1)
                return TipVrijednost();
        }
        return niz[index].second;
    }

    TipVrijednost &operator[](TipKljuc kljuc)
    {
        for (int i = 0; i < duzina; i++)
            if (niz[i].first == kljuc)
                return niz[i].second;

        if (duzina == kapacitet)
            prosiri();
        niz[duzina].first = kljuc;
        niz[duzina].second = TipVrijednost();
        duzina++;
        return niz[duzina - 1].second;
    }
    NizMapa &operator=(const NizMapa<TipKljuc, TipVrijednost> &m)
    {
        if (&m == this)
            return *this;
        delete[] niz;
        niz = new pair<TipKljuc, TipVrijednost>[m.kapacitet];
        kapacitet = m.kapacitet;
        duzina = m.duzina;
        for (int i = 0; i < duzina; i++)
        {
            niz[i] = m.niz[i];
        }
        return *this;
    }
};

void TMapa1()
{
    cout << endl
         << "Test 1" << endl;
    cout << "Očekivani izlaz: Mujic Hasic Fatic 3" << endl
         << "Program je ispisao: ";
    NizMapa<string, string> prezime;
    prezime["Mujo"] = "Mujic";
    cout << prezime["Mujo"] << " ";
    prezime["Haso"] = "Hasic";
    cout << prezime["Haso"] << " ";
    NizMapa<string, string> prezime2;
    prezime2 = prezime;
    prezime2["Fata"] = "Fatic";
    string p = prezime2["Fata"];
    cout << p << " " << prezime2.brojElemenata();
    cout << endl;
}

void TMapa2()
{
    cout << endl
         << "Test 2" << endl;
    cout << "Očekivani izlaz: nula deset dvadeset trideset 40" << endl
         << "Program je ispisao: ";
    NizMapa<int, string> zapis;
    zapis[0] = "nula";
    zapis[10] = "deset";
    zapis[20] = "dvadeset";
    zapis[30] = "trideset";
    for (int i = 0; i < 31; i += 10)
    {
        cout << zapis[i] << " ";
    }
    zapis[30] = "";
    cout << zapis.brojElemenata();
    cout << zapis[30];
    zapis.obrisi();
    cout << zapis.brojElemenata();
    cout << endl;
}

void TMapa3()
{
    cout << endl
         << "Test 3" << endl;
    cout << "Očekivani izlaz: 10000 40000 9000 1001 1000 0" << endl
         << "Program je ispisao: ";
    NizMapa<int, int> kvadrat;
    for (int i = 0; i < 1000; i++)
    {
        kvadrat[i] = i * 2;
    }
    for (int i = 1000; i >= 0; i--)
    {
        kvadrat[i] = i * i;
    }
    cout << kvadrat[100] << " " << kvadrat[200] << " " << kvadrat[300] << " ";
    NizMapa<int, int> kvadrat1(kvadrat);
    cout << kvadrat1.brojElemenata() << " ";
    for (int i = 0; i < 1; i++)
    {
        kvadrat1.obrisi(i);
    }
    cout << kvadrat1.brojElemenata() << " ";
    kvadrat.obrisi();
    cout << kvadrat.brojElemenata();
    cout << endl;
}

void TMapa4()
{
    cout << endl
         << "Test 4" << endl;
    cout << "Očekivani izlaz: 0 0" << endl
         << "Program je ispisao: ";
    NizMapa<string, string> prezime;
    const NizMapa<string, string> &prezime1(prezime);
    prezime["Mujo"] = "Mujic";
    prezime1["Fata"] = "Fatic";
    prezime1[""] = "";
    for (int i = 0; i < 200; i++)
    {
        prezime1["Mujo"] = i;
    }
    prezime1["Haso"] = "Hasic";
    prezime.obrisi();
    cout << prezime1.brojElemenata() << " " << prezime.brojElemenata() << " "
         << prezime["Mujo"] << " " << prezime["Fata"];
    cout << endl;
}

void TMapa5()
{
    cout << endl
         << "Test 5" << endl;
    cout << "Očekivani izlaz: 10021457812 8000 7998" << endl
         << "Program je ispisao: ";
    NizMapa<int, string> m;
    m[100] = "100";
    m[7812] = "7812";
    m[2145] = "2145";
    for (int i = 0; i < 8000; i++)
    {
        cout << m[i];
    }
    cout << " " << m.brojElemenata();
    m.obrisi(100);
    m.obrisi(2145);
    cout << " " << m.brojElemenata();
}

int main()
{
    cout << "---NizMapa---" << endl;
    TMapa1();
    TMapa2();
    TMapa3();
    TMapa4();
    TMapa5();
    return 0;
}