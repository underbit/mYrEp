
#include "MDLoader.hh"
#include <iostream>
#include "BasicElements/libfun.hh"
#include "BasicElements/arduino.hh"
#include "BasicElements/modulo.hh"
#include "BasicElements/base.hh"
#include "BasicElements/sensoreDig.hh"
#include "BasicElements/sensoreOneWire.hh"
#include "BasicElements/attuatoreDig.hh"
#include "BasicElements/orario.hh"
#include "BasicElements/controlObj.hh"
#include "sorgenteCalore.hh"
#include "termoZona.hh"

const char RIGA_COMMENTO = '?';
const char SEPARATORE_ELEMENTI = ':';
const char SEPARATORE_RIGHE = '\n';
const char MSG_NO_FILE[] = "[MDLoader] Problemi con l'apertura del file";
const char MSG_SAVED_ARD[] = "[MDLoader] Salvato arduino";
const char MSG_SAVED_SONDA[] = "[MDLoader] Salvato sensore";
const char MSG_SAVED_MOTORE[] = "[MDLoader] Salvato motore";
const char MSG_SAVED_PROGRAMMA[] = "[MDLoader] Salvato Programma";
const char MSG_SAVED_CONTROLLO[] = "[MDLoader] Salvato Controllo";
const char MSG_ARD_ERR[] = "[Loader] Arduino specificato inesistente";
const char MSG_ADD[] = "[Loader] Aggiunto ";
const char PATH_FILE_DEVICES[] = "./MDDevices.conf";
const char PATH_FILE_CONTROLS[] = "./MDControls.conf";
const char PATH_FILE_PROGRAMS[] = "./MDPrograms.conf";
const int NUM_MAX_ARD = 8;
const int NUM_MAX_ELE = 32;
using namespace std;

MDLoader::MDLoader() {
	_fileDevices = PATH_FILE_DEVICES;
	_filePrograms = PATH_FILE_PROGRAMS;
	_fileControls = PATH_FILE_CONTROLS;
	_num_sondeOW = 0;
	_num_motori = 0;
	_num_programmi = 0;
	_num_arduino = 0;
	_num_controlli = 0;
}

void MDLoader::CaricaImpostazioni(bool verb) {
	if(verb) cout << "[MDLoader] ***CARICAMENTO DEVICES***" << '\n';
	CaricaFile(_fileDevices);
	if(verb) cout << "[MDLoader] ***CARICAMENTO PROGRAMMI***" << '\n';
	CaricaFile(_filePrograms);
	if(verb) cout << "[MDLoader] ***CARICAMENTO CONTROLLI***" << '\n';
	CaricaFile(_fileControls);
	if(verb) cout << "[MDLoader] ***CARICAMENTO COMPLETATO***" << '\n';
}

