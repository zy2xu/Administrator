#ifndef __SODAPARENT_H__
#define __SODAPARENT_H__

#include "sodaBank.h"
#include "sodaPrinter.h"
 
using namespace std;

// ======== Header ========================================================
_Task Parent {
  // declare private vars 
  Printer &prt;
  Bank &bank;
  unsigned int numStudents, parentalDelay;
  void main();
 public:
  ~Parent();
  Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

// ======== Implementation ===============================================
// Constructor
Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ): prt( prt ), bank( bank ), numStudents( numStudents), parentalDelay( parentalDelay ) {} 

// Destructor 
Parent::~Parent() {
  prt.print( Printer::Kind::Parent, 'F' ); // parent finish
}

// main 
void Parent::main() {
  prt.print( Printer::Kind::Parent, 'S' ); // parent start
  unsigned int amount, sid; // declare int for the amount to deposit and the student id
  for (;;) {
    _Accept( ~Parent ) break; 
    _Else {
      yield( parentalDelay ); // yield delay
      amount = rand() % 3 + 1; // random amount 
      sid = rand() %  numStudents; // random student 
      bank.deposit( sid, amount ); // deposit the amount to the student 
      prt.print( Printer::Kind::Parent, 'D', sid, amount ); // print deposit 
    }
  }
}
#endif 
