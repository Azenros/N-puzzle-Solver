#ifndef INPUT_H
#define INPUT_H
#include "includes.h"

class Input {
  public:
    bool extract(string, string, string);
    bool verify(vector<string>, vector<int>&, unordered_map<int,int>&);
    void buildPuzzle();
    void printStatus(vector<vector<int>>);
    void enterMatrix(string&, string&, string&);
    void menu(int&, int&);
    void rerun(int&);
    //to make the program look nice
    string space = "  ";
    string bar = "--------------------------------------\n";

    //gloabl objects
    vector<vector<int>> puzzle;
    vector<vector<int>> solution;

};

#endif