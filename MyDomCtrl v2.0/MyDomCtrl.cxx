#include <pthread.h>
#include "MDSysCtrl.hh"
#include <iostream>
#include <cstring>      // Needed for memset
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions

const string WELCOME_STRING = "**** M Y  H O M E  S Y S T E M  C O N T R O L ****";
const int TEMP_IMPIANTO = 0;
const int PORTA_ASCOLTO= 5836;

void *ServizioRete(void *ptr) {
	// Puntatore al servizio di controllo
	MDSysCtrl *mySystem = (MDSysCtrl *)ptr;
	// Dichiara l'inizio del servizio
	int status;
	struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
	struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
	
	memset(&host_info, 0, sizeof host_info);

	host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
	host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.

	// Now fill up the linked list of host_info structs with google's address information.
	host_info.ai_flags = AI_PASSIVE; 
	status = getaddrinfo(NULL, "5555", &host_info, &host_info_list);
	// getaddrinfo returns 0 on succes, or some other value when an error occured.
	// (translated into human readable text by the gai_gai_strerror function).
	if (status != 0)  cout << "[MyNetServ] Errore nel recupero delle informazioni sul client. " << endl ;
    std::cout << "[MyNetServ] Creazione del socket in corso..."  << std::endl;
	int socketfd ; // The socket descripter
	socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (socketfd == -1)  std::cout << "[MyNetServ] Errore nel socket." << endl ;
    std::cout << "[MyNetServ] Binding del socket..."  << std::endl;
	int yes = 1;
	status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status == -1)  std::cout << "[MyNetServ] Errore nella creazione del bind." << std::endl ;
	std::cout << "[MyNetServ] In attesa di connessioni..."  << std::endl;
	status =  listen(socketfd, 5); // Numero massimo di connessioni
	if (status == -1)  std::cout << "[MyNetServ] Errore di connessione." << std::endl ;
	int new_sd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
	while (1) {
		new_sd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
		if (new_sd == -1) {
			cout << "[MyNetServ] Errore di connessione." << endl ;
			break;
		}
		else cout << "[MyNetServ] Connessione accettata." << std::endl;
		// Preparazione della ricezione
		ssize_t bytes_recieved;
		char incomming_data_buffer[256];
		while(1) {
			bytes_recieved = recv(new_sd, incomming_data_buffer, 256, 0);
			// If no data arrives, the program will just wait here until some data arrives.
			if (bytes_recieved == 0) std::cout << "[MyNetServ] Host shut down." << std::endl ;
			if (bytes_recieved == -1)std::cout << "[MyNetServ] Errore di ricezione." << std::endl ;
			string appo = ""; 
			for(int i = 0; i < bytes_recieved; i++)
				appo += incomming_data_buffer[i];
			// Elaborazione del messaggio
			const char *msg = mySystem->RicezioneNetMsg(appo).c_str();
			// Preparazione e invio della risposta
			int len;
			ssize_t bytes_sent;
			len = strlen(msg);
			bytes_sent = send(new_sd, msg, len, 0);
			cout << bytes_sent;
		}
	}

	// Fine del servizio e chiusura della connessione
	freeaddrinfo(host_info_list);
	close(new_sd);
	close(socketfd);        
	cout << "[MyDomCtrl] Chiusura della connessione" << endl << endl;
	return NULL;
}

