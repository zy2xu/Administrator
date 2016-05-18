#ifndef __SODASTUDENT_H__
#define __SODASTUDENT_H__

#include "sodaPrinter.h"
#include "sodaName.h"
#include "sodaOffice.h"
#include "sodaGroup.h"
#include "sodaCard.h"
#include "sodaVending.h"

// ========= Header ============================================================================
_Task Student {
  // declare vars 
  Printer &prt;
  NameServer &nameServer;
  WATCardOffice &cardOffice;
  Groupoff &groupoff;
  VendingMachine *vm; // a vm pointer 
  unsigned int id, maxPurchases;
  WATCard::FWATCard wcard, gcard; // future card for watcard and giftcard 
  void main();
 public:
  ~Student();
  Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
	   unsigned int id, unsigned int maxPurchases );
};

// ======== Implmentation =================================================================
// constructo
Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,  unsigned int id, unsigned int maxPurchases ): prt( prt ), nameServer( nameServer ), cardOffice( cardOffice ), groupoff( groupoff ), id( id ), maxPurchases( maxPurchases ) {}

// destructor
Student::~Student() {
  prt.print( Printer::Kind::Student, id, 'F' ); // student finished 
}

// main
void Student::main() {
  unsigned int flavour = rand() % 3; // select a random flavour
  unsigned int purchase = rand() % maxPurchases + 1; // select random number to purchase
  prt.print( Printer::Kind::Student, id, 'S', flavour, purchase ); // student starting  
  wcard = cardOffice.create( id, 5 ); // get the watcard
  gcard = groupoff.giftCard(); // get giftcard 
  _Select( wcard ); // wait for watcard has money 
  _Select ( gcard ); // wait for giftcard has money 
  vm = nameServer.getMachine( id ); // select a vm from nameserver 
  prt.print( Printer::Kind::Student, id, 'V', vm->getId() );
  for ( ;; ) {
    unsigned int random = rand() % 10 + 1;
    yield( random );  // yield ramdomly before buy 
    break;
  }
  delete wcard; // delete the futre watcard 
  delete gcard; // delete future giftcard 
}

#endif 
