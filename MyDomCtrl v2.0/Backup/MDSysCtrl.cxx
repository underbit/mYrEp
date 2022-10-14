#include "BasicElements/base.hh"
#include "BasicElements/arduino.hh"
#include "BasicElements/attuatoreDig.hh"
#include "BasicElements/sensoreOneWire.hh"
#include "BasicElements/libfun.hh"

int main()
{
	// Crea arduino 1
	base myb (1, "ARD1", "1 Piano");
	arduino *myard = new arduino(myb, false);
	// Crea l'attuatore digitale
	myb = base (2, "Risc1", "Camera da letto");
	attuatoreDig myatt (myb, myard, 13, false, false);
	// Crea il sensore di temperatura
	myb = base (3, "Temp1", "Camera da letto");
	 
	
	//myard->InviaPing();
	
	sensoreOneWire mysow (myb, myard, 2, false, false, true, "28 B9 BC 84 5 0 0 3");
	mysow.LeggiValore();
	cout << mysow.getUltimaLettura() << endl;
	
	// myard->RicercaDispositiviOneWire(2);
	//for(int i = 0; i<100; i++)	myatt.SetAttuatore(i % 2);

}
