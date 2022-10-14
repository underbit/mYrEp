MyDomCtrl: MyDomCtrl.o MDSysCtrl.o base.o arduino.o modulo.o attuatoreDig.o sensoreOneWire.o libfun.o sensoreDig.o MDLoader.o termoZona.o sorgenteCalore.o programma.o controlObj.o orario.o NetMessage.o
	g++ -o MyDomCtrl MyDomCtrl.o MDSysCtrl.o base.o arduino.o modulo.o attuatoreDig.o sensoreOneWire.o sensoreDig.o libfun.o MDLoader.o termoZona.o sorgenteCalore.o programma.o controlObj.o orario.o NetMessage.o -lpthread

MyDomCtrl.o: MyDomCtrl.cxx MDSysCtrl.o
	g++ -c MyDomCtrl.cxx 

MDSysCtrl.o: MDSysCtrl.hh base.o sensoreOneWire.o libfun.o MDLoader.o termoZona.o sorgenteCalore.o programma.o controlObj.o orario.o NetMessage.o
	g++ -c MDSysCtrl.cxx

MDLoader.o: MDLoader.cxx MDLoader.hh base.o sensoreDig.o sensoreOneWire.o attuatoreDig.o modulo.o libfun.o arduino.o ./BasicElements/modulo.hh termoZona.o sorgenteCalore.o controlObj.o 
	g++ -c MDLoader.cxx

termoZona.o: termoZona.hh termoZona.cxx controlObj.o sensoreOneWire.o programma.o 
	g++ -c termoZona.cxx

sorgenteCalore.o: sorgenteCalore.cxx sorgenteCalore.hh controlObj.o sensoreOneWire.o
	g++ -c sorgenteCalore.cxx

programma.o: programma.hh programma.cxx orario.o
	g++ -c programma.cxx

controlObj.o: ./BasicElements/controlObj.cxx ./BasicElements/controlObj.hh attuatoreDig.o
	g++ -c ./BasicElements/controlObj.cxx

sensoreDig.o: ./BasicElements/sensoreDig.cxx ./BasicElements/sensoreDig.hh modulo.o libfun.o
	g++ -c ./BasicElements/sensoreDig.cxx

sensoreOneWire.o: ./BasicElements/sensoreOneWire.cxx ./BasicElements/sensoreOneWire.hh modulo.o libfun.o
	g++ -c ./BasicElements/sensoreOneWire.cxx

attuatoreDig.o: ./BasicElements/attuatoreDig.cxx ./BasicElements/attuatoreDig.hh modulo.o 
	g++ -c ./BasicElements/attuatoreDig.cxx

modulo.o: ./BasicElements/modulo.hh ./BasicElements/modulo.cxx arduino.o libfun.o base.o
	g++ -c ./BasicElements/modulo.cxx

arduino.o : ./BasicElements/arduino.hh ./BasicElements/arduino.cxx base.o libfun.o
	g++ -c ./BasicElements/arduino.cxx

orario.o : ./BasicElements/orario.hh ./BasicElements/orario.cxx
	g++ -c ./BasicElements/orario.cxx

NetMessage.o : ./BasicElements/NetMessage.hh ./BasicElements/NetMessage.cxx
	g++ -c ./BasicElements/NetMessage.cxx

libfun.o : ./BasicElements/libfun.hh ./BasicElements/libfun.cxx
	g++ -c ./BasicElements/libfun.cxx

base.o : ./BasicElements/base.hh ./BasicElements/base.cxx
	g++ -c ./BasicElements/base.cxx
