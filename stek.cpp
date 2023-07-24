#include <iostream>

using namespace std;

template <typename Tip>
class Stek
{
private:
    struct cvor
    {
        Tip vrijednost;
        cvor *sljedeci;
        cvor(const Tip &vrijednost, cvor *sljedeci)
        {
            this->vrijednost = vrijednost;
            this->sljedeci = sljedeci;
        }
    };
    cvor *pvrh;
    int duzina;

public:
    Stek()
    {
        pvrh = nullptr;
        duzina = 0;
    }

    Stek(const Stek &stek) { kopiraj(stek); }
    ~Stek() { brisi(); }
    Stek &operator=(const Stek &stek)
    {
        if (&stek == this)
            return *this;
        brisi();
        kopiraj(stek);
        return *this;
    };

    void stavi(const Tip &element)
    {
        pvrh = new cvor(element, pvrh);
        duzina++;
    }
    Tip skini()
    {
        if (pvrh == nullptr)
            throw "stek je prazan";
        Tip vrijednost = pvrh->vrijednost;
        cvor *pok = pvrh->sljedeci;
        delete pvrh;
        pvrh = pok;
        duzina--;
        return vrijednost;
    }

    Tip &vrh() const
    {
        if (!duzina)
            throw("stek je prazan");
        return pvrh->vrijednost;
    }

    void kopiraj(const Stek &stek)
    {
        cvor *d, *v = stek.pvrh;
        while (v)
        {
            cvor *n = new cvor(v->vrijednost, nullptr);
            if (v != stek.pvrh)
                d->sljedeci = n;
            else
                pvrh = n;
            d = n;
            v = v->sljedeci;
        }
        d->sljedeci = nullptr;
        duzina = stek.duzina;
    }

    void brisi()
    {
        while (duzina)
            skini();
    }

    int brojElemenata() { return duzina; }

    void ispisi()
    {
        cvor *pok = pvrh;
        while (pok != nullptr)
        {
            cout << pok->vrijednost << endl;
            pok = pok->sljedeci;
        }
    }
};

void TSPrazanKonstruktor()
{
    cout << "Test praznog konstrukotra: ";
    Stek<int> s;
    if (!s.brojElemenata())
        cout << "✔" << endl;
    else
        cout << "✘" << endl;
}

void TStekKonstruktorKopije()
{
    cout << "Test konstruktora kopije: ";
    Stek<int> s1;
    for (int i = 0; i < 5; i++)
        s1.stavi(i);

    try
    {
        Stek<int> s2(s1);
        while (s1.brojElemenata())
        {
            if (s1.vrh() != s2.vrh())
            {
                cout << "✘" << endl;
                return;
            }
            s1.skini();
            s2.skini();
        }
        cout << "✔" << endl;
    }
    catch (...)
    {
        cout << "✘" << endl;
    }
}

void TSDestruktor()
{
    cout << "Test destruktora: ";
    try
    {
        Stek<int> *s = new Stek<int>;
        delete s;
    }
    catch (...)
    {
        cout << "✘" << endl;
    }
    cout << "✔" << endl;
}

void TStekBrojElemenata()
{
    cout << "Test broja elemenata: ";
    Stek<int> s;
    for (int i = 0; i < 5; i++)
    {
        s.stavi(i);
        if (s.brojElemenata() != i + 1)
        {
            cout << "✘" << endl;
            return;
        }
    }
    cout << "✔" << endl;
}

void TSOperatorDodjele()
{
    cout << "Test operatora dodjele: ";
    Stek<int> s1, s2;
    for (int i = 0; i < 5; i++)
        s1.stavi(i);

    try
    {
        s2 = s1;
        while (s1.brojElemenata())
        {
            if (s1.vrh() != s2.vrh())
            {
                cout << "✘" << endl;
                return;
            }
            s1.skini();
            s2.skini();
        }
        cout << "✔" << endl;
    }
    catch (...)
    {
        cout << "✘" << endl;
    }
}

void TStekVrh()
{
    cout << "Test dohvaćanja elementa na vrhu: ";
    Stek<int> s;
    bool flag;
    s.stavi(12);
    flag = s.vrh() == 12;
    if (!(s.vrh() == 12))
    {
        cout << "✘" << endl;
        return;
    }
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    if (!(s.vrh() == 3))
    {
        cout << "✘" << endl;
        return;
    }
    s.skini();
    if (!(s.vrh() == 2))
    {
        cout << "✘" << endl;
        return;
    }
    s.skini();
    if (!(s.vrh() == 1))
    {
        cout << "✘" << endl;
        return;
    }
    cout << "✔" << endl;
}

void TStekStavi()
{
    cout << "Test stavljanje na stek: ";
    Stek<int> s;
    for (int i = 0; i < 4; i++)
    {
        s.stavi(i);
        if (s.brojElemenata() != i + 1 || s.vrh() != i)
        {
            cout << "✘" << endl;
            return;
        }
    }
    cout << "✔" << endl;
}

void TStekSkini()
{
    cout << "Test skidanja sa steka: ";
    Stek<int> s;
    for (int i = 1; i <= 5; i++)
    {
        s.stavi(i * 100);
    }
    for (int i = 5; i >= 1; i--)
    {
        if (s.vrh() != i * 100)
        {
            cout << "✘" << endl;
            return;
        }
        s.skini();
    }
    cout << "✔" << endl;
}

void TStekBrisi()
{
    cout << "Test brisanja steka: ";
    Stek<int> s;
    for (int i = 0; i < 1000; i++)
        s.stavi(i);
    s.brisi();
    if (s.brojElemenata())
    {
        cout << "✘" << endl;
        return;
    }
    try
    {
        s.vrh(); // pokusaj dohvacanja elementa sa praznog steka baca izuzetak
        cout << "✘" << endl;
    }
    catch (...)
    {
        cout << "✔" << endl;
    }
}

int main()
{
    cout << "---Stek---" << endl;
    TSPrazanKonstruktor();
    TSDestruktor();
    TSOperatorDodjele();
    TStekStavi();
    TStekSkini();
    TStekBrisi();
    TStekBrojElemenata();
    TStekVrh();
    TStekKonstruktorKopije();
    return 0;
}