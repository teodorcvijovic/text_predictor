#include "TrieStablo.h"
#include <queue>
#include <stack>

#include <fstream>
#include <iostream>

#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

// TrieCvor

int index(char deoKljuca) 
{
    if (deoKljuca >= 'A' && deoKljuca <= 'Z') 
        deoKljuca += ('a' - 'A');

    return (static_cast<int>(deoKljuca) - 97);
}

char karakter(int index)
{
    return (static_cast<char>(index + 97));
}

TrieStablo::TrieCvor::TrieCvor(TrieStablo::TrieCvor *roditelj)
: m_roditelj(roditelj),
  m_info(0),
  m_brojPodstabala(0)
{
      for(int i = 0; i < 26; i++)
            m_niz_pokazivaca[i] = nullptr;

      m_kljuc = nullptr;
}

TrieStablo::TrieCvor::~TrieCvor()
{
      m_roditelj = nullptr;

      for (int i = 0; i < 26; i++)
          if (m_niz_pokazivaca[i]) m_niz_pokazivaca[i] = nullptr; //delete m_niz_pokazivaca[i];

      delete m_kljuc;
      m_kljuc = nullptr;
}


TrieStablo::TrieCvor *TrieStablo::TrieCvor::pronadjiPodstablo(char deoKljuca)
{
    return m_niz_pokazivaca[index(deoKljuca)];
}

void TrieStablo::TrieCvor::dodajPodstablo(char deoKljuca, TrieStablo::TrieCvor *cvor)
{
      if( ! m_niz_pokazivaca[index(deoKljuca)] )
      {
            m_niz_pokazivaca[index(deoKljuca)] = cvor;
            m_brojPodstabala++;
      }
}

void TrieStablo::TrieCvor::ukloniPodstablo(char deoKljuca)
{
      if( m_niz_pokazivaca[index(deoKljuca)] )
      {
            m_niz_pokazivaca[index(deoKljuca)] = 0;
            m_brojPodstabala--;
      }
}


int TrieStablo::TrieCvor::dohvatiInfo()
{
      return m_info;
}


void TrieStablo::TrieCvor::poseti(ostream &it)
{
      if( m_info )
            it << m_info << endl;

      for(int i = 0; i < 26; i++)
            if( m_niz_pokazivaca[i] )
                  m_niz_pokazivaca[i]->poseti(it);
}



// TrieStablo


TrieStablo::TrieCvor* TrieStablo::pronadjiCvor(const char *kljuc)
{
      if( ! m_koren )
            return 0;

      TrieCvor *tek = m_koren;

      for( int i = 0; i < strlen(kljuc) && tek; i++ )
            tek = tek->pronadjiPodstablo( kljuc[i] );

      return tek;
}

TrieStablo::TrieStablo()
: m_koren(0)
{
}

TrieStablo::~TrieStablo()
{
    /*     if( m_koren )
            delete m_koren;
    */

    if (m_koren) {
        queue<TrieStablo::TrieCvor*> q;
        TrieStablo::TrieCvor* tek,*dete;

        q.push(m_koren);

        while (!q.empty()) {
            tek = q.front();
            q.pop();

            for (int i = 0; i < 26; i++) {
                dete = tek->pronadjiPodstablo(static_cast<char>(i + 97));
                if (dete) q.push(dete);

            }

            delete tek;
        }

    }
}

bool TrieStablo::umetni(const char *kljuc)
{
      if (kljuc == nullptr) return false;
      if (!strlen(kljuc)) return false;

      if( ! m_koren )
      {
            m_koren = new TrieCvor(0);
            m_brCvorova++;
      }

      TrieCvor *tek = m_koren;
      TrieCvor *sledeci = 0;

      for( int i = 0; i < strlen(kljuc); i++ )
      {
      
            sledeci = tek->pronadjiPodstablo( kljuc[i] );

            if( ! sledeci )
            {
                  sledeci = new TrieCvor(tek);
                  tek->dodajPodstablo( kljuc[i], sledeci );
                  m_brCvorova++;
            }

            tek = sledeci;
      }

      bool rezultat = true;  // true, ako je kljuc prvi put umetnut
      if( sledeci->dohvatiInfo() )
            rezultat = false;
      else
      {
          m_brKljuceva++;
          sledeci->postaviKljuc(kljuc);
      }

      sledeci->uvecajInfo();

      return rezultat;
}

