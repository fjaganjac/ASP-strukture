#include <iostream>

using namespace std;

template <typename Tip>
class Iterator;

template <typename Tip>
struct Cvor
{
    Tip vrijednost;
    Cvor *prethodni;
    Cvor *sljedeci;
};

template <typename Tip>
class Lista
{
public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip &trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual Tip dodajIspred(const Tip &element) = 0;
    virtual Tip dodajIza(const Tip &element) = 0;
    virtual Tip &operator[](int pozicija) const = 0;
};

template <typename Tip>
class DvostrukaLista : public Lista<Tip>
{
private:
    Cvor<Tip> *ppocetak;
    Cvor<Tip> *pkraj;
    Cvor<Tip> *tekuci;
    int duzina;
    int poz_trenutnog;

public:
    DvostrukaLista()
    {
        ppocetak = pkraj = tekuci = 0;
        duzina = poz_trenutnog = 0;
    }

    int brojElemenata() const { return duzina; }

    int pozicijaTekuceg() const { return poz_trenutnog; }

    ~DvostrukaLista()
    {
        if (duzina != 0)
            pocetak();
        while (ppocetak != pkraj)
        {
            Cvor<Tip> *p = 0;
            p = ppocetak->sljedeci;
            delete ppocetak;
            ppocetak = p;
        }
        delete ppocetak;
        ppocetak = 0;
        pkraj = 0;
        tekuci = 0;
        duzina = 0;
        poz_trenutnog = 0;
    }

    Tip &trenutni() const
    {
        if (!duzina)
            throw "Dvostruka lista je prazna! :&trenutni()";
        return tekuci->vrijednost;
    }

    bool prethodni()
    {
        if (!duzina)
            throw "Dvostruka lista je prazna! :prethodni()";
        if (tekuci != ppocetak)
        {
            tekuci = tekuci->prethodni;
            poz_trenutnog--;
            return true;
        }
        return false;
    }

    bool sljedeci()
    {
        if (!duzina)
            throw "Dvostruka lista je prazna! :sljedeci()";
        if (tekuci != pkraj)
        {
            tekuci = tekuci->sljedeci;
            poz_trenutnog++;
            return true;
        }
        return false;
    }

    void kraj()
    {
        if (!duzina)
            throw "Dvostruka lista je prazna! :kraj()";
        tekuci = pkraj;
        poz_trenutnog = duzina - 1;
    }

    void pocetak()
    {
        if (!duzina)
            throw "Dvostruka lista je prazna! :pocetak()";
        tekuci = ppocetak;
        poz_trenutnog = 0;
    }

    Tip dodajIspred(const Tip &element)
    {
        Cvor<Tip> *pok = new Cvor<Tip>;
        pok->vrijednost = element;
        if (!duzina)
        {
            tekuci = ppocetak = pkraj = pok;
        }
        else if (tekuci == ppocetak)
        {
            pok->sljedeci = tekuci;
            tekuci->prethodni = ppocetak = pok;
        }
        else
        {
            pok->sljedeci = tekuci;
            pok->prethodni = tekuci->prethodni;
            tekuci->prethodni->sljedeci = pok;
            tekuci->prethodni = pok;
        }
        duzina++;
        poz_trenutnog++;
        return element;
    }

    Tip dodajIza(const Tip &element)
    {
        Cvor<Tip> *pok = new Cvor<Tip>;
        pok->vrijednost = element;
        if (!duzina)
            tekuci = ppocetak = pkraj = pok;
        if (tekuci == pkraj)
        {
            pok->prethodni = tekuci;
            tekuci->sljedeci = pok;
            pkraj = pok;
        }
        else
        {
            pok->prethodni = tekuci;
            pok->sljedeci = tekuci->sljedeci;
            tekuci->sljedeci->prethodni = pok;
            tekuci->sljedeci = pok;
        }
        duzina++;
        return element;
    }

    void obrisi()
    {
        if (!duzina)
            cout << "Dvostruka lista je prazna! :obrisi()";
        else if (duzina == 1)
        {
            delete tekuci;
            ppocetak = pkraj = tekuci = 0;
        }
        else if (tekuci == ppocetak)
        {
            ppocetak = tekuci->sljedeci;
            delete tekuci;
            tekuci = ppocetak;
        }
        else if (tekuci == pkraj)
        {
            pkraj = tekuci->prethodni;
            delete tekuci;
            tekuci = pkraj;
            poz_trenutnog--;
        }
        else
        {
            Cvor<Tip> *temp = tekuci;
            tekuci = tekuci->sljedeci;
            temp->prethodni->sljedeci = tekuci;
            tekuci->prethodni = temp->prethodni;
            delete temp;
        }
        duzina--;
    }

