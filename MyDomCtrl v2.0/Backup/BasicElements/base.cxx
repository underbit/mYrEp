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