bool TrieStablo::obrisi(const char *kljuc)
{
      if( ! m_koren )
            return false;

      TrieCvor *tek = pronadjiCvor(kljuc);
      if( ! tek )
            return false;

      tek->resetujInfo();
      if (tek->dohvatiKljuc()) delete tek->dohvatiKljuc();

      int i = strlen(kljuc)-1;
      
      while( i >= 0 && tek && tek->brojPodstabala() == 0 && !tek->dohvatiInfo() )
      {
      TrieCvor *roditelj = tek->roditelj();

            delete tek;
            m_brCvorova--;

            if( roditelj )
                  roditelj->ukloniPodstablo( kljuc[i--] );
            else
                  m_koren = 0;
            tek = roditelj;
      }

      m_brKljuceva--;
      return true;
}

void TrieStablo::obidji(ostream &it)
{
      if( m_koren )
            m_koren->poseti(it);
      else
            it << "Stablo je prazno." << endl;
}


int TrieStablo::dohvatiInfo(const char *kljuc)
{
      if( ! m_koren )
            return 0;

      TrieCvor *cvor = m_koren;
      int i = 0, max_i = strlen(kljuc);

      while( cvor && i < max_i )
            cvor = cvor->pronadjiPodstablo( kljuc[i++] );

      //TrieCvor *cvor = pronadjiCvor(kljuc);

      if( cvor )
            return cvor->dohvatiInfo();

      return 0;
}

// Dodatne metode i funkcije

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool lettersOnly(const char* word)
{
    if (!word) return false;

    bool flag = true;

    for (int i = 0; i < strlen(word); i++)  if (!isLetter(word[i])) {
        flag = false;
        break;
    }

    return flag;
}

void TrieStablo::readDirectory(string path)
{
    fstream file;
    string word;

    for (const auto& entry : fs::directory_iterator(path))
    {
        //cout << entry.path() << endl;

        file.open(entry.path());

        while (file >> word)
        {
            const char* c = word.c_str();

            //cout <<"word:"<< c <<";"<<endl;

            if (lettersOnly(c)) umetni(c);

        }

        file.close();
    }
}

char* generisiRec(const char* rec,char karakter,int i) 
{
    char* nova_rec = new char[strlen(rec) + 1];
    strcpy(nova_rec, rec);
    nova_rec[i] = karakter;
    return nova_rec;
}

char* generisiRec(const char* rec, char c1, char c2, char c3, int i, int j, int k)
{
    char* nova_rec = new char[strlen(rec) + 1];
    strcpy(nova_rec, rec);
    nova_rec[i] = c1;
    nova_rec[j] = c2;
    nova_rec[k] = c3;

    return nova_rec;
}


int susedi(char x, char y)
{
    //              0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25

     int m[26][26]={{ 0, 0, 0, 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0,	0,	1,	0,	0,  0},//0
                   {0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0},//1
                   {0,	0,	0,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0},//2
                   {0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	0,	0},//3
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	1,	0,	0,	0},//4
                   {0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	1,	0,	0,	0,	0},//5
                   {0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0,	1,	0},//6
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	1,	0},//7
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	1,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0},//8
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0},//9
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//10
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//11
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//12
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//13
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//14
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//15
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0},//16
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0},//17
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1},//18
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0},//19
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//20
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//21
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1},//22
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1},//23
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},//24
                   {0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} };//25

    return m[index(x)][index(y)] + m[index(y)][index(x)];
}
    

class minPrioritet 
{
public: 
    bool operator()(pair<char*,int> &x, pair<char*, int>& y)
    {
        return x.second > y.second;
    }
};

class maxPrioritet
{
public:
    bool operator()(pair<char*, int>& x, pair<char*, int>& y)
    {
        return x.second < y.second;
    }
};

bool match(priority_queue < pair<char*, int>, vector < pair<char*, int>>, minPrioritet > q, const char* str)
{
    while (!q.empty())
    {
        const char* s = q.top().first;
        q.pop();
        if (!strcmp(s, str)) return true;
    }
    return false;
}

