#include "complex.h"

Complex operator* (const Complex &a, const Complex &b)
{
  return Complex(a.Re*b.Re - a.Im*b.Im,  a.Re*b.Im + a.Im*b.Re);
}

/* la divisione per 0 ä una eccezione */
Complex operator/ (const Complex &a, const Complex &b)
{
  if (b==0) throw Complex::DivisionBy0();
  register double denum=b.Re*b.Re + b.Im*b.Im;  // modulo quadro di b
  return Complex((a.Re*b.Re + a.Im*b.Im)/denum, (a.Im*b.Re - a.Re*b.Im)/denum);
}

/* la formula utilizzata ä quella algebrica e non quella polare:
   poichä la classe memorizza i numeri in forma algebrica, la
   formula algebrica ä pió efficiente. Notare che funziona bene
   in tutti i casi (nel caso di numero complesso nullo restituisce zero) */
Complex sqrt(const Complex &c)
{

  /* qui abbiam bisogno che Modulus sia una funzione const per poterli
     passare un oggetto costante! */
  register double mod=c.Modulus();     // se c ä a+bj questo ä sqrt(a*a+b*b)

  return Complex(sqrt((mod+c.Re)/2), sqrt((mod-c.Re)/2)*Sign(c.Im));
}

ostream & operator << (ostream &out, Complex &c)
{
  return  out << '(' << c.Re << ", " << c.Im << ')';
}

istream & operator >> (istream &in, Complex &c)
{
  return  in >> c.Re >> c.Im;
}

