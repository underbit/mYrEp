#ifndef _SENSOREONEWIRE_H
#define _SENSOREONEWIRE_H

#include "modulo.hh"
#include <string>

using namespace std;

/*
 * Il sensore simula la rappresentazione di un sensore 
 * One WIre accoppiato ad arduino. Legge la temperatura 
 * in gradi celsius o fahreneight.
 */

class sensoreOneWire : public modulo {
  public:
    // id, nome, posizione, arduino, canale, verbose, logRisposte, celsius/fahren, indirizzo
    sensoreOneWire(int , string , string , arduino * , int , bool , bool , bool , const char * );
	// base, arduino, canale, log, rispostalunga, celsius, indirizzo
    sensoreOneWire(base , arduino * , int , bool , bool , bool , const char * );
    // Proprietà della sonda
    inline float getUltimaLettura() const { return _ultimaLettura; };
    // Ultimo valore letto
    inline bool getIsCelsius() const { return _celsius; };
	// Il tipo può essere: m=base, s=sonda, v=valvola
	virtual tipiModuli getTipo() const; 	
	// Il modo può essere 0 lettura, 1 scrittura 
	virtual modi getModo() const; 
    // Crea una riga di impostazioni
	virtual string CreaRigaImpostazioni();
	// Recupera il nome del modello
	virtual string getModel() const;
	// Restituisce l'indirizzo one wire
    inline string getIndirizzo() const { return _indirizzo; };
    // Imposta Celsius	
    void ImpostaCelsius();
    // Imposta Fahreneight
    void ImpostaFahre();
    // Legge un valore della sonda
    bool LeggiValore();

  private:
	
	// ultima lettura su arduino
    float _ultimaLettura;
	// Gradi celsius o fahreneight
    bool _celsius;
	// Indirizzo one wire
    string _indirizzo;

};
#endif