// Salva le impostazioni nel file di configurazione. Restituisce false in caso di problemi
void MDLoader::SalvaImpostazioni(bool verb) {
	SalvaImpostazioniDevices(_fileDevices, verb);
	SalvaImpostazioniControlli(_fileControls, verb);
	SalvaImpostazioniProgrammi(_filePrograms, verb);
}
// Salva le impostazioni nel file di configurazione. Restituisce false in caso di problemi
void MDLoader::SalvaImpostazioniProgrammi(const char * path, bool verb) {
	ofstream _myfile(path);
	// Se ci sono problemi con l'apertura esce
	if(!_myfile) { 
	  if(verb) cout << MSG_NO_FILE << endl ;
	  return ;
	}
	// Salva su file la lista di programma
	for(int i = 0; i < _num_programmi; i++) {
		_myfile << _listaProgrammi[i]->CreaRigaImpostazioni(); 
		if(verb) cout << MSG_SAVED_PROGRAMMA << endl;
	}
	_myfile.close();
}
// Salva le impostazioni nel file di configurazione. Restituisce false in caso di problemi
void MDLoader::SalvaImpostazioniControlli(const char * path, bool verb) {
	ofstream _myfile(path);
	// Se ci sono problemi con l'apertura esce
	if(!_myfile) { 
	  if(verb) cout << MSG_NO_FILE << endl ;
	  return ;
	}
	// Salva su file la lista di arduino
	for(int i = 0; i < _num_controlli; i++) {
		_myfile << _listaControlli[i]->CreaRigaImpostazioni(); 
		if(verb) cout << MSG_SAVED_CONTROLLO << endl;
	}
	_myfile.close();
}
// Salva le impostazioni nel file di configurazione. Restituisce false in caso di problemi
void MDLoader::SalvaImpostazioniDevices(const char * path, bool verb) {
	ofstream _myfile(path);
	// Se ci sono problemi con l'apertura esce
	if(!_myfile) { 
	  if(verb) cout << MSG_NO_FILE << endl ;
	  return ;
	}
	// Salva su file la lista di arduino
	for(int i = 0; i < _num_arduino; i++) {
		_myfile << _listaArd[i]->CreaRigaImpostazioni(); 
		if(verb) cout << MSG_SAVED_ARD << endl;
	}
	// Salva su file la lista di sonde
	for(int i = 0; i < _num_sondeOW; i++) {
		_myfile << _listaSonde[i]->CreaRigaImpostazioni(); 
		if(verb) cout << MSG_SAVED_SONDA << endl;
	}
	
	// Salva su file la lista di motori
	for(int i = 0; i < _num_motori; i++) {
		_myfile << _listaMotori[i]->CreaRigaImpostazioni(); 
		if(verb) cout << MSG_SAVED_MOTORE << endl;
	}

	_myfile.close();
}

