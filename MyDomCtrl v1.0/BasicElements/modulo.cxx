#include "modulo.hh"
#include "arduino.hh"
#include <string>

using namespace std;

// Costruttore della classe modulo. Imposta le variabili di log
modulo::modulo (int id, string nome, string posizione, arduino *mya, int canale, bool v, bool r)
	:base(id, nome, posizione) {
	if(canale>0 && canale<14) _canale=canale; 
	else canale = 13;
	_verbose = v;
	_logRisposte = r;
	_arduino = mya;
	_onAlarm = false;
}

// Costruttore della classe modulo. Imposta le variabili di log
modulo::modulo (base mybase, arduino *mya, int canale, bool v, bool r)
	:base(mybase.getId(), mybase.getNome(), mybase.getPosizione()) {
	if(canale>0 && canale<14) _canale=canale; 
	else canale = 13;
	_verbose = v;
	_logRisposte = r;
	_arduino = mya;
	_onAlarm = false;
}

// Imposta la modalità di visualizzazione dei log
void modulo::setVerbose(bool v) {
	_verbose = v;
}
// Imposta il tipo di risposta sui log 
void modulo::checkOnAlarm() {
	_arduino->InizializzaPortaSeriale();
	_onAlarm = !_arduino->getInizializzazioneOK();
}
// Imposta il tipo di risposta sui log 
void modulo::resetAlarm() {
	_onAlarm = false;
}
// Imposta il tipo di risposta sui log 
void modulo::setLogRisposte(bool l) {
	_logRisposte = l;
}
// Restituisce il modello o il nome del dispositivo
string modulo::getModel() const { return "Modulo"; };
// Imposta il menbro virtuale
tipiModuli modulo::getTipo() const { return NO_MODULO; };
// Imposta il menbro virtuale
modi modulo::getModo() const { return LETTURA; }; 

// Crea una riga con le impostazioni da salvare
string modulo::CreaRigaImpostazioni() {
	string appo = IntToString(getTipoBase()) + ":";
	appo += IntToString(getId()) + ":";
	appo += getNome() + ":" + getPosizione() + ":";
	appo += IntToString(_arduino->getId()) + ":" + IntToString(getTipo()) + ":";
	appo += IntToString(getCanale()) + ":";
	if(getVerbose()) appo += '1';
	else appo += '0';
	appo += ":";
	if(getLogRisposte()) appo += '1';
	else appo += '0';
	appo += ":\n";
	return appo;
}
