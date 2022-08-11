#if !defined(AFX_TRIESTABLO_H__EA14736F_A30C_4DA9_A044_C865B9328099__INCLUDED_)
#define AFX_TRIESTABLO_H__EA14736F_A30C_4DA9_A044_C865B9328099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stablo.h"
#include <queue>

class TrieStablo : public Stablo  
{
protected:

      class TrieCvor
      {
	      TrieCvor	*m_niz_pokazivaca[26]; // mala slova engleske abecede
          TrieCvor  *m_roditelj;
	      int		m_info; // broj pojavljivanja reci
          int        m_brojPodstabala;
          char*     m_kljuc;

          TrieCvor(const TrieCvor &) { }
          TrieCvor &operator=(const TrieCvor &) { }

      public:

          friend class TrieStablo;

	      TrieCvor(TrieCvor *roditelj);

	      virtual ~TrieCvor();

          TrieCvor *roditelj() 
          {
                return m_roditelj;
          }

	      TrieCvor *pronadjiPodstablo(char deoKljuca);
	      void dodajPodstablo(char deoKljuca, TrieCvor *cvor);
          void ukloniPodstablo(char deoKljuca);

          int brojPodstabala() const { return m_brojPodstabala; }

          int dohvatiInfo();
          void uvecajInfo() 
          { 
              m_info++; 
          }
          void resetujInfo() 
          {
              m_info = 0;
          }

          char* dohvatiKljuc() {
              return m_kljuc;
          }
          void postaviKljuc(const char* tekst) 
          {
              if (tekst)
              {
                  m_kljuc = new char[strlen(tekst) + 1];
                  strcpy(m_kljuc, tekst);

                  for (int i = 0; i < strlen(m_kljuc); i++)
                      m_kljuc[i] = tolower(m_kljuc[i]);
              }

          }


          void poseti(ostream &it);

          friend bool operator<(TrieStablo::TrieCvor& levi, TrieStablo::TrieCvor& desni) 
          {
              return levi.dohvatiInfo() < desni.dohvatiInfo();
          }

          friend bool operator>(TrieStablo::TrieCvor& levi, TrieStablo::TrieCvor& desni) 
          {
              return levi.dohvatiInfo() < desni.dohvatiInfo();
          }
      };


      TrieCvor   *m_koren;

      TrieStablo(const TrieStablo &) { }
      TrieStablo &operator=(const TrieStablo &) { }

      TrieCvor* pronadjiCvor(const char *kljuc);   // pronalazi cvor koji sadrzi dati kljuc

public:
	TrieStablo();
	virtual ~TrieStablo();

	virtual bool umetni(const char *kljuc);
	virtual bool obrisi(const char *kljuc);

    virtual void obidji(ostream &it);

    int dohvatiInfo(const char *kljuc);

    void readDirectory(string path);

    void predikcija1(const char* prefix);
    void predikcija2(const char* prefix);
    void predikcija(const char* prefix);
};

bool isLetter(char c);
bool lettersOnly(const char* word);

class minPrioritet;
class maxPriotitet;

int index(char);
char karakter(int);
int susedi(char, char);
char* generisiRec(const char* rec, char karakter, int i);
char* generisiRec(const char*, char, char, char, int, int, int);
bool match(priority_queue < pair<char*, int>, vector < pair<char*, int>>, minPrioritet > q, const char* str);

#endif // !defined(AFX_TRIESTABLO_H__EA14736F_A30C_4DA9_A044_C865B9328099__INCLUDED_)
