#ifndef __SODAGROUP_H__
#define __SODAGROUP_H__

#include "sodaCard.h"
#include "sodaPrinter.h"

// ========= Header ============================================================
_Task Groupoff {
  unsigned int numStudents, sodaCost, groupoffDelay, count = 0;
  int *gift_indicator; // an array to indicate which student has received gift 
  Printer &prt;
  WATCard::FWATCard *fcs; // an array of future card 
  void main();
 public:
  ~Groupoff();
  Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
  WATCard::FWATCard giftCard();
};

// ========= Helpter =====================================================
// helper to determine if all students are given gift 
bool all_assigned( int gift_indicator[], unsigned int numStudents ) {
  for ( unsigned int i = 0; i < numStudents; i++ ) {
    if ( gift_indicator[i] == 0 ) {
      return false;
    }
  }
  return true;
}

// ========= Implementation ======================================================
// constructor
Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ): prt( prt ), numStudents( numStudents ), sodaCost( sodaCost ), groupoffDelay( groupoffDelay ) {
  fcs = new WATCard::FWATCard[numStudents]; // initialize the array of future card with the number of students 
  gift_indicator = new int[numStudents]; // initialize the indicator array with number of students and make therm 0 
  for ( unsigned int i = 0; i < numStudents; i++ ) {
    gift_indicator[i] = 0;
  }
}

// destructor
Groupoff::~Groupoff() {
  prt.print( Printer::Kind::Groupoff, 'F' ); // groupoff finished 
  delete[] fcs;  // delete future card array 
  delete[] gift_indicator; // delete indicator array 
}

// giftCard
WATCard::FWATCard Groupoff::giftCard() {
  yield( groupoffDelay ); // yield
  WATCard::FWATCard fc; // dummy future card 
  fcs[count] = fc; // insert into the future card array 
  count = count + 1; // increase the count 
  return fc; 
}

// main
void Groupoff::main() {
  prt.print( Printer::Kind::Groupoff, 'S' ); // group off starts
  for ( ;; ) {  
    _Accept( ~Groupoff ) break;
    or _Accept( giftCard ) {}
    _Else {
      if ( all_assigned( gift_indicator, numStudents ) ) { // if all students reveived their gift card with money  
	break; // then terminate 
      }
      unsigned int random = rand() % numStudents; // random students 
      if ( count == numStudents && gift_indicator[random] == 0 ) { // if there are student didnt get the gift 
	gift_indicator[random] = 1; // set this student in the indicator array
	WATCard *c = new WATCard();
	c->deposit( sodaCost ); // deposit this student sodaCost money
	fcs[random].delivery( c ); // deliever to the future 
	prt.print( Printer::Kind::Groupoff, 'D', sodaCost ); // print delievered 
      }
    }
  }
}
#endif 
