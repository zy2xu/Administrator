
// Concurrent-Safe Pseudo Random-Number Generator : generate repeatable sequence of values that
//   appear random by scrambling the bits of an integer value.
_Monitor MPRNG {
  unsigned int seed_; 
 public:
 MPRNG( unsigned int seed = 1009 ) : seed_( seed ) { srand( seed ); } // set seed
  unsigned int seed() { return seed_; }        // set seed
  void seed( unsigned int seed ) { seed_ = seed; srand( seed ); } // set seed
  unsigned int operator()() { return rand(); } // [0,UINT_MAX]
  unsigned int operator()( unsigned int u ) { return operator()() % (u + 1); } // [0,u]
  unsigned int operator()( unsigned int l, unsigned int u ) { return operator()( u - l ) + l; } // [l,u]
}; // MPRNG


//static MPRNG prng;
