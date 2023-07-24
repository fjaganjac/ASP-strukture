#include <iostream>

using namespace std;

template <typename Tip>
struct cvor
{
    Tip vrijednost;
    cvor<Tip> *sljedeci;
};

template <typename Tip>
class Lista
{
private:
    void operator=(const Lista &);
    Lista(const Lista &);

public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip &trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual Tip obrisi() = 0;
    virtual Tip dodajIspred(const Tip &element) = 0;
    virtual Tip dodajIza(const Tip &element) = 0;
    virtual Tip &operator[](int pozicija) const = 0;
};

template <typename Tip>
class NizLista : public Lista<Tip>
{
protected:
    int kapacitet;
    int duzina;
    int tekuci;
    Tip *L;
    void prosiri()
    {
        if (kapacitet == duzina)
        {
            Tip *NL = new Tip[kapacitet * 2];
            for (int i = 0; i < kapacitet; i++)
            {
                NL[i] = L[i];
            }
            kapacitet *= 2;
            delete[] L;
            L = NL;
        }
    }

public:
    NizLista(int size = 10)
    {
        kapacitet = size;
        duzina = tekuci = 0;
        L = new Tip[kapacitet];
    };
    ~NizLista() { delete[] L; }

    int brojElemenata() const { return duzina; }

    Tip &trenutni() const
    {
        if (duzina <= 0)
        {
            throw "lista je prazna";
        }
        return L[tekuci];
    }

    bool prethodni()
    {
        if (duzina == 0)
        {
            throw "lista je prazna";
        }
        if (tekuci != 0)
        {
            tekuci--;
            return true;
        }
        return false;
    }

    bool sljedeci()
    {
        if (duzina == 0)
        {
            throw "lista je prazna";
        }
        if (tekuci < duzina - 1)
        {
            tekuci++;
            return true;
        }
        return false;
    }

    void pocetak()
    {
        if (duzina == 0)
        {
            throw "lista je prazna";
        }
        tekuci = 0;
    }

    void kraj()
    {
        if (duzina == 0)
        {
            throw "lista je prazna";
        }
        tekuci = duzina - 1;
    }

    Tip obrisi()
    {

        if (duzina == 0)
            throw "lista je prazna";
        // delete &L[tekuci];

        if (tekuci != 0 && tekuci == duzina - 1)
        {
            tekuci--;
            duzina--;
        }
        else
        {
            for (int i = tekuci; i < duzina - 1; i++)
            {
                L[i] = L[i + 1];
            }

            if (duzina == tekuci)
                tekuci--;
            duzina--;
        }
        return L[tekuci];
    }

    Tip dodajIza(const Tip &element)
    {
        prosiri();
        for (int i = duzina; i > tekuci + 1; i--)
        {
            L[i] = L[i - 1];
        }
        if (duzina != 0)
            tekuci++;
        L[tekuci] = element;
        duzina++;
        if (duzina != 1)
            tekuci--;

        return element;
    }

    Tip dodajIspred(const Tip &element)
    {
        prosiri();
        for (int i = duzina; i > tekuci; i--)
        {
            L[i] = L[i - 1];
        }
        L[tekuci] = element;
        duzina++;
        if (tekuci != duzina - 1)
        {
            tekuci++;
        }
        return element;
    }

    Tip &operator[](int pozicija) const
    {
        if (pozicija < 0 || pozicija >= duzina)
        {
            throw "trazena pozicija je van raspona";
        }
        return L[pozicija];
    }

    NizLista(const NizLista<Tip> &NL)
    {
        L = new Tip[NL.duzina];
        for (int i = 0; i < NL.duzina; i++)
        {
            L[i] = NL[i];
        }
        tekuci = NL.tekuci;
        duzina = NL.duzina;
    }

    NizLista &operator=(const NizLista<Tip> &NL)
    {
        if (&NL == this)
            return *this;

        if (duzina != 0)
        {
            kraj();
        }
        delete[] L;
        L = new Tip[NL.duzina];
        for (int i = 0; i < NL.duzina; i++)
        {
            L[i] = NL[i];
        }
        tekuci = NL.tekuci;
        duzina = NL.duzina;
        return *this;
    }

