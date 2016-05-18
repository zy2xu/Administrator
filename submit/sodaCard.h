#ifndef __SODACARD_H__
#define __SODACARD_H__

#include <uFuture.h>

// ========= Header ==============================================
class WATCard {
  unsigned int balance = 0;
  WATCard( const WATCard & );            // prevent copying
  WATCard &operator=( const WATCard & );
 public:
  typedef Future_ISM<WATCard *> FWATCard; // future watcard pointer
  WATCard();
  ~WATCard();
  void deposit( unsigned int amount );
  void withdraw( unsigned int amount );
  unsigned int getBalance();
};

// ========= Implementation ==================================================
// Constructor
WATCard::WATCard(){}

// Destructor
WATCard::~WATCard(){}

// deposite
void WATCard::deposit( unsigned int amount ) {
  balance = balance + amount;
}

// withdraw
void WATCard::withdraw( unsigned int amount ) {
  balance = balance - amount;
}
// getBalance
unsigned int WATCard::getBalance() {
  return balance;
}

#endif
