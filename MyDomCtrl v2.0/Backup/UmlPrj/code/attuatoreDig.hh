#ifndef _ATTUATOREDIG_H
#define _ATTUATOREDIG_H


#include "modulo.hh"
#include "base.hh"
#include <string>
using namespace std;
#include "sorgenteCalore.hh"

class arduino;

//L'attuatore digitale rappresenta una valvola o una pompa che possa essere attivata tramite un segnale digitale inviato tramite arduino.
class attuatoreDig : public modulo {
  public:
    attuatoreDig(base myb, arduino * , int , bool , bool );

    attuatoreDig(int , string , string , arduino * , int , bool , bool );

    void SetAttuatore(bool );

    // Il tipo d = attuatore digitale
    inline char tipo() const { return 'd'; };

    // Il modo è 1 scrittura 
    inline bool modo() const { return true; };

    inline bool getStato() const { return _stato; };


  private:
    bool _stato;

    sorgenteCalore ;

    termoZona ;

};
#endif
