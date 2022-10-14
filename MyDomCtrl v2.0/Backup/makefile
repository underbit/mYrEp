MDSysCtrl: MDSysCtrl.o base.o arduino.o modulo.o attuatoreDig.o sensoreOneWire.o libfun.o
	g++ -o MDSysCtrl MDSysCtrl.o base.o arduino.o modulo.o attuatoreDig.o sensoreOneWire.o sensoreDig.o libfun.o

MDSysCtrl.o: MDSysCtrl.cxx modulo.o
	g++ -c MDSysCtrl.cxx 

sensoreDig.o: ./BasicElements/sensoreDig.cxx ./BasicElements/sensoreDig.hh modulo.o libfun.o
	g++ -c ./BasicElements/sensoreDig.cxx


sensoreOneWire.o: ./BasicElements/sensoreOneWire.cxx ./BasicElements/sensoreOneWire.hh modulo.o libfun.o
	g++ -c ./BasicElements/sensoreOneWire.cxx

attuatoreDig.o: ./BasicElements/attuatoreDig.cxx ./BasicElements/attuatoreDig.hh modulo.o 
	g++ -c ./BasicElements/attuatoreDig.cxx

modulo.o: ./BasicElements/modulo.hh ./BasicElements/modulo.cxx arduino.o
	g++ -c ./BasicElements/modulo.cxx

arduino.o : ./BasicElements/arduino.hh ./BasicElements/arduino.cxx base.o libfun.o
	g++ -c ./BasicElements/arduino.cxx

libfun.o : ./BasicElements/libfun.hh ./BasicElements/libfun.cxx
	g++ -c ./BasicElements/libfun.cxx

base.o : ./BasicElements/base.hh ./BasicElements/base.cxx
	g++ -c ./BasicElements/base.cxx
