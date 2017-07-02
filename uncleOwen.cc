#include <iostream>
#include <climits>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

// Android related data
const int NUMDROIDMODELS = 25;
int WORKHOURS = 5;

typedef struct {
  string modelName;
  int    hoursMaintenance;
} AndroidData;

const AndroidData ANDROIDDATA[NUMDROIDMODELS] = {
  "x-75", 8,
  "fx-7", 10,
  "JK9", 15,
  "XAR-25", 5,
  "HC-2", 12,
  "Xj 23 v7.5", 10,
  "c3p0-2", 20,
  "r2d2", 15,
  "f9-x d27", 10,
  "-v65 +23", 11,
  "@prog2ua", 13,
  "#h45ht4g", 18,
  "fx-9", 10,
  "x-7", 12,
  "JK10", 7,
  "GHR tdi9", 10,
  "fx-8", 20,
  "fx-75", 5,
  "JK9-75", 14,
  "HC-3.7", 12,
  "xw-a87", 17,
  "tiDef 8", 25,
  "fx-5", 10,
  "fx-3", 8,
  "fx-4", 9
 };

enum Status {
  ST_WORKING, 
  ST_IDLE, 
  ST_MAINTENANCE0,
  ST_MAINTENANCE1
};

typedef struct {
   string model;
   int speed;
   int serialNumber;
   int hoursWorked;
   Status status;
} Android;

// Farm data structures
typedef struct {
  string name;
  int products;
  int hoursLeft;
  vector<Android> androids;
} Field;

const int DAILY_WORKING_HOURS=10;
typedef struct {
  string name;
  vector<Field> fields;
  vector<Android> androids;
} Farm;

// Error handling
enum Error {
  ERR_NO_FIELDS, 
  ERR_UNKNOWN_OPTION,
  ERR_WRONG_MODEL,
  ERR_WRONG_FIELD,
  ERR_WRONG_SPEED, 
  ERR_WRONG_PRODUCTS,
  ERR_NAME,
  ERR_OPEN_FILE,
  ERR_ARGS
};

void error(Error n) {
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
    
    case ERR_OPEN_FILE:
      cout << "Error, can't open file" << endl;
      break;
    
    case ERR_ARGS:
      cout << "Error, wrong arguments" << endl
           << "Usage: uncleOwen [-f fieldName] [-a androidsFile] [-p productsFile]" << endl;
      break;
  }
}

// Header strings
const string HEADER1="---- start ----";
const string HEADER2="---- distribution ----";
const string HEADER3="---- end ----";
const string HOUR="Hour: ";
const string SEPARATOR="===============================================";


const int MAXNAME=10;

// Binary file data structures
typedef struct {
  char model[MAXNAME];
   int speed;
   int serialNumber;
   Status status;
   int hoursWorked;
} AndroidBin;

typedef struct {
  char name[MAXNAME];
  unsigned int products;
  int hoursLeft;
  unsigned int numAndroids;
} FieldBin;

typedef struct {
  char name[MAXNAME];
  int nextSerialNumber;
  int hour;
  unsigned int numFields;
} FarmBin;


// Returns true if the given android model string exists
bool modelExists(string model) {
  bool exists = false;
  for (int i = 0; i < NUMDROIDMODELS; i++) {
    if (model == ANDROIDDATA[i].modelName)
      exists = true;
  }
  return exists;
}

// Returns index of the given field name. If it doesn't exist, returns -1
int getFieldNameIndex(string name, Farm &farm) {
  int index = -1;
  for (unsigned int i = 0; i < farm.fields.size(); i++) {
    if (farm.fields[i].name == name) index = i;
  }
  return index;
}

// Ask for file name, open said file and write given farm data
// Return wether the file was correctly opened
bool writeFarmData(const Farm &farm,int nextSerialNumber,int hour) {
  FarmBin fab;
  FieldBin fib;
  AndroidBin ab;
  bool correct = false;
  string fileName;
  cout << "Enter filename: ";
  cin.ignore();
  getline(cin, fileName);

  ofstream file;
  file.open(fileName.c_str(), ios::out | ios::binary);
  if (file.is_open()) {
    correct = true;
    // TODO: this seems not to build with the autocorrector script
    // Copy a fixed ammount of characters from one string to another
    strncpy(fab.name, farm.name.c_str(), MAXNAME);

    fab.numFields         = farm.fields.size();
    fab.hour              = hour;
    fab.nextSerialNumber  = nextSerialNumber;
    fab.name[MAXNAME - 1] = '\0';

    // Write data to file
    file.write((const char*) &fab, sizeof(fab));
    for (unsigned int i = 0; i < farm.fields.size(); i++) {
      strncpy(fib.name, farm.fields.at(i).name.c_str(), MAXNAME);

      fib.numAndroids       = farm.fields.at(i).androids.size();
      fib.products          = farm.fields.at(i).products;
      fib.hoursLeft         = farm.fields.at(i).hoursLeft;
      fib.name[MAXNAME - 1] = '\0';

      // Write data to file
      file.write((const char*) &fib, sizeof(fib));

      // Androids in each field
      for (int j = 0; j < farm.fields.at(i).androids.size(); j++) {
        strncpy(ab.model, farm.fields.at(i).androids.at(j).model.c_str(), MAXNAME);


        ab.serialNumber = farm.fields.at(i).androids.at(j).serialNumber;
        ab.status = farm.fields.at(i).androids.at(j).status;
        ab.speed = farm.fields.at(i).androids.at(j).speed;
        ab.hoursWorked = farm.fields.at(i).androids.at(j).hoursWorked;
        ab.model[MAXNAME - 1] = '\0';

        // Write data to file
        file.write((const char*) &ab, sizeof(ab));
      }
    }

    file.close();
  } else {
    correct = false;
    error(ERR_OPEN_FILE);
  }
  return correct;
}


