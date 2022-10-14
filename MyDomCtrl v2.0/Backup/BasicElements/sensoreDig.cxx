#include "sensoreDig.hh"
#include <string>

sensoreOneWire::sensoreOneWire (int id, string nome, string posizione, arduino *myarduino, int canale,
			  bool verb, bool log)
	: modulo(id, nome, posizione, myarduino, canale, verb, log) {
	_ultimaLettura = false;
}

sensoreOneWire::sensoreOneWire (base myb, arduino *myarduino, int canale,
			  bool verb, bool log)
	: modulo(myb.getId(), myb.getNome(), myb.getPosizione(), myarduino, canale, verb, log) {
	_ultimaLettura = false;
}

// Metodi della classe 
void sensoreOneWire::LeggiValore() {
	_arduino->InviaMessaggio(false, 'd', getCanale(), _verbose, _logRisposte);
	string dati = _arduino->LeggiRispostaAS();
	if(_arduino->getComandoEseguito()) 
		_ultimaLettura = StringToFloat(dati);
}
