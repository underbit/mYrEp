#include "termometro.hh"
#include <string>
#include "libfun.hh"

sensoreOneWire::sensoreOneWire (int id, string nome, string posizione, arduino *myarduino, int canale,
			  bool verb, bool log, bool celsius)
	: modulo(id, nome, posizione, myarduino, canale, verb, log) {
	_celsius = celsius;
	_temperatura = 0;
}

sensoreOneWire::sensoreOneWire (base myb, arduino *myarduino, int canale,
			  bool verb, bool log, bool celsius)
	: modulo(myb.getId(), myb.getNome(), myb.getPosizione(), myarduino, canale, verb, log) {
	_celsius = celsius;
	_temperatura = 0;
}
string sensoreOneWire::getModel() const { return "Termometro"; };
// Imposta il membro virtuale
tipiModuli sensoreOneWire::getTipo() const { return SONDA; };
// Imposta il membro virtuale
modi sensoreOneWire::getModo() const { return LETTURA; }; 

void sensoreOneWire::ImpostaCelsius() {
	_celsius = true;
}
void sensoreOneWire::ImpostaFahre() {
	_celsius = false;
}