// Ask for file name, open said file and read farm data
// Return wether the file was correctly opened
bool readFarmData(Farm &farm, int &nextSerialNumber,int &hour) {
  FarmBin fab;
  FieldBin fib;
  AndroidBin ab;
  Android android;
  Field field;
  bool correct = false;
  string fileName;
  ifstream file;
  cout << "Enter filename: ";
  getline(cin, fileName);
  
  // Opens file
  file.open(fileName.c_str(), ios::in | ios::binary);
  if (file.is_open()) {
    correct = true;
    // TODO: figure out what this does before it breaks something
    //clearFarm(farm);
    file.read((char *) &fab, sizeof(fab));

    farm.name = fab.name;
    hour = fab.hour;
    nextSerialNumber = fab.nextSerialNumber;

    // Add all data for field
    for (int i = 0; i < fab.numFields; i++) {
      file.read((char *) &fib, sizeof(fib));
      field.name = fib.name;
      field.products = fib.products;
      field.hoursLeft = fib.hoursLeft;

      field.androids.clear();

      // Add all data for android
      for (int j = 0; j < fib.numAndroids; j++) {
        file.read((char *) &ab, sizeof(ab));

        android.model        = ab.model;
        android.speed        = ab.speed;
        android.status       = ab.status;
        android.hoursWorked  = ab.hoursWorked;
        android.serialNumber = ab.serialNumber;

        // Add new android to androids vector in field
        field.androids.push_back(android);
      }
      // Add new field to fields vector in farm
      farm.fields.push_back(field);
    }

    // Get data from the androids in the binary farm...
    file.read((char *) &ab, sizeof(ab));
    while (!(file.eof())) {
      android.model        = ab.model;
      android.speed        = ab.speed;
      android.status       = ab.status;
      android.hoursWorked  = ab.hoursWorked;
      android.serialNumber = ab.serialNumber;

      // ...and add them to the "real" farm
      farm.androids.push_back(android);
    }
    
    file.close();
  } else {
    error(ERR_OPEN_FILE);
    correct = false;
  }
  return correct;
}

// Ask for file name, open said file and read android data
// Return wether the file was correctly opened
bool importAndroids(Farm &farm,int &nextSerialNumber, string path) {
  Android android;
  bool success = false; // File reading completion success
  bool correctData = true; // Data from text file correction
  string fileName;
  cout << "Enter filename: ";
  cin.ignore();
  getline(cin, fileName);
  string model; // It's not realistic to use more than 1000 androids
  int speed;
  int numberOfDroids = 0;

  ifstream file;
  file.open(fileName.c_str(), ios::in);
  if (file.is_open()) {
    success = true;
    string line;
    while (!(file.eof())) {
      while (file >> speed) {
        file >> model;
        if (speed < 0) {
          error(ERR_WRONG_SPEED);
          correctData = false;
        }
        if (!(modelExists(model))) {
          error(ERR_WRONG_MODEL);
          correctData = false;
        }
      }
      // If everything is ok, add the android to the farm
      if (correctData) {
        numberOfDroids++;
        android.serialNumber = nextSerialNumber;
        android.speed = speed;
        android.model = model;
        farm.androids.push_back(android);
        nextSerialNumber++;
      }
    }
    file.close();
  } else {
    error(ERR_OPEN_FILE);
    success = false;
  }

  return success;
}

