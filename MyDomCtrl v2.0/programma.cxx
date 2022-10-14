#include "programma.hh"
#include "BasicElements/libfun.hh"

// Crea un programma da un codice e da un nome
programma::programma(int code, string nome) {
	if(code > 0) _id = code;
	else _id = 0;
	if(nome != "") _nome = nome;
	else _nome = "Programma " + _id;
	for(int i = 0; i<7; i++) 
		_disponibile[i] = 0;
}

// Crea una fascia oraria
bool programma::creaFascia(int giorno, orario fine, float tempMin, float tempMax, temperaturaDaRaggiungere tr) {
	// Controlla se il giorno esiste, se esistono fascie disponibili e se l'orario di fine è congruo
	if(giorno > 0 && giorno < 8 && _disponibile[giorno-1] >= 0 
		&& _disponibile[giorno-1] < 8 && fine > _fascieOrarie[giorno-1][_disponibile[giorno-1]].fine ) {
		_fascieOrarie[giorno-1][_disponibile[giorno-1]].fine = fine;
		if(tempMax<=tempMin) tempMax = tempMin + 5;
		_fascieOrarie[giorno-1][_disponibile[giorno-1]].temperaturaMin = tempMin;
		_fascieOrarie[giorno-1][_disponibile[giorno-1]].temperaturaMax = tempMax;
		_fascieOrarie[giorno-1][_disponibile[giorno-1]].tr = tr;
		cout << "[Loader] Aggiunta fascia p=" << getNome() << " g=" << giorno << " h=" << fine.getOra() 
			 << " m=" << fine.getMinuti() << " tm=" << tempMin << " tM=" << tempMax << '\n';
		_disponibile[giorno-1]++;
		return true;
	} 
	return false;
}

// Restituisce true se siamo nella fascia e la temperatura è compresa
bool programma::controllaFascia(int giorno, orario ora, float tm, float tM) {
	bool appo = false;
	
	// Controlla in sequenza le fasce orarie
	for(int i = 0; i < _disponibile[giorno-1]; i++) {
		// Se trova la fascia o si trova all'ultima fascia del giorno
		if(_fascieOrarie[giorno-1][i].fine > ora || i == _disponibile[giorno-1]) {
			// Controlla quale temperatura deve raggiungere
			// Minima: almeno un termometro deve arrivare alla minima
			// Media: tutti i termometri devono arrivare alla minima
			// Massima: solo un termometro può arrivare alla massima
			switch(_fascieOrarie[giorno-1][i].tr) {
				case MINIMA: {						
					if(tM < _fascieOrarie[giorno-1][i].temperaturaMin) appo = true;
					break;
				}
				case MEDIA: {
					if(tm < _fascieOrarie[giorno-1][i].temperaturaMin) appo = true;
					break;
				}
				case MASSIMA: {
					if(tM < _fascieOrarie[giorno-1][i].temperaturaMax) appo = true;
					break;
				}
			}
			break;
		}
	}
	return appo;
}

// Crea una riga con le impostazioni da salvare
string programma::CreaRigaImpostazioni() {
	string appo = IntToString(getTipoBase()) + ":";
	appo += IntToString(getId()) + ":";
	appo += getNome() + ":\n";
	for(int i = 0; i<7; i++)
		for(int j = 0; j < _disponibile[i]; j++) {
			appo += "6:";
			appo += IntToString(getId()) + ":"; // Id del programma
			appo += IntToString(i) + ":";		// Numero del giorno della settimana
			orario myo = _fascieOrarie[i][j].fine;	// Carica l'ora di fine fascia
			appo += IntToString(myo.getOra()) + ":";
			appo += IntToString(myo.getMinuti()) + ":";
			appo += FloatToString(_fascieOrarie[i][j].temperaturaMin) + ":";
			appo += FloatToString(_fascieOrarie[i][j].temperaturaMax) + ":";
			appo += _fascieOrarie[i][j].tr + ":\n";
		}
	return appo;
}
