#ifndef __SODAOFFICE_H__
#define __SODAOFFICE_H__

#include <queue>
#include "sodaPrinter.h"
#include "sodaBank.h"
#include "sodaCard.h"
#include <vector>

// declare arg stucture 
struct Args {
  WATCard *card; 
  unsigned int sid;
  unsigned int amount;
};

// ========= Header ============================================
_Task WATCardOffice {
  Printer &prt;
  Bank &bank;
  uCondition lock; // used for sync between office and courier 
  bool end = false;  // used as a flag to indicate office is shutting down
  unsigned int numCouriers;
  struct Job {                           // marshalled arguments and return future
    Args args;                         // call arguments (YOU DEFINE "Args")
    WATCard::FWATCard result;          // return future
    Job( Args args ) : args( args ) {}
  };
  queue<Job*> requests;  // a queue of job request pointers 
  _Task Courier {
    Printer &pt;
    Bank &bk;
    WATCardOffice &off;
    unsigned int id, count = 0; // the student's id 
    void main();
  public:
    void stop();
    ~Courier();
    Courier( Printer &pt, Bank &bk, WATCardOffice &off, unsigned int id );
  };                 // communicates with bank
  vector<Courier*> couriers;  // a vector of courier pointers 
  void main();
 public:
  ~WATCardOffice();
  _Event Lost {};                        // lost WATCard
  WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
  WATCard::FWATCard create( unsigned int sid, unsigned int amount );
  WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
  Job *requestWork();
};


// ========= Implementation ====================================================
// Constructor
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ): prt( prt ), bank( bank ), numCouriers( numCouriers ) {}

// Constructor courier
WATCardOffice::Courier::Courier( Printer &pt, Bank &bk, WATCardOffice &off, unsigned int id ): pt( pt ), bk( bk ), off( off ), id( id ) {}

// Destructor
WATCardOffice::~WATCardOffice() {
  for ( unsigned int i = 0; i < numCouriers; i++){ // delete couriers
    couriers.pop_back();
    delete couriers[i];
  }
  prt.print( Printer::Kind::WATCardOffice, 'F' ); // office Finished
  couriers.clear();
}

// Destructor courier
WATCardOffice::Courier::~Courier() {
  pt.print( Printer::Kind::Courier, id, 'F' ); // courier finished 
}

// Main
void WATCardOffice::main() {
  prt.print( Printer::Kind::WATCardOffice, 'S' ); // office starts 
  for ( unsigned int i = 0; i < numCouriers; i++ ) { // initialize all the couriers 
    Courier *c = new Courier( prt, bank, *this, i );
    couriers.push_back(c);
  } 
  for( ;; ) {
    _Accept( ~WATCardOffice ) { 
      end = true; // office is shutting down
      lock.signal(); // wake up the last blocking call which is requestJob
      break;
    }
    or _Accept( create, transfer ) { 
      while ( !lock.empty() ) { // wake up all requestJob
	lock.signal(); 
      }
    } 
    or _Accept( requestWork ) {}
  }
}

// Main courier
void WATCardOffice::Courier::main() {
  pt.print( Printer::Kind::Courier, id, 'S' ); // courier starts 
  for( ;; ) {
    Job *j = off.requestWork(); // call requestWork
    if ( j == NULL ) { // if there is no work 
      break; // turn off courier 
    }
    // if there is work 
    pt.print( Printer::Kind::Courier, id, 't', j->args.sid, j->args.amount ); // start transfer money 
    bk.withdraw( j->args.sid, j->args.amount ); // withdraw from bank 
    j->args.card->deposit( j->args.amount ); // deposit into watcard
    j->result.delivery( j->args.card ); // deliever to the future 
    pt.print( Printer::Kind::Courier, id, 'T', j->args.sid, j->args.amount ); // transfer complete
    delete j; 
  }
}

// create
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  WATCard *card = new WATCard(); // create a new card 
  WATCard::FWATCard result; // dummy future card
  Args args; // dummy args 
  args.card = card; // input the vars into args 
  args.sid = sid;
  args.amount = amount;
  Job *j = new Job( args ); // create a new job 
  requests.push( j ); // push onto request queue
  prt.print( Printer::Kind::WATCardOffice, 'C', sid, amount ); // work request completed 
  result = j->result;
  return result;
}

// transfer 
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  Args args;
  args.card = card;
  args.sid = sid;
  args.amount = amount;
  Job *j = new Job( args );
  requests.push( j );
  prt.print( Printer::Kind::WATCardOffice, 'T', sid, amount ); // transfer request completed 
  return j->result;
}

// request work
WATCardOffice::Job* WATCardOffice::requestWork() {
  Job *j = NULL; // declare a dummy job pointer 
  while ( requests.empty() ) { // if not requests
    lock.wait(); // wait 
    if ( end ) { // if office is shutting down 
      return j; // return the null job pointer 
    }
  }
  j = requests.front(); // point j to the next job 
  requests.pop(); // pop next job 
  prt.print( Printer::Kind::WATCardOffice, 'W' ); // request work call complete 
  return j; // return the job 
}
#endif 
