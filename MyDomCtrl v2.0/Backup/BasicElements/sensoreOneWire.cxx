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

// Metodi della classe 
void sensoreOneWire::LeggiValore() {
	_arduino->InviaMessaggioOW(_indirizzo, _canale, _celsius, _verbose, _logRisposte);
	string dati = _arduino->LeggiRispostaAS();
	if(_arduino->getComandoEseguito()) 
		_ultimaLettura = StringToFloat(dati);
}

void sensoreOneWire::ImpostaCelsius() {
	_celsius = true;
}
void sensoreOneWire::ImpostaFahre() {
	_celsius = false;
}
