#include "attuatoreDig.hh"

const char END_MSG[] = "$%%$";
const char MSG_INVIO[] = "Invio comando : ";
const char MSG_OK[] = " [OK]";
const char MSG_FALLITO[] = " [FALLITO]";

attuatoreDig::attuatoreDig(int id, string nome, string posizione, arduino *mya, int canale, bool v, bool r)
	:modulo(id, nome, posizione, mya, canale, v, r) {
	_stato = false;
}

attuatoreDig::attuatoreDig(base myb, arduino *mya, int canale, bool v, bool r)
	:modulo(myb.getId(), myb.getNome(), myb.getPosizione(), mya, canale, v, r) {
	_stato = false;
}

string attuatoreDig::getModel() const { return "Attuatore Digitale Std"; };
// Imposta il menbro virtuale
tipiModuli attuatoreDig::getTipo() const { return VALVOLA; };
// Imposta il menbro virtuale
modi attuatoreDig::getModo() const { return SCRITTURA; }; 

string attuatoreDig::CreaRigaImpostazioni() { return modulo::CreaRigaImpostazioni(); }

// Imposta l'attuatore come richiesto
void attuatoreDig::setAttuatore(bool val) {
	int appo = val ? 1 : 0;
	if(_verbose) {
		cout << endl << "------[" << getNome() << "]" << MSG_INVIO;
		if(val) cout << "--open--";
		else cout << "--close--";
	}
	_arduino->InviaMessaggio(true, 'd', _canale, appo , _verbose, _logRisposte);
	_arduino->LeggiRispostaAS();
	if(_arduino->getComandoEseguito()) {
		if(_verbose) cout << MSG_OK << endl;
		if(getOnAlarm() == true) resetAlarm();
		_stato = val;
	} else {
		if(_verbose) cout << MSG_FALLITO << endl;	
		checkOnAlarm();
	}
}
