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

// Imposta l'attuatore come richiesto
void attuatoreDig::SetAttuatore(bool val) {
	if(_verbose) cout << "[" << getNome() << "]" << MSG_INVIO;
	_arduino->InviaMessaggio(true, 'd', _canale, val ? 1 : 0, _logRisposte);
	_arduino->LeggiRispostaAS();
	if(_arduino->getComandoEseguito()) {
		if(_verbose) cout << MSG_OK << endl;
		_stato = val;
	} else if(_verbose) cout << MSG_FALLITO << endl;	
}
