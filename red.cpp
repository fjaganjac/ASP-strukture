#include <iostream>

using namespace std;

template <typename Tip>
struct cvor
{
    Tip vrijednost;
    cvor *sljedeci = 0;
};

template <typename Tip>
class Red
{
    cvor<Tip> *ppocetak = 0;
    cvor<Tip> *pkraj = 0;
    int duzina = 0;

public:
    Red()
    {
        ppocetak = nullptr;
        pkraj = nullptr;
        duzina = 0;
    }

    Red(const Red &red) { kopija(red); }

    ~Red() { brisi(); }

    void kopija(const Red &r)
    {
        cvor<Tip> *p;
        p = r.ppocetak;
        while (p != 0)
        {
            stavi(p->vrijednost);
            duzina = r.duzina;
            p = p->sljedeci;
        }
    }

    Red &operator=(const Red &red)
    {
        if (&red == this)
            return *this;
        brisi();
        ppocetak = pkraj = 0;
        kopija(red);
        return *this;
    }

    void brisi()
    {
        while (ppocetak != 0)
            skini();
    }

    void stavi(const Tip &element)
    {
        cvor<Tip> *novi = new cvor<Tip>;
        novi->sljedeci = nullptr;
        novi->vrijednost = element;
        if (duzina == 0)
        {
            pkraj = novi;
            ppocetak = novi;
        }
        else
        {
            pkraj->sljedeci = novi;
            pkraj = novi;
        }
        duzina++;
    }

    Tip skini()
    {
        if (!duzina)
            throw "red je prazan! :skini()";
        cvor<Tip> *pok = ppocetak->sljedeci;
        Tip element = ppocetak->vrijednost;
        delete ppocetak;
        ppocetak = pok;
        duzina--;
        return element;
    }

    Tip &celo()
    {
        if (!duzina)
            throw "red je prazan! :&celo()";
        return ppocetak->vrijednost;
    }
    int brojElemenata() { return duzina; }
};

void TRedprazanKonstrukor()
{
    cout << "Test prazan konstruktor: ";
    Red<int> r;
    if (!r.brojElemenata())
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TRedbrojElemenata()
{
    cout << "Test brojElemenata: ";
    Red<int> r;
    r.stavi(100);
    r.stavi(200);
    r.stavi(300);
    r.stavi(400);
    if (r.brojElemenata() == 4)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TRedstavi()
{
    cout << "Test Stavi: ";
    Red<int> r;
    for (int i = 1; i < 10; i++)
    {
        r.stavi(i);
    }
    if (r.brojElemenata() == 9 && r.celo() == 1)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TRedskini()
{
    cout << "Test Skini: ";
    Red<int> r;
    for (int i = 1; i <= 2509; i++)
    {
        r.stavi(i * i);
        if (r.skini() != i * i)
        {
            cout << "PADA" << endl;
            return;
        }
    }
    cout << "PROLAZI" << endl;
}

void TRedcelo()
{
    cout << "Test celo: ";
    Red<int> r;
    for (int i = 1; i <= 250009; i++)
    {
        r.stavi(i + 1000);
        if (r.celo() != 1001)
        {
            cout << "PADA" << endl;
            return;
        }
    }
    cout << "PROLAZI" << endl;
}

void TRedbrisi()
{
    cout << "Test brisi: ";
    Red<int> r;
    for (int i = 1; i <= 250009; i++)
    {
        r.stavi(i - (i % 12));
    }
    r.brisi();
    if (!r.brojElemenata())
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TRedDestruktor()
{
    cout << "Test destruktora: ";
    try
    {
        Red<int> *r = new Red<int>;
        r->stavi(123);
        delete r;
    }
    catch (...)
    {
        cout << "PADA" << endl;
    }
    cout << "PROLAZI" << endl;
}

void TRedKonstruktorKopije()
{ // pada
    cout << "Test konstruktora kopije: ";
    Red<int> r1;
    for (int i = 0; i < 5; i++)
        r1.stavi(i);
    try
    {
        Red<int> r2(r1);
        while (r1.brojElemenata())
        {
            if (r1.celo() != r2.celo())
            {
                cout << "PADA" << endl;
                return;
            }
            r1.skini();
            r2.skini();
        }
        cout << "PROLAZI" << endl;
    }
    catch (...)
    {
        cout << "PADA" << endl;
    }
}

void TRedOperatorDodjele()
{
    cout << "Test operatora dodjele: ";
    Red<int> r1, r2;
    for (int i = 0; i < 5; i++)
        r1.stavi(i);

    try
    {
        r2 = r1;
        while (r1.brojElemenata())
        {
            if (r1.celo() != r2.celo())
            {
                cout << "PADA" << endl;
                return;
            }
            r1.skini();
            r2.skini();
        }
        cout << "PROLAZI" << endl;
    }
    catch (...)
    {
        cout << "PADA" << endl;
    }
}

int main()
{
    cout << "---Red---" << endl;
    TRedprazanKonstrukor();
    TRedbrojElemenata();
    TRedstavi();
    TRedskini();
    TRedcelo();
    TRedbrisi();
    TRedDestruktor();
    TRedKonstruktorKopije();
    TRedOperatorDodjele();
    return 0;
}