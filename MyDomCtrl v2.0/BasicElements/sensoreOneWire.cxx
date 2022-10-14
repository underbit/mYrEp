#include "sensoreOneWire.hh"
#include <string>
#include "libfun.hh"

sensoreOneWire::sensoreOneWire (int id, string nome, string posizione, arduino *myarduino, int canale,
			  bool verb, bool log, bool celsius, const char *indirizzo)
	: modulo(id, nome, posizione, myarduino, canale, verb, log) {
	_celsius = celsius;
	_indirizzo = indirizzo;
	_ultimaLettura = 0;
}

sensoreOneWire::sensoreOneWire (base myb, arduino *myarduino, int canale,
			  bool verb, bool log, bool celsius, const char *indirizzo)
	: modulo(myb.getId(), myb.getNome(), myb.getPosizione(), myarduino, canale, verb, log) {
	_celsius = celsius;
	_indirizzo = indirizzo;
	_ultimaLettura = 0;
}

// Restituisce il modello o il nome del dispositivo
string sensoreOneWire::getModel() const { return "Sensore One Wire"; };
// Imposta il menbro virtuale
tipiModuli sensoreOneWire::getTipo() const { return SONDAOW; };
// Imposta il menbro virtuale
modi sensoreOneWire::getModo() const { return LETTURA; }; 

// Metodi della classe 
bool sensoreOneWire::LeggiValore() {
	bool risposta = false;
	if(_verbose) cout << endl << "------[" << getNome() << "]" << "Lettura Valore ";
	_arduino->InviaMessaggioOW(_indirizzo, _canale, _celsius, _verbose, _logRisposte);
	string dati = _arduino->LeggiRispostaAS();
	if(_arduino->getComandoEseguito()){
		risposta = true;
		_ultimaLettura = StringToFloat(dati);
		if(_verbose) cout << "[OK]" << endl;
		if(getOnAlarm() == true) resetAlarm();
	} else {
		if(_verbose) cout << "[FAILED]" << endl;
		checkOnAlarm();
	}
	return risposta;
}

void sensoreOneWire::ImpostaCelsius() {
	_celsius = true;
}
void sensoreOneWire::ImpostaFahre() {
	_celsius = false;
}

// Crea una riga con le impostazioni da salvare
string sensoreOneWire::CreaRigaImpostazioni() {	
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
	appo += ":";
	if(getIsCelsius()) appo += '1';
	else appo += '0';
	appo += ":";
	appo += getIndirizzo();
	appo += ":\n";
	return appo;
}
