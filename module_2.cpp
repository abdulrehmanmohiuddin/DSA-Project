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
//           Core Helper: Date to Day
// ==========================================

/**
 * @brief Converts a date string (DD-MM-YYYY) to a Day Index (0=Mon...6=Sun).
 * Assumes a simple calculation or manual string parsing.
 * For this project, we will use a robust standard C algorithm.
 */
int MenuManager::getDayIndexFromDate(string dateStr) {
    // Format expected: DD-MM-YYYY (e.g., 21-11-2024)
    int d, m, y;
    char dash; // to consume the '-'
    stringstream ss(dateStr);
    
    if (ss >> d >> dash >> m >> dash >> y) {
        // Zeller's Congruence Algorithm for Day of Week
        if (m < 3) { m += 12; y -= 1; }
        int k = y % 100;
        int j = y / 100;
        int h = (d + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
        
        // Zeller: 0=Sat, 1=Sun, 2=Mon ... 6=Fri
        // We want: 0=Mon ... 6=Sun
        int mapToMySystem[] = {5, 6, 0, 1, 2, 3, 4}; 
        return mapToMySystem[h];
    }
    return -1; // Error
}

bool MenuManager::isValidDate(string date) {
    return (getDayIndexFromDate(date) != -1);
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

MenuItem* MenuManager::search(MenuItem* node, string name) {
    if (node == NULL || node->itemName == name) return node;
    if (name < node->itemName) return search(node->left, name);
    return search(node->right, name);
}

MenuItem* MenuManager::findMin(MenuItem* node) {
    while (node->left != NULL) node = node->left;
    return node;
}

MenuItem* MenuManager::deleteItem(MenuItem* node, string name) {
    if (node == NULL) return node;
    
    if (name < node->itemName) 
        node->left = deleteItem(node->left, name);
    else if (name > node->itemName) 
        node->right = deleteItem(node->right, name);
    else {
        // Found node
        if (node->left == NULL) {
            MenuItem* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == NULL) {
            MenuItem* temp = node->left;
            delete node;
            return temp;
        }
        MenuItem* temp = findMin(node->right);
        node->itemName = temp->itemName;
        node->category = temp->category;
        node->right = deleteItem(node->right, temp->itemName);
    }
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

void MenuManager::addFoodItem() {
    cout << "\n--- Add New Item to Database ---\n";
    cout << "Enter Item Name (e.g., Biryani): ";
    string name; getline(cin, name);
    
    if (search(root, name) != NULL) {
        cout << "Error: '" << name << "' already exists in the database.\n";
        return;
    }
    
    cout << "Enter Category (Breakfast/Lunch/Dinner): ";
    string cat; getline(cin, cat);
    
    // Note: We don't ask for price here anymore, as per your instruction.
    // The price is set per DAY, not per ITEM.
    
    MenuItem* newItem = new MenuItem(name, cat);
    root = insert(root, newItem);
    cout << "Item added to Master Database successfully.\n";
    saveFiles();
}

void MenuManager::deleteFoodItem() {
    cout << "Enter Item Name to delete: ";
    string name; getline(cin, name);
    
    if (search(root, name) == NULL) {
        cout << "Error: Item not found.\n";
        return;
    }
    root = deleteItem(root, name);
    cout << "Item deleted from database.\n";
    saveFiles();
}

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

void MenuManager::setDailyMenu() {
    cout << "\n--- Set Weekly Menu Schedule ---\n";
    cout << "Select Day to Update (or 0 to cancel):\n";
    for(int i=0; i<7; i++) cout << i+1 << ". " << days[i] << endl;
    
    int choice;
    cout << "Choice: ";
    if (!(cin >> choice)) {
        cout << "Invalid input.\n";
        cin.clear(); cin.ignore(10000,'\n');
        return;
    }
    cin.ignore(); // clear buffer
    
    if (choice == 0) return; // FIX: Allow exiting immediately
    if (choice < 1 || choice > 7) {
        cout << "Invalid choice.\n";
        return;
    }
    
    int dayIdx = choice - 1;
    cout << "\nUpdating Menu for: " << days[dayIdx] << endl;
    cout << "(Enter '0' at any time to cancel and go back)\n";
    
    // 1. Breakfast
    string b;
    while(true) {
        cout << "Set Breakfast Item: "; getline(cin, b);
        if (b == "0") return; // FIX: Escape hatch
        if (search(root, b) != NULL) break;
        cout << "Error: '" << b << "' not found in DB. You must ADD it first (Option 2).\n";
    }
    
    // 2. Lunch
    string l;
    while(true) {
        cout << "Set Lunch Item: "; getline(cin, l);
        if (l == "0") return; // FIX: Escape hatch
        if (search(root, l) != NULL) break;
        cout << "Error: '" << l << "' not found in DB.\n";
    }
    
    // 3. Dinner
    string d;
    while(true) {
        cout << "Set Dinner Item: "; getline(cin, d);
        if (d == "0") return; // FIX: Escape hatch
        if (search(root, d) != NULL) break;
        cout << "Error: '" << d << "' not found in DB.\n";
    }
    
    // 4. Set Daily Price
    float price = getValidatedFloat("Enter Total Daily Rate (Rs) for this day: ");
    
    // Update Array
    weeklySchedule[dayIdx].breakfast = b;
    weeklySchedule[dayIdx].lunch = l;
    weeklySchedule[dayIdx].dinner = d;
    weeklySchedule[dayIdx].dailyPrice = price;
    
    cout << "Menu for " << days[dayIdx] << " updated successfully!\n";
    saveFiles();
}

void MenuManager::runAdminMenu() {
    int choice;
    do {
        cout << "\n==== Menu Management (Admin) ====\n";
        cout << "1. View Food Database (BST)\n";
        cout << "2. Add New Food Item (to DB)\n";
        cout << "3. Delete Food Item (from DB)\n";
        cout << "4. Set Daily Menu & Price\n";
        cout << "5. View Weekly Schedule\n";
        cout << "6. Back\n";
        cout << "Choice: ";
        
        // --- INPUT FIX START ---
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();           // Reset error flag
            cin.ignore(10000, '\n'); // Clear the invalid input
            choice = 0;            // Set dummy value to loop again
        } else {
            cin.ignore(10000, '\n'); // Clear buffer on success
        }
        // --- INPUT FIX END ---
        
        switch(choice) {
            case 1: displayFoodDB(); break;
            case 2: addFoodItem(); break;
            case 3: deleteFoodItem(); break;
            case 4: setDailyMenu(); break;
            case 5: 
                for(int i=0; i<7; i++) {
                    cout << days[i] << ": " << weeklySchedule[i].dailyPrice << " Rs\n";
                    cout << "   [B] " << weeklySchedule[i].breakfast 
                         << " [L] " << weeklySchedule[i].lunch 
                         << " [D] " << weeklySchedule[i].dinner << endl;
                }
                break;
            case 6: return;
        }
    } while (choice != 6);
}

// ==========================================
//           Student Interface
// ==========================================

void MenuManager::displayDayMenu(string date) {
    int idx = getDayIndexFromDate(date);
    if (idx == -1) {
        cout << "Error: Invalid Date Format (Use DD-MM-YYYY)\n";
        return;
    }
    
    cout << "\n==== Menu for " << days[idx] << " (" << date << ") ====\n";
    cout << "------------------------------------------\n";
    cout << "Breakfast:  " << weeklySchedule[idx].breakfast << endl;
    cout << "Lunch:      " << weeklySchedule[idx].lunch << endl;
    cout << "Dinner:     " << weeklySchedule[idx].dinner << endl;
    cout << "------------------------------------------\n";
    cout << "Daily Rate: Rs. " << fixed << setprecision(2) << weeklySchedule[idx].dailyPrice << endl;
    cout << "------------------------------------------\n";
}

float MenuManager::getDailyPrice(string date) {
    int idx = getDayIndexFromDate(date);
    if (idx != -1) return weeklySchedule[idx].dailyPrice;
    return 0.0;
}

// ==========================================
//           File Persistence
// ==========================================

void MenuManager::saveMenuRecursive(MenuItem* node, ofstream& file) {
    if (node != NULL) {
        file << node->itemName << "," << node->category << "\n";
        saveMenuRecursive(node->left, file);
        saveMenuRecursive(node->right, file);
    }
}

void MenuManager::saveFiles() {
    // 1. Save DB
    ofstream dbFile(MENU_DB_FILE.c_str());
    if (dbFile.is_open()) {
        saveMenuRecursive(root, dbFile);
        dbFile.close();
    }
    
    // 2. Save Schedule
    ofstream schFile(WEEKLY_FILE.c_str());
    if (schFile.is_open()) {
        for(int i=0; i<7; i++) {
            schFile << weeklySchedule[i].breakfast << ","
                    << weeklySchedule[i].lunch << ","
                    << weeklySchedule[i].dinner << ","
                    << weeklySchedule[i].dailyPrice << "\n";
        }
        schFile.close();
    }
}

void MenuManager::loadFiles() {
    // 1. Load DB
    ifstream dbFile(MENU_DB_FILE.c_str());
    string line;
    if (dbFile.is_open()) {
        while(getline(dbFile, line)) {
            stringstream ss(line);
            string name, cat;
            if (getline(ss, name, ',') && getline(ss, cat, ',')) {
                root = insert(root, new MenuItem(name, cat));
            }
        }
        dbFile.close();
    }
    
    // 2. Load Schedule
    ifstream schFile(WEEKLY_FILE.c_str());
    if (schFile.is_open()) {
        for(int i=0; i<7; i++) {
            if(getline(schFile, line)) {
                stringstream ss(line);
                string b, l, d, pStr;
                getline(ss, b, ',');
                getline(ss, l, ',');
                getline(ss, d, ',');
                getline(ss, pStr, ',');
                
                weeklySchedule[i].breakfast = b;
                weeklySchedule[i].lunch = l;
                weeklySchedule[i].dinner = d;
                weeklySchedule[i].dailyPrice = (float)atof(pStr.c_str());
            }
        }
        schFile.close();
    }
}