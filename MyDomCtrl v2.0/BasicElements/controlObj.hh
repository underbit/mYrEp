#ifndef CONTROLOBJ_H
#define CONTROLOBJ_H

#include "attuatoreDig.hh"
#include "base.hh"

/*
 * La classe è la super classe di tutti quegli oggetti 
 * che hanno il controllo sui dispositivi del sistema.
 */
 
const float VALORE_NULL = -100.0;

enum tendenze { NESSUNA, SCESA, STABILE, SALITA };

enum tipiControlli { NO_CONTROL, SORGENTE, ZONA };

class controlObj : public base {
	private:
		// La variabile indica che se è una periferica a controllo automatica,
		// è settata sul funzionamento automatico. Agisce solo sulla valvola
		// e non sull'intera zona
		bool _isOnAuto;
		bool _ricCal;
		bool _ricDis;
		// Temperatura di inizio esercizio
		float _temperaturaMinEsercizio;
		// Riferimento all'arduino
		attuatoreDig *_valvola;
		// Stato della valvola
		bool _isOpen;
		
	protected:
		// Tendenze
		float _temperatura1, _temperatura2;
		void setStatoValvola(bool);
		void setTemperatura(float);
		
	public:
		// REstituisce se ha una valvola o è una sorgente solo da monitorare
		bool getHasValvola() const { return _valvola!=NULL; };
		// Restituisce l'id del motore
		int getIdMotore() const { return _valvola->getId(); };
		// Recupera se c'è stata una richiesta di calore
		bool getRichiestaCalore() const { return _ricCal; };
		// Un controllo ha richiesto calore
		void setRichiestaCalore(bool);
		// Recupera se c'è stata una richiesta di calore
		bool getRichiestaDissipazione() const { return _ricDis; };
		// Un controllo ha richiesto calore
		void setRichiestaDissipazione(bool);
		// Restituisce la tendenza
		virtual tendenze getTendenza();
		// Restituisce il carattere della tendenza
		char getCharTendenza();
		// Imposta la temperatura di apertura della valvola
		void setTemperaturaMinEsercizio(float);
		// Restituisce la temperatura di apertura della valvola
		float getTemperaturaMinEsercizio() const { return _temperaturaMinEsercizio; };
		// Crea l'oggetto come da classe base
		controlObj(int, string, string, attuatoreDig*, float);
		controlObj(int, string, string, attuatoreDig*, float, bool);
		// Il programma cede il controllo al gestore di un controllo
		// il primo parametro è la temperatura dell'impianto, il secondo e il terzo se c'è una richiesta 
		// di calore e di dissipazione. La risposta positiva indica un allarme.
		virtual bool ControlloDispositivo(float, bool, bool);
		 // Crea una riga di impostazioni
		virtual string CreaRigaImpostazioni();
		// Recupera il nome del modello
		virtual string getModel() const { return ""; }; 
		// Attivazione automatica
		void setOnAuto();
		// Attivazione manuale
		void setOnManual();		
		// Restituisce lo stato automatico
		bool getIsAuto() const { return _isOnAuto; };
		// Restituisce lo stato della valvola
		bool getStatoValvola() const { return _valvola->getStato(); };
		// Restituisce la temperatura
		float getTemperatura() const { return _temperatura2; };
		// Apre la valvola e imposta in manuale
		void ApriValvola();
		// Chiude la termozona
		void ChiudiValvola();
		bool getMustBeOpen() const { return _isOpen; };
		// Procedura per la gestione dei messaggi di rete
		virtual string NetComputing(int, int) = 0;
};
#endif
