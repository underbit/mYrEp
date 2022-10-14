#include "MDSysCtrl.hh"
#include "MDLoader.hh"
#include "BasicElements/arduino.hh"
#include "BasicElements/orario.hh"
#include "BasicElements/sensoreDig.hh"
#include "BasicElements/sensoreOneWire.hh"
#include "BasicElements/attuatoreDig.hh"
#include "BasicElements/base.hh"
#include "BasicElements/controlObj.hh"
#include <list>
#include <ctime>

const string WELCOME_STRING = "**** M Y  H O M E  S Y S T E M  C O N T R O L ****";
const int TEMP_IMPIANTO = 0;

MDSysCtrl::MDSysCtrl() {
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
	// Inizializza le variabili di comunicazione con i controlli
	_richiestaCalore = false;
	_richiestaDissipazione = false;
	_onAlarm = false;
}

void MDSysCtrl::AvviaServizio() {
	bool appo, lastRic = false, lastDis = false;
	float temp;
	time_t currentTime;
	_fermaServizio = false;
	cout << endl << "[MSSysCtrl] Avvio del servizio" << endl << endl;
	// Finchè qualcuno non blocca manualmente il servizio dall'esterno.
	if(MyLoader._num_controlli == 0) {
		cout << "[MSSysCtrl] !!!!Nessun controllo da eseguire!!!" << endl;
		return;
	}
	while(!_fermaServizio) {
		time(&currentTime);
		struct tm *localTime = localtime(&currentTime);
		orario oraAttuale(localTime->tm_hour, localTime->tm_min);
		cout << "[MSSysCtrl] Ciclo h=" << oraAttuale.getOra() << " m=" << oraAttuale.getMinuti();
		lastRic = _richiestaCalore;
		lastDis = _richiestaDissipazione;
		if(lastRic) cout << " ###RICHIESTO CALORE### ";
		if(lastDis) cout << " ###RICHIESTA DISSIPAZIONE### ";
		cout << endl;
		_richiestaCalore = false;
		_richiestaDissipazione = false;
		// Legge la temperatura dell'impianto
		if(tempImpianto != NULL) {
			tempImpianto->LeggiValore();
			temp = tempImpianto->getUltimaLettura();
			cout << "---[OPT1] Temperatura impianto: " << temp;
			if(tempImpianto->getOnAlarm()) cout << " *ALLARME* ";
			cout << endl;
		} else temp = TEMP_IMPIANTO;
		// Legge la temperatura esterna
		if(tempEsterna != NULL) {
			tempEsterna->LeggiValore();
			temp = tempEsterna->getUltimaLettura();
			cout << "---[OPT2] Temperatura Esterna: " << temp;
			if(tempEsterna->getOnAlarm()) cout << " *ALLARME* ";
			cout << endl;
		}
		// Inizializza la lista dei controlli: zone e sorgenti.
		for (int i = 0; i < MyLoader._num_controlli; i++) {
			controlObj* pp = MyLoader._listaControlli[i];
			// Esegue la procedura di controllo delle zone e delle sorgenti
			cout << "---[" << pp->getNome() << "]";
			appo = pp->ControlloDispositivo(temp, _richiestaCalore || lastRic, _richiestaDissipazione || lastDis); 
			// Recupera le necessità dei controlli
			_richiestaCalore |= pp->getRichiestaCalore();
			_richiestaDissipazione |= pp->getRichiestaDissipazione();
			
			
			/*
			 * Spazio per l'utilizzo di codice per controllare il rapporto tra temperatura 
			 * esterna ed interna del sistema.			 * 
			 */
			
			
			// Verifica gli allarmi
			_onAlarm &= appo;
			cout << endl;
		}
	}
}

int main()
{
	// Presenta il messaggio di apertura
	cout << endl << WELCOME_STRING << endl << endl;
	// Crea l'oggetto della classe principale
	MDSysCtrl MyCtrl;
	// Avvia il servizio
	MyCtrl.AvviaServizio();
	
}
