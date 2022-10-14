#ifndef MDLOADER_H
#define MDLOADER_H

#include <string>
#include <fstream>
#include "BasicElements/base.hh"
#include "BasicElements/attuatoreDig.hh"
#include "BasicElements/arduino.hh"
#include "BasicElements/sensoreOneWire.hh"
#include "BasicElements/controlObj.hh"
#include "programma.hh"
#include <list>


using namespace std;

/* 
 * Il loader si occupa di caricare e salvare i dati relativi 
 * al sistema da e verso il file di default.
 * Tramite delle righe di testo formattate il sistema riesce a
 * risalire all'oggetto e a ricrearlo in memoria.
 * Il loader contiene anche tutti gli oggetti reali del sistema:
 * arduino, sonde e motori.
 */
 
class MDLoader {
	public:
		MDLoader();
		// Restituisce l'arduino con il codice specificato
		arduino * getArduino(int);
		programma * getProgramma(int);
		attuatoreDig * getMotore(int);
		sensoreOneWire * getSonda(int);
		// Carica le impostazioni dal file predefinito
		void CaricaImpostazioni(bool);
		// Salva le impostazioni nel file di configurazione.
		void SalvaImpostazioni(bool);
		// Salva le impostazioni nel file di configurazione.
		void SalvaImpostazioniDevices(const char *, bool);
		// Salva le impostazioni nel file di configurazione.
		void SalvaImpostazioniControlli(const char *, bool);
		// Salva le impostazioni nel file di configurazione.
		void SalvaImpostazioniProgrammi(const char *, bool);
		// Aggiunge un arduino
		bool AggiungiArduino(int , string , string , string , bool);
		// Aggiunge un sensore
		void AggiungiSensore(int , string , string , arduino* , int , bool , bool , bool , const char * );
		// Aggiunge un motore
		void AggiungiMotore(int , string , string , arduino * , int , bool , bool);
		// Aggiunge un programma
		void AggiungiProgramma(int , string);
		// Aggiunge un controllo
		void AggiungiControllo(int, string, string, attuatoreDig*, float, sensoreOneWire*[], programma*, int, bool, bool, float);	
		// Aggiunge un controllo
		void AggiungiControllo(int, string, string, attuatoreDig *, float, sensoreOneWire*, float, bool, bool);
		// Lista dei controlli
		controlObj* _listaControlli[24];
		int _num_controlli;
	private:
		// Il file che contiene le impostazioni del sistema
		const char* _fileDevices;
		const char* _fileControls;
		const char* _filePrograms;
		// Lista gli arduino
		arduino* _listaArd[5];
		// Lista le sonde
		sensoreOneWire* _listaSonde[24];
		// Lista i motori
		attuatoreDig* _listaMotori[24];
		// Lista dei programmi
		programma* _listaProgrammi[24];
		int _num_sondeOW;
		int _num_motori;
		int _num_programmi;
		int _num_arduino;
		// Elabora una riga del file di configurazione
		bool ElaboraRiga(tipoBase, list<string>);
		bool ElaboraRigaDevice(tipoBase, list<string>);
		bool ElaboraRigaProgramma(tipoBase, list<string>);
		bool ElaboraRigaControllo(tipoBase, list<string>);
		// Carica il primo elemento della riga
		tipoBase CaricaPrimoElemento(int);
		// Carica il tipo di modulo dal codice
		tipiModuli CaricaTipoModulo(int);
		tipiControlli CaricaTipoControllo(int);
		temperaturaDaRaggiungere CaricaTemperaturaRagg(int);
		void CaricaFile(const char *);
		
};

#endif
