#ifndef _ATTUATOREDIG_H
#define _ATTUATOREDIG_H


#include "modulo.hh"
#include "base.hh"
#include "libfun.hh"
#include <string>

/*
 * L'attuatore digitale rappresenta una valvola o una pompa che possa 
 * essere attivata tramite un segnale digitale inviato tramite arduino. 
 * La classe mantiene lo stato della pompa (accesa o spenta).
 */

using namespace std;

class attuatoreDig : public modulo {
  public:
	// Crea l'attuatore da una base, arduino, canale, log e risposta lunga AS
    attuatoreDig(base myb, arduino * , int , bool , bool );
    // Crea l'attuatore da id, nome, posizione, arduino, canale, log e risposta lunga AS
    attuatoreDig(int , string , string , arduino * , int , bool , bool );
    // Imposta l'attuatore
    void setAttuatore(bool );
    // Il tipo può essere: m=base, s=sonda, v=valvola
	virtual tipiModuli getTipo() const; 	
	// Il modo può essere 0 lettura, 1 scrittura 
	virtual modi getModo() const; 
	// Crea una stringa per le impostazioni
	virtual string CreaRigaImpostazioni();
	// Recupera il nome del modello
	virtual string getModel() const;
	// Restituisce lo stato della valvola
    inline bool getStato() const { return _stato; };
	
  private:
	// Stato corrente della valvola
    bool _stato;

};
#endif