    void prikazi()
    {
        for (int i = 0; i < duzina; i++)
        {
            std::cout << "Element " << i << ": " << L[i] << std::endl;
        }
    }
};

template <typename Tip>
class JednostrukaLista : public Lista<Tip>
{
private:
    cvor<Tip> *ppocetak;
    cvor<Tip> *pkraj;
    cvor<Tip> *tekuci;

    int kapacitet;
    int duzina;
    int pozicija_tekuceg;
    cvor<Tip> *L;

public:
    JednostrukaLista()
    {
        duzina = 0;
        ppocetak = nullptr;
        pkraj = nullptr;
        tekuci = nullptr;
        pozicija_tekuceg = 0;
    }
    ~JednostrukaLista()
    {
        if (duzina != 0)
            pocetak();
        while (ppocetak != pkraj)
        {
            cvor<Tip> *p = nullptr;
            p = ppocetak->sljedeci;
            delete ppocetak;
            ppocetak = p;
        }
        delete ppocetak;
        ppocetak = nullptr;
        pkraj = nullptr;
        tekuci = nullptr;
        duzina = 0;
        pozicija_tekuceg = 0;
    }

    int brojElemenata() const { return duzina; }

    Tip &trenutni() const
    {
        if (duzina == 0)
        {
            throw "lista je prazna";
        }
        return tekuci->vrijednost;
    }

    bool prethodni()
    {
        if (duzina == 0)
        {
            throw "lista je prazna";
        }
        if (tekuci != ppocetak)
        {
            cvor<Tip> *p, *n = ppocetak;
            for (p = nullptr; n != tekuci; p = n, n = n->sljedeci)
                ;
            tekuci = p;
            pozicija_tekuceg--;
            return true;
        }
        return false;
    }

    bool sljedeci()
    {
        if (duzina == 0)
        {
            throw "lista je prazna";
        }
        if (tekuci != pkraj)
        {
            tekuci = tekuci->sljedeci;
            return true;
            pozicija_tekuceg++;
        }
        return false;
    }

    void pocetak()
    {
        tekuci = ppocetak;
        pozicija_tekuceg = 0;
    }

    void kraj()
    {
        tekuci = pkraj;
        pozicija_tekuceg = duzina - 1;
    }

    Tip obrisi()
    {
        cvor<Tip> *pok;
        Tip element;
        if (tekuci == pkraj)
        {
            element = pkraj->vrijednost;
            pok = pkraj;
            cvor<Tip> *prethodni = new cvor<Tip>, *n = ppocetak;                   // pri kraju petlje prethodni
                                                                                   // pokazuje na cvor ispred trenutnog
            for (prethodni = nullptr; n != tekuci; prethodni = n, n = n->sljedeci) //
                ;
            prethodni->sljedeci = nullptr;
            pkraj = prethodni;
            tekuci = prethodni;
            delete pok;
            duzina--;
        }
        else if (tekuci == ppocetak)
        {
            element = tekuci->vrijednost;
            pok = ppocetak;
            tekuci = tekuci->sljedeci;
            ppocetak = tekuci;
            delete pok;
            duzina--;
        }
        else
        {
            element = tekuci->vrijednost;
            pok = tekuci;
            cvor<Tip> *prethodni = new cvor<Tip>, *n = ppocetak;                   // pri kraju petlje prethodni
                                                                                   // pokazuje na cvor ispred trenutnog
            for (prethodni = nullptr; n != tekuci; prethodni = n, n = n->sljedeci) //
                ;
            prethodni->sljedeci = tekuci->sljedeci;
            tekuci = tekuci->sljedeci;
            duzina--;
            delete pok;
        }
        return tekuci->vrijednost;
    }