// Funzione di gestione delle apparecchiature elettroniche (thread1)
void *ControlloDispositivi(void *ptr) {
	MDSysCtrl *mySystem = (MDSysCtrl *)ptr;
	bool appo, lastRic = false, lastDis = false;
	float temp;
	time_t currentTime;
	// Finchè qualcuno non blocca manualmente il servizio dall'esterno.
	mySystem->setIsStopped(false);
	if(mySystem->MyLoader._num_controlli == 0) 
		cout << "[MyDomCtrl] !!!!Nessun controllo da eseguire!!!" << endl;
	else {
		cout << "[MyDomCtrl] Servizio ControlloDispositivi Avviato (MultiThreading)" << endl;
		while(!mySystem->getIsStopped()) {
			time(&currentTime);
			struct tm *localTime = localtime(&currentTime);
			orario oraAttuale(localTime->tm_hour, localTime->tm_min);
			cout << "[MyDomCtrl] Ciclo h=" << oraAttuale.getOra() << " m=" << oraAttuale.getMinuti();
			lastRic = mySystem->getRichiestaCalore();
			lastDis = mySystem->getRichiestaDissipazione();
			if(lastRic) cout << " ###RICHIESTO CALORE### ";
			if(lastDis) cout << " ###RICHIESTA DISSIPAZIONE### ";
			cout << endl;
			mySystem->setRichiestaCalore(false);
			mySystem->setRichiestaDissipazione(false);
			// Legge la temperatura dell'impianto
			if(mySystem->tempImpianto != NULL) {
				mySystem->tempImpianto->LeggiValore();
				temp = mySystem->tempImpianto->getUltimaLettura();
				cout << "---[OPT1] Temperatura impianto: " << temp;
				if(mySystem->tempImpianto->getOnAlarm()) cout << " *ALLARME* ";
				cout << endl;
			} else temp = TEMP_IMPIANTO;
			// Legge la temperatura esterna
			if(mySystem->tempEsterna != NULL) {
				mySystem->tempEsterna->LeggiValore();
				temp = mySystem->tempEsterna->getUltimaLettura();
				cout << "---[OPT2] Temperatura Esterna: " << temp;
				if(mySystem->tempEsterna->getOnAlarm()) cout << " *ALLARME* ";
				cout << endl;
			}
			// Inizializza la lista dei controlli: zone e sorgenti.
			for (int i = 0; i < mySystem->MyLoader._num_controlli; i++) {
				controlObj* pp = mySystem->MyLoader._listaControlli[i];
				// Esegue la procedura di controllo delle zone e delle sorgenti
				cout << "---[" << pp->getNome() << "]";
				appo = pp->ControlloDispositivo(temp, mySystem->getRichiestaCalore() || lastRic, mySystem->getRichiestaDissipazione() || lastDis); 
				// Recupera le necessità dei controlli
				mySystem->setRichiestaCalore(pp->getRichiestaCalore() || mySystem->getRichiestaCalore());
				mySystem->setRichiestaDissipazione(mySystem->getRichiestaDissipazione() | pp->getRichiestaDissipazione());
				
				
				/*
				 * Spazio per l'utilizzo di codice per controllare il rapporto tra temperatura 
				 * esterna ed interna del sistema.			 * 
				 */
				
				
				// Verifica gli allarmi
				mySystem->setIsOnAlarm(mySystem->getIsOnAlarm() & appo);
				cout << endl;
			}
		}
	}
	cout << "[MyDomCtrl] Servizio Stoppato" << endl;
	return NULL;
}

int main()
{
	// Presenta il messaggio di apertura
	cout << endl << WELCOME_STRING << endl << endl;
	// Crea l'oggetto sistema
	MDSysCtrl MyCtrl;
	// Carica le impostazioni dai files
	MyCtrl.CaricaImpostazioni();
	// Crea iol thread del servizio
	pthread_t CoreThread, NetThread;
	// Attiva il thread del servizio
	int iret1 = pthread_create( &CoreThread, NULL, ControlloDispositivi, (void *) &MyCtrl);
	if(iret1) cout << "[MyDomCtrl] Creazione del thread Core fallita." << endl;
	else cout << endl << "[MyDomCtrl] Avvio del servizio ControlloDispositivi (MultiThreading)" << endl << endl;
	int iret2 = pthread_create( &NetThread, NULL, ServizioRete, (void *) &MyCtrl);
	if(iret2) cout << "[MyDomCtrl] Creazione del thread Net Management fallita." << endl;
	else cout << "[MyDomCtrl] Servizio NetManagement Avviato (MultiThreading)" << endl << endl;
	pthread_join(CoreThread, NULL);
	
	
}
