#include "termometroOneWire.hh"
#include <string>
#include "libfun.hh"

termometroOneWire::termometroOneWire (int id, string nome, string posizione, arduino *myarduino, int canale,
			  bool verb, bool log, bool celsius, const char *indirizzo)
	: termometro(id, nome, posizione, myarduino, canale, verb, log, _celsius) {
	_indirizzo = indirizzo;
}

termometroOneWire::termometroOneWire (base myb, arduino *myarduino, int canale,
			  bool verb, bool log, bool celsius, const char *indirizzo)
	: termometro(myb.getId(), myb.getNome(), myb.getPosizione(), myarduino, canale, verb, log, _celsius) {
	_indirizzo = indirizzo;
}

string termometroOneWire::getModel() const { return "Termometro One Wire"; };

// Metodi della classe 
void termometroOneWire::LeggiValore() {
	_arduino->InviaMessaggioOW(_indirizzo, _canale, _celsius, _verbose, _logRisposte);
	string dati = _arduino->LeggiRispostaAS();
	if(_arduino->getComandoEseguito()) 
		_temperatura = StringToFloat(dati);
}

// Crea una riga con le impostazioni da salvare
string termometroOneWire::CreaRigaImpostazioni() {	
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