// Process the string with field and product data
void processProducts(string data, Farm &farm) {
  int i = 0;
  string name;
  string s_products;
  int productsIndex = 0;
  int nameIndex = 0;
  int i_products = 0;
  
  while (i < s_products.length()) {
    name = "";
    s_products = "";
  }

  // Find quotes
  while (i < s_products.length() && s_products.at(i) != '"')
    i++;
  
  // Skip quotes
  i++;

  // Get the name
  while (i < s_products.length() && s_products[i] != '"') {
    name += s_products.at(i);
    i++;
  }

  // Find products' value's starting index
  while (s_products.at(i) < '0' || s_products.at(i) > '9')
    i++;
  
  // Get number
  while (i < s_products.length() && s_products.at(i) != ' ') {
    s_products += s_products.at(i);
    i++;
  }

  // Check if the field's name exists
  nameIndex = getFieldNameIndex(name, farm);
  if (nameIndex == -1)
    error(ERR_WRONG_FIELD);
  else {
    // Convert string to integer
    i_products = atoi(s_products.c_str());
    if (productsIndex == 0) 
      error(ERR_WRONG_PRODUCTS);
    else {
      farm.fields.at(nameIndex).products += i_products;
    }
  }

}

// Ask for file name, open said file and read products data
// Return wether the file was correctly opened
bool importProducts(Farm &farm, string path) {
  bool correct = false;
  string fileName;
  string line;
  cout << "Enter filename: ";
  cin.ignore();
  getline(cin, fileName);

  ifstream file;
  file.open(fileName.c_str(), ios::in);
  if (file.is_open()) {
    correct = true;
    getline(file, line);

    while (!(file.eof())) {
      processProducts(line, farm);
      getline(file, line);
    }
    file.close();
  } else {
    error(ERR_OPEN_FILE);
    correct = false;
  }

  return correct;
}

// Returns wether the introduced execution arguments were correct
// and modifies the application's behaviour in consequence
bool processArguments(int argc, char *argv[], Farm &farm, int &nextSerialNumber) {
  string pfn = ""; // Products file name
  string afn = ""; // Androids file name
  bool right = false;
  bool found;
  int correctParams = 0;
  vector<string> fieldVector;
  Field field;

  if (argc % 2 == 1) {
    right = true;
    for (int i = 0 + 1; i < argc; i += 2) {
      if (strcmp(argv[1], "-f") == 0) {
        fieldVector.push_back(argv[i + 1]);
      }
      else {
        if (strcmp(argv[i], "-a") == 0) {
          if (afn != "") {
            correctParams = 0;
          }
          else {
            afn = argv[i + 1];
          }
        }
        else {
          if (strcmp(argv[i], "-p") == 0) {
            if (pfn != "") {
              correctParams = 0;
            }
            else {
              pfn = argv[i + 1];
            }
          }
          else {
            correctParams = 0;
          }
        }
      }
    }
  }

  if (!right) 
    error(ERR_ARGS);
  else {
    if (afn != "")
      importAndroids(farm, nextSerialNumber, afn);

      for (unsigned int i = 0; i < fieldVector.size(); i++) {
        found = false;
        for (int j = 0; j < farm.fields.size(); j++) {
          if (farm.fields.at(j).name == fieldVector.at(i))
            error(ERR_NAME);
        }

        if (!found) {
          field.name = fieldVector[i];
          field.products = 0;
          field.hoursLeft = DAILY_WORKING_HOURS;

          farm.fields.push_back(field);
        }
      }
      if (pfn != "") {
        importProducts(farm, pfn);
      }
  }

  return right;
}

// prints android info
void printAndroid(const Android &android) {
  cout << "[" 
       << android.model 
       <<  "sn=" << android.serialNumber
       <<  " s=" << android.speed
       << " st=" << android.status
       << "]"    << endl;
}

// Print field information
void printField(const Field &field) {
  cout << "{Field: " << field.name << "(" << field.products << ")" << endl;
  for (unsigned int i = 0; i < field.androids.size(); i++) {
    printAndroid(field.androids[i]);
  }
  cout << "}" << endl;
}