// Carica le impostazioni dei controlli dal file predefinito
void MDLoader::CaricaFile(const char * myfile) {
  string mystring = "";
  char ch;
  bool tipoElaborato = false, elabOK = true;
  tipoBase tp = TB_NO_BASE;
  list<string> listapar;
  // Apre il file
  ifstream f(myfile);
  if(!f) { 
	  cout << MSG_NO_FILE << endl ;
	  return ;
  }
  bool rigacommento = false;
  // Legge tutto il file
  while(!f.eof())
  {	
	f.get(ch);
	if(ch == SEPARATORE_RIGHE) {
		if(rigacommento) {
			cout << "[MDLoader] ?: " << '\n';
			// Resetta il commento
			rigacommento = false;
		}
		else {
			cout << "[MDLoader] R: ";
			// La fine riga prevede la rielaborazione del tipo
			tipoElaborato = false;
			// L'elaborazione della riga
			if(!listapar.empty() && tp > TB_BASE) {
				elabOK = ElaboraRiga(tp, listapar);
				if(!elabOK) cout << " **** Errore nella riga ****" << endl;
				elabOK = true;
			}
			// Resetta il tipo base
			tp = TB_NO_BASE;
			// Pulisce la lista
			listapar.clear();	
			// La rinizializzazione del buffer dei parametri
			mystring = "";			
		}
	}
	else
		if(!rigacommento) {
			// Costrutto necessario al recupero del tipo di oggetto
			if(ch == SEPARATORE_ELEMENTI) {
				if(!tipoElaborato) {
					// Carica il tipo di oggetto da elaborare
					tp = CaricaPrimoElemento(StringToInt(mystring));
					// Primo elemento recuperato
					tipoElaborato = true;
				} else listapar.push_back(mystring);
				//Inizializza il buffer della riga
				mystring = "";
			} else if(ch == RIGA_COMMENTO) rigacommento = true;
					else mystring += ch;
		}
  }
  cout << '\n';
  // Chiude il file
  f.close();
}
bool MDLoader::ElaboraRiga(tipoBase mytp, list<string> buff) {
	if(mytp < TB_CONTROLLO) return ElaboraRigaDevice(mytp, buff);
	else if(mytp == TB_CONTROLLO) return ElaboraRigaControllo(mytp, buff);
	else return ElaboraRigaProgramma(mytp, buff);
}
bool MDLoader::ElaboraRigaProgramma(tipoBase mytp, list<string> buff) {
	programma* myp = NULL;
	// Crea un programma
	if(mytp == TB_PROGRAMMA) {
		int r_id;
		string r_nome;
		if(!buff.empty()) {
			// Recupera l'id 
			r_id = StringToInt(buff.front());
			// Cancella il primo elemento
			buff.pop_front();
		}
		if(r_id < 0) return false;	
		if(!buff.empty()) {
			// Recupera il nome
			r_nome = buff.front();
			// Cancella il primo elemento
			buff.pop_front();
		}
		// Controlla i dati
		if(r_nome.length() < 3)	return false;
		// Crea il programma
		AggiungiProgramma(r_id, r_nome);
		return true;
	} else { // Crea una fascia
		temperaturaDaRaggiungere tr;
		int prog = 0;
		int giorno = 0;
		int ora= -1;
		int min= -1;
		float tmin= -100;
		float tmax= -100;
		// Recupera il programma
		if(!buff.empty()) {
			prog = StringToInt(buff.front());
			// Cancella il primo elemento
			buff.pop_front();
		}
		// Controlla e carica il programma
		myp = getProgramma(prog);
		if(myp == NULL) return false;
		// Recupera il giorno
		if(!buff.empty()) {
			giorno = StringToInt(buff.front());
			// Cancella il primo elemento
			buff.pop_front();
		}
		// Controlla la coerenza del giorno
		if(giorno < 0 || giorno > 7) return false;
		// Recupera l'ora di fine
		if(!buff.empty()) {
			ora = StringToInt(buff.front());
			// Cancella il primo elemento
			buff.pop_front();
		}
		// Controlla la coerenza dell'ora
		if(ora < 0 || ora > 23) return false;
		// Recupera i minuti
		if(!buff.empty()) {
			min = StringToInt(buff.front());
			// Cancella il primo elemento
			buff.pop_front();
		}
		// Controlla la coerenza dei minuti di fine
		if(min < 0 || min > 59) return false; 
		// Recupera la temperatura min		 
		if(!buff.empty()) {
			tmin = StringToFloat(buff.front());
			// Cancella il primo elemento
			buff.pop_front();
		}
		// Controlla la coerenza della temperatura min
		if(tmin < 10 || tmin > 35) return false;
		// Recupera la temperatura max
		if(!buff.empty()) {
			tmax = StringToFloat(buff.front());
			// Cancella il primo elemento
			buff.pop_front();
		}
		// Controlla la coerenza della temperatura max
		if(tmin < 10 || tmin > 35 || tmin >= tmax) return false;
		if(!buff.empty()) tr = CaricaTemperaturaRagg(StringToInt(buff.front()));
		orario myo (ora, min);
		// Crea la fascia se esiste l'orario
		return myp->creaFascia(giorno, myo, tmin, tmax, tr); 
	}
	
}
bool MDLoader::ElaboraRigaControllo(tipoBase mytp, list<string> buff) {

	int r_id = -1, idm = 0, mys = 0, appo2 = 0;
	string r_nome, r_posizione, appo;
	float f = 0, critica = 100, zonaMan=20;
	tipiControlli tc = NO_CONTROL;
	bool b, isAuto = false, bZona = false;
	if(!buff.empty()) {
		r_id = StringToInt(buff.front());
		// Cancella il primo elemento
		buff.pop_front();
	}
	if(r_id < 0) return false;
	// Recupera il nome
	if(!buff.empty()) {
		r_nome = buff.front();
		// Cancella il primo elemento
		buff.pop_front();
	}
	if(r_nome.length() < 3) return false;
	// Recupera la posizione
	if(!buff.empty()) {
		r_posizione = buff.front();
		// Cancella il primo elemento
		buff.pop_front();
	}
	if(r_posizione.length() < 3) return false;
	// Recupera l'id del motore
	if(!buff.empty()) {
		idm = StringToInt(buff.front());
		// Cancella il primo elemento
		buff.pop_front();
	}
	attuatoreDig* motore = getMotore(idm);
	// Temperatura d'esercizio
	if(!buff.empty()) {
		f = StringToFloat(buff.front());
		buff.pop_front();
	}
	// Azione valvola (automatica 1, manuale 0)
	if(!buff.empty()) {
		isAuto = StringToBool(buff.front());
		buff.pop_front();
	}
	// Recupera il tipo di controllo (sorgente o zona)
	if(!buff.empty()) {
		tc = CaricaTipoControllo(StringToInt(buff.front()));
		// Recupera il blocco successivo
		buff.pop_front();
	}
	// Recupera la prima s
	if(!buff.empty()) {
		appo = buff.front();
	}
	// Differenzia i due tipi di controlli
	switch(tc) {
		case(NO_CONTROL) : {
				return false;
			} break;
		case(SORGENTE): {
				buff.pop_front();
				if(!buff.empty()) {
					mys = StringToInt(buff.front());
					buff.pop_front();
				}
				sensoreOneWire* sow = getSonda(mys);
				
				if(!buff.empty()) {
					critica = StringToFloat(buff.front());
					buff.pop_front();
				}
				if(!buff.empty()) {
					b = StringToBool(buff.front());
					buff.pop_front();
				}
				// Può esistere solo una sorgente senza motore (ad aria) o senza termometro (caldaia)
				if(sow == NULL && motore == NULL) return false;
				AggiungiControllo(r_id, r_nome, r_posizione, motore, f, sow, critica, b, isAuto);
			} break;
		case(ZONA): {
			// Imposta l'array dei sensori 
			sensoreOneWire* sow[8];
			int nums = 0;
			// Crea la lista delle sonde 
			// fintanto che abbiamo una s o 8 sonde
			while(appo == "s" && nums < 8) {
				buff.pop_front();
				if(!buff.empty()) {
					mys = StringToInt(buff.front());
					buff.pop_front();
				}
				sow[nums] = getSonda(mys);
				if(sow[nums] != NULL) nums++;
				if(tc == SORGENTE) break;
				if(!buff.empty()) {
					appo = buff.front();
				}
			}
			if(nums==0) return false;
			// Codice del programma
			if(!buff.empty()) {
				appo2 = StringToInt(buff.front());
				buff.pop_front();
			}
			// Controllo di zona (auto da prog o manuale)
			if(!buff.empty()) {
				bZona = StringToBool(buff.front());
				buff.pop_front();
			}
			// Temperatura manuale di zona
			if(!buff.empty()) {
				zonaMan = StringToFloat(buff.front());
				buff.pop_front();
			}
			programma * prog = getProgramma(appo2);
			AggiungiControllo(r_id, r_nome, r_posizione, motore, f, sow, prog, nums, isAuto, bZona, zonaMan);
			} break;
	}
	return true;	
}
bool MDLoader::ElaboraRigaDevice(tipoBase mytp, list<string> buff) {
	int r_id = -1;
	string r_nome, r_posizione;
	// Recupera l'id 
	if(!buff.empty()) {
		r_id = StringToInt(buff.front());
		// Cancella il primo elemento
		buff.pop_front();
	}
	// COntrolla coerenza dell'id
	if(r_id < 0) return false;
	// Recupera il nome
	if(!buff.empty()) {
		r_nome = buff.front();
		// Cancella il primo elemento
		buff.pop_front();
	}
	// Controlla la coerenza della lughezza del nome
	if(r_nome.length() < 3) return false;
	// Recupera la posizione
	if(!buff.empty()) {
		r_posizione = buff.front();
		// Cancella il primo elemento
		buff.pop_front();
	}
	// Controlla la coerenza della lughezza della posizione
	if(r_posizione.length() < 3) return false;
	// Crea un oggetto base per memorizzare arduini e moduli
	if(mytp ==  TB_ARDUINO) {
		string r_myfile;
		bool r_verb = false;
		// Carica il file della seriale
		if(!buff.empty()) {
			r_myfile = buff.front();
			// Cancella il primo elemento
			buff.pop_front();
		}
		// Controlla la coerenza della lughezza del nome del device di arduino
		if(r_myfile.length() < 10) return false;
		// Recupera il valore di verbose
		if(!buff.empty()) r_verb = (StringToBool(buff.front()));
		// Aggiunge l'arduino nella lista
		return AggiungiArduino(r_id, r_nome, r_posizione, r_myfile, r_verb);
	} else {	
		// Se esistono arduino
		if(_num_arduino == 0) return false;
		int ard = 0,canale = 0;
		bool verb = false, log = false;
		tipiModuli myt = NO_MODULO;
		// Carica il codice di arduino
		if(!buff.empty()) {
			ard = StringToInt(buff.front());
			buff.pop_front();
		}
		// Cerca l'arduino specificato
		arduino* myard = getArduino(ard);
		// Se esiste l'arduino associato
		if(myard == NULL) return false;
		// Carica il tipo di modulo
		if(!buff.empty()) {
			myt = CaricaTipoModulo(StringToInt(buff.front()));
			buff.pop_front();			
		}
		// Carica il canale del modulo
		if(!buff.empty()) {
			canale = StringToInt(buff.front());
			buff.pop_front();
		}
		// Controlla la coerenza del canale arduino
		if(canale < 1 || canale > 13) return false; 
		// Carica il flag del verbose response
		if(!buff.empty()) {
			verb = StringToBool(buff.front());
			buff.pop_front();
		}
		// Carica il flag della risposta lunga ardshell
		if(!buff.empty()) {
			log = StringToBool(buff.front());
			buff.pop_front();
		}
		switch(myt) {
			case(NO_MODULO): {
				return false;
				} break;
			case(SONDA): 
				break;
			case(SONDAOW): {
				bool celsius = true;
				const char *indirizzo;
				// Carica il flag della risposta lunga ardshell
				if(!buff.empty()) {
					celsius = StringToBool(buff.front());
					buff.pop_front();
				}
				// Carica l'indirizzo del sensore OneWire
				if(!buff.empty()) indirizzo = (buff.front()).c_str();
				// Lo aggiunge alla lista delle sonde
				AggiungiSensore(r_id, r_nome, r_posizione, myard, canale, verb, log, celsius, indirizzo);
				} break;
			case(VALVOLA): {
				// Crea un nuovo motore (valvola, pompa, ..)
				// Lo aggiunge alla lista dei motori
				AggiungiMotore(r_id, r_nome, r_posizione, myard, canale, verb, log);
				} break;
		}
	}
	return true;
}

