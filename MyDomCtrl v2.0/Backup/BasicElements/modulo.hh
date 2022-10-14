/*
	La classe rappresenta il genitore di tutti i moduli che possiamo
	collegare ad arduino. Quindi un requisito essenziale è la presenza
	di un oggetto arduino e del canale al quale è collegato. Inoltre
	bisogna rappresentare le informazioni di posizione e identità tramite
	la classe "base". 
	La classe gestisce i log e l'abbinamento con arduino.
*/

#ifndef MODULO_H
#define MODULO_H

#include "base.hh"
#include "arduino.hh"
#include "libfun.hh"

class modulo : public base {
	public:
		// La classe necessita delle informazioni base, un arduino, il canale, log, risposta lunga
		modulo(int , string , string , arduino *, int, bool, bool);
		// La classe necessita di una base, un arduino, il canale, log, risposta lunga
		modulo(base , arduino *, int, bool, bool);
		// Il tipo può essere: m=base, s=sonda, v=valvola
		virtual tipiModuli tipo() const { return BASE; }; 	
		// Il modo può essere 0 lettura, 1 scrittura 
		virtual modi modo() const { return LETTURA; }; 
		// Recupera il canale di arduino utilizzato
		int getCanale() const { return _canale; };	
		// Recupera lo stato del log
		bool getVerbose() const { return _verbose; };	
		// Recupera lo stato della risposta lungo ArdShell
		bool getLogRisposte() const { return _logRisposte; };
		// const arduino* getArduino() const { return _arduino; };	
		// Imposta il log
		void setVerbose(bool);		
		// Imposta la risposta lunga ArdShell
		void setLogRisposte(bool);
	protected:	
		// Riferimento all'arduino utilizzato
		arduino *_arduino;
		// Canale utilizzato su arduino	
		int _canale;
		// Imposta il log sulle operazioni di arduino
		bool _verbose;
		// Imposta il log di ArdShell
		bool _logRisposte;
		
};

#endif
