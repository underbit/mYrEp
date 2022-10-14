
#include "arduino.hh"
#include "libfun.hh"

const char MSG_APRI_SER[] = "Provo ad aprire la seriale ";
const char MSG_FALLITO[] = "[FALLITO]";
const char MSG_OK[] = "[OK]";
const char MSG_TO_RIGA[] = "TimeOut della Riga";
const char MSG_INI[] = "Inizializzazione conclusa";
const int DELAY_RIGA = 10000;
const int DELAY_NUOVO_CMD = 1000000;
const int DELAY_INI = 800000;

//Costanti necessarie ad ArdShell
const char FINE_RIGA = '\n';
const int TENTATIVI_EOC = 20;		// Quante volte prova a leggere la seriale
const int TENTATIVI_EOL = 50;		// Quante volte in una riga attende il carattere fine riga
const char END_MSG[] = "$%%$\n";
const char PORTA_DEFAULT[] = "/dev/ttyACM0";

int wait_flag = TRUE;		/* TRUE while no signal received */

void signal_handler_IO (int status){
	wait_flag = FALSE;
}

void arduino::setDelayIni(int val) {
	if(val > 1000) _delayIni = val;
}
void arduino::setDelayCmd(int val) {
	if(val > 1000) _delayCmd = val;
}
void arduino::setTimeOutRiga(int val) {
	if(val > 1000) _toRiga = val;
}
void arduino::setTentativiEOL(int val) {
	if(val > 1) _tentEOL = val;
}
void arduino::setTentativiEOC(int val) {
	if(val > 1) _tentEOC = val;
}

void arduino::ChiudiSer(){
	// imposta gli attributi della comunicazione
	tcsetattr(fd, TCSANOW, &oldtio);
	// Chiude l'handle
	close(fd);
}

void arduino::ImpostaSer(){
   /* install the signal handler before making the device asynchronous */
	saio.sa_handler = signal_handler_IO;
	//      saio.sa_mask = 0;
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO,&saio,NULL);
	/* allow the process to receive SIGIO */
	fcntl(fd, F_SETOWN, getpid());
	/* Make the file descriptor asynchronous (the manual page says only
	   O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
	fcntl(fd, F_SETFL, FASYNC);
	// Recupera i parametri attuali
	tcgetattr(fd,&oldtio); /* save current port settings */
	
	/* set new port settings for canonical input processing */
	newtio.c_cflag =  CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR | ICRNL;
	//newtio.c_oflag &=  ~OPOST;
	newtio.c_lflag |= (ICANON | ECHO | ECHOE | ISIG);
	newtio.c_cc[VMIN]=1;
	newtio.c_cc[VTIME]=0;
	cfsetispeed(&newtio, BAUDRATE);
	cfsetospeed(&newtio, BAUDRATE);
	// Aggiorna la seriale
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);
}


// La funzione apre la seriale abbinata alla porta specificata
bool arduino::ApriSer(string mynome, bool verbose){
	char *nome = const_cast<char*>(mynome.c_str());
	if(verbose) cout << "[" << getNome() << "]" << MSG_APRI_SER << nome << " : ";
	// Inizializza l'handle della serial
	fd = open(nome, O_RDWR | O_NOCTTY | O_NONBLOCK);
	// Se riceviamo un errore, Mandiamo un messaggio
	if (fd < 0) {
		if(verbose) cout << MSG_FALLITO << endl;
		return false;
	} else {
		ImpostaSer();
		if(verbose) cout << MSG_OK << endl;
		return true;
	}
}

string arduino::LeggiRigaSer(bool stampa){
	// la seriale non è aperta restituisce una stringa vuota
	if(fd < 0) return "";
	// Inizializza un buffer stringa
	string buff;
	// Inizializza il carattere letto dalla seriale
	unsigned char tmp = '-';
	int to = 0;
	do {
		usleep(_toRiga);
		to++;
		// Se la seriale è pronta
		if (wait_flag == FALSE) {
			// Legge un carettere dalla seriale
			read(fd, &tmp, 1);
			// Lo appende al buffer
			buff += tmp;
		}
	} while (tmp != FINE_RIGA && to < _tentEOL);
	if(stampa && buff!="\n") {
		cout << "---[" << getNome() << "]";
		if(to == _tentEOL) 
			cout << MSG_TO_RIGA << endl; 
		else cout << buff;
	}
	return buff;
}

void arduino::InviaMessaggioSer(string mymsg, bool verbose) {
	// la seriale non è aperta restituisce una stringa vuota
	if(fd < 0) return;
	// Trasforma una stringa in un array di caratteri
	usleep(_delayCmd);
	mymsg += '\n';
	char *msg = const_cast<char*>(mymsg.c_str());
	if(verbose) cout << "---" << msg << "---";
	int appo = mymsg.length();
	// Invia l'array di caratteri
	write(fd, msg, appo);
}

