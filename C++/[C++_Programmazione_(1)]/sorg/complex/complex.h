                   /** ** ** ** ** ** ** ** ** ** ** ** **/
                   /* una classe per i numeri complessi  */
                   /*      autore: Antonio Bonifati      */
                   /*   e-mail: abonifati@tiscalinet.it  */
                   /** ** ** ** ** ** ** ** ** ** ** ** **/

                             //pubblico dominio//
                   
#ifndef COMPLEX
#define COMPLEX

#include <iostream.h>
#include <math.h>

/* funzione macro per convertire da RADIANTI->GRADI */
/* utile da usare sul risultato del metodo Phase:   */
/*
#include <iostream.h>
#include <iomanip.h>
#include "complex.h"


int main()
{
  Complex x(1,2);
  cout << setprecision(4) << ToDeg(x.Phase()) << "¯\n";

  return 0;
}
*/
inline double ToDeg(double Rad);

/* altra funzione macro di utilitÖ generale (usata nella nostra sqrt):
   ecco la famosa e semplice funzione matematica "segno":
   
                          +--
                          | 1    se  x >= 0
                sign(x) = |-1    se  x < 0
                          |
                          +--
                          
   Per convenzione ritorna 1 in caso di argomento zero. */
inline double Sign(double);

struct Complex
{
  double Re, Im;
  
  Complex (double r = 0, double i = 0) { Re = r; Im = i; }
  /* oppure:
     Complex (double r = 0, double i = 0) : Re(r), Im(i) {} */

  /* eccezioni: */
  class PhaseOf0 {};
  class DivisionBy0 {};
  
  /* operatori di cast, cioä di conversione Complex->...
     di cui si parla anche nel tutorial, non li ho implementati
     a causa di problemi che causano (vedi manuale.txt)
     operator float() { return Re; }
     operator double() { return Re; } */
  
  inline double Modulus() const;
  inline double Phase() const;  // in radianti (vedi sotto per maggiori info)

  /* questi overloading servono per supportare la sintassi C-like */
  /* per come sono implementate (cioä come una specie di sinonimi
     dei rispettivi metodi), le dichiarazioni friend sono superflue */
  inline /*friend*/ double Modulus(const Complex &);
  inline /*friend*/ double Phase(const Complex &);

  /* questo operatore ä stato provvisto in questa forma
     allo scopo di ottimizzare gli assegnamenti di numeri
     reali a numeri complessi (vedi anche manuale.txt).
     In particolare far restituire un riferimento rende
     gli operatori pió veloci, ma permette anche "cose turche"
     del tipo Complex x; (x=5)=7;
     Comunque devo dire che queste "cose turche" con i compilatori
     che ho sono permesse anche se non vengono usati i riferimenti
     e senza che ci sia nemmeno un warning (!) */
  inline Complex & operator= (double);
  
  /* operatori: ho scelto funzioni friend perchä mi sembra sia pió chiaro
     quali sono i parametri, ma anche funzioni membro vanno benissimo */
  /* oppure friend inline: inline non ä necessario qui, ma ä stato
     messo come informazione utile da ricordare all'utilizzatore. */
  /* notare che queste dichiarazioni non sono necessarie, poichä Re ed Im
     sono pubblici, ma lo saranno se decidi di renderli private o protected */
  /* notare anche che il qualificatore const + necessario. Ad es. se uno
     scrive:
            d=0-d;
     dove d ä un Complex, viene chiamato il costruttore per costruire un
     oggetto temporaneo (il numero complesso 0; il costruttore viene
     utilizzato come convertitore di tipo!) e poi l'operator-
     (infine l'operatore di assegnamento, che ä quello di default).
     Se l'operator- non ha gli argomenti const, il compilatore si rifiuta
     di compilare. Si ha un errore simile a quello che si ha in questo
     programma di prova quando const ä omesso:

#include <iostream.h>

void foo(const int & i) // prova ad omettere const
{
   cout << i;
}

int main()
{
   foo(2);

   return(0);
}

     Ovviamente ä pió semplice scrivere d=-d; infatti ho fatto l'overloading
     anche degli operatori + e - unari per permettere questo.
*/
  /* operatori binari */
  inline friend Complex operator+ (const Complex &, const Complex &);
  inline friend Complex operator- (const Complex &, const Complex &);
  inline friend int operator== (const Complex &, const Complex &);
  inline friend int operator!= (const Complex &, const Complex &);
  inline friend Complex operator+= (Complex &, const Complex &);
  inline friend Complex operator-= (Complex &, const Complex &);
  inline friend Complex operator*= (Complex &, const Complex &);
  inline friend Complex operator/= (Complex &, const Complex &);
  /* questi sono un po' pió lunghi e non sono inline */
  friend Complex operator* (const Complex &, const Complex &);
  friend Complex operator/ (const Complex &, const Complex &);
  /* operatori unari */
  inline friend Complex operator+ (const Complex &);
  inline friend Complex operator- (const Complex &);
  inline friend Complex operator~ (const Complex &);
  /* funzioni: a te il compito di aggiungerne altre */
  /* attenzione: sqrt restituisce una sola radice.
     l'altra ä sempre l'opposta */
  friend Complex sqrt(const Complex &);
  /* la stessa, ma implementata come metodo inline "sinonimo"
     della precedente */
  inline Complex sqrt() const;

  /* operatori per I/O stream */
  friend ostream & operator << (ostream &, Complex &);
  friend istream & operator >> (istream &, Complex &);
};

/* parte non proprio necessaria da leggere ai fini dell'utilizzo:          */
/* definizione delle funzioni inline. DJGPP ha giÖ PI definita in <math.h> */
#ifndef __DJGPP__
  #define PI     3.14159265358979323846
