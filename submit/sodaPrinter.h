#ifndef __SODAPRINTER_H__
#define __SODAPRINTER_H__

#include "vector"
#include "string"
#include "sstream"

using namespace std;

// ========= Header ========================================
_Monitor Printer {
  unsigned int numStudents, numVendingMachines, numCouriers, total;
  string *buffer; // string buffer for the msgs
  int *overwrites; // and indicator array to indicate if an object already has the buffer slot filled 
 public:
  enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
  Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
  ~Printer();
  void print( Kind kind, char state );
  void print( Kind kind, char state, int value1 );
  void print( Kind kind, char state, int value1, int value2 );
  void print( Kind kind, unsigned int lid, char state );
  void print( Kind kind, unsigned int lid, char state, int value1 );
  void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};

// ========= Helpter ===============================================
// helper to print the program header
void print_headers( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers, unsigned int total ) {
  ostringstream convert; // instance to convert int to string  
  cout << "Parent" << '\t'; // header for Parent 
  cout << "Gropoff" << '\t'; // header for Groupoff
  cout << "WATOff" << '\t'; // header for WATOff
  cout << "Names" << '\t'; // header for Names
  cout << "Truck" << '\t'; // header for Truck
  cout << "Plant" << '\t'; // header for Plant
  // header to print all students 
  for ( unsigned int i = 0; i < numStudents; i++) {
    convert << i;
    cout << "Stud" << convert.str() << '\t';
    convert.str("");
  }
  // header to print all vending machines 
  for ( unsigned int i = 0; i < numVendingMachines; i++) {
    convert << i;
      cout << "Mach" << convert.str() << '\t';
    convert.str("");
  }
  // header to print all couriers 
  for ( unsigned int i = 0; i < numCouriers; i++) {
    convert << i;
      cout << "Cour" << convert.str() << '\t';
    convert.str("");
  }
  cout << endl;
  for ( unsigned int i = 0; i < total; i++) {
    cout << "*******" << '\t';
  }
  cout << endl;
} 

// helpter to print the ending
void print_ending() {
  cout << "***********************" << endl;
} 

// flush the buffer to emply strings, and overwrites to 0
void flush( unsigned int total, int overwrites[], string buffer[] ){
  for ( unsigned int i = 0; i < total; i++ ) {
    buffer[i] = "";
    overwrites[i] = 0;
  }
}

// helper to print the line before a buffer needs to flushed
void print_helper( unsigned int total, string buffer[] ){
  for ( unsigned int i = 0; i < total; i++ ) {
    cout << buffer[i] << '\t';
  }
  cout << endl;
}

// helper to print Finished row
void print_F( unsigned int total, unsigned int location ) {
  for ( unsigned int i = 0; i < total; i++ ) {
    if ( i == location ) {
      cout << "F" << '\t';
    }
    else {
      cout <<  "..." << '\t';
    }
  }
  cout << endl;
}

// helper to get the location of the object in the indicator array that has mulptiple instances 
unsigned int get_location( Printer::Kind kind, unsigned int lid, unsigned int numStudents, unsigned int numVendingMachines ) {
  switch( kind ){
  case Printer::Kind::Student: return 6 + lid; break;
  case Printer::Kind::Vending: return 6 + numStudents + lid; break;
  case Printer::Kind::Courier: return 6 + numStudents + numVendingMachines + lid; break;
  }
}

// ========= Definition =====================================================
// constructor
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ): numStudents( numStudents ), numVendingMachines( numVendingMachines ), numCouriers( numCouriers ){
  total =  numStudents + numVendingMachines + numCouriers + 6; // calculate total number of columns 
  overwrites = new int[total]; // create an array of int used as indicator if the entry will be overwrite
  buffer = new string[total]; // buffer to store the output
  print_headers( numStudents, numVendingMachines, numCouriers, total ); // print the header
}

// destructor
Printer::~Printer(){  // delete the pointers
  // print_helper( voters, buffer ); // print the last line that is stored in the buffer before terminate
  delete[] overwrites;
  delete[] buffer; 
  print_ending(); // print the ending
}

// first printer function with only kind and state
void Printer::print( Kind kind, char state ) {
  unsigned int location = kind;
  if ( overwrites[location] == 1 ) { // if there is already an entry in the buffer for the given kind
    print_helper( total, buffer ); // print the line from buffer
    flush( total, overwrites, buffer ); // flush the buffer 
  }
  if ( state == 'F' ){ // print and return if we hit Finished
    print_F( total, location );
    return;
  }
  overwrites[location] = 1; // flip the flicker for the kind in the overwrite indicator array
  switch( state ){ // store the output into correct buffer
  case 'S': buffer[location] = "S"; break; // Start for Parent, Groupoff, Watcardoffice, Name, Truck and plant 
  case 'W': buffer[location] = "W"; break; // request work call complete for Watcardoffice 
  case 'P': buffer[location] = "P"; break; // shipment Picked up by truck for Truck
  }
}
 