// Esegue una ricerca all'interno della lista di arduino e 
// restituisce il dispositivo con quel codice
arduino * MDLoader::getArduino(int code) {
	arduino *myard = NULL;
	for(int i = 0; i < _num_arduino; i++) {
		if(_listaArd[i]->getId() == code) {
			myard = _listaArd[i];
			break;
		}
	}
	return myard;
}

// Esegue una ricerca all'interno della lista di programmi e 
// restituisce il programma con quel codice
programma * MDLoader::getProgramma(int code) {
	programma* appo = NULL;
	
	for(int i = 0; i < _num_programmi; i++) {
		if(_listaProgrammi[i]->getId() == code) {
			appo = _listaProgrammi[i];
			break;
		}
	}
	return appo;
}

// Esegue una ricerca all'interno della lista di programmi e 
// restituisce il programma con quel codice
attuatoreDig* MDLoader::getMotore(int code) {
	attuatoreDig* appo = NULL;
	for(int i = 0; i < _num_motori; i++) {
		if(_listaMotori[i]->getId() == code) {
			appo = _listaMotori[i];
			break;
		}
	}
	return appo;
}

// Esegue una ricerca all'interno della lista di programmi e 
// restituisce il programma con quel codice
sensoreOneWire* MDLoader::getSonda(int code) {
	sensoreOneWire* appo = NULL;
	for(int i = 0; i < _num_sondeOW; i++) {
		//cout << endl << _listaSonde[i]->getIndirizzo() << "-" << _listaSonde[i]->getId() << endl;
		if(_listaSonde[i]->getId() == code) {
			appo = _listaSonde[i];
			break;
		}
	}
	return appo;
}

