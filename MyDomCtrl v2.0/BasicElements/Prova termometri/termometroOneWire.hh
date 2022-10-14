#ifndef TERMOMETROONEWIRE_H
#define TERMOMETROONEWIRE_H

#include "termometro.hh"
#include <string>

using namespace std;

/*
 * Il sensore simula la rappresentazione di un sensore 
 * One WIre accoppiato ad arduino. Legge la temperatura 
 * in gradi celsius o fahreneight.
 */


class termometroOneWire : public termometro {
  public:
    // id, nome, posizione, arduino, canale, verbose, logRisposte, celsius/fahren, indirizzo
    termometroOneWire(int , string , string , arduino * , int , bool , bool , bool , const char * );
	// base, arduino, canale, log, rispostalunga, celsius, indirizzo
    termometroOneWire(base , arduino * , int , bool , bool , bool , const char * );
    // Crea una riga di impostazioni
	virtual string CreaRigaImpostazioni();
	// Recupera il nome del modello
	virtual string getModel() const;
	// Restituisce l'indirizzo one wire
    inline string getIndirizzo() const { return _indirizzo; };
    // Legge un valore della sonda
    virtual void LeggiValore();

  private:
	// Indirizzo one wire
    const char * _indirizzo;

};
#endif