#endif
inline double ToDeg(double Rad)
{
  return Rad*180/PI;
}

inline double Sign(double d)
{
  return d<0 ?  -1 : 1;
}

/********** implementazione delle funzioni membro o friend inline **********/
inline double Complex::Modulus() const
{
  return ::sqrt(Re*Re + Im*Im);
}

inline double Modulus(const Complex &c)
{
  /* invece di scrivere: return sqrt(c.Re*c.Re + c.Im*c.Im);
     ä lo stesso fare: */
  return c.Modulus();
  /* questo rende inutile averla dichiarata friend, comunque
     notate che non guasta un friend in pió :)
     (vedi la dichiarazione a proposito) */
}

  /* Esiste una funzione C fatta apposta per trovare la fase
     (ecco un'altro motivo per imparare bene il C)
     se non sapete che cos'ä la fase cercate su un buon libro
     di algebra! */
     
  /* La funzione si chiama atan2(y, x) e restituisce l'arcotangente,
     in radianti, di y/x.
     A differenza di una chiamata tipo atan(y/x), essa dÖ dei valori
     di ritorno appropriati a seconda che x sia positivo, negativo o
     nullo. Infatti la fase secondo una ben nota convenzione va
     calcolata cosç:

                            +-  atan(y/x)               se x>0
                            |   PI-atan(-y/x)           se x<0 e y>=0
                            |   atan(y/x)-PI            se x<0 e y<0
atan2(y,x) = fase di x+yj = |
                            |   PI/2                    se x=0 e y>0
                            |  -PI/2                    se x=0 e y<0
                            ≥  indeterminata            se x=0 e y=0
                            +-

         (nota: questa definizione puï essere usata per implementare atan2)
         
     Che succede se x e y sono entrambi zero? Evidentemente il numero
     complesso zero non ha alcuna fase oppure potremmo dire che ne ha
     infinite (tanto il modulo ä zero e qualsiasi sia la fase si ottiene
     sempre zero).

     atan2(y,x) di <math.h> si comporta proprio come la funzione scritta
     sopra!
     Notare che se x e y sono nulli, restituisce un errore: il risultato
     non ä un numero, ma ä, a seconda della convenzione nessun numero
     o infiniti. In tal caso errno (definito in <errno.h>) vale EDOM,
     costante usata per segnalare errori di dominio da tutte le funzioni
     matematiche del buon vecchio C.
     Nel nostro caso (C++) lanceremo una eccezione che ho scelto di chiamare
     PhaseOf0, nome che indica che si ä cercato di calcolare la fase di 0.

     Se avete dei problemi di comprensione, ecco degli esempi:
     
     es. atan2(1,2), cioä la fase di 2+j ä circa 0.46 radianti, circa 27¯
     
         atan2(1,-2), cioä la fase di -2+j ä circa 2.68 rad, cioä circa
         153¯ e non ä uguale ad atan(1.0/-2) che fa -0.46, cioä circa
         -27¯.

         Non possiamo utilizzare la funzione atan(y/x) poichÇ
         vogliamo che la fase sia espressa come la funzione precedente,
         cioä come l'angolo tra il vettore rappresentato dal numero
         complesso e l'asse positivo delle x (l'asse dei reali),
         espresso nel range (-PI,PI] (una scelta convenzionale molto usata).

      altri es: atan2(1, 0) ritorna PI/2 ed
                atan2(0, -1) ritorna PI                                  */
inline double Complex::Phase() const
{
  if (Im==0  &&  Re==0) // oppure if (*this==0)
     throw PhaseOf0();
  return atan2(Im, Re);
}

inline double Phase(const Complex &c)
{
  return c.Phase();
}

inline Complex & Complex::operator= (double d)
{
  Re = d; Im = 0;
  return *this;
}

inline Complex operator+ (const Complex &a, const Complex &b)
{
  return Complex (a.Re + b.Re, a.Im + b.Im);
}

inline Complex operator- (const Complex &a, const Complex &b)
{
  return Complex (a.Re - b.Re, a.Im - b.Im);
}

inline Complex operator+ (const Complex &c)
{
  /* invece di scrivere: return Complex (+c.Re, +c.Im);
     ä pió ottimizzato fare cosç (infatti non c'ä niente da fare!) */
  return c;
}

inline Complex operator- (const Complex &c)
{
  return Complex (-c.Re, -c.Im);
}

inline int operator== (const Complex &a, const Complex &b)
{
  return a.Re==b.Re  &&  a.Im==b.Im;
}

inline int operator!= (const Complex &a, const Complex &b)
{
  return a.Re!=b.Re  ||  a.Im!=b.Im;
}

inline Complex operator+= (Complex &dest, const Complex &sorg)
{
  /* basterebbe un
                return dest=dest+sorg;
     ma cosç ä pió efficiente. */
  dest.Re += sorg.Re;
  dest.Im += sorg.Im;
  return dest;
}

inline Complex operator-= (Complex &dest, const Complex &sorg)
{
  dest.Re -= sorg.Re;
  dest.Im -= sorg.Im;
  return dest;
}

/* qui ho usato l'implementazione pió semplice */
inline Complex operator*= (Complex &dest, const Complex &sorg)
{
  return dest=dest*sorg;
}

inline Complex operator/= (Complex &dest, const Complex &sorg)
{
  return dest=dest/sorg;
}

inline Complex operator~ (const Complex &c)
{
  return Complex(c.Re, -c.Im);
}

inline Complex Complex::sqrt() const
{
  return ::sqrt(*this);
}

#endif

