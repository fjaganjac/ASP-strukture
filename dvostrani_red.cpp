#include <iostream>

using namespace std;

template <typename Tip>
struct cvor
{
    Tip vrijednost;
    cvor *sljedeci = 0;
    cvor *prethodni = 0;
};

template <typename Tip>
class DvostraniRed
{
    cvor<Tip> *pcelo = 0;
    cvor<Tip> *pvrh = 0;
    int duzina = 0;

public:
    DvostraniRed()
    {
        pcelo = nullptr;
        pvrh = nullptr;
        duzina = 0;
    }

    DvostraniRed(const DvostraniRed &red) { kopija(red); }

    ~DvostraniRed() { brisi(); }

    void kopija(const DvostraniRed &r)
    {
        cvor<Tip> *p;
        p = r.pcelo;
        while (p != 0)
        {
            staviNaVrh(p->vrijednost);
            duzina = r.duzina;
            p = p->sljedeci;
        }
    }

    DvostraniRed &operator=(const DvostraniRed &red)
    {
        if (&red == this)
            return *this;
        brisi();
        pcelo = pvrh = 0;
        kopija(red);
        return *this;
    }

    void brisi()
    {
        int duz = brojElemenata();
        for (int i = 0; i < duz; i++)
        {
            skiniSaCela();
        }
    }

    void staviNaVrh(const Tip &element)
    {
        cvor<Tip> *novi = new cvor<Tip>;
        novi->sljedeci = nullptr;
        novi->vrijednost = element;
        if (duzina == 0)
        {
            pvrh = novi;
            pcelo = novi;
        }
        else
        {
            pvrh->sljedeci = novi;
            novi->prethodni = pvrh;
            pvrh = novi;
        }
        duzina++;
    }

    void staviNaCelo(const Tip &element)
    {
        cvor<Tip> *novi = new cvor<Tip>;
        novi->sljedeci = nullptr;
        novi->vrijednost = element;
        if (duzina == 0)
        {
            pvrh = novi;
            pcelo = novi;
        }
        else
        {
            pcelo->prethodni = novi;
            novi->sljedeci = pcelo;
            pcelo = novi;
        }
        duzina++;
    }

    Tip skiniSaCela()
    {
        if (!duzina)
            throw "red je prazan! :skiniSaCela()";
        cvor<Tip> *pok = pcelo->sljedeci;
        Tip element = pcelo->vrijednost;
        delete pcelo;
        pcelo = pok;
        duzina--;
        return element;
    }

    Tip skiniSaVrha()
    {
        if (!duzina)
            throw "red je prazan! :skiniSaVrha()";
        cvor<Tip> *pok = pvrh->prethodni;
        Tip element = pvrh->vrijednost;
        delete pvrh;
        pvrh = pok;
        duzina--;
        return element;
    }

    Tip &celo()
    {
        if (!duzina)
            throw "red je prazan! :&celo()";
        return pcelo->vrijednost;
    }

    Tip &vrh()
    {
        if (!duzina)
            throw "red je prazan! :&vrh()";
        return pvrh->vrijednost;
    }

    int brojElemenata() { return duzina; }
};

