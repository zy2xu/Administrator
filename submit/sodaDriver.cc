#include <iostream>
#include "config.cc"
#include "MPRNG.h"
#include "string"
#include "sodaPrinter.h"
#include "sodaParent.h"
#include "sodaBank.h"
#include "sodaOffice.h"
#include "sodaGroup.h"
#include "sodaName.h"
#include "sodaVending.h"
#include "sodaPlant.h"
#include "sodaStudent.h"

using namespace std;
MPRNG prng;

// ========= helpters ==================================
// helper to check if the given char is an int
bool legal_int( char *str ) {
  while ( *str )
    if ( !isdigit( *str++ ) )
      return false;
  return true;
}

// ========= main =======================================
void uMain::main(){
  int seed = getpid(); // initialize seed
  string msg = "Usage: ./soda [ config-file [ random-seed (> 0) ] ]"; // command line error msg
  ConfigParms c; 
  
  switch( argc ){
  case 1: processConfigFile( "soda.config", c ); break; // case where no input is supplid 
  case 2: processConfigFile( argv[1], c); break; // case where only the file name is supplied 
  case 3: if ( !legal_int( argv[2] )) {cerr << msg << endl; exit(0);} processConfigFile( argv[1], c); seed = atoi( argv[2] ); break; // case where file name and seed parameter are supplied
  default: cerr << msg << endl; exit(0); // when there is more than 2 parameter   
  }

  // seed the prng
  prng.seed( seed );

  // initialize objects
  VendingMachine *vms[c.numVendingMachines];
  Student *sts[c.numStudents];
  BottlingPlant *plant;
  Printer printer( c.numStudents, c.numVendingMachines, c.numCouriers );
  Bank bank( c.numStudents );
  Parent parent( printer, bank, c.numStudents, c.parentalDelay );
  WATCardOffice office( printer, bank, c.numCouriers );
  Groupoff group( printer, c.numStudents, c.sodaCost, c.groupoffDelay );
  NameServer nameServer( printer, c.numVendingMachines, c.numStudents );
  for ( unsigned int i = 0; i < c.numVendingMachines; i++ ) {
    vms[i] = new VendingMachine( printer, nameServer, i, c.sodaCost, c.maxStockPerFlavour );
  }
  plant = new BottlingPlant( printer, nameServer, c.numVendingMachines, c.maxShippedPerFlavour, c.maxStockPerFlavour, c.timeBetweenShipments );
  for ( unsigned int i = 0; i < c.numStudents; i++ ) {
    sts[i] = new Student( printer, nameServer, office, group, i, c.maxPurchases );
  }
  for ( unsigned int i = 0; i < c.numStudents; i++ ) {
    delete sts[i];
  }
  delete plant;
  for ( unsigned int i = 0; i < c.numVendingMachines; i++ ) {
    delete vms[i];
  }
} // end uMain 
