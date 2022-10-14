#ifndef _BASE_H
#define _BASE_H

/*
 * La classe racchiude le informazioni di id, nome e posizione 
 * per tutti i dispositivi con l'intento di inventariarli e 
 * capirne la posizione.
 */

#include <string>
using namespace std;

class base {
  public:
    // Oggetto base creato da un codice id un nome
    base(int , string , string );
    // Oggetto base creato solo con il codice
    base(int );
    // Recupera l'id
    inline int getId() const { return _id; };
	// Recupera il nome
    inline string getNome() const { return _nome; };
	// Recupera la posizione
    inline string getPosizione() const { return _posizione; };

  private:
	// Codice identificativo dell'oggetto
    int _id;
	// Nome dell'oggetto
    string _nome;
	// Posizione dell'oggetto
    string _posizione;

};
#endif