void TrieStablo::predikcija1(const char* prefix)
{
    if (!m_koren || !prefix)
        return;

    int brojGreski = 0, i, max_count = 0;

    TrieCvor* tek = m_koren ,*cvor;


    for (i = 0; i < strlen(prefix) && tek; i++)
        tek = tek->pronadjiPodstablo(prefix[i]);

    if (i == strlen(prefix))
    {
        cout << "Uneta je rec: " << prefix << endl;

        stack<TrieCvor*> s;
        priority_queue < pair<char*, int>, vector < pair<char*, int>>, minPrioritet > PQmin; //prioritetni red

        s.push(tek);

        while (!s.empty())
        {
            cvor = s.top();
            s.pop();

            while(cvor)
            {
                //obrada

                if (max_count < 3 && cvor->dohvatiInfo())
                {
                    PQmin.push({ cvor->dohvatiKljuc(),cvor->dohvatiInfo() });
                    max_count++;
                }
                else if (max_count == 3 && cvor->dohvatiInfo())
                {
                    pair<char*, int> min = PQmin.top();
                    PQmin.pop();

                    if (min.second < cvor->dohvatiInfo()) min.first = cvor->dohvatiKljuc(), min.second = cvor->dohvatiInfo();
                    PQmin.push(min);
                }

                for (int i = 1; i < 26; i++) if (cvor->m_niz_pokazivaca[i]) s.push(cvor->m_niz_pokazivaca[i]);
                cvor = cvor->m_niz_pokazivaca[0];
            }
        }

        pair<char*, int>* rezultat = new pair<char*, int> [max_count];

        for (int j = max_count; j > 0; j--)
        {
            rezultat[j-1] = PQmin.top();
            PQmin.pop();
        }

        for (int j = 0; j < max_count; j++)
        {
            cout << "Predlog: " << rezultat[j].first << " [" << rezultat[j].second << "]" << endl;
        }

    }
    else 
    {
        cout << " / " << endl;
    }
}

void TrieStablo::predikcija2(const char* prefix)
{
    if (!m_koren || !prefix)
        return;

    int i,j,k, max_count = 0;
    bool greska = false;

    TrieCvor* tek = m_koren, * pret = nullptr;
    char* prefix1 = nullptr, * prefix2 = nullptr, * prefix3 = nullptr;
    const char *tek_prefix;

    queue<const char*> q;

    cout << "Uneta je rec: " << prefix << endl;

    for (i = 0; i < strlen(prefix); i++)
    {
        pret = tek;
        tek = tek->pronadjiPodstablo(prefix[i]);

        if (!tek)
        {
            greska = true;
            for (int nxt = 0; nxt < 26; nxt++)
                if (susedi(karakter(nxt), prefix[i]))
                {
                    prefix1 = generisiRec(prefix, karakter(nxt), i);
                    q.push(prefix1);
                    cout << "prefix1: " << prefix1 << endl;

                    tek = pret;

                    for (j = i; j < strlen(prefix); j++)
                    {
                        pret = tek;
                        tek = tek->pronadjiPodstablo(prefix1[j]);

                        if (!tek && i != j)
                        {
                            for (int nxt = 0; nxt < 26; nxt++)
                                if (susedi(karakter(nxt), prefix[j]))
                                {
                                    prefix2 = generisiRec(prefix1, karakter(nxt), j);
                                    q.push(prefix2);
                                    cout << "prefix2: " << prefix2 << endl;

                                    tek = pret;

                                    for (k = j; k < strlen(prefix); k++)
                                    {
                                        pret = tek;
                                        tek = tek->pronadjiPodstablo(prefix2[k]);

                                        if (!tek && j != k)
                                        {
                                            for (int nxt = 0; nxt < 26; nxt++)
                                                if (susedi(karakter(nxt), prefix[k]))
                                                {
                                                    prefix3 = generisiRec(prefix2, karakter(nxt), k);
                                                    q.push(prefix3);
                                                    cout << "prefix3: " << prefix3 << endl;
                                                }
                                            break;
                                        }
                                        else if (!tek && k == j)
                                        {
                                            tek = pret;
                                            break;
                                        }
                                    }
                                }
                            break;
                        }
                        else if (!tek && i == j)
                        {
                            tek = pret;
                            break;
                        }
                    }
                }
            break;
        }

    }
    
    if (!greska) q.push(prefix);

    stack<TrieCvor*> s;
    priority_queue < pair<char*, int>, vector < pair<char*, int>>, minPrioritet > PQmin; //prioritetni red
    TrieCvor* cvor;

    while (!q.empty())
    {
        tek = m_koren;

        tek_prefix = q.front();
        q.pop();

        //cout << tek_prefix << endl;

        for (i = 0; i < strlen(prefix) && tek; i++)
            tek = tek->pronadjiPodstablo(tek_prefix[i]);

        if (i == strlen(prefix))
        {
            s.push(tek);

            while (!s.empty())
            {
                cvor = s.top();
                s.pop();

                while (cvor)
                {
                    //obrada

                    if (max_count < 3 && cvor->dohvatiInfo())
                    {
                        PQmin.push({ cvor->dohvatiKljuc(),cvor->dohvatiInfo() });
                        max_count++;
                    }
                    else if (max_count == 3 && cvor->dohvatiInfo())
                    {
                        pair<char*, int> min = PQmin.top();
                        PQmin.pop();

                        if (min.second < cvor->dohvatiInfo()) min.first = cvor->dohvatiKljuc(), min.second = cvor->dohvatiInfo();
                        PQmin.push(min);
                    }

                    for (int i = 1; i < 26; i++) if (cvor->m_niz_pokazivaca[i]) s.push(cvor->m_niz_pokazivaca[i]);
                    cvor = cvor->m_niz_pokazivaca[0];
                }
            }

            

        }
    }

    pair<char*, int>* rezultat = new pair<char*, int>[max_count];

    for (int j = max_count; j > 0; j--)
    {
        rezultat[j - 1] = PQmin.top();
        PQmin.pop();
    }

    for (int j = 0; j < max_count; j++)
    {
        cout << "Predlog: " << rezultat[j].first << " [" << rezultat[j].second << "]" << endl;
    }
}

