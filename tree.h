#ifndef TREE_H
#define TREE_H
#include "includes.h"
#include "input.h"

class Node {
  public:
    Node* parent = 0;
    Node* up = 0;
    Node* down = 0;
    Node* left = 0;
    Node* right = 0;
    Node(vector<vector<int>> v): place(v) {};
    Node(vector<vector<int>> v, Node* p): place(v), parent(p) {};
    double getG() {return gCost;}
    double getH() {return hCost;}
    void setG(double g) {gCost = g;}
    void setH(double h) {hCost = h;}
    void printPlace();
    vector<vector<int>> place;
  
  private:
    double gCost = 0;
    double hCost = 0;
};

struct compare {
  bool operator()(Node* a, Node* b) {
    //swap if a's path cost is higher than b's path cost 
    return a->getG() + a->getH() > b->getG() + b->getH();
  }
};

class Tree {
  public:
    vector<vector<int>> state;
    vector<vector<int>> solu;
    void run();
    Node* graphSearch(Node*, int);
    bool goalcheck(Node*);
    vector<Node*> genChildren(Node*);
    bool isinPG(Node*);
    bool isinDone(Node*);
    bool isMatch(Node*, Node*);
    double heuristic(Node*, int);
    void resetPuzzle();

  private:
    Input input;
    vector<Node*> done;
    priority_queue<Node*,vector<Node*>, compare> pq; 
};



#endif