#ifndef __SODATRUCK_H__
#define __SODATRUCK_H__

#include "sodaPrinter.h"
#include "sodaName.h"
#include "sodaPlant.h"

// ========= header =======================================================
_Task Truck {
  Printer &prt;
  NameServer &nameServer;
  BottlingPlant &plant;
  unsigned int numVendingMachines, maxStockPerFlavour;
  void main();
 public:
  ~Truck();
  Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
	 unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

// ========= Implementation =============================================
// constructor
Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour ): prt( prt ), nameServer( nameServer ), plant( plant ), numVendingMachine( numVendingMachine ), maxStockPerFlavour( maxStockPerFlavour ) {
  prt.print( Printer::Kind::Truck, 'S' );
}

// Destructor
Truck::~Truck() {
  prt.print( Printer::Kind::Truck, 'F' );
}

// main
void Truck::main() {}

#emdif