    Tip &operator[](int pozicija) const
    {
        if (!duzina)
            throw "Dvostruka lista je prazna :operator[]";
        Cvor<Tip> *pok = ppocetak;
        for (int i = 0; i < pozicija; i++)
        {
            pok = pok->sljedeci;
        }
        return pok->vrijednost;
    }

    DvostrukaLista<Tip> &operator=(const DvostrukaLista<Tip> &DL)
    {
        if (&DL == this)
            return *this;
        if (duzina)
        {
            pocetak();
        }
        kopija(DL);
        return *this;
    }

    DvostrukaLista(const DvostrukaLista &DL) { kopija(DL); }

    void kopija(const DvostrukaLista &DL)
    {
        ppocetak = 0;
        pkraj = 0;
        tekuci = 0;
        duzina = 0;
        poz_trenutnog = 0;
        Cvor<Tip> *pok = DL.ppocetak;
        for (int i = 0; i < DL.brojElemenata(); i++)
        {
            dodajIza(pok->vrijednost);
            sljedeci();
            pok = pok->sljedeci;
        }
        pocetak();
        for (int i = 0; i < DL.pozicijaTekuceg(); i++)
        {
            sljedeci();
        }
        this->duzina = DL.brojElemenata();
    }

    void prikazi()
    {
        int T = 0;
        for (Cvor<Tip> *pok = ppocetak; pok != tekuci; pok = pok->sljedeci)
            T++;
        Cvor<Tip> *pok = ppocetak;
        for (int i = 0; i < duzina; i++)
        {
            cout << "element " << i << ": " << pok->vrijednost;
            if (i == T)
                cout << "<---tekuci";
            cout << endl;
            pok = pok->sljedeci;
        }
    }

    friend class Iterator<Tip>;
};

template <typename Tip>
class Iterator
{ // klasa Iterator nije koristena u drugom zadatku
private:
    const DvostrukaLista<Tip> *L;
    Cvor<Tip> *tekuci;

public:
    Iterator(const DvostrukaLista<Tip> &L)
    {
        this->L = &L;
        tekuci = L.tekuci;
    }

    Tip &trenutni()
    {
        if (!L->brojElemenata())
        {
            throw "Dvostruka lista je prazna :&trenutni()";
        }
        return tekuci->vrijednost;
    }

    bool prethodni()
    {
        if (!L->brojElemenata())
        {
            throw "Dvostruka lista je prazna :prethodni()";
        }
        if (tekuci != L->ppocetak)
        {
            tekuci = tekuci->prethodni;
            return true;
        }
        return false;
    }

    bool sljedeci()
    {
        if (!L->brojElemenata())
        {
            throw "Dvostruka lista je prazna :sljedeci()";
        }
        if (tekuci != L->pkraj)
        {
            tekuci = tekuci->sljedeci;
            return true;
        }
        return false;
    }

    void pocetak()
    {
        if (!L->brojElemenata())
        {
            throw "Dvostruka lista je prazna :pocetak()";
        }
        tekuci = L->ppocetak;
    }

    void kraj()
    {
        if (!L->brojElemenata())
        {
            throw "Dvostruka lista je prazna :kraj()";
        }
        tekuci = L->pkraj;
    }
};

template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &L)
{ // dajMaksimum bez Iteratora
    Tip max = 0;
    if (!L.brojElemenata())
    {
        return 0;
    }
    Lista<Tip> *lista = (DvostrukaLista<Tip> *)(&L);
    int *temp = &(lista->trenutni());
    lista->pocetak();
    max = lista->trenutni();
    for (int i = 0; i < L.brojElemenata(); i++)
    {
        if (lista->trenutni() > max)
        {
            max = lista->trenutni();
        }
        lista->sljedeci();
    }

    while (&lista->trenutni() != temp)
    {
        lista->prethodni();
    }
    return max;
}

////////////////////////////////////////////////////////////////////////TESTOVI////////////////////////////////////////////////////////////////////////

