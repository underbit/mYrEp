#ifndef _SORGENTECALORE_H
#define _SORGENTECALORE_H


#include "attuatoreDig.hh"
#include "sensoreOneWire.hh"
#include "base.hh"

//La sorgente di calore rappresenta un oggetto fisico che pu� produrre calore. La temperatura dell'acqua locale � rappresentata tramite il termometro e dopo che la temperatura di esercizio � stata raggiunta, la valvola si apre per far entrare in funzione il riscaldamento. La protezione della temperatura critica indica al sistema di aprire tutte le valvole zona per portar via il calore prodotto. Un esempio � un termocamino. La sorgente pu� essere automatica o meno. Se � automatica � possibile attivarla con l'operazione attiva.
class sorgenteCalore : public base {
  private:
    attuatoreDig _valvola;

    sensoreOneWire _termometro;


  public:
    tendenze getTendenza();


  private:
    float _letture;


  public:
    bool getStatoValvola();

    void Attiva();

    void Disattiva();

    sorgenteCalore();

    ~sorgenteCalore();

};
#endif
