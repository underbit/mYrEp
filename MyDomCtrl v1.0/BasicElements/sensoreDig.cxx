#include "sensoreDig.hh"
#include <string>

sensoreDig::sensoreDig (int id, string nome, string posizione, arduino *myarduino, int canale,
			  bool verb, bool log)
	: modulo(id, nome, posizione, myarduino, canale, verb, log) {
	_ultimaLettura = false;
}

sensoreDig::sensoreDig (base myb, arduino *myarduino, int canale,
			  bool verb, bool log)
	: modulo(myb.getId(), myb.getNome(), myb.getPosizione(), myarduino, canale, verb, log) {
	_ultimaLettura = false;
}

string sensoreDig::CreaRigaImpostazioni() { return modulo::CreaRigaImpostazioni(); }
string sensoreDig::getModel() const { return "Sensore Digitale Std"; };
// Imposta il menbro virtuale
tipiModuli sensoreDig::getTipo() const { return SONDA; };
// Imposta il menbro virtuale
modi sensoreDig::getModo() const { return LETTURA; }; 
// Restituisce se il modulo è in errore
bool sensoreDig::getOnAlarm() const { return false; };

// Metodi della classe 
void sensoreDig::LeggiValore() {
	_arduino->InviaMessaggio(false, 'd', getCanale(), _verbose, _logRisposte);
	string dati = _arduino->LeggiRispostaAS();
	if(_arduino->getComandoEseguito()) 
		_ultimaLettura = StringToFloat(dati);
}