arduino::arduino(int id, string nome, string posizione, bool verb)
	:base(id, nome, posizione){
		_verbose = verb;
		ApriPortaSeriale();
}

arduino::arduino(int id, string nome, string posizione, string fileseriale, bool verb)
	:base(id, nome, posizione){
		_fileporta = fileseriale;
		_verbose = verb;
		ApriPortaSeriale();
}

arduino::arduino(base myb, string fileseriale, bool verb)
	:base(myb.getId(), myb.getNome(), myb.getPosizione()){
		_fileporta = fileseriale;
		_verbose = verb;
		ApriPortaSeriale();
}

arduino::arduino(base myb, bool verb)
	:base(myb.getId(), myb.getNome(), myb.getPosizione()){
		_verbose = verb;
		ApriPortaSeriale();
}

arduino::~arduino() {
	ChiudiSer();
}

void arduino::ApriPortaSeriale(){
	_comandoEseguito = false;
	// Inizializza i delay
	_delayIni = DELAY_INI;
	_delayCmd = DELAY_NUOVO_CMD;
	_toRiga = DELAY_RIGA;
	_tentEOL = TENTATIVI_EOL;
	_tentEOC = TENTATIVI_EOC;
	if(_fileporta.empty()) _fileporta = PORTA_DEFAULT;
	if(ApriSer(_fileporta, _verbose)) {
		// Attende l'inizializzazione
		usleep (_delayIni);
		if(_verbose) cout << "[" << getNome() << "]" << MSG_INI << endl;
	}
}

string arduino::LeggiRispostaAS(){
	string readMsg;
	int to = 0;
	// Legge un comando ArdShell dalla seriale
	// Termina con END_MSG e lo stampa
	string line = "";
	do {
		readMsg += line;
		line = LeggiRigaSer(_verbose);
	} while(line != END_MSG && to++ < _tentEOC);
	_comandoEseguito = (line == END_MSG);
	return readMsg;
}

// Ricerca i dispositivi OneWire sul canale e li visualizza nei log
void arduino::RicercaDispositiviOneWire(int canale) {
	string comando = "rovs" + IntToString(canale);
	InviaMessaggioArduino(comando, true);
	LeggiRispostaAS();
}

void arduino::InviaMessaggioArduino(string mymsg, bool verb) {
	_comandoEseguito = false;
	InviaMessaggioSer(mymsg, verb);
}

void arduino::InviaMessaggioOW(const char *indirizzo, int canale, bool celsius, bool verbose, bool ris) {
	string comando = "";
	// Imposta il comando
	comando += 'r';
	// Imposta il tipo di comando
	comando += 'o';
	// Imposta verbose
	if(ris) comando += 'v';
	else comando += '0';
	// Inserisce l'argomento
	comando += celsius ? 'c' : 'f';
	// Imposta il canale sul comando
	if (canale > 13) canale = 13;
	comando += IntToString(canale);
	// Inserisce uno spazio
	comando += ' ';
	// Inserisce l'indirizzo
	for(unsigned int i = 0; i < 22; i++)
		comando += *(indirizzo+i);
	InviaMessaggioArduino(comando, verbose);
}

void arduino::InviaMessaggio(bool operazione, char tipo, int canale, int pwm, bool verbose, bool rislunga) {
	string comando;
	// Imposta il comando
	if(operazione) comando += 'w';
	else comando += 'r';
	// Imposta il tipo di comando
	comando += tipo;
	// Imposta verbose
	if(rislunga) comando += 'v';
	else comando += '0';
	// Inserisce l'argomento
	comando += '0';
	
	// Imposta il canale sul comando
	if (canale > 13) canale = 13;
	comando += IntToString(canale);

	// Inserisce uno spazio
	comando += ' ';
	// Inserisce il pwm
	if(pwm > 1023) pwm = 1023;
	comando += IntToString(pwm);
	InviaMessaggioArduino(comando, verbose);
}

void arduino::InviaMessaggio(bool operazione, char tipo, int canale, bool verbose, bool rislunga) {
	
	string comando;
	// Imposta il comando
	if(operazione) comando += 'w';
	else comando += 'r';
	// Imposta il tipo di comando
	comando += tipo;
	// Imposta verbose
	if(rislunga) comando += 'v';
	else comando += '0';
	// Inserisce l'argomento
	comando += '0';
	
	// Imposta il canale sul comando
	if (canale > 13) canale = 13;
	comando += IntToString(canale);

	InviaMessaggioArduino(comando, verbose);
}

void arduino::InviaPing()  {
	InviaMessaggioArduino("00", false);
	LeggiRispostaAS();
}

void arduino::InviaMessaggio(string mia)  {
	InviaMessaggioArduino(mia, true);
}
