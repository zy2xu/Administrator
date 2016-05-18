#ifndef __SODAPLANT_H__
#define __SODAPLANT_H__

#include "sodaPrinter.h"
#include "sodaName.h"
#include <vector>

// ======== Plant Header ======================================================
_Task BottlingPlant {
  Printer &prt;
  NameServer &nameServer;
  bool close = false; // flag to shut down 
  uCondition lock; // lock for sync 
  // array of sodas produced   
  unsigned int numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments, sodaProduced = 0, *sodas, vmid;
  void main();
 public:
  ~BottlingPlant();
  _Event Shutdown {
  public:
    bool &close;
  Shutdown( bool &close ): close( close ){}
  };                    // shutdown plant
  BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
		 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
		 unsigned int timeBetweenShipments );
  void getShipment( unsigned int cargo[] );
};

// ========= Truck Header ========================================================
_Task Truck {
  Printer &prt;
  NameServer &nameServer;
  BottlingPlant &plant;
  VendingMachine **vs; // an array of vm pointers 
  unsigned int numVendingMachines, maxStockPerFlavour, *cargo, *sodas, currentvm = 0; // cargo array and sodas array to hold inventory 
  void main();
 public:
  ~Truck();
  Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
         unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

// ========= Helpers ====================================================
// returns true if the cargo is empty
bool cargo_empty( unsigned int cargo[] ) {
  for ( int i = 0; i < 4; i++ ) {
    if ( cargo[i] <= 0 ) {
      return false;
    }
  }
  return true;
}

// stock soda function
void stock_soda( unsigned int cargo[], unsigned int sodas[], unsigned int maxStockPerFlavour ) {
  for ( unsigned int j = 0; j < 4; j++ ) { // for each flavour in the vm
    if ( sodas[j] < maxStockPerFlavour ) {  // if the flavour is not fully stocked
      if ( cargo[j] > 0 ) { // if the cargo has this flavour
	unsigned int numSodaRequired;
	numSodaRequired = maxStockPerFlavour - sodas[j]; // calcalate the number requried to fully stock this flavour
	if ( numSodaRequired < cargo[j] ) { // if cargo has more of this flavour than needed
	  sodas[j] = maxStockPerFlavour; // stock to full
	  cargo[j] = cargo[j] - numSodaRequired; // take out the required number from cargo
	}
	else { // if the cargo dont have enough of this flavour to fully stock
	  sodas[j] = sodas[j] + cargo[j]; // give all of it to the vm
	  cargo[j] = 0; // and this flavour in the cargo is none
	}
      }
    }
  }
}

// ========= Plant Implementation ===========================================
// Constructor
BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ): prt( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ), maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ), timeBetweenShipments( timeBetweenShipments ) {
  sodas = new unsigned int[4]; // initialize sodas array 
}

// Destructor
BottlingPlant::~BottlingPlant() {
  prt.print( Printer::Kind::BottlingPlant, 'F' ); // plant finished 
  delete[] sodas; // delete sodas 
}

// get shipment
void BottlingPlant::getShipment( unsigned int cargo[] ) {
  // lock.signal();
  if ( close == 1 ) { // if the plant is closing down 
    //  lock.signal();
    _Throw Shutdown( close ); // throw exception 
  }
  int totalsoda = 0; 
  for ( int i = 0; i < 4; i++ ) { 
    cargo[i] = sodas[i]; // copy the soda from sodas array to cargo array
    totalsoda = sodas[i] + totalsoda; // update the total numbers produced 
  }
  prt.print( Printer::Kind::Truck, 'P', totalsoda ); // print total soda picked up 
}

// main
void BottlingPlant::main() {
  prt.print( Printer::Kind::BottlingPlant, 'S' ); // plant starting 
  Truck *truck;
  truck = new Truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour ); // start a truck
  for ( int i = 0; i < 4; i++ ) { // first production 
    sodas[i] = random() % maxShippedPerFlavour;
    sodaProduced = sodaProduced + sodas[i];
  }
  prt.print( Printer::Kind::BottlingPlant, 'G', sodaProduced ); // first production finished 
  for ( ;; ) {
    _Accept ( ~BottlingPlant ) {
      close = 1; // set close to 1 
      // lock.wait();
      break;
    }
    or _Accept ( getShipment ) {
      prt.print( Printer::Kind::BottlingPlant, 'P' );  // shipment picked up
      sodaProduced = 0; // reset the soda count 
      for ( int i = 0; i < 4; i++ ) { // start production again
	sodas[i] = random() % maxShippedPerFlavour;
	sodaProduced = sodaProduced + sodas[i];
      }
      prt.print( Printer::Kind::BottlingPlant, 'G', sodaProduced ); // production finished   
      yield( timeBetweenShipments ); // yield
    }
  }
  delete truck; 
}

// ========= Truck Implementation ============================================
// constructor
Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour ): prt( prt ), nameServer( nameServer ), plant( plant ), numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour ) {
  cargo = new unsigned int[4]; // initialize cargo array 
}

// Destructor
Truck::~Truck() {
  prt.print( Printer::Kind::Truck, 'F' ); // truck finished 
  delete[] cargo; 
}

// main
void Truck::main() {
  unsigned int random;
  prt.print( Printer::Kind::Truck, 'S' ); // truck starts 
  vs = nameServer.getMachineList(); // get the vm list from nameserver 
  for ( ;; ) {
    try {
      _Accept( ~Truck ) break;
      _Else {
	plant.getShipment( cargo ); // get a shipment 
	random = rand() % 10 + 1;  
	yield( random ); // go to tim horton
	for ( unsigned int i = 0; i < numVendingMachines; i++ ) { // start a cycle to stock up inventory 
	  sodas = vs[currentvm]->inventory();   // go to first vm and get its inventory 
	  stock_soda( cargo, sodas, maxStockPerFlavour ); // stock up soda 
	  vs[currentvm]->restocked(); // first vm stocked up
	  currentvm = currentvm + 1; // go to next vm 
	  if ( currentvm - 1 == numVendingMachines - 1 ) { // if the current vm is the last one in the list 
	    currentvm = 0; // restart from the first one 
	  }
	  if ( cargo_empty ) { // if the cargo is empty before all vms are stocked up, break the loop
	    break;
	  }
	}
      }
    }
    catch( BottlingPlant::Shutdown( &close ) ) {
      break;
    }
  }
}

#endif
