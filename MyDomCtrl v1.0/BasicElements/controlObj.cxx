#include "controlObj.hh"

// Restituisce la tendenza della temperatura
tendenze controlObj::getTendenza() {
  if(_temperatura1 == -100.0 || _temperatura2 == -100.0) return NESSUNA;
  if(_temperatura1 == _temperatura2) return STABILE;
  if(_temperatura1 > _temperatura2) return SCESA;
  if(_temperatura1 < _temperatura2) return SALITA;
  return NESSUNA;
}

char controlObj::getCharTendenza() {
 if(_temperatura1 == -100.0 || _temperatura2 == -100.0) return ' ';
  if(_temperatura1 == _temperatura2) return '=';
  if(_temperatura1 > _temperatura2) return '<'; 
  if(_temperatura1 < _temperatura2) return '>';
  return ' ';
}

// Apre la termozona
void controlObj::ApriValvola() {
	_isOnAuto = false;
	_isOpen = true;
}

// Chiude la termozona
void controlObj::ChiudiValvola() {
	_isOnAuto = false;
	_isOpen = false;
}

// Un controllo ha richiesto calore
void controlObj::setRichiestaCalore(bool c) {
	_ricCal = c;
}

// Un controllo ha richiesto calore
void controlObj::setRichiestaDissipazione(bool c) {
	_ricDis = c;
}

// Rispristina automatica
void controlObj::setOnAuto() {
	_isOnAuto = true;
}

// Imposta il controllo su manuale
void controlObj::setOnManual() {
	_isOnAuto = false;
}

// Imposta lo stato dell'elettrovalvola
void controlObj::setStatoValvola(bool val) {
	_valvola->setAttuatore(val);
	if(_valvola->getOnAlarm()) cout << " *ALLARME* ";
}

// Imposta la temperatura attuale
void controlObj::setTemperatura(float temp) {
	_temperatura1 = temp;
	_temperatura2 = temp;
}

controlObj::controlObj(int id, string nome, string posizione, attuatoreDig* ad, float tempNE)
	:base(id, nome, posizione) {
	_ricCal = false;
	_ricDis = false;
	_isOnAuto = true;
	_temperatura1 = VALORE_NULL;
	_temperatura2 = VALORE_NULL;
	_temperaturaMinEsercizio = tempNE;
	_valvola = ad;
	_isOpen = false;
}

controlObj::controlObj(int id, string nome, string posizione, attuatoreDig* ad, float tempNE, bool vAuto)
	:base(id, nome, posizione) {
	_ricCal = false;
	_ricDis = false;
	_isOnAuto = vAuto;
	_temperatura1 = VALORE_NULL;
	_temperatura2 = VALORE_NULL;
	_temperaturaMinEsercizio = tempNE;
	_valvola = ad;
	_isOpen = false;
}

// Il programma cede il controllo al gestore di un controllo
bool controlObj::ControlloDispositivo(float f, bool b, bool d) { return false; }
// Crea una riga di impostazioni
string controlObj::CreaRigaImpostazioni() { return ""; }