// Restituisce il tipo di oggetto da caricare
tipoBase MDLoader::CaricaPrimoElemento(int ele) {
	tipoBase appo = TB_NO_BASE;
	switch(ele) {
		case(1):
			appo = TB_BASE;
			break;
		case(2):
			appo = TB_ARDUINO;
			break;
		case(3):
			appo = TB_MODULO;
			break;
		case(4):
			appo = TB_CONTROLLO;
			break;
		case(5):
			appo = TB_PROGRAMMA;
			break;
		case(6):
			appo = TB_FASCIA;
			break;
	}
	return appo;
}

tipiControlli MDLoader::CaricaTipoControllo(int ele) {
	tipiControlli appo;
	switch(ele) {
		case(0):
			appo = NO_CONTROL;
			break;
		case(1):
			appo = SORGENTE;
			break;
		case(2):
			appo = ZONA;
			break;
		default:
			appo = NO_CONTROL;
			break;
	}
	return appo; 
}

tipiModuli MDLoader::CaricaTipoModulo(int ele) {
	tipiModuli appo;
	switch(ele) {
		case(0):
			appo = NO_MODULO;
			break;
		case(1):
			appo = SONDA;
			break;
		case(2):
			appo = SONDAOW;
			break;
		case(3):
			appo = VALVOLA;
			break;
		default:
			appo = NO_MODULO;
			break;
	}
	return appo; 
}

