#include <ctime>
#include <iostream>
#include <string>

using namespace std;

template <typename TipKljuc, typename TipVrijednost>
struct BinStabloMapaCvor
{
    TipKljuc kljuc;
    TipVrijednost vrijednost;
    BinStabloMapaCvor *lijevi = nullptr;
    BinStabloMapaCvor *desni = nullptr;
    BinStabloMapaCvor *roditelj = nullptr;
};

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
class BinStabloMapa : public Mapa<TipKljuc, TipVrijednost>
{
private:
    BinStabloMapaCvor<TipKljuc, TipVrijednost> *korijen;
    int duzina;

public:
    BinStabloMapa()
    {
        korijen = nullptr;
        duzina = 0;
    }

    ~BinStabloMapa()
    {
        if (duzina)
            obrisi();
    }

    int brojElemenata() const { return duzina; }

    void obrisi(const TipKljuc &br)
    {
        BinStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;
        BinStabloMapaCvor<TipKljuc, TipVrijednost> *q = nullptr;
        BinStabloMapaCvor<TipKljuc, TipVrijednost> *y = nullptr;

        // p = korijen;
        while (p && p->kljuc != br)
        {
            q = p;
            if (p->kljuc > br)
            {
                p = p->lijevi;
            }
            else
            {
                p = p->desni;
            }
        }
        if (p == nullptr)
        {
            return;
        }
        if (!p->lijevi)
        {
            y = p->desni;
        }
        else
        {
            if (!p->desni)
            {
                y = p->lijevi;
            }
            else
            {
                y = p->lijevi;
                BinStabloMapaCvor<TipKljuc, TipVrijednost> *x; // = nullptr;
                BinStabloMapaCvor<TipKljuc, TipVrijednost> *r; // = nullptr;
                x = y->lijevi;
                r = p;
                while (x != nullptr)
                {
                    r = y;
                    y = x;
                    x = y->desni;
                }
                if (r != p)
                {
                    r->desni = y->lijevi;
                    y->lijevi = p->lijevi;
                }
                y->desni = p->desni;
            }
        }

        if (q == nullptr)
        {
            korijen = y;
        }
        else
        {
            if (p == q->desni)
            {
                q->desni = y;
            }
            else
            {
                q->lijevi = y;
            }
        }
        duzina--;
        delete p;
    }

    void obrisi()
    {
        while (korijen != nullptr)
            obrisi(korijen->kljuc);
    }

    TipVrijednost &operator[](TipKljuc noviKljuc)
    {
        BinStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;
        while (p != nullptr)
        {
            if (p->kljuc == noviKljuc)
                return p->vrijednost;
            else if (p->kljuc < noviKljuc)
                p = p->desni;
            else if (p->kljuc > noviKljuc)
                p = p->lijevi;
        }

        BinStabloMapaCvor<TipKljuc, TipVrijednost> *y = korijen;
        BinStabloMapaCvor<TipKljuc, TipVrijednost> *q = nullptr;
        p = new BinStabloMapaCvor<TipKljuc, TipVrijednost>;
        p->kljuc = noviKljuc;
        p->vrijednost = TipVrijednost();
        while (y != nullptr)
        {
            q = y;
            if (y->kljuc < p->kljuc)
            {
                y = y->desni;
            }
            else
            {
                y = y->lijevi;
            }
        }
        if (q == nullptr)
        {
            korijen = p;
        }
        else if (q->kljuc > p->kljuc)
        {
            q->lijevi = p;
            p->roditelj = q;
        }
        else
        {
            q->desni = p;
            p->roditelj = q;
        }

        duzina++;
        return p->vrijednost;
    }

    TipVrijednost operator[](const TipKljuc noviKljuc) const
    {
        BinStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;

        while (p != nullptr)
        {
            if (p->kljuc == noviKljuc)
                return p->vrijednost;
            else if (p->kljuc < noviKljuc)
                p = p->desni;
            else if (p->kljuc > noviKljuc)
                p = p->lijevi;
        }

        TipVrijednost t = TipVrijednost();
        return t;
    }

    void kopiraj(BinStabloMapaCvor<TipKljuc, TipVrijednost> *korijen)
    {
        if (korijen)
        {
            (*this)[korijen->kljuc] = korijen->vrijednost;
            kopiraj(korijen->lijevi);
            kopiraj(korijen->desni);
        }
    }

