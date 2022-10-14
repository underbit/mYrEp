#include "orario.hh"

orario::orario() {
	_ore = 0;
	_minuti = 0;
}

orario::orario(int ore, int minuti) {
	if(ore >= 0 && ore < 24) _ore = ore;
	else _ore = 0;
	if(minuti >= 0 && minuti < 60) _minuti = minuti;
}

bool orario::operator >(const orario &uno) {
	if(_ore > uno.getOra() || 
	  (_ore == uno.getOra() && _minuti > uno.getMinuti())) return true;
	else return false;
}

orario &orario::operator=(const orario &uno) {
	_ore = uno.getOra();
	_minuti = uno.getMinuti();
	return *this;
}

orario &orario::operator+(const int uno) {
	int ore = 0;
	int minuti = uno;
	if(uno>59) {
		ore = minuti / 60;
		minuti = minuti % 60;		
	}
	_ore += ore;
	_minuti += minuti;
	return *this;
}
