#ifndef __SODAVENDING_H__
#define __SODAVENDING_H__

#include "sodaPrinter.h"
#include "sodaName.h"
#include "sodaCard.h"

// ========= Header =========================================================
_Task VendingMachine {
  Printer &prt;
  NameServer &nameServer;
  unsigned int id, sodaCost, maxStockPerFlavour, *sodas; // an array that holds 4 different sodas 
  void main();
 public:
  ~VendingMachine();
  enum Flavours { Sweet, Sour, Salty, Spicy };                 // flavours of soda (YOU DEFINE)
  _Event Funds {};                       // insufficient funds
  _Event Stock {};                       // out of stock for particular flavour
  VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour );
  void buy( Flavours flavour, WATCard &card );
  unsigned int *inventory();
  void restocked();
  _Nomutex unsigned int cost();
  _Nomutex unsigned int getId();
};

// ========= Implementation =============================================
// Constructor
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ): prt( prt ), nameServer( nameServer ), id( id ), sodaCost( sodaCost ), maxStockPerFlavour( maxStockPerFlavour ) {
  sodas = new unsigned int[4]; // initialize the soda array
}

// Destructor
VendingMachine::~ VendingMachine() {
  delete[] sodas; // delete the soda array
  prt.print( Printer::Kind::Vending, id, 'F' ); // vm finished 
} 

// buy
void VendingMachine::buy( Flavours flavour, WATCard &card ) {}

// inventory
unsigned int* VendingMachine::inventory() {
  prt.print( Printer::Kind::Vending, id, 'r' ); // start restocking 
  return sodas; 
}

// restocked
void VendingMachine::restocked() {
  prt.print( Printer::Kind::Vending, id, 'R' ); // finished restocking 
}

// cost 
unsigned int VendingMachine::cost() {
  return sodaCost; 
}

// getid 
unsigned int VendingMachine::getId() {
  return id;
}

// main 
void VendingMachine::main() {
  prt.print( Printer::Kind::Vending, id, 'S', sodaCost ); // vm started 
  nameServer.VMregister( this );
  for ( ;; ) {
    _Accept ( ~VendingMachine ) { break; }
    or _Accept ( inventory, restocked ) {} // accepts inventory and restocked 
  }
}
#endif 
