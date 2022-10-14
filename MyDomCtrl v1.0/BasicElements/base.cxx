#include "base.hh"

base::base(int id, string name, string position)
{
	_id = id;
	_nome = name;
	_posizione = position;
}

base::base(int id)
{
	_id = id;
}

void base::InizializzaBase(int id, string name, string position) {
	_id = id;
	_nome = name;
	_posizione = position;
}
//string base::getModel() const { return ""; };
