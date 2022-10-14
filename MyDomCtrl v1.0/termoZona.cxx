#include "termoZona.hh"
#include "BasicElements/orario.hh"
#include <ctime>

// Crea la termozona
termoZona::termoZona(int id, string nome, string posizione, attuatoreDig* ad, float tme, sensoreOneWire* sow[],
					 programma* p, int nt, bool isAuto, bool ZonaAuto, float tempMan) 
	:controlObj(id, nome, posizione, ad, tme, isAuto) {
	setTemperatura(0);
	_num_termometri = nt;
	for(int i = 0; i < nt; i++) {
		_termometri[i] = sow[i];
	}
	_programma = p;
	if(p!=NULL) _zonaAuto = ZonaAuto;
	else _zonaAuto = false;
	_temperaturaManuale = tempMan;
}

// Attivazione Programma
void termoZona::setZonaAuto(programma* p) {
	if(p!=NULL) {
		_programma = p; 
		_zonaAuto = true;
	}
}
// Attivazione manuale alla temperatura
void termoZona::setZonaManual(float t) {
	_zonaAuto = false;
	_temperaturaManuale = t;
}
void termoZona::setZonaManual() {
	_zonaAuto = false;	
}		

// Restituisce la temperature del termometro indicato
float termoZona::getTemperaturaTermometro(int nt) {
	// Se al numero richiesto corrisponde un termometro, ne legge la temperatura
	float appo = -100;
	if(nt < _num_termometri) {
		_termometri[nt]->LeggiValore();
		appo = _termometri[nt]->getUltimaLettura();
	}
	return appo;
}

tendenze termoZona::getTendenza() {
  return controlObj::getTendenza();
}


// Cede il controllo alla termozona per eseguire le sue operazioni
bool termoZona::ControlloDispositivo(float t, bool r, bool d) {
	bool richiesta = false;
	float minT = 100;
	float maxT = -100;
	float appo = -100;
	cout << "---Temperature:";
	// Recupera le temperature e ne memorizza minimo e massimo
	for(int i = 0; i < _num_termometri; i++) {
		if(_termometri[i]!=NULL) {
			appo = getTemperaturaTermometro(i);
			cout << " t" << i << "=" << appo;
			if(_termometri[i]->getOnAlarm()) cout << " *ALLARME* ";
			if(appo < minT) minT = appo;
			if(appo > maxT) maxT = appo;
		}
	}
	// Aggiorna i valori delle temperature memorizzate
	_temperatura1 = _temperatura2;
	_temperatura2 = minT;
	cout << " -tend " << getCharTendenza() << "-";
	// Dissipa in base alla richiesta critica
	if(d) { 
		setStatoValvola(true);
		cout << " *FORZATA APERTURA*";
	} else {
		if(getIsAuto()) {
			cout << " -ValvAuto-";
			if(_zonaAuto) {		
				// Recupera l'ora attuale e la imposta in un oggetto orario
				time_t currentTime;
				time(&currentTime);
				struct tm *localTime = localtime(&currentTime);
				int oggi = localTime->tm_wday;
				orario oraAttuale(localTime->tm_hour, localTime->tm_min);
				// Controlla se c'è una necessità di calore nella zona
				cout << " (" << _programma->getNome() << ")";
				richiesta = _programma->controllaFascia(oggi, oraAttuale, minT, maxT);
			} else {
				cout << " (MANUALE " << _temperaturaManuale << ")";
				richiesta = _temperatura2<_temperaturaManuale;
			}
			// Controlla se la richiesta è soddisfabile o richiede calore
			if(richiesta) {
				// L'impianto non ha un boiler tampone
				if(t==0) {
					setStatoValvola(true);
					cout << " ---OPEN";
					setRichiestaCalore(true);					
				} else {
					// Boiler tampone a temperatura
					if(t>getTemperaturaMinEsercizio()) {
						setStatoValvola(true);
						cout << " ---OPEN";
						setRichiestaCalore(false);
					} // Altrimenti
					else {
						setStatoValvola(false);
						cout << " ---CLOSE";
						setRichiestaCalore(true);
					}
				}
			} else {
				setStatoValvola(false);
			}
		} else { 
			cout << " -ValvMan-";
			// Impostazione manuale della valvola
			setStatoValvola(getMustBeOpen());
			if(getMustBeOpen()) cout << " ---OPEN";
			else cout << " ---CLOSE";
			richiesta = t<getTemperaturaMinEsercizio();
		}
		setRichiestaCalore(richiesta);
	}
	return false;
}

// Crea una riga con le impostazioni da salvare
string termoZona::CreaRigaImpostazioni() {
	string appo = IntToString(getTipoBase()) + ":";
	appo += IntToString(getId()) + ":";
	appo += getNome() + ":" + getPosizione() + ":";
	appo += getIdMotore() + ":";
	appo += "1:";	//Tipo termozona
	for(int i = 0; i < _num_termometri; i++)
		appo += "s:" + IntToString(_termometri[i]->getId()) + ":";
	appo += IntToString(_programma->getId()) + ":";
	appo += "\n";
	return appo;
}
