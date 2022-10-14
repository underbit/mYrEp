#ifndef TERMOZONA_H
#define TERMOZONA_H

#include "BasicElements/attuatoreDig.hh"
#include "BasicElements/sensoreOneWire.hh"
#include "BasicElements/controlObj.hh"
#include "programma.hh"

/* 
 * La termozona è la rappresentazione di un'area che condivide un
 * regolatore di temperatura comandato da un programma e da 8 sensori
 * di temperatura.
 */
 
class termoZona : public controlObj {
	private:
		// Termometri collegati alla termozona
		sensoreOneWire* _termometri[8];
		// Termometri a disposizione
		int _num_termometri;
		// Programma abbinato alla zona
		programma* _programma;
		// Aperta o chiusa manuale
		bool _zonaAuto;
		// Temperatura da utilizzare nel caso manuale
		float _temperaturaManuale;
		
	public:
		termoZona(int, string, string, attuatoreDig*, float, sensoreOneWire*[], programma*, int, bool, bool, float);
		// Cede il controllo alla termozona per eseguire le sue operazioni con vaso tampone
		virtual bool ControlloDispositivo(float, bool, bool);
		// Restituisce la temperature del termometro indicato
		float getTemperaturaTermometro(int);
		// Restituisce la tendenza
		virtual tendenze getTendenza();
		// Recupera il nome del modello
		virtual string getModel() const { return "TermoZona"; };
		// Crea una riga di impostazioni
		virtual string CreaRigaImpostazioni();
		// Attivazione Programma
		void setZonaAuto(programma*);
		// Attivazione manuale alla temperatura
		void setZonaManual(float);
		void setZonaManual();		
		// Procedura per la gestione dei messaggi di rete
		virtual string NetComputing(int, int);
};

#endif
