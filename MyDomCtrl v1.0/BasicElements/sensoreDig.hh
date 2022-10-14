#ifndef _SENSOREDIG_H
#define _SENSOREDIG_H

#include "modulo.hh"

/*
 * Il sensore simula la rappresentazione di un sensore 
 * digitale accoppiato ad arduino. Legge lo stato  
 * attivo o disattivo.
 */


class sensoreDig : public modulo {
  public:
    // id, nome, posizione, arduino, canale, verbose, logRisposte, celsius/fahren, indirizzo
    sensoreDig(int , string , string , arduino * , int , bool , bool );
	// base, arduino, canale, log
    sensoreDig(base , arduino * , int , bool , bool );
    // Proprietà della sonda
    inline bool getUltimaLettura() const { return _ultimaLettura; };
    // Il tipo può essere: m=base, s=sonda, v=valvola
	virtual tipiModuli getTipo() const; 	
	// Il modo può essere 0 lettura, 1 scrittura 
	virtual modi getModo() const; 
	// Crea una stringa per le impostazioni
	virtual string CreaRigaImpostazioni();
	// Recupera il nome del modello
	virtual string getModel() const;
	// Recupera se c'è un problema
	virtual bool getOnAlarm() const;
    // Legge un valore della sonda
    void LeggiValore();

  private:
	// ultima lettura su arduino
    bool _ultimaLettura;

};
#endif
