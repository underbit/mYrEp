#ifndef NETMESSAGE_H
#define NETMESSAGE_H
/*
	 * La richiesta è formata dal tipo e dal codice,
	 * dal comando e dal parametro del comando
	 * e.g. 4.1.1.0 (4: controllo, 1: codice, 1: leggi)
	 * e.g. 4.1.2.1 (4: controllo, 1: codice, 2: scrivi 1: acceso)
	 * Il tipo 0 viene usato per comandi overview
	 * Il comando 0.0.0.0 ferma il servizio
	 * il comando 0.0.0.1 avvia il servizio
	 * Il comando 0.0.1.0 legge l'elenco dei controlli
	 * Il comand0 0.0.1.1 legge lo stato degli allarmi
	 * 
	 * 
 */
 
#include <string>

using namespace std;

class NetMessage {

	private:
		int _tipo, _code, _op, _val;
		std::string _risposta;
		
	public:
		int getTipo() const { return _tipo; };
		int getCodice() const { return _code; };
		int getOperazione() const { return _op; };
		int getValore() const { return _val; };
		std::string getRisposta() const { return _risposta; };
		void setRisposta(string);
		bool getMessaggioValido() const { return (_tipo != -1) && (_code != -1) && (_op != -1) && (_val != -1); };
		NetMessage();
		NetMessage(int);
		NetMessage(int, int);
		NetMessage(int, int, int, int);
		bool CaricaMessaggio(std::string msg);
};

#endif
