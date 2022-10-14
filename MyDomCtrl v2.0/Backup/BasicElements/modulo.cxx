#include "modulo.hh"
#include "arduino.hh"

// Costruttore della classe modulo. Imposta le variabili di log
modulo::modulo (int id, string nome, string posizione, arduino *mya, int canale, bool v, bool r)
	:base(id, nome, posizione) {
	if(canale>0 && canale<14) _canale=canale; 
	else canale = 13;
	_verbose = v;
	_logRisposte = r;
	_arduino = mya;
}

// Costruttore della classe modulo. Imposta le variabili di log
modulo::modulo (base mybase, arduino *mya, int canale, bool v, bool r)
	:base(mybase.getId(), mybase.getNome(), mybase.getPosizione()) {
	if(canale>0 && canale<14) _canale=canale; 
	else canale = 13;
	_verbose = v;
	_logRisposte = r;
	_arduino = mya;
}

// Imposta la modalità di visualizzazione dei log
void modulo::setVerbose(bool v) {
	_verbose = v;
}

// Imposta il tipo di risposta sui log 
void modulo::setLogRisposte(bool l) {
	_logRisposte = l;
}

