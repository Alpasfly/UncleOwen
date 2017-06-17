
#include <iostream>
#include <climits>
#include <vector>
#include <string>

using namespace std;

// ---------------- android -------------------------
const int NUMDROIDMODELS=5;

const string droidModels[NUMDROIDMODELS] = { 
   "x-75",
   "fx-7",
   "JK9",
   "XAR-25",
   "HC-2"
};


enum Status {ST_WORKING, ST_IDLE};

typedef struct {
   string model; 
   int speed;
   int serialNumber;
   Status status; 
} Android;

// ---------------- field -------------------------

typedef struct {
   string name;
   int products;
   vector<Android> androids;
} Field;

// ---------------- farm --------------------------

typedef struct {
  string name;
  vector<Field> fields;
  vector<Android> androids;
} Farm;

// ---------------- error -------------------------

enum Error {ERR_NO_FIELDS, ERR_UNKNOWN_OPTION, ERR_WRONG_MODEL, ERR_WRONG_FIELD, ERR_WRONG_SPEED, ERR_WRONG_PRODUCTS, ERR_NAME}; 

void error(Error n)
{
  switch (n) {
   
    case ERR_NO_FIELDS:
      cout << "Error, there are no fields in the farm" << endl;
      break;
      
    case ERR_UNKNOWN_OPTION:
      cout << "Error, unknown option" << endl;
      break;

    case ERR_WRONG_MODEL:
      cout << "Error, wrong android model name" << endl;
      break;

    case ERR_WRONG_FIELD:
      cout << "Error, wrong field name" << endl;
      break;

    case ERR_WRONG_SPEED:
      cout << "Error, wrong speed" << endl;
      break;

    case ERR_WRONG_PRODUCTS:
      cout << "Error, wrong number of products" << endl;
      break;

    case ERR_NAME:
      cout << "Error, field name repeated" << endl;
      break;
    
  }
}

//---------------------------------------------------------------
const string HEADER1="---- start ----";
const string HEADER2="---- distribution ----";
const string HEADER3="---- end ----";
//---------------------------------------------------------------

 // prints android info
void printAndroid(const Android &android)
{
}

// prints field info
void printField(const Field &field)
{
}

// prints farm info
void printFarm(const Farm &farm) 
{
}

// Creates a new android asking the data to the user given nextSerialNumber, and
// adds the android to the farm
void createAndroid(Farm &farm, int &nextSerialNumber)
{
}

// creates a new field in the farm with the name provided by the user
void createField(Farm &farm)
{
}


// distributes the farm's androids among its fields
void distributeAndroids(Farm &farm)
{
}

// simulates the collection of products in a field by its androids
void collectField(Field &field)
{
}

// collects the products in the farm's fields
void collectFarm(Farm &farm)
{
}

// asks for products data in the farm's fields, then collects them
void startWorkingDay(Farm &farm)
{
}

void menu()
{
  cout << "-----========== Farm manager ==========-----" << endl
       << "1- List farm info" << endl
       << "2- Add field" << endl
       << "3- Add android" << endl
       << "4- Start working day" << endl
       << "q- Quit" << endl
       << "Option: " ;
}


int main()
{
   Farm farm;
   farm.name = "west farm";
   char option;
   int nextSerialNumber = 100;  

   do {
        menu();
        cin >> option; cin.get();
        
        switch (option) { 
          case '1': {
            break;
          }
          case '2': { 
            break;
          }
          case '3': {
            break;
          }
          case '4': {
            break;
          }
          case 'q': {
            break;
          }
          default:  {
            error(ERR_UNKNOWN_OPTION); 
            break;
          }
        }
   } while (option != 'q');
   
   return 0;
}