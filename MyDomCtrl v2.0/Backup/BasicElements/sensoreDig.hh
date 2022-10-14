#ifndef _SENSOREDIG_H
#define _SENSOREDIG_H

#include "modulo.hh"

/*
 * Il sensore simula la rappresentazione di un sensore 
 * digitale accoppiato ad arduino. Legge lo stato  
 * attivo o disattivo.
 */


class sensoreOneWire : public modulo {
  public:
    // id, nome, posizione, arduino, canale, verbose, logRisposte, celsius/fahren, indirizzo
    sensoreOneWire(int , string , string , arduino * , int , bool , bool );
	// base, arduino, canale, log, rispostalunga, celsius, indirizzo
    sensoreOneWire(base , arduino * , int , bool , bool );
    // Proprietà della sonda
    inline bool getUltimaLettura() const { return _ultimaLettura; };
    // Il modo è LETTURA				
    inline modi modo() const { return LETTURA; };
    // Restituisce SONDAOW
    inline tipiModuli tipo() const { return SONDA; };
    // Legge un valore della sonda
    void LeggiValore();

  private:
	// ultima lettura su arduino
    bool _ultimaLettura;

};
#endif
