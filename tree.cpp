#include "tree.h"



bool trace = false;

void Tree::run() {
  int getNum = 6;
  int getMethod = 6;
  input.buildPuzzle();
  solu = input.solution;
  while(getNum != 3) {
    input.menu(getNum, getMethod);
    if (getNum == 3) {
      break;
    }
    state = input.puzzle;
    Node* begin = new Node(state);
    if (getNum == 1) {
      trace = true;
    }
    Node* done = graphSearch(begin, getMethod);
    while (done != 0) {
      input.printStatus(done->place);
      done = done->parent;
      cout << "-----\n";
    }
    //input.rerun(getNum);
    //resetPuzzle();
    getNum = 3;
  }
}

void Tree::resetPuzzle() {
  for (int i = 0; i < input.puzzle.size(); i++) {
    for (int j = 0; j < input.puzzle.at(i).size(); j++) {
      input.puzzle.at(i).at(j) = 0;
    }
  }
}

Node* Tree::graphSearch(Node* root, int method) {
  root->setH(heuristic(root, method));
  Node* curr;
  bool first = true;
  long count = 0;
  long queuecount = 1;
  long maxqueue = queuecount;
  pq.push(root);
  while(!pq.empty()) {
    //take node with least cost
    curr = pq.top();
    queuecount--;
    pq.pop();
    count++;

    if (trace) {
      if (first) {
        cout << "Expanding state\n";
        first = false;
      }
      else {
        cout << "The best state to expand with g(n) = " << curr->getG() << " and h(n) = " << curr->getH() << " is...\n";
      }
      input.printStatus(curr->place);
      cout << "Expanding this node...\n";
    }

    if (goalcheck(curr)) {
      cout << "\nSolution found.\n# of Explored Nodes: " << count << "\nMax Queue Size: " << maxqueue << "\nSteps in reverse order: \n\n";
      return curr;
    }
    
      

    vector<Node*> next = genChildren(curr);
    
    for (int i = 0; i < next.size(); i++) {
      int nextCost = curr->getG() + 1;

      if (isinPG(next.at(i))) { //if child is in the list of to-be explored nodes
        if (next.at(i)->getG() <= nextCost) {
          continue;
        }
      }
      else if (isinDone(next.at(i))) { //if child has already been expanded
        if (next.at(i)->getG() <= nextCost) {
          continue;
        }
        //if cost is lower, than move node back to the priority queue
        int index = 0;
        for (int i = 0; i < done.size(); i++) {
          if (done.at(i) == next.at(i)) {
            index = i;
            break;
          }
        }
        done.erase(done.begin() + index);
        pq.push(next.at(i));
        queuecount++;
        if (queuecount > maxqueue) {
          maxqueue = queuecount;
        }
      }  
      else {
        //set g(n) and h(n) costs and push node to priority queue
        next.at(i)->setG(nextCost);
        next.at(i)->setH(heuristic(next.at(i), method));
        pq.push(next.at(i));
        queuecount++;
        if (queuecount > maxqueue) {
          maxqueue = queuecount;
        }
      }
      next.at(i)->setG(nextCost);
      next.at(i)->parent = curr;
    }
    //push searched node to expanded node list
    done.push_back(curr);
  }
  if (!goalcheck(curr)) {
    cout << "Error. No solution found\n";
  }
  return 0;
}

bool Tree::goalcheck(Node* node) {
  //checks if current state matches goal state
  for (int i = 0; i < node->place.size(); i++) {
    for (int j = 0; j < node->place.at(i).size(); j++) {
      if (node->place.at(i).at(j) != solu.at(i).at(j)) {
        return false;
      }
    }
  }
  return true;
}

vector<Node*> Tree::genChildren(Node* node) {
  //makes children of node
  int row = 0;
  int column = 0;
  vector<Node*> result;

  //find where 0 is
  for (int i = 0; i < node->place.size(); i++) {
    for (int j = 0; j < node->place.at(i).size(); j++) {
      if (node->place.at(i).at(j) == 0) {
        row = i;
        column = j;
      }
    }
  }
  
  //based on location of 0, make children
  if (row <= 1) { //make "down" child
    vector<vector<int>> temp = node->place;
    swap(temp[row][column], temp[row + 1][column]);
    node->down = new Node(temp, node);
    result.push_back(node->down);
  }
  if (row >= 1) { //make "up" child
    vector<vector<int>> temp = node->place;
    swap(temp[row][column], temp[row - 1][column]);
    node->up = new Node(temp, node);
    result.push_back(node->up);
  }
  if (column <= 1) { //make "right" child
    vector<vector<int>> temp = node->place;
    swap(temp[row][column], temp[row][column + 1]);
    node->right = new Node(temp, node);
    result.push_back(node->right);
  }
  if (column >= 1) { // make "left" child
    vector<vector<int>> temp = node->place;
    swap(temp[row][column], temp[row][column - 1]);
    node->left = new Node(temp, node);
    result.push_back(node->left);
  }
  return result;
}
    
bool Tree::isinPG(Node* curr){
  //check whether node is in the priority queue
  priority_queue<Node*,vector<Node*>, compare> temp = pq;
  vector<Node*> tempv;
  while(!temp.empty()) {
    tempv.push_back(temp.top());
    temp.pop();
  }
  for (int i = 0; i < tempv.size(); i++) {
    if (isMatch(curr, tempv.at(i))) {
      return true;
    }
  }
  return false;
}

bool Tree::isinDone(Node* curr){
  //check whether the node has already been explored
  for (int i = 0; i < done.size(); i++) {
    if (isMatch(curr, done.at(i))) {
      return true;
    }
  }
  return false;
}

bool Tree::isMatch(Node* a, Node* b) {
  //checks if 2 nodes are the same state
  vector<vector<int>> vecA, vecB;
  vecA = a->place;
  vecB = b->place;
  for (int i = 0; i < vecA.size(); i++) {
    for (int j = 0; j < vecA[0].size(); j++) {
      if (vecA[i][j] != vecB[i][j]) {
        return false;
      }
    }
  }
  return true;
}

double Tree::heuristic(Node* node, int method) {
  if (method == 1) { //Uniform Cost
    return 0;
  }
  else if (method == 2) { //Misplaced Tile
    vector<vector<int>> v = node->place;
    int count = 0;
    for (int i = 0; i < v.size(); i++) {
      for (int j = 0; j < v[0].size(); j++) {
        if (v[i][j] == 0) {
          continue;
        }
        if (v[i][j] != solu[i][j]) {
          count++;
        }
      }
    }
    return count;
  }
  else { //method == 3, Eucledian Distance
    vector<vector<int>> v = node->place;
    int count = 0;
    int row1, column1, row2, column2;
    for (int i = 0; i < 9; i++) {
      row1 = i / 3;
      column1 = i % 3;
      for (int j = 0; j < v.size(); j++) {
        for (int k = 0; k < v[0].size(); k++) {
          if (i == 8 && v[j][k] == 0) {
            row2 = j;
            column2 = k;
          }
          else if (v[j][k] == i + 1) {
            row2 = j;
            column2 = k;
          }
        }
      }
      count += sqrt(pow((row2 - row1), 2) + pow((column2 - column1), 2));
    }
    return count;
    count = 0;
  }
  return 0;
}