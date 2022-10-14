#include "NetMessage.hh"
#include "libfun.hh"

// Inizializza tutto a zero
NetMessage::NetMessage() {
	_tipo = -1;
	_code = -1;
	_op = -1;
	_val = -1;
}

// Usato per avviare e per chiudere il servizio
NetMessage::NetMessage(int val) {
	_tipo = 0;
	_code = 0;
	_op = 0;
	if(val>0) _val = 1;
	else _val = 0;
}

// Inizializza l'oggetto con tutte le sue variabili
NetMessage::NetMessage(int tp, int code, int op, int val) {
	_tipo = tp;
	_code = code;
	_op = op;
	_val = val;
}

void NetMessage::setRisposta(string msg) {
	_risposta = msg;
} 

// Carica il messaggio nella struttura
bool NetMessage::CaricaMessaggio(std::string msg) {
	// Inizializziamo le variabili dell'oggetto a nessun messaggio
	_tipo = -1;
	_code = -1;
	_op = -1;
	_val = -1;
	_risposta = "";
	// Creiamo e inizializiamo le variabili interne
	unsigned int i = 0;
	int para = 0;
	string appo ="";
	// Controlla tutta la stringa msg
	while(i < msg.length()){
		// Memorizza il carattere da controllare
		char c = msg[i];
		// Se non è il carattere fine intero, memorizza il numero
		// nella variabile di lavoro
		if(c != '.') appo += c;
		// Altrimenti memorizza appo convertito in numero
		// nella variabile giusta.
		else {
			// Conversione da stringa a int
			int n = StringToInt(appo);
			// Memorizzazione con scelta della variabile
			switch(para) {
				case 0:
					_tipo = n;
				case 1:
					_code = n;
				case 2:
					_op = n;
				case 3:
					_val = n;
			}
			appo = "";
			para++;
		}
	}
	return getMessaggioValido();
}
