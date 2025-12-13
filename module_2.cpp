// module_2.cpp

#include "module_2.hpp"
#include <sstream>
#include <algorithm>

// ==========================================
//          Constructor & Destructor
// ==========================================

MenuManager::MenuManager() {
    root = NULL;
    days[0] = "Monday"; days[1] = "Tuesday"; days[2] = "Wednesday";
    days[3] = "Thursday"; days[4] = "Friday"; days[5] = "Saturday";
    days[6] = "Sunday";
}

MenuManager::~MenuManager() {
    clearBST(root);
}

void MenuManager::clearBST(MenuItem* node) {
    if (node != NULL) {
        clearBST(node->left);
        clearBST(node->right);
        delete node;
    }
}

// ==========================================
//           BST Operations (Database)
// ==========================================

MenuItem* MenuManager::insert(MenuItem* node, MenuItem* newItem) {
    if (node == NULL) return newItem;
    if (newItem->itemName < node->itemName) 
        node->left = insert(node->left, newItem);
    else if (newItem->itemName > node->itemName) 
        node->right = insert(node->right, newItem);
    return node;
}

void MenuManager::inorder(MenuItem* node) {
    if (node != NULL) {
        inorder(node->left);
        cout << left << setw(25) << node->itemName << "| " << node->category << endl;
        inorder(node->right);
    }
}

// ==========================================
//           Admin Functions
// ==========================================

void MenuManager::displayFoodDB() {
    cout << "\n==== Master Food Database (BST) ====\n";
    if (root == NULL) {
        cout << "(Database is empty)\n";
    } else {
        cout << left << setw(25) << "Item Name" << "| Category\n";
        cout << "------------------------------------------\n";
        inorder(root);
    }
    cout << "------------------------------------------\n";
}