    BinStabloMapa(BinStabloMapa<TipKljuc, TipVrijednost> &BSM)
    {
        if (BSM.duzina != 0)
        {
            kopiraj(BSM.korijen);
        }
        duzina = BSM.brojElemenata();
    }

    BinStabloMapa &operator=(const BinStabloMapa<TipKljuc, TipVrijednost> &BSM)
    {
        if (this == &BSM)
            return *this;
        obrisi();
        kopiraj(BSM.korijen);
        duzina = BSM.brojElemenata();
        return *this;
    }
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
        int index = 0;
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

void test()
{
    NizMapa<int, int> NM;
    BinStabloMapa<int, int> BSM;

    srand(static_cast<int>(time(nullptr)));
    for (int i = 0; i < 5000; i++)
    {
        NM[rand() % 100000] = rand() % 100000;
    }
    for (int i = 0; i < 5000; i++)
    {
        BSM[rand() % 100000] = rand() % 100000;
    }

    clock_t dodajNM1, dodajNM2, pristupiNM1, pristupiNM2, dodajBSM1, dodajBSM2,
        pristupBSM1, pristupBSM2;

    cout << "---NizMapa---" << endl;

    dodajNM1 = clock();
    NM[1000001] = 1000001;
    dodajNM2 = clock();
    // cout << NM.brojElemenata();
    cout << "za dodavanje novog elementa u mapu potrebno je "
         << (dodajNM2 - dodajNM1) / (CLOCKS_PER_SEC / 1000) << " ms." << endl;

    pristupiNM1 = clock();
    NM[1000001] = 1;
    pristupiNM2 = clock();
    cout << "za pristup postojećem elementu u mapi potrebno je "
         << (pristupiNM2 - pristupiNM1) / (CLOCKS_PER_SEC / 1000) << " ms."
         << endl;

    cout << "---BinStabloMapa---" << endl;

    dodajBSM1 = clock();
    BSM[1000001] = 1000001;
    dodajBSM2 = clock();

    cout << "za dodavanje novog elementa u mapu potrebno je "
         << (dodajBSM2 - dodajBSM1) / (CLOCKS_PER_SEC / 1000) << " ms." << endl;

    pristupBSM1 = clock();
    BSM[1000001] = 1;
    pristupBSM2 = clock();
    cout << "za pristup postojećem elementu u mapi potrebno je "
         << (pristupBSM2 - pristupBSM1) / (CLOCKS_PER_SEC / 1000) << " ms."
         << endl;
}

// izgleda da je dodavanje novog elementa u obje mape minimalno dok je pristup
// postojecem elementu znatno sporije za binarno stablo mapa

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////OVDJE
/// POCINJE AVL
/// STABLO////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TipKljuc, typename TipVrijednost>
struct AVLStabloMapaCvor
{
    TipKljuc kljuc;
    TipVrijednost vrijednost;
    AVLStabloMapaCvor *lijevi = nullptr;
    AVLStabloMapaCvor *desni = nullptr;
    AVLStabloMapaCvor *roditelj = nullptr;
    int balans = 0;
};

template <typename TipKljuc, typename TipVrijednost>
class AVLStabloMapa : public Mapa<TipKljuc, TipVrijednost>
{
private:
    AVLStabloMapaCvor<TipKljuc, TipVrijednost> *korijen;
    int duzina;

public:
    AVLStabloMapa()
    {
        korijen = nullptr;
        duzina = 0;
    }

    ~AVLStabloMapa()
    {
        if (duzina)
            obrisi();
    }

    int brojElemenata() const { return duzina; }

    void rotirajDesno(AVLStabloMapaCvor<TipKljuc, TipVrijednost> *cvor)
    {
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *rotD = cvor->lijevi->desni;
        cvor->balans = 0;
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *l = cvor->lijevi;
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *lr = cvor->roditelj;
        l->roditelj = lr;
        l->desni = cvor;
        bool flag = false;

        if (l->roditelj == nullptr || lr->desni == cvor)
        {
            flag = true;
            if (l->roditelj == nullptr)
            {
                korijen = l;
            }

            else if (lr->desni == cvor)
            {
                l->roditelj->desni = l;
            }
        }
        if (flag == false)
        {
            l->roditelj->lijevi = l;
        }
        if (rotD != nullptr)
        {
            rotD->roditelj = cvor;
        }
        l->balans = 0;
        cvor->lijevi = rotD;
        cvor->roditelj = l;
    }

