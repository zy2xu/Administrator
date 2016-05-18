#ifndef __SODABANK_H__
#define __SODABANK_H__

// ========= Header ============================================
_Monitor Bank {
  unsigned int numStudents;
  unsigned int *accounts; // array of accounts for each student
 public:
  Bank( unsigned int numStudents );
  ~Bank();
  void deposit( unsigned int id, unsigned int amount );
  void withdraw( unsigned int id, unsigned int amount );
};


// ======== Implementation =========================================
// Constructor
Bank::Bank( unsigned int numStudents ): numStudents( numStudents ) {
  accounts = new unsigned int[numStudents];
  for ( unsigned int i = 0; i < numStudents; i++ ) { // initialize all bank accounts to 0
    accounts[i] = 0;
  }
}

// Destructor 
Bank::~Bank() {
  delete[] accounts;
}

// deposit 
void Bank::deposit( unsigned int id, unsigned int amount ) {
  accounts[id] = accounts[id] + amount;
}

// withdraw
void Bank::withdraw( unsigned int id, unsigned int amount ) {
  accounts[id] = accounts[id] - amount;
}

#endif 