void TDRedprazanKonstrukor()
{
    cout << "Test prazan konstruktor: ";
    DvostraniRed<int> r;
    if (!r.brojElemenata())
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDRedbrojElemenata()
{
    cout << "Test brojElemenata: ";
    DvostraniRed<int> r;
    r.staviNaCelo(100);
    r.staviNaCelo(200);
    r.staviNaCelo(300);
    r.staviNaCelo(400);
    if (r.brojElemenata() == 4)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDRedStaviNaCelo()
{
    cout << "Test staviNaCelo: ";
    DvostraniRed<int> r;
    for (int i = 1; i < 10; i++)
    {
        r.staviNaCelo(i);
    }
    if (r.brojElemenata() == 9 && r.celo() == 9)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDRedStaviNaVrh()
{
    cout << "Test staviNaVrh: ";
    DvostraniRed<int> r;
    for (int i = 1; i < 10; i++)
    {
        r.staviNaVrh(i);
    }
    if (r.brojElemenata() == 9 && r.vrh() == 9)
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDRedSkiniSaVrha()
{
    cout << "Test skiniSaVrha: ";
    DvostraniRed<int> r;
    for (int i = 1; i <= 2509; i++)
    {
        r.staviNaVrh(i * i);
        if (r.skiniSaVrha() != i * i)
        {
            cout << "PADA" << endl;
            return;
        }
    }
    cout << "PROLAZI" << endl;
}

void TDRedSkiniSaCela()
{
    cout << "Test skiniSaCela: ";
    DvostraniRed<int> r;
    for (int i = 1; i <= 2509; i++)
    {
        r.staviNaCelo(i * i);
        if (r.skiniSaCela() != i * i)
        {
            cout << "PADA" << endl;
            return;
        }
    }
    cout << "PROLAZI" << endl;
}

void TDRedCelo()
{
    cout << "Test celo: ";
    DvostraniRed<int> r;
    for (int i = 1; i <= 250009; i++)
    {
        r.staviNaCelo(i + 1000);
        if (r.celo() != i + 1000)
        {
            cout << "PADA" << endl;
            return;
        }
    }
    cout << "PROLAZI" << endl;
}

void TDRedVrh()
{
    cout << "Test vrh: ";
    DvostraniRed<int> r;
    for (int i = 1; i <= 250009; i++)
    {
        r.staviNaVrh(i + 1000);
        if (r.vrh() != i + 1000)
        {
            cout << "PADA" << endl;
            return;
        }
    }
    cout << "PROLAZI" << endl;
}

void TDRedBrisi()
{
    cout << "Test brisi: ";
    DvostraniRed<int> r;
    for (int i = 1; i <= 250009; i++)
    {
        r.staviNaCelo(i - (i % 12));
    }
    r.brisi();
    if (!r.brojElemenata())
        cout << "PROLAZI" << endl;
    else
        cout << "PADA" << endl;
}

void TDRedDestruktor()
{
    cout << "Test destruktora: ";
    try
    {
        DvostraniRed<int> *r = new DvostraniRed<int>;
        r->staviNaCelo(123);
        delete r;
    }
    catch (...)
    {
        cout << "PADA" << endl;
    }
    cout << "PROLAZI" << endl;
}

void TDRedKonstruktorKopije()
{
    cout << "Test konstruktora kopije: ";
    DvostraniRed<int> r1;
    for (int i = 0; i < 5; i++)
        r1.staviNaCelo(i);
    try
    {
        DvostraniRed<int> r2(r1);
        while (r1.brojElemenata())
        {
            if (r1.celo() != r2.celo())
            {
                cout << "PADA" << endl;
                return;
            }
            r1.skiniSaCela();
            r2.skiniSaCela();
        }
        cout << "PROLAZI" << endl;
    }
    catch (...)
    {
        cout << "PADA" << endl;
    }
}

void TDRedOperatorDodjele()
{
    cout << "Test operatora dodjele: ";
    DvostraniRed<int> r1, r2;
    for (int i = 0; i < 5; i++)
        r1.staviNaCelo(i);

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
            r1.skiniSaCela();
            r2.skiniSaCela();
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
    cout << "---Dvostrani Red---" << endl;
    TDRedprazanKonstrukor();
    TDRedbrojElemenata();
    TDRedStaviNaCelo();
    TDRedStaviNaVrh();
    TDRedSkiniSaVrha();
    TDRedSkiniSaCela();
    TDRedCelo();
    TDRedVrh();
    TDRedBrisi();
    TDRedDestruktor();
    TDRedKonstruktorKopije();
    TDRedOperatorDodjele();
    return 0;
}