    void rotirajLijevo(AVLStabloMapaCvor<TipKljuc, TipVrijednost> *cvor)
    {
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *rotL = cvor->desni->lijevi;
        cvor->balans = 0;
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *d = cvor->desni;
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *dr = cvor->roditelj;
        d->roditelj = dr;
        d->lijevi = cvor;
        bool flag = false;

        if (d->roditelj == nullptr || dr->desni == cvor)
        {
            flag = true;
            if (d->roditelj == nullptr)
            {
                korijen = d;
            }

            else if (dr->desni == cvor)
            {
                d->roditelj->desni = d;
            }
        }
        if (flag == false)
        {
            d->roditelj->lijevi = d;
        }
        if (rotL != nullptr)
        {
            rotL->roditelj = cvor;
        }
        d->balans = 0;
        cvor->desni = rotL;
        cvor->roditelj = d;
    }

    void balansR(AVLStabloMapaCvor<TipKljuc, TipVrijednost> *cvor, int flag = 0)
    {

        if (!cvor)
        {
            return;
        }
        if (!flag)
        {
            cvor->balans++;
        }
        else
        {
            cvor->balans--;
        }

        if (cvor->roditelj && cvor->roditelj->desni == cvor)
            flag = 1;
        else
            flag = 0;

        if (cvor->balans == -1)
        {
            if (cvor->desni->balans == -1)
            {
                rotirajLijevo(cvor);
            }
            else
            {
                rotirajDesno(cvor->desni);
                rotirajLijevo(cvor);
            }
        }
        else if (cvor->balans == 2)
        {
            if (cvor->lijevi->balans == 1)
            {
                rotirajDesno(cvor);
            }
            else
            {
                rotirajLijevo(cvor->lijevi);
                rotirajDesno(cvor);
            }
        }
        else
        {
            if (cvor->balans == 0)
                return;

            balansR(cvor->roditelj, flag);
        }
    }

    void obrisi(const TipKljuc &br)
    {
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *q = nullptr;
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *y = nullptr;

        // p = korijen;
        while (p && p->kljuc != br)
        {
            q = p;
            if (p->kljuc > br)
            {
                p = p->lijevi;
            }
            else
            {
                p = p->desni;
            }
        }
        if (p == nullptr)
        {
            return;
        }
        if (!p->lijevi)
        {
            y = p->desni;
        }
        else
        {
            if (!p->desni)
            {
                y = p->lijevi;
            }
            else
            {
                y = p->lijevi;
                AVLStabloMapaCvor<TipKljuc, TipVrijednost> *x; // = nullptr;
                AVLStabloMapaCvor<TipKljuc, TipVrijednost> *r; // = nullptr;
                x = y->desni;
                r = p;
                while (x != nullptr)
                {
                    r = y;
                    y = x;
                    x = y->desni;
                }
                if (r != p)
                {
                    r->desni = y->lijevi;
                    if (y->lijevi)
                    {
                        y->lijevi->roditelj = r;
                    }
                    y->lijevi = p->lijevi;
                    if (p->lijevi)
                    {
                        p->lijevi->roditelj = y;
                    }
                }
                y->desni = p->desni;
                if (p->desni)
                {
                    p->desni->roditelj = y;
                }
            }
        }

        if (q == nullptr)
        {
            korijen = y;
        }
        else
        {
            if (p == q->lijevi)
            {
                q->lijevi = y;
                q->balans--;
            }
            else
            {
                q->lijevi = y;
                q->balans--;
            }
        }

        if (y != nullptr)
        {
            y->roditelj = q;
        }
        p->roditelj = nullptr;
        p->desni = nullptr;
        p->lijevi = nullptr;
        duzina--;
        delete p;
    }

    void obrisi()
    {
        while (korijen != nullptr)
            obrisi(korijen->kljuc);
    }

