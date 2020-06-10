#include "input.h"
#include "tree.h"



void Input::buildPuzzle() {
  //initialize puzzle and solution vectors
  for (int i = 0; i < 3; i++) {
    vector<int> add = {10,10,10};
    vector<int> sol = {3 * i + 1, 3 * i + 2, 3 * i + 3};
    puzzle.push_back(add);
    solution.push_back(sol);
  }
  int last = solution.size() - 1;
  solution.at(last).at(solution.at(last).size() - 1) = 0;
}

void Input::menu(int& getNum, int& getMethod) {
  cout << "Which Puzzle do you want to solve?\n";
  cout << "1: Default 8-Puzzle\n2: Your own 8-Puzzle\n3: Exit\n" << bar;
  cin >> getNum;

  if (getNum == 1) {
    cout << "Default puzzle chosen.\n";
    cout << space << "1 0 3\n";
    cout << space << "4 2 6\n";
    cout << space << "7 5 8\n";
    //insert
    extract("1 0 3", "4 2 6", "7 5 8");
    //choose method
    cout << "\nWhich Method?\n1. Uniform Cost\n2. A* w/ Misplaced Tile\n3. A* w/ Eucledian Distance\n" << bar;
    cin >> getMethod;
    cout << "Compiling Solution...\n";
  }
  else if (getNum == 2) {
    string row1, row2, row3;
    enterMatrix(row1, row2, row3);
    cout << "Which Method?\n1. Uniform Cost\n2. A* w/ Misplaced Tile\n3. A* w/ Eucledian Distance\n" << bar;
    cin >> getMethod;
    cout << "Compiling Solution...\n";
  }
  else if (getNum == 3) {
    cout << "Exiting.\n";
  }
  else {
    cout << "Invalid Choice.\n";
    cin.clear();
    cin.ignore(10000, '\n');
  }
  
  cout << "\n";
}

void Input::enterMatrix(string& row1, string& row2, string& row3) {
  //function to enter elements of custom matrix
  string s = "";
  bool verified = false;
  bool extracted = false;
  cin.clear();
  cin.ignore(10000, '\n');
  //clear buffer
  while (s != "y") {
    cout << "Enter your puzzle, one row at a time.\nUse 0 to represent a blank.\nUse spaces to seperate numbers.\n\n";
    cout << "Row 1: ";
    getline(cin, row1);
    cout << "Row 2: ";
    getline(cin, row2);
    cout << "Row 3: ";
    getline(cin, row3); //enter rows as strings
    extracted = extract(row1, row2, row3);
    if (!extracted) {
      cout << "Verification/Extraction failed.\n" << bar;
      continue;
    } 
    cout << "\nInput: \n";
    printStatus(puzzle);
    cout << "\nIs this correct? (y/n)\n";
    getline(cin, s);
    cout << "\n";
  }
  
}

bool Input::extract(string row1, string row2, string row3) {
  //creates puzzle entries 
  unordered_map<int, int> maps;
  vector<string> r1,r2,r3;
  vector<int> i1, i2, i3;
  
  //tokenize strings and place them into string vectors
  boost::split(r1, row1, boost::is_any_of(" "));
  boost::split(r2, row2, boost::is_any_of(" "));
  boost::split(r3, row3, boost::is_any_of(" "));

  if (!verify(r1, i1, maps) || !verify(r2, i2, maps) || !verify(r3, i3, maps)) {
    return false; //if strings have error return false
  }
  //else copy extracted vectors to global puzzle and return true
  puzzle.at(0) = i1;
  puzzle.at(1) = i2;
  puzzle.at(2) = i3;
  return true;
}

bool Input::verify(vector<string> before, vector<int>& after, unordered_map<int,int> &maps) {
  //verifies that entries are single digit numbers without duplicates
  char stemp;
  int temp = 0;
  if (before.size() != 3) {
    return false;
  }
  for (int i = 0; i < before.size(); i++) {
    istringstream iss(before.at(i));
    //checks for 1 character
    if (before.at(i).size() != 1) {
      return false;
    }
    iss >> stemp;
    temp = stemp - 48;
    //makes sure character entered is 0-8 (48-56)
    if (temp < 0 || temp > 8) {
      return false;
    }
    //checks for duplicate entries
    if (maps.find(temp) != maps.end()) {
      return false;
    }
    //add to list
    maps.emplace(temp,1);
    after.push_back(temp);
  }
  return true;
}

void Input::printStatus(vector<vector<int>> v) {
  //prints all entries in matrix format
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v.at(i).size(); j++) {
      cout << v.at(i).at(j) << " ";
    }
    cout << endl;
  }
}

void Input::rerun(int& num) {
  char c;
  cout << "Solve another puzzle? (y/n)\n";
  cin >> c;
  if (c == 'n') {
    cout << "Exiting.\n";
    num = 3;
  }
}

    