temperaturaDaRaggiungere MDLoader::CaricaTemperaturaRagg(int ele) {
	temperaturaDaRaggiungere appo;
	switch(ele) {
		case(0):
			appo = MINIMA;
			break;
		case(1):
			appo = MEDIA;
			break;
		case(2):
			appo = MASSIMA;
			break;
		default:
			appo = MINIMA;
			break;
	}
	return appo; 
}

// Aggiunge un arduino
bool MDLoader::AggiungiArduino(int r_id, string r_nome, string r_posizione, string r_myfile, bool r_verb) {
	
	// Controlla l'unicità dell'id
	for(int i = 0; i < _num_arduino; i++)
		if(_listaArd[i]->getId() == r_id) return false;
	// Aggiunge arduino
	if(_num_arduino < NUM_MAX_ARD) {
		_listaArd[_num_arduino] = new arduino(r_id, r_nome, r_posizione, r_myfile, r_verb);
		if(_listaArd[_num_arduino]->getInizializzazioneOK()) {
			_num_arduino++;
			cout << MSG_ADD << "Arduino (" 
				 << _listaArd[_num_arduino-1]->getId() << "-" 
				 << _listaArd[_num_arduino-1]->getNome() << "-" 
				 << _listaArd[_num_arduino-1]->getFilePorta() << ")" << endl;
			return true;
		} else return false;
	}
	return false;
}

