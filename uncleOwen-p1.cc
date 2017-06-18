
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
int getFieldNameIndex(string name, Farm &farm) {
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
  if (getFieldNameIndex(name, farm) == -1) {
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

int nextField(Farm &farm) {
  int nfi;                // Next Field Index
  int m        = INT_MAX; // Min ammouont of andorids in fields
  int maxProds = 0;       // Max ammount of products in fields
  vector<Field> fields;

  // Find lowest ammount of androids in a field
  for (int i = 0; i < farm.fields.size(); i++) {
    if (m > farm.fields[i].androids.size())
      m = farm.fields[i].androids.size();
  }

  // Push all the less populated fields to a vector
  for (int i = 0; i < farm.fields.size(); i++) {
    if (farm.fields[i].androids.size() == m) fields.push_back(farm.fields[i]);
  }

  // Find highest ammount of products amongst the minimally populated fields
  for (int i = 0; i < fields.size(); i++) {
    if (maxProds < fields[i].products)
      maxProds = fields[i].products;
  }

  // Find the next field index
  for (int i = 0; i < farm.fields.size(); i++) {
    if (farm.fields[i].androids.size() == m && farm.fields[i].products == maxProds)
      nfi = i;
  }

  return nfi;
}

// distributes the farm's androids among its fields
void distributeAndroids(Farm &farm) {
  bool distributable = false; // Are there any fields to distribute androids in
  int nfi;                    // Next Field Index
  int fai;                    // Fastest Android Index
  int maxSpeed = 0;           // Max speed in farm's androids

  cout << HEADER2 << endl;
  // Check if there are fields to collect
  for (int i = 0; i < farm.fields.size(); i++) {
    if (farm.fields[i].products > 0) distributable = true;
  }

  while (distributable) {
    nfi = nextField(farm);
    // Find fastest android in farm
    for (int i = 0; i < farm.androids.size(); i++) {
      if (farm.androids[i].status == ST_IDLE && farm.androids[i].speed > maxSpeed) {
        maxSpeed = farm.androids[i].speed;
        fai = i;
      }
    }

    // Move fastest android to next field
    farm.fields[nfi].androids.push_back(farm.androids[fai]);
    farm.androids.erase(farm.androids.begin() + fai);
  }
}

// simulates the collection of products in a field by its androids
void collectField(Field &field) {
  int workingHours = 5; // This might change in the future
  int totalProducts = 0;
  // Calculate the ammount of collected products by multiplying
  // each android's speed (in products/hour) by the ammount of working hours
  for (int i = 0; i < field.androids.size(); i++) {
    totalProducts += field.androids[i].speed * workingHours;
  }

  // Subtract the total collectable products from the field's products
  field.products -= totalProducts;
  if (field.products < 0) field.products = 0;

  // Finish working session by setting all androids to idle status...
  for (int i = 0; i < field.androids.size(); i++) {
    field.androids[i].status = ST_IDLE;
  }
}

// Move androids from field to farm
void returnAndroids(Field field, Farm farm) {
  for (int i = 0; i < field.androids.size(); i++) {
    farm.androids.push_back(field.androids[i]);
    field.androids.erase(field.androids.begin() + i);
  }
}

// collects the products in the farm's fields
void collectFarm(Farm &farm) {
  cout << HEADER1 << endl;
  printFarm(farm);
  distributeAndroids(farm);
  // Collect every field in the farm
  for (int i = 0; i < farm.fields.size(); i++) {
    collectField(farm.fields[i]);
    // Return androids from fields to farm
    returnAndroids(farm.fields[i], farm);
  }

  cout << HEADER3 << endl;
  printFarm(farm);
}

// asks for products data in the farm's fields, then collects them
void startWorkingDay(Farm &farm) {
  bool correctName = false;
  int products = 0;
  string name = "q";
    if (farm.fields.size() > 0) {
      // The farm contains fields that can be collected
      do {
        // Ask for field name
        cout << "Enter field name: ";
        getline(cin, name);

        // If the field exists or the user entered a "q", ask for products
        int index = getFieldNameIndex(name, farm);
        if (index != -1 || name == "q") {
          cout << "Products: ";
          cin >> products;

          cin.clear();
          cin.ignore(INT_MAX, '\n');
          
          if (products > 0) {
            farm.fields[index].products += products;
          }
          else {
            error(ERR_WRONG_PRODUCTS);
            correctName = false;
          }
        }
        else
          error(ERR_WRONG_FIELD);
      } while (name == "q" || correctName);
      
      collectFarm(farm);
    }
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