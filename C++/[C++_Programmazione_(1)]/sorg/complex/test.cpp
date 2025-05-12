                       /* TEST per la classe Complex */
      /* risoluzione di una equazione di 2ø grado nel campo complesso */

/* notate che se sostituite la parola Complex con double il programma
   risolve una equazione in R ! Questo perchŠ il tipo Complex si
   comporta come il tipo nativo double!
   in entrambi i casi a deve essere != 0. Per semplicit… il programma
   non controlla questo fatto. Aggiungete un controllo per esercizio. */

#include "complex.h"

int main()
{
  Complex a,b,c, x1, x2, delta;       // sostituite Complex con double

  cout << "Risolve equaz. di 2ø grado nel campo C\n"
       << "immetti coefficienti a, b, c\n\n";

  cin >> a >> b >> c;

  cout << "equazione: " << a << "x^2 + " << b << "x + " << c << " = 0\n";

  delta=sqrt(b*b-4*a*c);
  
  x1 = (-b+delta)/(2*a);
  x2 = (-b-delta)/(2*a);
  
  cout << "x1=" << x1 << '\t' << "x2=" << x2 << '\n';

  return(0);
}
