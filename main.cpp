/* 

Nathan Zhou
4/28/23

Red-black tree implementation, part 1 - insertion

*/

#include <iostream>
#include <fstream>
#include <cstring>

const int BLACK = 0;
const int RED = 1;

struct RBNode {
  RBNode* parent;
  RBNode* left;
  RBNode* right;
  bool color;
  int value;
};


//rotate to bring node down left/right
//0 left rotation, 1 right
void rotate(RBNode*& root, RBNode* subtree, bool dir) {
  RBNode* grandparent = subtree->parent;
  RBNode* s = (dir ? subtree->left : subtree->right); //child of node that will take its spot
  subtree->parent = s;
  s->parent = grandparent;
  if (grandparent != nullptr) { //not root
    if (grandparent->left == subtree) grandparent->left = s;
    else grandparent->right = s;
  }
  else root = s;
  
  if (!dir) { //left
    subtree->right = s->left;
    if (subtree->right != nullptr) subtree->right->parent = subtree;
    s->left = subtree;
    
  }
  else { //right
    subtree->left = s->right;
    if (subtree->left != nullptr) subtree->left->parent = subtree;
    s->right = subtree;
  }
}

//insert a new node into tree
void insert(RBNode*& root, RBNode* n) {
  RBNode* prev = nullptr;
  RBNode* curr = root;
  while (curr != nullptr) { //find location
    prev = curr;
    if (n->value > curr->value)
      curr = curr->right; //larger values on right, smaller on left
    else curr = curr->left;
  }

  n->color = RED;
  n->parent = prev;
  RBNode* parent;
  RBNode* grandparent;
  RBNode* uncle;
  parent = prev;
  if (parent == nullptr) { //tree is empty, n should be black root
    n->color = BLACK;
    root = n;
    return;
  }

  //add to tree
  if (n->value > parent->value) {
    parent->right = n;
  }
  else {
    parent->left = n;
  }

  //check conditions
  if (parent->color == BLACK) { //no change
    return;
  }

  //balance until all conditions are fulfilled
  while (n->parent != nullptr && n->parent->color != BLACK && n->color == RED) {
    parent = n->parent;
    grandparent = parent->parent;
    uncle = (grandparent->left == parent ? grandparent->right : grandparent->left);
    
    //parent and uncle are red
    if (uncle != nullptr && uncle->color == RED) {
      parent->color = BLACK;
      uncle->color = BLACK;
      grandparent->color = RED;
      n = grandparent; //reassign and repeat steps to recolor

      root->color = BLACK;
      continue;
    }

    //uncle is black
    if (grandparent->right == parent) {
      if (parent->right == n) { //P is right child of G, n is right child of P
  rotate(root, grandparent, 0); //left rotate to make G sibling of n
  grandparent->color = RED;
  parent->color = BLACK;
      }
      else { //n is left child of P
  rotate(root, parent, 1); //rotate to bring all nodes to right, reduce case to previous
  rotate(root, grandparent, 0);
  grandparent->color = RED;
  n->color = BLACK;
      }
    }
    if (grandparent->left == parent) { //mirror of first case
      if (parent->left == n) {
  rotate(root, grandparent, 1);
  grandparent->color = RED;
  parent->color = BLACK;
      }
      else { //mirror of second case
  rotate(root, parent, 0);
  rotate(root, grandparent, 1);
  grandparent->color = RED;
  n->color = BLACK;
      }
    }
  }
}

/*
//find an element in tree
RBNode* search(RBNode*& root, int value) {
  RBNode* current = root;
  while (current != nullptr && current->value != value) {
    if (current->value > value) current = current->left;
    else current = current->right;
  }
  return current;
}

//replace node and set position of new node
void shift(RBNode*& root, RBNode* del, RBNode* rep) {
  if (rep != nullptr) rep->parent = del->parent;
  
  if (del->parent == nullptr) { //deleting root
    root = rep;
    return;
  }
  
  if (del == (del->parent->left)) del->parent->left = rep; //deleted node is left
  if (del == del->parent->right) del->parent->right = rep; //deleted node is right
}

//remove a specific node

void remove(RBNode*& root, RBNode* node) {

  if (node->left == nullptr) { //right child exists or leaf node
    shift(root, node, node->right);
  }
  else if (node->right == nullptr) { //left child only
    shift(root, node, node->left);
  }

  else { //node has two children
    RBNode* successor = node->right; //find smallest node greater than node to be deleted
    while (successor->left != nullptr) successor = successor->left;
    if (successor->parent != node) { //successor is not immediate right child
      shift(root, successor, successor->right);
      successor->right = node->right;
      successor->right->parent = successor;
    }
    //immediate right child
    shift(root, node, successor);
    successor->left = node->left;
    successor->left->parent = successor;
  }
  delete node;
}
*/

//print out the tree
void display(RBNode* current, int depth) {
  if (current->right != nullptr) display(current->right, depth+1);
  
  //output value and color
  for (int i = 0; i < depth; ++i) std::cout << '\t';
  std::cout << current->value << " " << (current->color ? "R" : "B") << "\n\n";

  if (current->left != nullptr) display(current->left, depth+1);

}

int main() {
  RBNode* root = nullptr;

  char input[10];
  int num;
  int n;

  std::cout << "Choose input method ('1' for manual entry; '2' for file):\n";
  std::cin >> input;
  std::cout << "Enter the input size (1 - 100):\n";
  std::cin >> num;
  
  if (input[0] == '1') { //take from input stream
    std::cout << "Enter the series of numbers:\n";
    for (int i = 1; i <= num; ++i) {
      std::cin >> n;
      RBNode* newn = new RBNode();
      newn->value = n;
      insert(root, newn);
    }
  }

  if (input[0] == '2') { //take from file
    std::cout << "Enter the file name:\n";
    std::cin >> input;
    
    std::ifstream ifs;
    ifs.open(std::strcat(input, ".txt"));
    for (int i = 1; i <= num; ++i) {
      ifs >> n;
      RBNode* newn = new RBNode();
      newn->value = n;
      insert(root, newn);
    }
    ifs.close();
  }

  bool running = true;
  while (running) {
    std::cout << "Enter a command (add, print, quit):\n";
    std::cin >> input;

    if (strncmp(input, "add", 3) == 0) {
      std::cout << "Enter the number to be added:\n";
      std::cin >> n;
      RBNode* newn = new RBNode();
      newn->value = n;
      insert(root, newn);
      std::cout << n << " added to tree\n";
    }
    /*
    if (strncmp(input, "remove", 6) == 0) {
      std::cout << "Enter the number to be removed:\n";
      std::cin >> n;
      remove(root, search(root, n));
      std::cout << n << " removed from tree\n";
    }
    if (strncmp(input, "search", 6) == 0) {
      std::cout << "Enter the number you are looking for:\n";
      std::cin >> n;
      if (!search(root, n)) std::cout << n << " is not in the tree\n";
      else std::cout << n << " is in the tree\n";
    }
    */
    if (strncmp(input, "print", 6) == 0) {
      display(root, 0);
    }
    if (strncmp(input, "quit", 4) == 0) {
      running = false;
    }
  }
  return 0;
}
