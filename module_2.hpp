//module_2.hpp (Header File)

#ifndef MODULE2_MENUMANAGEMENT_H
#define MODULE2_MENUMANAGEMENT_H

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime> // For date handling

using namespace std;

// --- Constants ---
const string MENU_DB_FILE = "menu.txt";
const string WEEKLY_FILE = "weeklymenu.txt";

// --- Helper Functions ---
// Validates positive float input
inline float getValidatedFloat(string prompt) {
    float value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= 0) {
                cin.ignore(10000, '\n'); // Clear buffer
                return value;
            } else {
                cout << "Error: Price cannot be negative.\n";
            }
        } else {
            cout << "Error: Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

// --- BST Node: Food Item ---
struct MenuItem {
    string itemName;
    string category; // Breakfast, Lunch, Dinner (Informational only now)
    
    MenuItem* left;
    MenuItem* right;

    MenuItem(string name, string cat) {
        itemName = name;
        category = cat;
        left = NULL;
        right = NULL;
    }
};

// --- Daily Menu Structure ---
struct DailyMenu {
    string breakfast;
    string lunch;
    string dinner;
    float dailyPrice; // The specific rate for this entire day

    DailyMenu() {
        breakfast = "N/A";
        lunch = "N/A";
        dinner = "N/A";
        dailyPrice = 0.0;
    }
};

// --- Menu Manager Class ---
class MenuManager {
private:
    MenuItem* root;
    DailyMenu weeklySchedule[7]; // 0=Mon, 6=Sun
    string days[7];

    // --- BST Private Helpers ---
    MenuItem* insert(MenuItem* node, MenuItem* newItem);
    MenuItem* search(MenuItem* node, string name);
    MenuItem* deleteItem(MenuItem* node, string name);
    MenuItem* findMin(MenuItem* node);
    void inorder(MenuItem* node);
    void saveMenuRecursive(MenuItem* node, ofstream& file);
    void clearBST(MenuItem* node);

    // --- Date Helper ---
    int getDayIndexFromDate(string dateStr); // Returns 0-6 based on date

public:
    MenuManager();
    ~MenuManager();

    // Admin Functions
    void runAdminMenu();
    void addFoodItem();      // Adds to BST
    void deleteFoodItem();   // Removes from BST
    void displayFoodDB();    // Shows BST content
    void setDailyMenu();     // The Core Logic: Picks from BST, Sets Price

    // Student/Public Functions
    void displayDayMenu(string date); // Auto-detects day from date
    float getDailyPrice(string date); // Used by Module 3
    bool isValidDate(string date);    // Basic validation

    // File Handling
    void loadFiles();
    void saveFiles();
};

#endif
