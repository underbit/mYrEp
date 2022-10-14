#ifndef MDSYSCTRL_H
#define MDSYSCTRL_H

#include "BasicElements/sensoreOneWire.hh"
#include "MDLoader.hh"
#include <list>

class MDSysCtrl {
	
	public:
		MDSysCtrl();
		void AvviaServizio();
	private:
		MDLoader MyLoader;
		// Sensore della temperatura esterna. Opzionale
		sensoreOneWire * tempEsterna;
		// Sensore della temperatura del tampone. Necessaria.
		sensoreOneWire * tempImpianto;
		// Variabile che memorizza la richiesta di calore delle zone e/o acs
		bool _richiestaCalore;
		// Variabile che memorizza la richiesta di dissipazione delle sorgenti
		bool _richiestaDissipazione;
		// Variabile che determina se uno o più dispositivi sono in allarme
		bool _onAlarm;
		// La variabile serve ad interrompere il servizio
		bool _fermaServizio;
};

#endif
