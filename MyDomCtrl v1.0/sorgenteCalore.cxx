
#include "sorgenteCalore.hh"
#include "BasicElements/base.hh"

// Crea l'oggetto sorgente di calore 
sorgenteCalore::sorgenteCalore(int id, string nome, string posizione, attuatoreDig * att, float tempNE,
									sensoreOneWire * sow, float tempC, bool automa, bool isAuto) 
	:controlObj(id, nome, posizione, att, tempNE, isAuto) {

	setTemperatura(0);
	_temperaturaCritica = tempC;
	_isAutomatica = automa;
	_termometro = sow;
}

// Imposta la temperatura critica
void sorgenteCalore::setTemperaturaCritica(float T) {
	_temperaturaCritica = T;
}

// Procedura per il controllo dello strumento. Nel caso di una caldaia,
// il disposito non ha un termometro ed entra in funzione automaticamente
bool sorgenteCalore::ControlloDispositivo(float t, bool r, bool d) {
	bool risposta = false;
	if(_termometro != NULL) {
		// Recupera la temperatura del sensore
		_termometro->LeggiValore();
		_temperatura1 = _temperatura2;
		_temperatura2 = _termometro->getUltimaLettura();
		cout << "---Temperatura: " << _temperatura2;
		if(_termometro->getOnAlarm()) cout << " *ALLARME* ";
		cout << " -tend " << getCharTendenza() << "-";
		// Controlla la valvola di funzionamento se presente
		if(getHasValvola()) {
			cout << " -HasValv-";
			if(getIsAuto()) {
				cout << " -ValvAuto-";
				if(_temperatura2 > t && _temperatura2 > getTemperaturaMinEsercizio()) {
					setStatoValvola(true);
					cout << " ---OPEN";
				} else { 
					setStatoValvola(false);
					cout << " ---CLOSE";
				}
				risposta = (_temperatura2 > _temperaturaCritica);
				setRichiestaDissipazione(risposta);
			} else { 
				setStatoValvola(getMustBeOpen()); 
				cout << " -ValvMan-";
			}
		} 
	} else {
		cout << "---NoTermo";
		if(getIsAuto()) {
			cout << " -ValvAuto-";
			if(_isAutomatica) {
				cout << " -AccAuto-";
				// La sorgente con accensione automatica è l'unica 
				// che può intercettare la richiesta di calore.
				if(r) {
					setStatoValvola(true);
					cout << " *FORZATA ACCENSIONE*";
				} else { setStatoValvola(false); }			
			} else { cout << " -AccMan-"; }
		} else {
			cout << " -ValvMan-";
			if(_isAutomatica) cout << " -AccAuto-";
			else cout << " -AccMan-";
			setStatoValvola(getMustBeOpen());
			if(getMustBeOpen()) cout << " ---OPEN";
			else cout << " ---CLOSE";
						
		}
	}
	// Restituisce se è stata raggiunta la temperatura critica.
	return risposta;
}

// Crea una riga con le impostazioni da salvare
string sorgenteCalore::CreaRigaImpostazioni() {
	string appo = IntToString(getTipoBase()) + ":";
	appo += IntToString(getId()) + ":";
	appo += getNome() + ":" + getPosizione() + ":";
	appo += getIdMotore() + ":";
	appo += "0:";
	appo += "s:" + IntToString(_termometro->getId()) + ":";
	appo += FloatToString(_temperaturaCritica) + ":";
	if(_isAutomatica) appo += "1:";
	else appo += "0:";
	appo += ":\n";
	return appo;
}

// Restituisce la tendenza della temperatura
tendenze sorgenteCalore::getTendenza() {
  return controlObj::getTendenza();
}
