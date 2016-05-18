#ifndef __SODANAME_H__
#define __SODANAME_H__

#include "sodaPrinter.h"
#include <vector>

_Task VendingMachine;

// ========= Header ====================================================================
_Task NameServer {
  unsigned int numVendingMachines, numStudents, count = 0, currentvm = 0;
  int *ids; // array of student ids 
  Printer &prt; 
  vector<VendingMachine*> vms; // vector of vending machines 
  VendingMachine **vs; // a pointer of vending machine pointers 
  void main();
 public:
  ~NameServer();
  NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
  void VMregister( VendingMachine *vendingmachine );
  VendingMachine *getMachine( unsigned int id );
  VendingMachine **getMachineList();
};

// ========= Implmentation =======================================================
// Constructor
NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ): prt( prt ), numVendingMachines( numVendingMachines ), numStudents( numStudents ) {
  ids = new int[numStudents]; // initialize student id array
  vs = new VendingMachine*[numVendingMachines]; // initialize vending mahine pointer array 
}

// Destructor 
NameServer::~NameServer() {
  prt.print( Printer::Kind::NameServer, 'F' ); // name server finished 
  delete[] ids; // delete student id array
  delete[] vs; // delete vending machine pointer array
}

// VMregister
void NameServer::VMregister( VendingMachine *vendingmachine ) {
  prt.print( Printer::Kind::NameServer, 'R', count ); // print registered 
  count = count + 1; // increase the count of vending mahhine
  vms.push_back( vendingmachine ); // put into the vector 
}

// getMachine
VendingMachine* NameServer::getMachine( unsigned int id ) {
  currentvm = currentvm + 1;  // update current 
  if ( currentvm == numVendingMachines ) { // if the current vending machine move past the last vending machine 
    currentvm = 0; // reset the current to the first vending machine 
  }
  return vms[currentvm]; // return the current vm 
}

// get MachineList
VendingMachine** NameServer::getMachineList() {
  for ( unsigned int i = 0; i < numVendingMachines; i++ ) {
    vs[i] = vms[i]; // copy the vms in the vm vector to the vm pointer list
  }
  return vs; 
}

// main 
void NameServer::main() {
  prt.print( Printer::Kind::NameServer, 'S' ); // start nameserver 
  for ( ;; ) {
    _Accept( ~NameServer ) break;
    or _Accept( VMregister ) {}
    or _Accept( getMachineList ) {}
    or _Accept( getMachine ) {}
  }
}



#endif 
