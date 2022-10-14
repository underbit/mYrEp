#ifndef _ARDUINO_H
#define _ARDUINO_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <iostream>
#include "base.hh"
#include <string>

using namespace std;

#define BAUDRATE B9600
#define _POSIX_SOURCE 1 				/* POSIX compliant source */
#define FALSE 0
#define TRUE 1

/*
 * Questa classe si occupa di comunicare con il dispositivo 
 * fisico arduino e di controllarne il flusso. 
 * Ingloba il protocollo di comunicazione ArdShell.
 * Tutta la logica per la comunicazione seriale è inserita 
 * in questa classe ed è trasparente all'utente.
 * Utilizza delle variabili per affinare il timeout della 
 * seriale e per ottenere il logging di arduino.
 * Utilizza la classe base per le informazioni di naming e posizione.
 */

class arduino : public base {
  public:
    // Crea da id, nome, posizione, fileporta. verbose
    arduino(int , string , string , string , bool );
    // Crea da id, nome, posizione, verbose. Viene assegnata la porta di def.
    arduino(int , string , string , bool );
    // Crea dalla classe base, il nome della porta e verbose
    arduino(base , string , bool );
    // Crea dalla classe base e verbose. Viene assegnata la porta di def.
    arduino(base , bool );
    arduino();
    // Quando l'oggetto viene distrutto, viene chiusa la seriale
    ~arduino();
    // Comando, tipo, canale, pwd, verbose, rispostalunga
    void InviaMessaggio(bool , char , int , int , bool, bool );
    // Comando, tipo, canale, verbose, rispostalunga
    void InviaMessaggio(bool , char , int , bool, bool );
    // OneWire : indirizzo, canale, opzioni, celsius, verbose, rispostalunga
    void InviaMessaggioOW(string , int , bool , bool , bool );
    // Invio messaggio generico
    void InviaMessaggio(string );
    // Effettua la ricerca dei dispositivi OneWire sul canale
    void RicercaDispositiviOneWire(int );
    // Invia un ping all'apparato
    void InviaPing();
    // Crea ed inizializza la porta seriale
    void InizializzaPortaSeriale();
    // La funzione carica i dati se ce ne sono e aggiorna la variabile
    // _comandoEseguito
    string LeggiRispostaAS();
    //Recuper il file della porta seriale
    inline string getFilePorta() const { return _fileporta; };
    // Se stampa i log è true
    inline bool getVerbose() const { return _verbose; };
    // Dopo aver ricevuto un messaggio di fine comando $%%$ è impostato a true
    inline bool getComandoEseguito() const { return _comandoEseguito; };
    // Recupera il delay impostato per l'inizializzazione della seriale
    inline int getDelayIni() const { return _delayIni; };
    // Recupera il delay impostato prima dell'esecuzione di un comando
    inline int getDelayCmd() const { return _delayCmd; };
    // Recupera l'attesa impostata prima del recupero di un carattere
    inline int getTimeOutRiga() const { return _toRiga; };
    // Recupera il numero di tentativi eseguiti per il recupero del carattere eol
    inline int getTentativiEOL() const { return _tentEOL; };
    // Recupera il numero di tentativi eseguiti per il recupero del fine comando
    inline int getTentativiEOC() const { return _tentEOC; };
    // Recupera lo stato dell'inizializzazione
    inline bool getInizializzazioneOK() const { return _inizializzazioneOK; };
    // Recupera il tipo di elemento
	tipoBase getTipoBase() const { return TB_ARDUINO; };
    // Imposta il delay per l'inizializzazione della seriale
    void setDelayIni(int );
    // Imposta il delay prima dell'esecuzione di un comando
    void setDelayCmd(int );
    // Imposta l'attesa prima del recupero di un carattere
    void setTimeOutRiga(int );
    // Imposta il numero di tentativi eseguiti per il recupero del carattere eol
    void setTentativiEOL(int );
    // Imposta il numero di tentativi eseguiti per il recupero dei carattere eoc $%%$
    void setTentativiEOC(int );
    // Chiude la comunicazione seriale con arduino
    void Chiudi();
    // Recupera il nome del modello
	virtual string getModel() const { return "Arduino"; };
	// Crea una stringa per le impostazioni
	string CreaRigaImpostazioni();

  private:
    // Invia fisicamente un messaggio ardshell ad arduino
    void InviaMessaggioArduino(string, bool );
    // Operazioni sulla seriale
    void ChiudiSer();
    // Chiude la seriale
    void ImpostaSer();
    // Imposta i parametri della connessione
    bool ApriSer(string , bool );
    // Apre la seriale
    string LeggiRigaSer(bool );
    // Legge una riga dalla seriale
    void InviaMessaggioSer(string , bool );
    // Porta seriale di arduino
	string _fileporta;
	// Se verbose vengono visualizzati i messaggi di debug
    bool _verbose;
    // Al termine di un comando/lettura controlla l'esistenza del $%%$
    bool _comandoEseguito;
    // Handle del file
    int fd;
    // Struttura per i parametri di com
    struct sigaction saio;
    struct termios oldtio;
    struct termios newtio;
    // Delay impostato prima dell'esecuzione di un comando
    int _delayCmd;
    // Delay per l'inizializzazione della seriale
    int _delayIni;
    // Attesa prima del recupero di un carattere
    int _toRiga;
    // Numero di tentativi eseguiti per il recupero del carattere eol
    int _tentEOL;
    // Numero di tentativi eseguiti per il recupero dei carattere eoc $%%$
    int _tentEOC;
    // Indica se l'arduino è stato inizializzato correttamente
    bool _inizializzazioneOK;
};
#endif
