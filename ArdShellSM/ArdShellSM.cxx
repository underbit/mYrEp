#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <iostream>
#include <string>

using namespace std;

#define BAUDRATE B9600
#define _POSIX_SOURCE 1 				/* POSIX compliant source */
#define FALSE 0
#define TRUE 1

int fd;
struct termios oldtio,newtio;
struct sigaction saio;           		/* definition of signal action */
void signal_handler_IO (int status);   /* definition of signal handler */
int wait_flag=TRUE;                    	/* TRUE while no signal received */

const char FINE_RIGA = '\n';
const int CICLI_TIME_OUT = 10;
const char END_MSG[] = "$%%$";
const char PORTA_DEFAULT[] = "/dev/ttyACM0";

void signal_handler_IO (int status){
	wait_flag = FALSE;
}

void Chiudi(){
	// imposta gli attributi della comunicazione
	tcsetattr(fd, TCSANOW, &oldtio);
	// Chiude l'handle
	close(fd);
}

void Imposta(){
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
bool Apri(char *nome, bool verbose){
	if(verbose) cout << "Provo ad aprire la seriale " << nome << " : ";
	// Inizializza l'handle della serial
	fd = open(nome, O_RDWR | O_NOCTTY | O_NONBLOCK);
	// Se riceviamo un errore, Mandiamo un messaggio
	if (fd < 0) {
		if(verbose) cout << "[FALLITO]" << endl;
		return false;
	} else {
		Imposta();
		if(verbose) cout << "[OK]" << endl;
		return true;
	}
}

// La funzione apre la seriale abbinata alla porta specificata
bool Apri(bool verbose){
	if(verbose) cout << "Provo ad aprire la seriale : " << PORTA_DEFAULT << " : " ;
	// Inizializza l'handle della serial
	fd = open(PORTA_DEFAULT, O_RDWR | O_NOCTTY | O_NONBLOCK);
	// Se riceviamo un errore, Mandiamo un messaggio
	if (fd < 0) {
		if(verbose) cout << "[FALLITO]" << endl;
		return false;
	} else {
		Imposta();
		if(verbose) cout << "[OK]" << endl;
		return true;
	}
}


string LeggiRiga(bool stampa){
	// la seriale non è aperta restituisce una stringa vuota
	if(fd < 0) return "";
	// Inizializza un buffer stringa
	string buff;
	// Inizializza il carattere letto dalla seriale
	unsigned char tmp = '-';
	int to = 0;
	while(to++ < CICLI_TIME_OUT){
		usleep(10000);
		// Se la seriale è pronta
		if (wait_flag == FALSE) {
			do {
				// Legge un carettere dalla seriale
				read(fd, &tmp, 1);
				// Lo appende al buffer
				buff += tmp;
			} while (tmp != FINE_RIGA);
		}
	}
	if(stampa) cout << buff << endl;
	return buff;
}

string LeggiRispostaHS(bool stampa){
	// la seriale non è aperta restituisce una stringa vuota
	if(fd < 0) return "";
	string readMsg = "";
	int to = 0;
	// Legge un comando ArdShell dalla seriale
	// Termina con END_MSG e lo stampa
	while(readMsg != END_MSG && to++ < CICLI_TIME_OUT) {
		readMsg = LeggiRiga(stampa);
	}
	return readMsg;
}

void InviaMessaggio(string mymsg, bool verbose) {
	// la seriale non è aperta restituisce una stringa vuota
	if(fd < 0) return;
	// Trasforma una stringa in un array di caratteri
	char msg[45];
	for(unsigned int i = 0; i < mymsg.length(); i++) msg[i] = mymsg[i];
	// Invia l'array di caratteri
	write(fd, msg, sizeof(msg));
	if(verbose) cout << "Messaggio inviato: " << msg << endl;
}

void InviaMessaggio(char *mymsg, int lenmymsg, bool verbose) {
	// la seriale non è aperta restituisce una stringa vuota
	if(fd < 0) return;
	// Invia l'array di caratteri
	write(fd, mymsg, lenmymsg);
	if(verbose) cout << "Messaggio inviato: " << mymsg << endl;
}

int main(){
	
	
	// Inizializza il layout
	cout << endl << endl;
	
	// Apre la seriale di default
	if(Apri(true)) {	
		char msg[45];
		do {
			// Richiede il messaggio
			cout << "Messaggio da inviare: ";
			cin.getline(msg, sizeof(msg));
			cout << "Messaggio inviato: " << msg << endl;
			InviaMessaggio(msg, 45);
			LeggiRispostaHS(true);
		} while (msg[0] != 'q');	
		// Chiude la seriale 
		Chiudi();
	}
	
	// Finisce il layout dell'app
	cout << endl << endl;
	
	// Chiude l'applicazione
	return 0;
}
