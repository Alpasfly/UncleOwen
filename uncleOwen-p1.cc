
#include <iostream>
#include <cstdlib>
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

void error(Error n){
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
void printAndroid(const Android &android) {

}

// prints field info
void printField(const Field &field) {
  cout << "{Field: " << field.name << "(" << field.products << ")" << endl;
  for (int i = 0; i < field.androids.size(); i++) {
    printAndroid(field.androids[i]);
  }
  cout << "}" << endl;
}

// prints farm info
void printFarm(const Farm &farm) {
  cout << "Farm: " << farm.name << endl;
  // Print every field in the farm
  for (int i = 0; i < farm.fields.size(); i++) {
    printField(farm.fields[i]);
  }
  // Print every android in the farm
  for (int i = 0; i < farm.androids.size(); i++) {
    printAndroid(farm.androids[i]);
  }
}

// Returns true if the given android model string exists
bool modelExists(string model) {
  bool exists = false;
  for (int i = 0; i < NUMDROIDMODELS; i++) {
    if (droidModels[i] == model) exists = true;
  }
  return exists;
}

// Creates a new android asking the data to the user given nextSerialNumber, and
// adds the android to the farm
void createAndroid(Farm &farm, int &nextSerialNumber) {
  int speed;
  string model;
  cout << "Enter android speed: ";
  cin >> speed;

  cin.clear();
  cin.ignore(INT_MAX, '\n');

  if (speed > 0) {
    // Speed is ok
    cout << "Enter android model: ";
    getline(cin, model);
    if (modelExists(model)) {
      // Create android
      Android android;
      android.model        = model;
      android.serialNumber = nextSerialNumber;
      android.status       = ST_IDLE;

      // Add it to the farm
      farm.androids.push_back(android);
    }
    else {
      error(ERR_WRONG_MODEL);
    }
  }
  else {
    error(ERR_WRONG_SPEED);
  }
}

// Returns index of the given field name. If it doesn't exist, returns -1
int fieldExists(string name, Farm &farm) {
  int index = -1;
  for (int i = 0; i < farm.fields.size(); i++) {
    if (farm.fields[i].name == name) index = i;
  }
  return index;
}

// creates a new field in the farm with the name provided by the user
void createField(Farm &farm) {
  string name;
  cout << "Enter field name: ";
  getline(cin, name);
  if (fieldExists(name, farm) == -1) {
    // The given name is not repeated, create the field...
    Field field;
    field.name = name;
    field.products = 0;
    // ...and add it to the farm
    farm.fields.push_back(field);
  }
  else {
    error(ERR_NAME);
  }
}


// distributes the farm's androids among its fields
void distributeAndroids(Farm &farm) {

}

// simulates the collection of products in a field by its androids
void collectField(Field &field) {

}

// collects the products in the farm's fields
void collectFarm(Farm &farm) {

}

// asks for products data in the farm's fields, then collects them
void startWorkingDay(Farm &farm) {
  bool correctName = false;
  string name;
  do {
    if (farm.fields.size() > 0) {
      // The farm contains fields that can be worked
      // Ask for field name
      cout << "Enter field name: ";
      getline(cin, name);

      // Check if a field with that name exists
      int index = fieldExists(name, farm);
      if (index != -1 && name != "q")
        correctName = true;
    }
    else {
      error(ERR_NO_FIELDS);
    }
  } while (!correctName);
}

void menu() {
  cout << "-----========== Farm manager ==========-----" << endl
       << "1- List farm info" << endl
       << "2- Add field" << endl
       << "3- Add android" << endl
       << "4- Start working day" << endl
       << "q- Quit" << endl
       << "Option: " ;
}


int main() {
  Farm farm;
  farm.name = "west farm";
  char option;
  int nextSerialNumber = 100;  

  do {
    menu();
    cin >> option; cin.get();
        
    switch (option) { 
      case '1': {
        printFarm(farm);
        break;
      }
      case '2': { 
        createField(farm);
        break;
      }
      case '3': {
        createAndroid(farm, nextSerialNumber);
        break;
      }
      case '4': {
        startWorkingDay(farm);
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