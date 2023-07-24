#include <ctime>
#include <iostream>
#include <string>

using namespace std;

template <typename TipKljuc, typename TipVrijednost> struct BinStabloMapaCvor {
  TipKljuc kljuc;
  TipVrijednost vrijednost;
  BinStabloMapaCvor *lijevi = nullptr;
  BinStabloMapaCvor *desni = nullptr;
  BinStabloMapaCvor *roditelj = nullptr;
};

template <typename TipKljuc, typename TipVrijednost> class Mapa {
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
class HashMapa : public Mapa<TipKljuc, TipVrijednost> {
private:
  int max;
  pair<TipKljuc, TipVrijednost> *niz;
  int duzina;
  unsigned int (*hashFunkcija)(TipKljuc, unsigned int);

  void prosiri() {
    max *= 2;
    pair<TipKljuc, TipVrijednost> *p = new pair<TipKljuc, TipVrijednost>[max];
    for (int i = 0; i < duzina; i++) {
      p[i] = niz[i];
    }
    delete[] niz;
    niz = p;
    p = nullptr;
  }

public:
  int brojElemenata() const { return duzina; }

  HashMapa(int size = 5) {
    hashFunkcija = nullptr;
    max = size;
    duzina = 0;
    niz = new pair<TipKljuc, TipVrijednost>[max];
  }

  void definisiHashFunkciju(unsigned int (*hash)(TipKljuc ulaz,
                                                 unsigned int opseg_izlaza)) {
    hashFunkcija = hash;
  }

  HashMapa(const HashMapa<TipKljuc, TipVrijednost> &m) {
    niz = new pair<TipKljuc, TipVrijednost>[m.max];
    hashFunkcija = m.hashFunkcija;
    for (int i = 0; i < m.duzina; i++) {
      niz[i] = m.niz[i];
    }
    max = m.max;
    duzina = m.duzina;
  }

  ~HashMapa() { delete[] niz; }

  void obrisi(const TipKljuc &kljuc) {
    for (int i = 0; i < duzina; i++) {
      if (kljuc == niz[i].first)
        break;

      if (i == duzina - 1)
        throw("ne postoji element sa datim kljucem");
    }

    for (int i = 0; i < duzina; i++) {
      if (kljuc == niz[i].first) {
        duzina--;
        for (int j = i; j < duzina; j++) {
          niz[j] = niz[j + 1];
        }
        break;
      }
    }
  }

  void obrisi() {
    duzina = 0;
    max = 2;
    delete[] niz;
    niz = new pair<TipKljuc, TipVrijednost>[max];
  }

  TipVrijednost operator[](const TipKljuc kljuc) const {
    if (hashFunkcija == nullptr) {
      throw("Nije definisana hash funkcija");
    }

    TipVrijednost t = TipVrijednost();
    bool flag = 0;
    if (kljuc == niz[hashFunkcija(kljuc, max)].first) {
      return niz[hashFunkcija(kljuc, max)].second;
    }

    for (int i = hashFunkcija(kljuc, max) + 1; i < max; i++) {
      if (kljuc == niz[i].first) {
        flag = 1;
        t = niz[i].second;
      }
    }
    if (flag == 1)
      return t;

    for (int i = 0; i < hashFunkcija(kljuc, max); i++) {
      if (kljuc == niz[i].first) {
        flag = 1;
        t = niz[i].second;
      }
    }
    if (flag == 1) {
      return t;
    }
    return t;
  }

  TipVrijednost &operator[](TipKljuc kljuc) {
    if (hashFunkcija == nullptr) {
      throw("Nije definisana hash funkcija");
    }
    for (int i = 0; i < duzina; i++)
      if (niz[i].first == kljuc)
        return niz[i].second;

    if (duzina == max)
      prosiri();

    niz[duzina].first = kljuc;
    niz[duzina].second = TipVrijednost();
    duzina++;
    return niz[duzina - 1].second;
  }

  HashMapa &operator=(const HashMapa<TipKljuc, TipVrijednost> &HM) {
    if (&HM == this)
      return *this;
    delete[] niz;
    niz = new pair<TipKljuc, TipVrijednost>[HM.max];
    max = HM.max;
    duzina = HM.duzina;
    hashFunkcija = HM.hashFunkcija;
    for (int i = 0; i < HM.duzina; i++) {
      niz[i] = HM.niz[i];
    }
    return *this;
  }
};

//////////////////NIZMAPA///////////////////////////////////

template <typename TipKljuc, typename TipVrijednost>
class NizMapa : public Mapa<TipKljuc, TipVrijednost> {
private:
  int kapacitet;
  pair<TipKljuc, TipVrijednost> *niz;
  int duzina;
  void prosiri() {
    if (kapacitet == duzina) {
      pair<TipKljuc, TipVrijednost> *noviNiz =
          new pair<TipKljuc, TipVrijednost>[kapacitet * 2];
      for (int i = 0; i < kapacitet; i++) {
        noviNiz[i] = niz[i];
      }
      kapacitet *= 2;
      delete[] niz;
      niz = noviNiz;
    }
  }

public:
  int brojElemenata() const { return duzina; }

  NizMapa(int size = 5) {
    kapacitet = size;
    duzina = 0;
    niz = new pair<TipKljuc, TipVrijednost>[kapacitet];
  }

  NizMapa(const NizMapa<TipKljuc, TipVrijednost> &m) {
    niz = new pair<TipKljuc, TipVrijednost>[m.duzina];
    for (int i = 0; i < m.duzina; i++)
      niz[i] = m.niz[i];
    duzina = m.duzina;
    kapacitet = m.kapacitet;
  }

  ~NizMapa() { delete[] niz; }

  void obrisi() {
    duzina = 0;
    kapacitet = 5;
    delete[] niz;
    niz = new pair<TipKljuc, TipVrijednost>[kapacitet];
  }

  void obrisi(const TipKljuc &kljuc) {
    int index = 0;
    for (int i = 0; i < duzina; i++) {
      if (niz[i].first == kljuc) {
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

  TipVrijednost operator[](const TipKljuc kljuc) const {
    int index;
    for (int i = 0; i < duzina; i++) {
      if (niz[i].first == kljuc) {
        index = i;
        break;
      }
      if (i == duzina - 1)
        return TipVrijednost();
    }
    return niz[index].second;
  }

  TipVrijednost &operator[](TipKljuc kljuc) {
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
  NizMapa &operator=(const NizMapa<TipKljuc, TipVrijednost> &m) {
    if (&m == this)
      return *this;
    delete[] niz;
    niz = new pair<TipKljuc, TipVrijednost>[m.kapacitet];
    kapacitet = m.kapacitet;
    duzina = m.duzina;
    for (int i = 0; i < duzina; i++) {
      niz[i] = m.niz[i];
    }
    return *this;
  }
};

template <typename TipKljuc, typename TipVrijednost>
class BinStabloMapa : public Mapa<TipKljuc, TipVrijednost> {
private:
  BinStabloMapaCvor<TipKljuc, TipVrijednost> *korijen;
  int duzina;

public:
  BinStabloMapa() {
    korijen = nullptr;
    duzina = 0;
  }

  ~BinStabloMapa() {
    if (duzina)
      obrisi();
  }

  int brojElemenata() const { return duzina; }

  void obrisi(const TipKljuc &br) {
    BinStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;
    BinStabloMapaCvor<TipKljuc, TipVrijednost> *q = nullptr;
    BinStabloMapaCvor<TipKljuc, TipVrijednost> *y = nullptr;

    // p = korijen;
    while (p && p->kljuc != br) {
      q = p;
      if (p->kljuc > br) {
        p = p->lijevi;
      } else {
        p = p->desni;
      }
    }
    if (p == nullptr) {
      return;
    }
    if (!p->lijevi) {
      y = p->desni;
    } else {
      if (!p->desni) {
        y = p->lijevi;
      } else {
        y = p->lijevi;
        BinStabloMapaCvor<TipKljuc, TipVrijednost> *x; // = nullptr;
        BinStabloMapaCvor<TipKljuc, TipVrijednost> *r; // = nullptr;
        x = y->lijevi;
        r = p;
        while (x != nullptr) {
          r = y;
          y = x;
          x = y->desni;
        }
        if (r != p) {
          r->desni = y->lijevi;
          y->lijevi = p->lijevi;
        }
        y->desni = p->desni;
      }
    }

    if (q == nullptr) {
      korijen = y;
    } else {
      if (p == q->desni) {
        q->desni = y;
      } else {
        q->lijevi = y;
      }
    }
    duzina--;
    delete p;
  }

  void obrisi() {
    while (korijen != nullptr)
      obrisi(korijen->kljuc);
  }

  TipVrijednost &operator[](TipKljuc noviKljuc) {
    BinStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;
    while (p != nullptr) {
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
    while (y != nullptr) {
      q = y;
      if (y->kljuc < p->kljuc) {
        y = y->desni;
      } else {
        y = y->lijevi;
      }
    }
    if (q == nullptr) {
      korijen = p;
    } else if (q->kljuc > p->kljuc) {
      q->lijevi = p;
      p->roditelj = q;
    } else {
      q->desni = p;
      p->roditelj = q;
    }

    duzina++;
    return p->vrijednost;
  }

  TipVrijednost operator[](const TipKljuc noviKljuc) const {
    BinStabloMapaCvor<TipKljuc, TipVrijednost> *p = korijen;

    while (p != nullptr) {
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

  void kopiraj(BinStabloMapaCvor<TipKljuc, TipVrijednost> *korijen) {
    if (korijen) {
      (*this)[korijen->kljuc] = korijen->vrijednost;
      kopiraj(korijen->lijevi);
      kopiraj(korijen->desni);
    }
  }

  BinStabloMapa(BinStabloMapa<TipKljuc, TipVrijednost> &BSM) {
    if (BSM.duzina != 0) {
      kopiraj(BSM.korijen);
    }
    duzina = BSM.brojElemenata();
  }

  BinStabloMapa &operator=(const BinStabloMapa<TipKljuc, TipVrijednost> &BSM) {
    if (this == &BSM)
      return *this;
    obrisi();
    kopiraj(BSM.korijen);
    duzina = BSM.brojElemenata();
    return *this;
  }
};

unsigned int hash(string ulaz, unsigned int max) {
  int g = 31;
  unsigned int hash = 0;
  for (int i = 0; i < ulaz.length(); i++) {
    hash = g * hash + ulaz.at(i);
  }
  return hash % max;
}

void test() {
  NizMapa<int, int> NM;
  BinStabloMapa<int, int> BSM;
  HashMapa<int, int> HM;

  srand(static_cast<int>(time(nullptr)));
  for (int i = 0; i < 5000; i++) {
    NM[rand() % 100000] = rand() % 100000;
  }
  for (int i = 0; i < 5000; i++) {
    BSM[rand() % 100000] = rand() % 100000;
  }
  for (int i = 0; i < 5000; i++) {
    HM[rand() % 100000] = rand() % 100000;
  }

  clock_t dodajNM1, dodajNM2, pristupiNM1, pristupiNM2, dodajBSM1, dodajBSM2,
      pristupBSM1, pristupBSM2, dodajHM1, dodajHM2, pristupiHM1, pristupiHM2;

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

  cout << "---HashMapa---" << endl;

  dodajHM1 = clock();
  HM[1000001] = 1000001;
  dodajHM2 = clock();

  cout << "za dodavanje novog elementa u mapu potrebno je "
       << (dodajHM2 - dodajHM1) / (CLOCKS_PER_SEC / 1000) << " ms." << endl;

  pristupiHM1 = clock();
  HM[1000001] = 1;
  pristupiHM2 = clock();
  cout << "za pristup postojećem elementu u mapi potrebno je "
       << (pristupiHM2 - pristupiHM1) / (CLOCKS_PER_SEC / 1000) << " ms."
       << endl;
}

int main() {
  test();
  return 0;
}