// Print farm information
void printFarm(const Farm &farm) {
  cout << "Farm: " << farm.name << endl;
  // Print every field in the farm
  for (unsigned int i = 0; i < farm.fields.size(); i++) {
    printField(farm.fields[i]);
  }
  // Print every android in the farm
  for (unsigned int i = 0; i < farm.androids.size(); i++) {
    printAndroid(farm.androids[i]);
  }
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

// Create a new field in the farm with the name provided by the user
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

// Returns the index of the most suitable field to which send an android to
int nextField(Farm &farm) {
  int nfi;                  // Next Field Index
  unsigned int m = INT_MAX; // Min ammouont of andorids in fields
  int maxProds   = 0;       // Max ammount of products in fields
  vector<Field> fields;

  // Find lowest ammount of androids in a field
  for (unsigned int i = 0; i < farm.fields.size(); i++) {
    if (m > farm.fields[i].androids.size())
      m = farm.fields[i].androids.size();
  }

  // Push all the less populated fields to a vector
  for (unsigned int i = 0; i < farm.fields.size(); i++) {
    if (farm.fields[i].androids.size() == m) fields.push_back(farm.fields[i]);
  }

  // Find highest ammount of products amongst the minimally populated fields
  for (unsigned int i = 0; i < fields.size(); i++) {
    if (maxProds < fields[i].products)
      maxProds = fields[i].products;
  }

  // Find the next field index
  for (unsigned int i = 0; i < farm.fields.size(); i++) {
    if (farm.fields[i].androids.size() == m && farm.fields[i].products == maxProds)
      nfi = i;
  }

  return nfi;
}

// Distributes the farm's androids among its fields
void distributeAndroids(Farm &farm) {
  bool distributable = false; // Are there any fields to distribute androids in
  int nfi;                    // Next Field Index
  int fai;                    // Fastest Android Index
  int maxSpeed = 0;           // Max speed in farm's androids

  cout << HEADER2 << endl;
  // Check if there are fields to collect
  for (unsigned int i = 0; i < farm.fields.size(); i++) {
    if (farm.fields[i].products > 0) distributable = true;
  }

  while (distributable) {
    nfi = nextField(farm);
    // Find fastest android in farm
    for (unsigned int i = 0; i < farm.androids.size(); i++) {
      if (farm.androids[i].status == ST_IDLE && farm.androids[i].speed > maxSpeed) {
        maxSpeed = farm.androids[i].speed;
        fai = i;
      }
    }

    // Put fastest android to work on determined field
    farm.fields[nfi].androids[fai].status = ST_WORKING;
    farm.fields[nfi].androids.push_back(farm.androids[fai]);
    farm.androids.erase(farm.androids.begin() + fai);
  }
}

// Simulates the collection of products in a field by its androids
void collectField(Field &field) {
  int workingHours = 5; // This might change in the future
  int totalProducts = 0;
  // Calculate the ammount of collected products by multiplying
  // each android's speed (in products/hour) by the ammount of working hours
  for (unsigned int i = 0; i < field.androids.size(); i++) {
    totalProducts += field.androids[i].speed * workingHours;
  }

  // Subtract the total collectable products from the field's products
  field.products -= totalProducts;
  if (field.products < 0) field.products = 0;

  // Finish working session by setting all androids to idle status...
  for (unsigned int i = 0; i < field.androids.size(); i++) {
    field.androids[i].status = ST_IDLE;
  }
}

// Move androids from field to farm
void returnAndroids(Field field, Farm farm) {
  for (unsigned int i = 0; i < field.androids.size(); i++) {
    farm.androids.push_back(field.androids[i]);
    field.androids.erase(field.androids.begin() + i);
  }
}

// Collects the products in the farm's fields
void collectFarm(Farm &farm, int &hour) {
  cout << SEPARATOR << endl;
  cout << HOUR << hour << endl;

  cout << HEADER1 << endl;
  printFarm(farm);
  distributeAndroids(farm);
  // Collect every field in the farm
  for (unsigned int i = 0; i < farm.fields.size(); i++) {
    collectField(farm.fields[i]);
    // Return androids from fields to farm
    returnAndroids(farm.fields[i], farm);
  }

  cout << HEADER3 << endl;
  printFarm(farm);
}

// TODO: refactor this to startworkinghour
// Ask for products data in the farm's fields, then collect them
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
      
      //collectFarm(farm, hour);
    }
}

// Ask for products data in the farm's fields, then collect them
void startWorkingHour(Farm &f,int &hour) {
  string data;
  if (f.fields.size() <= 0)
    error(ERR_NO_FIELDS);
}

void menu(int hour) {
  cout << "-----========== Farm manager ==========-----" << endl
       << "1- List farm info"                            << endl
       << "2- Add field"                                 << endl
       << "3- Add android"                               << endl
       << "4- Start working hour (" << hour << ")"       << endl
       << "5- Write farm data"                           << endl
       << "6- Read farm data"                            << endl
       << "7- Import androids"                           << endl
       << "8- Import products"                           << endl
       << "q- Quit"                                      << endl
       << "Option: " ;
}

int main(int argc,char *argv[]) {
  Farm farm;
  farm.name = "west farm";
  char option;
  int nextSerialNumber = 100;
  int hour = 1;

  if (processArguments(argc,argv,farm,nextSerialNumber))
    do {
      menu(hour);
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
          if (farm.fields.size() != 0)
            startWorkingHour(farm, hour);
          else
            error(ERR_NO_FIELDS);
          break;
        }
        case '5': {
          writeFarmData(farm, nextSerialNumber, hour);
          break;
        }
        case '6': {
          readFarmData(farm, nextSerialNumber, hour);
          break;
        }
        case '7': {
          importAndroids(farm, nextSerialNumber, "");
          break;
        }
        case '8': {
          importProducts(farm, "");
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