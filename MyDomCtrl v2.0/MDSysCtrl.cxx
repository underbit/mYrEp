#include "MDSysCtrl.hh"
#include "MDLoader.hh"
#include "BasicElements/controlObj.hh"
#include "BasicElements/arduino.hh"
#include "BasicElements/orario.hh"
#include "BasicElements/sensoreDig.hh"
#include "BasicElements/sensoreOneWire.hh"
#include "BasicElements/attuatoreDig.hh"
#include "BasicElements/base.hh"
#include "BasicElements/controlObj.hh"
#include <list>


MDSysCtrl::MDSysCtrl() {
	// Inizializza le variabili di comunicazione con i controlli
	_richiestaCalore = false;
	_richiestaDissipazione = false;
	_onAlarm = false;
}

void MDSysCtrl::CaricaImpostazioni() {
	// Carica da file i device, i programmi e i controlli
	MyLoader.CaricaImpostazioni(true);
	// Recupera la temperatura dell'impianto
	tempImpianto = MyLoader.getSonda(1);
	cout << endl << "[OPT1] Sonda Impianto: ";
	if(tempImpianto!=NULL) 
		cout << tempImpianto->getId() << " - " 
		 << tempImpianto->getNome() << " - " 
		 << tempImpianto->getPosizione() << " - "
		 << tempImpianto->getIndirizzo() << '\n';
	else cout << "NON PRESENTE\n";
	// Recupera la temperatura esterna
	tempEsterna = MyLoader.getSonda(100);
	cout << "[OPT2] Sonda Esterna: ";
	if(tempEsterna!=NULL) cout << tempEsterna->getId() << " - " 
							   << tempEsterna->getNome() << " - " 
							   << tempEsterna->getPosizione() << " - "
							   << tempEsterna->getIndirizzo() << '\n';
	else cout << "NON PRESENTE\n";
}

void MDSysCtrl::setRichiestaCalore(bool val) {
	_richiestaCalore = val;
}

void MDSysCtrl::setRichiestaDissipazione(bool val) {
	_richiestaDissipazione = val;
}
void MDSysCtrl::setIsOnAlarm(bool val) {
	_onAlarm = val;
}

void MDSysCtrl::setIsStopped(bool val) {
	_fermaServizio = val;
}

string MDSysCtrl::RicezioneNetMsg(string richiesta) {
	
	/*
	 * La richiesta è formata dal tipo e dal codice,
	 * dal comando e dal parametro del comando
	 * e.g. 4.1.1.0 (4: controllo, 1: codice, 1: leggi )
	 * e.g. 4.1.2.1 (4: controllo, 1: codice, 2: scrivi 1: acceso)
	 * Il tipo 0 viene usato per comandi overview
	 * Il comando 0.0.0.0 ferma il servizio
	 * il comando 0.0.0.1 avvia il servizio
	 * Il comando 0.0.1.0 legge l'elenco dei controlli
	 * Il comand0 0.0.1.1 legge lo stato degli allarmi
	 * 
	 * 
	 */
	 // Carica il messaggio
	 NetMessage myMsg; 
	 string r = "EEE";
	 myMsg.CaricaMessaggio(richiesta);
	 // Processa il messaggio a seconda del tipo
	 switch (myMsg.getTipo()) {
		case 0: 
			// Overview
			r = processaControlliOW(myMsg.getOperazione(), myMsg.getValore());
			break;
		case 1: {
				// Elabora un messaggio diretto ad un Controllo
				controlObj* MCO = MyLoader.CercaControllo(myMsg.getCodice());
				r = MCO->NetComputing(myMsg.getOperazione(), myMsg.getValore());
			}
			break;
		case 2:
			// Zona
			break;
	 }
	 // Invia la risposta
	 return r;
}

string MDSysCtrl::processaControlliOW(int op, int val) {
	string r;
	switch (op) {
		case 0: {
				// Ferma e avvia il servizio
				_fermaServizio = val;
				r = "00";
			}
			break;
		case 1:
			// Elenca i controlli
			r = ElencaControlliNetComp();
			break;
		case 2:
			// Leggi le sonde del sistema
			r = leggiSondeSys();
		default:
			// Errore nella richiesta
			r="EEE";
			break;
	}
	r += "FFF";
	return r;
}

string MDSysCtrl::leggiSondeSys() {
	string g ="02";
	// Legge la temperatura dell'impianto
	if(tempImpianto != NULL)
		g += tempImpianto->getUltimaLettura();
	g += "XXX";
	// Legge la temperatura esterna
	if(tempEsterna != NULL)
		g += tempEsterna->getUltimaLettura();
	g += "XXX";
	return g;
}

string MDSysCtrl::ElencaControlliNetComp() {
	string r = "01";
	for(int i = 0; i < MyLoader._num_controlli; i++) {
		r += MyLoader._listaControlli[i]->getId();
		r += ".";
		r += MyLoader._listaControlli[i]->getNome();
		r += ".";
		r += MyLoader._listaControlli[i]->getPosizione();
		r += ".";
		r += MyLoader._listaControlli[i]->getModel();
		r += ".";
		r += MyLoader._listaControlli[i]->getRichiestaCalore();
		r += ".";
		r += MyLoader._listaControlli[i]->getRichiestaDissipazione();
		r += ".";
		r += MyLoader._listaControlli[i]->getCharTendenza();
		r += ".";
		r += MyLoader._listaControlli[i]->getTemperaturaMinEsercizio();
		r += ".";
		r += MyLoader._listaControlli[i]->getIsAuto();
		r += ".";
		r += MyLoader._listaControlli[i]->getStatoValvola();
		r += ".";
		r += MyLoader._listaControlli[i]->getTemperatura();
		r += ".";
		r += MyLoader._listaControlli[i]->getMustBeOpen();
		// Chiude il controllo
		r += "XXX";
	}
	return r;
}