// Aggiunge un sensore
void MDLoader::AggiungiSensore(int r_id, string r_nome, string r_posizione, arduino* myard, int canale, 
								 bool verb, bool log, bool celsius, const char* indirizzo) {
	// Controlla l'unicità dell'id
	
	for(int i = 0; i < _num_sondeOW; i++)
		if(_listaSonde[i]->getId() == r_id) return;
	
	// Aggiunge l'elemento
	if(_num_sondeOW < NUM_MAX_ELE) {
		_listaSonde[_num_sondeOW] = new sensoreOneWire(r_id, r_nome, r_posizione, myard, canale, verb, log, celsius, indirizzo);
		_num_sondeOW++;
		cout <<  MSG_ADD << "Sensore OW (" 
		     << _listaSonde[_num_sondeOW-1]->getId() << "-"
			 << _listaSonde[_num_sondeOW-1]->getNome() << "-ch" 
			 << _listaSonde[_num_sondeOW-1]->getCanale() << "-" 
			 << _listaSonde[_num_sondeOW-1]->getIndirizzo() << ")" << endl;
	}
}

// Aggiunge un motore
void MDLoader::AggiungiMotore(int r_id, string r_nome, string r_posizione, arduino* myard, int canale, 
								bool verb, bool log) {
	// Controlla l'unicità dell'id
	for(int i = 0; i < _num_motori; i++)
			if(_listaMotori[i]->getId() == r_id) return;
	// Aggiunge un motore
	if(_num_motori < NUM_MAX_ELE) {
		_listaMotori[_num_motori] = new attuatoreDig(r_id, r_nome, r_posizione, myard, canale, verb, log);
		_num_motori++;
		// Log del motore aggiunto
		cout <<  MSG_ADD << "Motore (" << _listaMotori[_num_motori-1]->getId() << "-"
			 << _listaMotori[_num_motori-1]->getNome() << "-ch" << _listaMotori[_num_motori-1]->getCanale() << ")" << endl;
	}
}

// Aggiunge un motore
void MDLoader::AggiungiControllo(int r_id, string r_nome, string r_posizione, attuatoreDig* motore, float f, sensoreOneWire* sow[], programma* prog, int nums, bool vauto, bool bzona, float tman) {
	// Controlla l'unicità dell'id
	for(int i = 0; i < _num_controlli; i++)
		if(_listaControlli[i]->getId() == r_id) return;
	// Aggiunge l'elemento	
	if(_num_controlli < NUM_MAX_ELE) {
		_listaControlli[_num_controlli] = new termoZona (r_id, r_nome, r_posizione, motore, f, sow, prog, nums, vauto, bzona, tman);
		_num_controlli++;
		cout << "Aggiunta Termozona (" << _listaControlli[_num_controlli-1]->getId()
			 << "-" << _listaControlli[_num_controlli-1]->getNome() << ")" << endl;
	}
}
void MDLoader::AggiungiControllo(int r_id, string r_nome, string r_posizione, attuatoreDig* motore, float f, sensoreOneWire* sow, float critica, bool b, bool vauto) {
	// Controlla l'unicità dell'id
	for(int i = 0; i < _num_controlli; i++)
		if(_listaControlli[i]->getId() == r_id) return;
	// Aggiunge l'elemento	
	if(_num_controlli < NUM_MAX_ELE) {
		_listaControlli[_num_controlli] = new sorgenteCalore(r_id, r_nome, r_posizione, motore, f, sow, critica, b, vauto);
		_num_controlli++;
		cout << "Aggiunta Sorgente (" << _listaControlli[_num_controlli-1]->getId()
			 << "-" << _listaControlli[_num_controlli-1]->getNome() << ")" << endl;
	}
}

// Aggiunge un programma
void MDLoader::AggiungiProgramma(int r_id, string r_nome) {
	
	// Controlla l'unicità dell'id
	for(int i = 0; i < _num_programmi; i++)
			if(_listaProgrammi[i]->getId() == r_id) return;
	// Aggiunge il programma
	_listaProgrammi[_num_programmi] = new programma(r_id, r_nome);
	_num_programmi++;
	cout << MSG_ADD << "Programma" << endl;
}
