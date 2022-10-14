#ifndef PROGRAMMA_H
#define PROGRAMMA_H

#include "BasicElements/orario.hh"
#include "BasicElements/base.hh"
#include <string>

using namespace std;

/* 
 * Il programma trasforma in codice le necessità settimanali 
 * di una utenza. Si possono impostare due orari 
 */

// Minima: almeno un termometro deve arrivare alla minima
// Media: tutti i termometri devono arrivare alla minima
// Massima: solo un termometro può arrivare alla massima
enum temperaturaDaRaggiungere { MINIMA, MEDIA, MASSIMA };

struct fasciaOraria {
	orario fine;
	float temperaturaMin;
	float temperaturaMax;
	temperaturaDaRaggiungere tr;
};

class programma {
	private:
		// Id univoco del programma
		int _id;
		// Nome mnemonico del programma
		string _nome;
		// le 8 fascie orarie della settimana
		fasciaOraria _fascieOrarie[7][8];
		// La prima fascia utile del giorno
		int _disponibile[7];
		
	public:
		// REstituisce l'id del programma
		int getId() const { return _id; };
		// Restituisce il nome del programma
		string getNome() const { return _nome; };
		// Recupera il tipo di elemento
		tipoBase getTipoBase() const { return TB_PROGRAMMA; };
		// Crea un programma
		programma(int, string);
		// Crea una fascia oraria
		bool creaFascia(int, orario, float, float, temperaturaDaRaggiungere);
		// Restituisce true se siamo nella fascia e la temperatura è compresa
		bool controllaFascia(int, orario, float, float);
		// Recupera il nome del modello
		virtual string getModel() const { return "Programma"; };
		// Crea una riga di impostazioni
		virtual string CreaRigaImpostazioni();
};

#endif