// second printer function with only kind and state and int value
void Printer::print( Kind kind, char state, int value1 ) {
  unsigned int location = kind;
  if ( overwrites[location] == 1 ) { // if there is already an entry in the buffer for the given kind
    print_helper( total, buffer ); // print the line from buffer
    flush( total, overwrites, buffer ); // flush the buffer
  }
  overwrites[location] = 1;
  ostringstream convert; // used to convert an int to string 
  convert << value1;
  switch( state ){
  case 'D': buffer[location] = "D" + convert.str(); break; // deposit gift for groupoff
  case 'R': buffer[location] = "R" + convert.str(); break; // register machine for nameserver 
  case 'P': buffer[location] = "P" + convert.str(); break; // pick up a shipment by truck
  case 'G': buffer[location] = "G" + convert.str(); break; // generating soda by plant
  }
}

// Third printer function with only kind and state and 2 int values
void Printer::print( Kind kind, char state, int value1, int value2 ) {
  unsigned int location = kind;
  if ( overwrites[location] == 1 ) { // if there is already an entry in the buffer for the given kind
    print_helper( total, buffer ); // print the line from buffer
    flush( total, overwrites, buffer ); // flush the buffer
  }
  overwrites[location] = 1;
  ostringstream convert1, convert2;
  convert1 << value1;
  convert2 << value2;
  switch( state ) {
  case 'D': buffer[location] = "D" + convert1.str() + "," + convert2.str(); break; // deposit gift by parent or end delivery by truck
  case 'C': buffer[location] = "C" + convert1.str() + "," + convert2.str(); break; // Create call complete by office
  case 'T': buffer[location] = "T" + convert1.str() + "," + convert2.str(); break; // transfer complete by office
  case 'N': buffer[location] = "N" + convert1.str() + "," + convert2.str(); break; // new vending machine by name server
  case 'd': buffer[location] = "d" + convert1.str() + "," + convert2.str(); break; // begin delievery by truck
  case 'U': buffer[location] = "U" + convert1.str() + "," + convert2.str(); break; // unsuccessful fill by truck
  }
}

// forth printer function with lid, kind and state
void Printer::print( Kind kind, unsigned int lid, char state ) {
  unsigned int location;
  location = get_location( kind, lid, numStudents, numVendingMachines );
  if ( overwrites[location] == 1 ) { // if there is already an entry in the buffer for the given kind
    print_helper( total, buffer ); // print the line from buffer
    flush( total, overwrites, buffer ); // flush the buffer
  }
  if ( state == 'F' ){ // print and return if we hit Finished
    print_F( total, location );
    return;
  }
  overwrites[location] = 1; // flip the flicker for the kind in the overwrite indicator array
  switch( state ){ // store the output into correct buffer
  case 'L': buffer[location] = "L"; break; // watcard lost for student 
  case 'r': buffer[location] = "r"; break; // start reloading by truck for vm
  case 'R': buffer[location] = "R"; break; // finished reloading by truck for vm
  case 'S': buffer[location] = "S"; break; // starting for courier
  }
}

// fifth printer function with kind, lid  and state and int value
void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
  unsigned int location;
  location = get_location( kind, lid, numStudents, numVendingMachines );
  if ( overwrites[location] == 1 ) { // if there is already an entry in the buffer for the given kind
    print_helper( total, buffer ); // print the line from buffer
    flush( total, overwrites, buffer ); // flush the buffer
  }
  overwrites[location] = 1;
  ostringstream convert;
  convert << value1;
  switch( state ){
  case 'V': buffer[location] = "V" + convert.str(); break; // select vm for student
  case 'G': buffer[location] = "G" + convert.str(); break; // gifted a soda for student 
  case 'B': buffer[location] = "B" + convert.str(); break; // bought a soda for student 
  case 'S': buffer[location] = "S" + convert.str(); break; // starting for vm 
  }
}

// Sisth printer function with kind, lid and state and 2 int values
void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
  unsigned int location;
  location = get_location( kind, lid, numStudents, numVendingMachines );
  if ( overwrites[location] == 1 ) { // if there is already an entry in the buffer for the given kind
    print_helper( total, buffer ); // print the line from buffer
    flush( total, overwrites, buffer ); // flush the buffer
  }
  overwrites[location] = 1;
  ostringstream convert1, convert2;
  convert1 << value1;
  convert2 << value2;
  switch( state ) {
  case 'S': buffer[location] = "S" + convert1.str() + "," + convert2.str(); break; // starting for student
  case 'B': buffer[location] = "B" + convert1.str() + "," + convert2.str(); break; // student bought a soda for vm
  case 't': buffer[location] = "t" + convert1.str() + "," + convert2.str(); break; // start funds transfer for courier
  case 'T': buffer[location] = "T" + convert1.str() + "," + convert2.str(); break; // complete fund transfer for courier
  }
}

#endif 