void TrieStablo::predikcija(const char* prefix)
{
    if (!m_koren || !prefix)
        return;

    bool greska = false;

    TrieCvor* tek = m_koren, * pret = nullptr;
    char* prefix1;
    const char* tek_prefix;

    int i = 0, max_count=0;

    queue <const char* >q;

    for (i = 0; i < strlen(prefix); i++)
    {
        pret = tek;
        tek = tek->pronadjiPodstablo(prefix[i]);

        if (!tek)
        {
            tek = m_koren;
            pret = nullptr;

            greska = true;
            break;
        }
    }


    if (greska)
    {
        for (int i = 0; i < strlen(prefix); i++)
        {
            for (int c1 = 0; c1 < 26; c1++)
            {
                if (susedi(prefix[i], karakter(c1)) || prefix[i] == karakter(c1))
                {
                    for (int j = 0; j < strlen(prefix); j++)
                    {
                        for (int c2 = 0; c2 < 26; c2++)
                        {
                            if ((susedi(prefix[j], karakter(c2)) || prefix[j] == karakter(c2)) && j!=i)
                            {
                                for (int k = 0; k < strlen(prefix); k++)
                                {
                                    for (int c3 = 0; c3 < 26; c3++)
                                    {
                                        if ((susedi(prefix[k], karakter(c3)) || prefix[k] == karakter(c3)) && j != i && k!=j && k!=i)
                                        {
                                            prefix1 = generisiRec(prefix, karakter(c1), karakter(c2), karakter(c3), i, j, k);
                                            q.push(prefix1);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else q.push(prefix);

    stack<TrieCvor*> s;
    priority_queue < pair<char*, int>, vector < pair<char*, int>>, minPrioritet > PQmin; //prioritetni red
    TrieCvor* cvor;

    cout << "Uneta je rec: " << prefix << endl;

    while (!q.empty())
    {
        tek = m_koren;

        tek_prefix = q.front();
        q.pop();

        //cout << tek_prefix << endl;

        for (i = 0; i < strlen(prefix) && tek; i++)
            tek = tek->pronadjiPodstablo(tek_prefix[i]);

        if (i == strlen(prefix))
        {
            s.push(tek);

            while (!s.empty())
            {
                cvor = s.top();
                s.pop();

                while (cvor)
                {
                    //obrada

                    if (max_count < 3 && cvor->dohvatiInfo() && !match(PQmin, cvor->dohvatiKljuc()))
                    {
                        PQmin.push({ cvor->dohvatiKljuc(),cvor->dohvatiInfo() });
                        max_count++;
                    }
                    else if (max_count == 3 && cvor->dohvatiInfo() && !match(PQmin, cvor->dohvatiKljuc()))
                    {
                        pair<char*, int> min = PQmin.top();
                        PQmin.pop();

                        if (min.second < cvor->dohvatiInfo()) min.first = cvor->dohvatiKljuc(), min.second = cvor->dohvatiInfo();
                        PQmin.push(min);
                    }

                    for (int i = 1; i < 26; i++) if (cvor->m_niz_pokazivaca[i]) s.push(cvor->m_niz_pokazivaca[i]);
                    cvor = cvor->m_niz_pokazivaca[0];
                }
            }
        }
    }

    pair<char*, int>* rezultat = new pair<char*, int>[max_count];

    for (int j = max_count; j > 0; j--)
    {
        rezultat[j - 1] = PQmin.top();
        PQmin.pop();
    }

    for (int j = 0; j < max_count; j++)
    {
        cout << "Predlog: " << rezultat[j].first << " [" << rezultat[j].second << "]" << endl;
    }
}
 