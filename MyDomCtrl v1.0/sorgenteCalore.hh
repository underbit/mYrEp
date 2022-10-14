#ifndef SORGENTECALORE_H
#define SORGENTECALORE_H

#include "BasicElements/sensoreOneWire.hh"
#include "BasicElements/controlObj.hh"

/* 
 * La sorgente di calore rappresenta un oggetto fisico che pu� 
 * produrre calore. La temperatura dell'acqua locale � visualizzata
 * tramite il termometro e dopo che la temperatura di esercizio � 
 * stata raggiunta, la valvola si apre per far entrare in funzione 
 * il riscaldamento. La protezione della temperatura critica indica 
 * al sistema di aprire tutte le valvole zona per portar via il 
 * calore prodotto. Un esempio � un termocamino. 
 * La sorgente pu� essere automatica o meno. Se � automatica � 
 * possibile attivarla con l'operazione attiva.
 */
 
class sorgenteCalore : public controlObj {
  private:
    // Termometro della sorgente
    // La caldaia ha il termometro = NULL
    sensoreOneWire *_termometro;
	// Temperatura critica
	float _temperaturaCritica;
	// Indica se la sorgente si pu� attivare automaticamente o
	// � necessario l'intervento dell'utente
	bool _isAutomatica;
	
  public:
    // codice, nome, posizione, valvola, termometro, temperatura esercizio, temperatura critica, automatica
    sorgenteCalore(int, string, string, attuatoreDig *, float, sensoreOneWire*, float, bool, bool);
    // Restituisce la condizione d'allarme
	bool getIsOnAlarm() const { return _termometro!=NULL && getTemperatura() >= _temperaturaCritica; }; 
    // Imposta la temperatura critica
    void setTemperaturaCritica(float);
    // Funzione nella quale la sorgente riceve il controllo
	virtual bool ControlloDispositivo(float, bool, bool);
	// Recupera il nome del modello
	virtual string getModel() const { return "SorgenteCalore"; };
	// Crea una riga di impostazioni
	virtual string CreaRigaImpostazioni();
	// Restituisce la tendenza
    virtual tendenze getTendenza();
};
#endif