    TipVrijednost &operator[](TipKljuc noviKljuc)
    {
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;

        while (p != nullptr)
        {
            if (p->kljuc == noviKljuc)
                return p->vrijednost;
            else if (p->kljuc < noviKljuc)
                p = p->desni;
            else if (p->kljuc > noviKljuc)
                p = p->lijevi;
        }

        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *y = korijen;
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *q = nullptr;
        p = new AVLStabloMapaCvor<TipKljuc, TipVrijednost>;
        p->kljuc = noviKljuc;
        p->vrijednost = TipVrijednost();
        while (y != nullptr)
        {
            q = y;
            if (y->kljuc < p->kljuc)
            {
                y = y->desni;
            }
            else
            {
                y = y->lijevi;
            }
        }
        if (q == nullptr)
        {
            korijen = p;
        }
        else if (q->kljuc > p->kljuc)
        {
            q->lijevi = p;
            p->roditelj = q;
        }
        else
        {
            q->desni = p;
            p->roditelj = q;
        }

        if (q != nullptr)
        {
            if (q->lijevi != nullptr && q->desni != nullptr)
            {
                q->balans = 0;
            }
            else
            {
                int flag = 0;
                if (q->desni)
                {
                    flag = 1;
                }
                balansR(q, flag);
            }
        }
        duzina++;
        return p->vrijednost;
    }

    AVLStabloMapa(const AVLStabloMapa &AVL)
    {
        kopiraj(korijen, AVL.korijen);
        duzina = AVL.duzina;
    }

    TipVrijednost operator[](const TipKljuc noviKljuc) const
    {
        AVLStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;

        while (p != nullptr)
        {
            if (p->kljuc == noviKljuc)
                return p->vrijednost;
            else if (p->kljuc < noviKljuc)
                p = p->desni;
            else if (p->kljuc > noviKljuc)
                p = p->lijevi;
        }

        TipVrijednost t = TipVrijednost();
        return t;
    }

    void kopiraj(AVLStabloMapaCvor<TipKljuc, TipVrijednost> *&c1, AVLStabloMapaCvor<TipKljuc, TipVrijednost> *c2)
    {
        if (c2 == nullptr)
        {
            return;
        }
        c1 = new AVLStabloMapaCvor<TipKljuc, TipVrijednost>;
        c1->vrijednost = c2->vrijednost;
        c1->kljuc = c2->kljuc;

        c1->roditelj = c2->roditelj;
        c1->lijevi = nullptr;
        c1->desni = nullptr;
        c1->balans = c2->balans;
        kopiraj(c1->lijevi, c2->lijevi);
        kopiraj(c1->desni, c2->desni);
    }

    AVLStabloMapa(AVLStabloMapa<TipKljuc, TipVrijednost> &AVLM)
    {
        if (AVLM.duzina != 0)
        {
            kopiraj(AVLM.korijen);
        }
        duzina = AVLM.brojElemenata();
    }

    AVLStabloMapa &operator=(const AVLStabloMapa<TipKljuc, TipVrijednost> &AVLM)
    {
        if (this == &AVLM)
            return *this;
        obrisi();
        kopiraj(AVLM.korijen, AVLM.korijen);
        duzina = AVLM.duzina;
        return *this;
    }

    AVLStabloMapa &operator=(AVLStabloMapa &&AVLM)
    {
        if (this == &AVLM)
        {
            return *this;
        }
        obrisi();
        korijen = AVLM.korijen;
        duzina = AVLM.duzina;
        return *this;
    }
};

int main()
{
    // Osnovni test funkcionalnosti AVLStabloMapa
    AVLStabloMapa<std::string, std::string> m;
    m["Sarajevo"] = "BiH";
    m["Zagreb"] = "Hrvatska";
    cout << m.brojElemenata() << " " << m["Zagreb"] << " '"; // 2 Hrvatska

    /* // Test praznih elemenata
    cout << m["Beograd"] << "' '"; // ''
    cout << m["zagreb"] << "' ";   // ''
    m.obrisi("Zagreb");

    // Potpuno korektna AVLStabloMapa nece kreirati objekte "Beograd" i "zagreb"
    // jer oni nemaju vrijednosti pa ce se ispod ispisati 1
    // Ali to zahtijeva implementaciju "Pristupnika" pa cemo priznavati
    // rjesenje koje ispise broj 3
    cout << m.brojElemenata(); // 3 ili 1

    // Test brisanja
    cout << " '" << m["Zagreb"] << "' "; // ''
    m.obrisi();
    cout << m.brojElemenata();             // 0
    cout << " '" << m["Sarajevo"] << "' "; // '' */
    return 0;
}