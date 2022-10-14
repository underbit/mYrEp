#ifndef MDSYSCTRL_H
#define MDSYSCTRL_H

#include "BasicElements/sensoreOneWire.hh"
#include "MDLoader.hh"
#include "BasicElements/NetMessage.hh"
#include <list>

class MDSysCtrl {
	
	public:
		MDSysCtrl();
		void AvviaServizio();
		void CaricaImpostazioni();
		MDLoader MyLoader;
		bool getIsStopped() const { return _fermaServizio; };
		void setIsStopped(bool);
		bool getRichiestaCalore() const { return _richiestaCalore; };
		bool getRichiestaDissipazione() const { return _richiestaDissipazione; };
		void setRichiestaCalore(bool);
		void setRichiestaDissipazione(bool);
		bool getIsOnAlarm() const { return _onAlarm; };
		void setIsOnAlarm(bool);
		// Procedura per la gestione dei messagi di rete
		string RicezioneNetMsg(string);
		// Sensore della temperatura esterna. Opzionale
		sensoreOneWire * tempEsterna;
		// Sensore della temperatura del tampone. Necessaria.
		sensoreOneWire * tempImpianto;
		
	private:
		// Variabile che memorizza la richiesta di calore delle zone e/o acs
		bool _richiestaCalore;
		// Variabile che memorizza la richiesta di dissipazione delle sorgenti
		bool _richiestaDissipazione;
		// Variabile che determina se uno o più dispositivi sono in allarme
		bool _onAlarm;
		// La variabile serve ad interrompere il servizio
		bool _fermaServizio;
		NetMessage myMsg;
		// Processa i comandi in modalità overview
		string processaControlliOW(int, int);
		// Legge i valori delle sonde di sistema
		string leggiSondeSys();
		// Elenca i controlli e tutte le informazioni generali
		string ElencaControlliNetComp();
};

#endif