    Tip dodajIspred(const Tip &element)
    {

        if (duzina == 0)
        {
            cvor<Tip> *novi = new cvor<Tip>;
            novi->vrijednost = element;
            ppocetak = novi;
            pkraj = novi;
            tekuci = novi;
            duzina++;
            // delete novi;
        }
        else if (tekuci == ppocetak)
        {
            cvor<Tip> *novi = new cvor<Tip>;
            novi->vrijednost = element;
            novi->sljedeci = tekuci;
            ppocetak = novi;
            duzina++;
            // delete novi;
        }
        else
        {
            cvor<Tip> *p, *n = ppocetak; // p->prethodni
            for (p = nullptr; n != tekuci; p = n, n = n->sljedeci)
                ;
            cvor<Tip> *novi = new cvor<Tip>;
            novi->vrijednost = element;
            novi->sljedeci = p->sljedeci;
            p->sljedeci = novi;
            duzina++;
            // delete novi;
        }
        return element;
    }

    Tip dodajIza(const Tip &element)
    {
        if (duzina == 0)
        {
            cvor<Tip> *novi = new cvor<Tip>;
            novi->vrijednost = element;
            ppocetak = novi;
            pkraj = novi;
            tekuci = novi;
            duzina++;
        }
        else if (tekuci == pkraj)
        {
            cvor<Tip> *novi = new cvor<Tip>;
            novi->vrijednost = element;
            pkraj->sljedeci = novi;
            pkraj = novi;
            duzina++;
        }
        else
        {
            cvor<Tip> *novi = new cvor<Tip>;
            novi->vrijednost = element;
            novi->sljedeci = tekuci->sljedeci;
            tekuci->sljedeci = novi;
            duzina++;
        }
        return element;
    }

    Tip &operator[](int pozicija) const
    {
        cvor<Tip> *pok = ppocetak;
        for (int i = 0; i < pozicija; i++)
        {
            pok = pok->sljedeci;
        }
        return pok->vrijednost;
    }

    JednostrukaLista &operator=(const JednostrukaLista &JL)
    {
        if (brojElemenata() != 0)
        {
            pocetak();
        }
        auto V = brojElemenata();
        for (int i = 0; i < V; i++)
        {
            obrisi();
        }
        ppocetak = 0;
        pkraj = 0;
        tekuci = 0;
        for (int i = 0; i < JL.brojElemenata(); i++)
        {
            dodajIza(JL[i]);
            sljedeci();
        }
        for (int i = 0; i < JL.pozicija_tekuceg; i++)
        {
            sljedeci();
        }
        return *this;
    }

    JednostrukaLista(const JednostrukaLista<Tip> &JL)
    {
        // L = new Tip[NL.duzina];
        L = new cvor<Tip>;
        duzina = JL.duzina;
        ppocetak = JL.ppocetak;
        pkraj = JL.pkraj;
        for (int i = 0; i < JL.duzina; i++)
        {
            L[i] = JL[i];
        }
        tekuci = JL.tekuci;
    }

    void prikazi()
    {
        auto pok = ppocetak;
        while (pok != pkraj)
        {
            cout << pok->vrijednost << " ";
            pok = pok->sljedeci;
        }
    }
};

//////TESTOVI NIZLISTA//////