void TDLPrazanKonstruktor()
{
    cout << "Test prazan konstruktor: ";
    DvostrukaLista<int> DL;
    if (!DL.brojElemenata())
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDLBrojElemenata()
{
    cout << "Test brojElemenata(): ";
    DvostrukaLista<int> DL;
    DL.dodajIza(1);
    DL.dodajIza(1);
    DL.dodajIza(1);
    if (DL.brojElemenata() == 3)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDLDestruktor()
{
    cout << "Test destruktora: ";
    try
    {
        DvostrukaLista<int> *pok = new DvostrukaLista<int>;
        delete pok;
    }
    catch (...)
    {
        cout << "PADA" << endl;
    }
    cout << "PROLAZI" << endl;
}

void TDLKonstruktorKopije()
{
    cout << "Test konstruktora kopija: ";
    try
    {
        DvostrukaLista<int> DL1;
        for (int i = 0; i < 10; i++)
        {
            DL1.dodajIza(i * i);
        }
        DvostrukaLista<int> DL2(DL1);
        for (int i = 0; i < DL1.brojElemenata(); i++)
        {
            if (DL1.trenutni() != DL2.trenutni())
            {
                cout << "PADA" << endl;
                return;
            }
            DL1.sljedeci();
            DL2.sljedeci();
        }
        cout << "PROLAZI" << endl;
    }
    catch (...)
    {
        cout << "PADA" << endl;
    }
}

void TDLOperatorDodijele()
{
    cout << "Test operatora dodijele: ";
    try
    {
        DvostrukaLista<int> DL1;
        for (int i = 0; i < 10; i++)
        {
            DL1.dodajIza(i * i);
        }
        DvostrukaLista<int> DL2(DL1);
        for (int i = 0; i < DL1.brojElemenata(); i++)
        {
            if (DL1.trenutni() != DL2.trenutni())
            {
                cout << "PADA" << endl;
                return;
            }
            DL1.sljedeci();
            DL2.sljedeci();
        }
        cout << "PROLAZI" << endl;
    }
    catch (...)
    {
        cout << "PADA" << endl;
    }
}

void TDLTrenutni()
{
    cout << "Test trenutni(): ";
    DvostrukaLista<int> DL;
    for (int i = 0; i < 5; i++)
    {
        DL.dodajIza(i + 5);
        DL.sljedeci();
        if (DL.trenutni() != i + 5)
        {
            cout << "PADA" << endl;
            return;
        }
    }
    cout << "PROLAZI" << endl;
}

void TDLTrenutniDodjela()
{
    cout << "Test dodjela trenutnog: ";
    DvostrukaLista<int> DL;
    for (int i = 0; i < 5; i++)
    {
        DL.dodajIza(i + 5);
        DL.sljedeci();
        if (DL.trenutni() != i + 5)
        {
            cout << "PADA" << endl;
            return;
        }
    }
    DL.trenutni() = 10001;
    if (DL.trenutni() == 10001)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDLSljedeci()
{
    cout << "Test sljedeci(): ";
    DvostrukaLista<int> DL;
    DL.dodajIza(1);
    DL.dodajIza(4);
    DL.dodajIza(3);
    DL.dodajIza(2); // DL = 1,2,3,4
    for (int i = 1; i < 5; i++)
    {
        if (DL.trenutni() != i)
        {
            cout << "PADA" << endl;
            return;
        }
        DL.sljedeci();
    }
    cout << "PROLAZI" << endl;
}

void TDLPrethodni()
{
    cout << "Test prethodni(): ";
    DvostrukaLista<int> DL;
    DL.dodajIspred(1);
    DL.dodajIspred(4);
    DL.dodajIspred(3);
    DL.dodajIspred(2); // DL = 4,3,2,1
    for (int i = 1; i <= 4; i++)
    {
        if (DL.trenutni() != i)
        {
            cout << "PADA" << endl;
            return;
        }
        DL.prethodni();
    }
    cout << "PROLAZI" << endl;
}

void TDLPocetak()
{
    cout << "Test pocetak(): ";
    DvostrukaLista<int> DL;
    for (int i = 0; i <= 45000; i++)
    {
        DL.dodajIza(i);
        DL.sljedeci();
    }
    DL.pocetak();
    if (DL.trenutni() == 0)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDLKraj()
{
    cout << "Test kraj(): ";
    DvostrukaLista<int> DL;
    for (int i = 0; i <= 45000; i++)
    {
        DL.dodajIspred(i);
        DL.prethodni();
    }
    DL.kraj();
    if (DL.trenutni() == 0)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDLObrisi()
{
    cout << "Test obrisi(): ";
    DvostrukaLista<int> DL;
    for (int i = 0; i <= 10; i++)
    {
        DL.dodajIza(i);
        DL.sljedeci();
    }

    while (DL.brojElemenata() > 5)
    { // DL = 0,1,2,3,:4:
        DL.obrisi();
    }
    if (DL.brojElemenata() != 5 || DL.trenutni() != 4)
    {
        cout << "PADA" << endl;
        return;
    }
    DL.pocetak();
    for (int i = 0; i < DL.brojElemenata(); i++)
    {
        if (DL.trenutni() != i)
        {
            cout << "PADA" << endl;
            return;
        }
        DL.sljedeci();
    }
    cout << "PROLAZI" << endl;
}

void TDLDodajIspred()
{
    cout << "Test dodajIspred(): ";
    DvostrukaLista<int> DL;
    for (int i = 0; i < 20; i++)
    {
        DL.dodajIza(i * (312 / 52) - 211);
    }
    DL.dodajIspred(1);
    DL.prethodni();
    if (DL.trenutni() == 1)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDLDodajIza()
{
    cout << "Test dodajIza(): ";
    DvostrukaLista<int> DL;
    for (int i = 0; i < 20; i++)
    {
        DL.dodajIspred(i * (312 / 52) - 211);
    }
    DL.dodajIza(1);
    DL.sljedeci();
    if (DL.trenutni() == 1)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDLOperatorPristupaClana()
{
    cout << "Test operatora pristupa clanu: ";
    DvostrukaLista<int> DL;
    for (int i = 0; i < 10; i++)
    {
        if (!i % 2)
            DL.dodajIza(i * i - 100);
        else
        {
            DL.dodajIspred(35 * i);
            DL.prethodni(); // DL = :35:,280,245,210,175,140,105,70,35,-100,
        }
    }
    int ispravnaLista[10] = {315, 280, 245, 210, 175, 140, 105, 70, 35, -100};
    for (int i = 0; i < DL.brojElemenata(); i++)
    {
        if (ispravnaLista[i] != DL[i])
        {
            cout << "PADA" << endl;
            return;
        }
    }
    cout << "PROLAZI" << endl;
}

void TdajMaksimum1()
{
    cout << "Test dajMaksimum (najveci je u sredini): ";
    DvostrukaLista<int> DL;
    int n = 200000;
    int MAXIMUM = 200001;
    for (int i = 0; i <= n; i++)
    {
        if (i == n / 2)
            DL.dodajIspred(MAXIMUM);
        else
            DL.dodajIspred(i);
        DL.prethodni(); // DL = 200000,199999,...,200001,...,1,0
    }
    if (dajMaksimum(DL) == MAXIMUM)
    {
        cout << "PROLAZI" << endl;
    }
    else
        cout << "PADA" << endl;
}

void TdajMaksimum2()
{
    cout << "Test dajMaksimum (najveci je na pocetku): ";
    DvostrukaLista<int> DL;
    int n = 10000;
    for (int i = 0; i <= n; i++)
    {
        if (i > 3545)
        {
            DL.obrisi();
            DL.dodajIza(i);
        }
        else
        {
            DL.dodajIspred(i * i);
            DL.prethodni();
        }
    }
    DL.dodajIspred(999999999);
    DL.pocetak();
    if (DL.trenutni() == dajMaksimum(DL))
    {
        cout << "PROLAZI" << endl;
    }
    else
        cout << "PADA" << endl;
}

void TdajMaksimum3()
{
    cout << "Test dajMaksimum (najveci je na kraju): ";
    DvostrukaLista<int> DL;
    int n = 10000;
    for (int i = 0; i <= n; i++)
    {

        if (!i % 3)
        {
            DL.dodajIza(i * 3);
        }
        else
        {
            DL.dodajIza(i * i);
            DL.sljedeci();
        }
    }
    DL.kraj();
    if (DL.trenutni() == dajMaksimum(DL))
    {
        cout << "PROLAZI" << endl;
    }
    else
        cout << "PADA" << endl;
}

void TdajMaksimum4()
{
    cout << "Test dajMaksimum (rucno punjenje liste): ";
    DvostrukaLista<int> DL;
    DL.dodajIza(12032);
    DL.dodajIspred(11);
    DL.dodajIspred(-4211);
    DL.obrisi();
    DL.kraj();
    DL.dodajIza(82101);
    DL.dodajIspred(1000261);
    DL.prethodni();
    DL.prethodni();
    DL.dodajIspred(99999999); // DL = 11,99999999,1000261,-4211,82101
    if (99999999 == dajMaksimum(DL))
    {
        cout << "PROLAZI" << endl;
    }
    else
        cout << "PADA" << endl;
}

int main()
{
    cout << "---Dvostruka Lista---" << endl;
    TDLPrazanKonstruktor();
    TDLBrojElemenata();
    TDLDestruktor();
    TDLKonstruktorKopije();
    TDLOperatorDodijele();
    TDLTrenutni();
    TDLTrenutniDodjela();
    TDLSljedeci();
    TDLPrethodni();
    TDLPocetak();
    TDLKraj();
    TDLObrisi();
    TDLDodajIspred();
    TDLDodajIza();
    TDLOperatorPristupaClana();
    cout << endl
         << "---dajMaksimum(const Lista<Tip> &L)---" << endl;
    TdajMaksimum1();
    TdajMaksimum2();
    TdajMaksimum3();
    TdajMaksimum4();
    return 0;
}