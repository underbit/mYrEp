#ifndef TERMOMETRO_H
#define TERMOMETRO_H

#include "modulo.hh"
#include <string>

using namespace std;

/*
 * Il sensore generalizza e astrae le sonde di temperatura.
 */

class termometro : public modulo {
  public:
    // id, nome, posizione, arduino, canale, verbose, logRisposte, celsius/fahren
    termometro(int , string , string , arduino * , int , bool , bool , bool);
	// base, arduino, canale, log, rispostalunga, celsius
    termometro(base , arduino * , int , bool , bool , bool);
    // Proprietà della sonda
    inline float getTemperatura() const { return _temperatura; };
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
    // Legge un valore della sonda
    virtual void LeggiValore() = 0;
    // Imposta Celsius	
    void ImpostaCelsius();
    // Imposta Fahreneight
    void ImpostaFahre();
    

  protected:
	// ultima lettura su arduino
    float _temperatura;
	// Gradi celsius o fahreneight
    bool _celsius;

};
#endif