void TNLPrazanKonstrukor()
{
    // prazan konstruktor treba kreirati listu duzine 0
    NizLista<int> nl;
    cout << "Test prazan konstruktor: ";
    if (!nl.brojElemenata())
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLUnosPrvogElementa()
{
    // metode dodajIza i dodajIspred trebaju pravilno dodati element u praznu
    // listu
    cout << "Test unos prvog elementa: ";
    NizLista<int> nl1, nl2;
    nl1.dodajIza(100);
    nl2.dodajIspred(100);
    if (nl1[0] == 100 && nl2[0] == 100)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLDodajIza()
{
    // metoda dodajIza treba dodati novi član iza tekuceg
    NizLista<int> nl;
    cout << "Test dodaj iza: ";
    for (int i = 0; i < 10; i++)
    { //  nl = 0, 9, 8, 7, 6, 5, 4, 3, 2, 1
        nl.dodajIza(i);
    }
    if (nl[0] == 0 && nl[9] == 1)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLDodajIspred()
{
    // metoda dodajIspred treba dodati novi član iza tekuceg
    NizLista<int> nl;
    cout << "Test dodaj ispred: ";
    for (int i = 0; i < 10; i++)
    { //  nl = 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
        nl.dodajIspred(i);
    }
    if (nl[0] == 1 && nl[9] == 0)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLBrojElemenata()
{
    // dodavanje elemenata bez potrebe za prosirenjem kapaciteta (min kapacitet =
    // 10)
    cout << "Test broj elemenata bez prosirenja: ";
    NizLista<int> nl;
    for (int i = 0; i < 5; i++)
    {
        nl.dodajIza(i);
    }
    if (nl.brojElemenata() == 5)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLBrojElemenataP()
{
    // dodavanje elemenata sa prosirerenjem
    cout << "Test broj elemenata sa prosirenjem: ";
    NizLista<int> nl;
    for (int i = 0; i < 10000; i++)
    {
        nl.dodajIza(i);
    }
    if (nl.brojElemenata() == 10000)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLtrenutni()
{
    // metoda treba vratiti ispravnu trenutnu vrijednost
    cout << "Test trenutni: ";
    NizLista<int> nl;
    for (int i = 0; i < 5; i++)
    {
        nl.dodajIza(i);
    }
    if (nl.trenutni() == 0)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLprethodni()
{
    // metoda treba pomjeriti cursor na prethodni element
    cout << "Test prethodni: ";
    NizLista<int> nl;
    for (int i = 0; i < 5; i++)
    {
        nl.dodajIspred(i); // 1,2,3,4,0
    }
    if (nl.trenutni())
    {
        cout << "✘";
        return;
    }
    for (int i = 0; i < nl.brojElemenata(); i++)
    {
        nl.prethodni();
    }
    if (nl.trenutni() == 1)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLsljedeci()
{
    // metoda treba pomjeriti kursor na sljedeci element
    cout << "Test sljedeci: ";
    NizLista<int> nl;
    for (int i = 0; i < 5; i++)
    {
        nl.dodajIza(i); // 0,4,3,2,1
    }
    if (nl.trenutni())
    {
        cout << "✘";
        return;
    }
    for (int i = 0; i < nl.brojElemenata(); i++)
    {
        nl.sljedeci();
    }
    if (nl.trenutni() == 1)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLPrethodniSljedeciBool()
{
    // metode prethodni i sljedeci trebaju vratiti logicku vrijednost u zavisnosti
    // da li se kursor moze pomjeriti u datom smjeru
    cout << "Test PrethodniSljedeciBool: ";
    NizLista<int> nl1, nl2;
    for (int i = 0; i < 5; i++)
    {
        nl1.dodajIza(i);    // -0-,4,3,2,1
        nl2.dodajIspred(i); // 1,2,3,4,-0-
    }
    if (!nl1.prethodni() || !nl2.sljedeci())
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLPocetak()
{
    // moteda treba kursor postaviti na prvi clan u listi
    cout << "Test pocetak: ";
    NizLista<int> nl;
    for (int i = 0; i < 100; i++)
    {
        nl.dodajIspred(i); // nl = 99,98,97,...,3,2,1,0
        nl.prethodni();
    }
    nl.pocetak();
    if (nl.trenutni() == 99)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLKraj()
{
    // metoda treba kursor postaviti na zadnji clan u listi
    cout << "Test kraj: ";
    NizLista<int> nl;
    for (int i = 0; i < 100; i++)
    {
        nl.dodajIza(i); // nl = 0,1,2,3,...,97,98,99
        nl.sljedeci();
    }
    nl.kraj();
    if (nl.trenutni() == 99)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLObrisi()
{
    // metoda treba obrisati trenutni element i postaviti kursor na sljedeci
    // odnosno prethodni ako je obrisan posljednji clan
    cout << "Test obrisi: ";
    NizLista<int> nl;
    for (int i = 0; i < 10; i++)
    {
        nl.dodajIza(i); // nl = 0,1,2,3,4,5,6,7,8,-9-
        nl.sljedeci();
    }
    nl.prethodni();
    nl.obrisi(); // nl = 0,1,2,3,4,5,6,7,-9-
    if (nl.brojElemenata() != 9 || nl.trenutni() != 9)
    {
        cout << "✘" << endl;
        return;
    }
    nl.obrisi(); // nl = 0,1,2,3,4,5,6,-7-
    if (nl.brojElemenata() != 8 || nl.trenutni() != 7)
    {
        cout << "✘" << endl;
        return;
    }
    cout << "✔" << endl;
}

void TNLOperatorJednako()
{
    // operator = treba varijabli dodijeliti ispravan izraz
    cout << "Test operator jednako: ";
    NizLista<int> nl1, nl2;
    for (int i = 1; i < 25; i++)
    {
        nl1.dodajIspred(i * 2);
    }
    nl2 = nl1;
    if (nl2.brojElemenata() == 24 && nl2.trenutni() == 2)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLOperatorUglasteZagrade()
{
    // operator [] treba vratiti referencu datog clana niza
    cout << "Test operator uglaste zagrade: ";
    NizLista<int> nl;
    for (int i = 1; i <= 15; i++)
    {
        nl.dodajIspred(i); // nl = 15,14,13,12,...,1
        nl.prethodni();
    }
    nl[1] = 1001;
    nl.sljedeci();
    if (nl.trenutni() == 1001)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TNLKopirajuciKonstruktor()
{
    // vrijednosti se mogu dodijeliti koristeci kopirajuci konstruktor
    cout << "Test kopirajuci konstruktor: ";
    NizLista<int> nl;
    for (int i = 1; i <= 10; i++)
    {
        nl.dodajIspred(i); // nl = 2,4,6,8,10,-9-,7,5,3,1
        if (i % 2)
            nl.prethodni();
    }
    NizLista<int> nl2(nl);
    if (nl2.brojElemenata() == nl.brojElemenata() && nl2.trenutni() == 9)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

//////TESTOVI JEDNOSTRUKA LISTA//////

void TJLPrazanKonstrukor()
{
    // prazan konstruktor treba kreirati listu duzine 0
    JednostrukaLista<int> nl;
    cout << "Test prazan konstruktor: ";
    if (!nl.brojElemenata())
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLUnosPrvogElementa()
{
    // metode dodajIza i dodajIspred trebaju pravilno dodati element u praznu
    // listu
    cout << "Test unos prvog elementa: ";
    JednostrukaLista<int> nl1, nl2;
    nl1.dodajIza(1);
    nl2.dodajIspred(1);
    if (nl1[0] == 1 && nl2[0] == 1)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLDodajIza()
{
    // metoda dodajIza treba dodati novi član iza tekuceg
    JednostrukaLista<int> nl;
    cout << "Test dodaj iza: ";
    for (int i = 0; i < 10; i++)
    { //  nl = 0, 9, 8, 7, 6, 5, 4, 3, 2, 1
        nl.dodajIza(i);
    }
    if (nl[0] == 0 && nl[9] == 1)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLDodajIspred()
{
    // metoda dodajIspred treba dodati novi član iza tekuceg
    JednostrukaLista<int> nl;
    cout << "Test dodaj ispred: ";
    for (int i = 0; i < 10; i++)
    { //  nl = 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
        nl.dodajIspred(i);
    }
    if (nl[0] == 1 && nl[9] == 0)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLBrojElemenata()
{
    // dodavanje elemenata bez potrebe za prosirenjem kapaciteta (min kapacitet =
    // 10)
    cout << "Test broj elemenata: ";
    JednostrukaLista<int> nl;
    for (int i = 0; i < 5; i++)
    {
        nl.dodajIza(i);
    }
    if (nl.brojElemenata() == 5)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLtrenutni()
{
    // metoda treba vratiti ispravnu trenutnu vrijednost
    cout << "Test trenutni: ";
    JednostrukaLista<int> nl;
    for (int i = 0; i < 5; i++)
    {
        nl.dodajIza(i);
    }
    if (nl.trenutni() == 0)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLprethodni()
{
    // metoda treba pomjeriti cursor na prethodni element
    cout << "Test prethodni: ";
    JednostrukaLista<int> nl;
    for (int i = 0; i < 5; i++)
    {
        nl.dodajIspred(i); // 1,2,3,4,0
    }
    if (nl.trenutni())
    {
        cout << "✘";
        return;
    }
    for (int i = 0; i < nl.brojElemenata(); i++)
    {
        nl.prethodni();
    }
    if (nl.trenutni() == 1)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLsljedeci()
{
    // metoda treba pomjeriti kursor na sljedeci element
    cout << "Test sljedeci: ";
    JednostrukaLista<int> nl;
    for (int i = 0; i < 5; i++)
    {
        nl.dodajIza(i); // 0,4,3,2,1
    }
    if (nl.trenutni())
    {
        cout << "✘";
        return;
    }
    for (int i = 0; i < nl.brojElemenata(); i++)
    {
        nl.sljedeci();
    }
    if (nl.trenutni() == 1)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLPocetak()
{
    // moteda treba kursor postaviti na prvi clan u listi
    cout << "Test pocetak: ";
    JednostrukaLista<int> nl;
    for (int i = 0; i < 100; i++)
    {
        nl.dodajIspred(i); // nl = 99,98,97,...,3,2,1,0
        nl.prethodni();
    }
    nl.pocetak();
    if (nl.trenutni() == 99)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLKraj()
{
    // metoda treba kursor postaviti na zadnji clan u listi
    cout << "Test kraj: ";
    JednostrukaLista<int> nl;
    for (int i = 0; i < 100; i++)
    {
        nl.dodajIza(i); // nl = 0,1,2,3,...,97,98,99
        nl.sljedeci();
    }
    nl.kraj();
    if (nl.trenutni() == 99)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLObrisi()
{
    // metoda treba obrisati trenutni element i postaviti kursor na sljedeci
    // odnosno prethodni ako je obrisan posljednji clan
    cout << "Test obrisi: ";
    JednostrukaLista<int> nl;
    for (int i = 0; i < 10; i++)
    {
        nl.dodajIza(i); // nl = 0,1,2,3,4,5,6,7,8,-9-
        nl.sljedeci();
    }
    nl.prethodni();
    nl.obrisi(); // nl = 0,1,2,3,4,5,6,7,-9-
    if (nl.brojElemenata() != 9 || nl.trenutni() != 9)
    {
        cout << "✘" << endl;
        return;
    }
    nl.obrisi(); // nl = 0,1,2,3,4,5,6,-7-
    if (nl.brojElemenata() != 8 || nl.trenutni() != 7)
    {
        cout << "✘" << endl;
        return;
    }
    cout << "✔" << endl;
}

void TJLOperatorJednako()
{
    // operator = treba varijabli dodijeliti ispravan izraz
    cout << "Test operator jednako: ";
    JednostrukaLista<int> nl1, nl2;
    for (int i = 1; i < 25; i++)
    {
        nl1.dodajIspred(i * 2);
    }
    nl2 = nl1;
    if (nl2.brojElemenata() == 24 && nl2.trenutni() == 2)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TJLOperatorUglasteZagrade()
{
    // operator [] treba vratiti referencu datog clana niza
    cout << "Test operator uglaste zagrade: ";
    JednostrukaLista<int> nl;
    for (int i = 1; i <= 15; i++)
    {
        nl.dodajIspred(i); // nl = 15,14,13,12,...,1
        nl.prethodni();
    }
    nl[1] = 1001;
    nl.sljedeci();
    if (nl.trenutni() == 1001)
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

int main()
{
    cout << "---Niz Lista---" << endl;
    TNLPrazanKonstrukor();
    TNLBrojElemenata();
    TNLBrojElemenataP();
    TNLUnosPrvogElementa();
    TNLDodajIza();
    TNLDodajIspred();
    TNLtrenutni();
    TNLprethodni();
    TNLPrethodniSljedeciBool();
    TNLsljedeci();
    TNLPocetak();
    TNLKraj();
    TNLObrisi();
    TNLOperatorJednako();
    TNLOperatorUglasteZagrade();
    TNLKopirajuciKonstruktor();
    cout << "---Jednostruka Lista---" << endl;
    TJLPrazanKonstrukor();
    TJLUnosPrvogElementa();
    TJLDodajIza();
    TJLDodajIspred();
    TJLBrojElemenata();
    TJLtrenutni();
    TJLprethodni();
    TJLsljedeci();
    TJLPocetak();
    TJLKraj();
    TJLObrisi();
    TJLOperatorJednako();
    TJLOperatorUglasteZagrade();
    return 0;
}
