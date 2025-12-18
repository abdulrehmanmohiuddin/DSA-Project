#include <iostream>
#include <limits> // For clearing input buffer
#include "module_1.hpp" // User Management
#include "module_2.hpp" // Menu Management
#include "module_3.hpp" // Billing & Attendance

using namespace std;

// --- Helper for Cleaner Main ---
void waitForInput() {
    cout << "\nPress Enter to continue...";
    // Check if buffer is already empty to avoid double pause or skipping
    if (cin.peek() == '\n') cin.ignore(); 
    cin.get();
}

// --- Helper: Robust Integer Input ---
// This function replaces simple 'cin >>' to prevent crashes
int getIntegerInput() {
    int value;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();             // Clear error flag
        cin.ignore(10000, '\n'); // Discard invalid characters
    }
    cin.ignore(10000, '\n');     // Clear the newline left in buffer
    return value;
}

// --- Menu Functions Declarations ---
void runAdminPanel(Admin& admin, MenuManager& menuMgr, BillingSystem& billSys);
void runStudentPanel(Student& student, MenuManager& menuMgr, BillingSystem& billSys);

int main() {
    // 1. Initialize Modules
    Admin admin;
    MenuManager menuManager;
    BillingSystem billingSystem;

    // 2. Load Data
    cout << "Loading System Data...\n";
    admin.loadFromFile();
    menuManager.loadFiles();

    // 3. Set Simulation Date
    string date;
    cout << "\n==== HOSTEL MESS SYSTEM INITIALIZATION ====\n";
    cout << "Set Simulation Date (e.g., 21-11-2024): ";
    getline(cin, date); // Changed to getline to be safer
    
    // Validate Date using Module 2's helper
    if (!menuManager.isValidDate(date)) {
        cout << "Warning: Invalid date format. Defaulting to 'Monday-Sim'.\n";
    }
    billingSystem.setSystemDate(date);
    cout << "System Date set to: " << billingSystem.getSystemDate() << endl;
    waitForInput();

    // 4. Main Loop
    int choice;
    do {
        system("cls"); // Clear screen (Windows)
        cout << "\n==========================================\n";
        cout << "   HOSTEL MESS MANAGEMENT SYSTEM\n";
        cout << "   Current Date: " << billingSystem.getSystemDate() << "\n";
        cout << "==========================================\n";
        cout << "1. Admin Login\n";
        cout << "2. Student Login\n";
        cout << "3. Exit & Save\n";
        cout << "Choice: ";
        
        // FIX: Use robust input helper
        choice = getIntegerInput();

        switch (choice) {
            case 1:
                if (admin.login()) {
                    runAdminPanel(admin, menuManager, billingSystem);
                }
                break;
            case 2: {
                cout << "\n==== Student Login ====\n";
                string reg, pass;
                cout << "Reg No: "; getline(cin, reg);
                cout << "Password: "; getline(cin, pass);

                // Access Student DB safely
                unordered_map<string, Student>& db = admin.getStudentDB();
                if (db.find(reg) != db.end() && db[reg].getPassword() == pass) {
                    cout << "Login Successful!\n";
                    waitForInput();
                    runStudentPanel(db[reg], menuManager, billingSystem);
                } else {
                    cout << "Error: Invalid Credentials.\n";
                    waitForInput();
                }
                break;
            }
            case 3:
                cout << "Saving Data...\n";
                admin.saveToFile();
                menuManager.saveFiles();
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
                waitForInput();
        }
    } while (choice != 3);

    return 0;
}

// ==========================================
//            Admin Panel Logic
// ==========================================
void runAdminPanel(Admin& admin, MenuManager& menuMgr, BillingSystem& billSys) {
    int choice;
    do {
        system("cls");
        cout << "\n==== ADMIN PANEL (" << billSys.getSystemDate() << ") ====\n";
        cout << "1. Manage Students (Add/Delete/Search)\n";
        cout << "2. Manage Menu (DB/Daily Schedule)\n";
        cout << "3. Clear Student Dues (Mark Paid)\n";
        cout << "4. Change Simulation Date\n";
        cout << "5. Logout\n";
        cout << "Choice: ";
        
        // FIX: Use robust input helper
        choice = getIntegerInput();

        switch (choice) {
            case 1: {
                int sub;
                cout << "\n--- Student Management ---\n";
                cout << "1. Add New Student\n2. Delete Student\n3. Search Student\n4. View All\nChoice: ";
                
                // FIX: Use robust input helper
                sub = getIntegerInput();

                if (sub == 1) admin.addStudent();
                else if (sub == 2) admin.deleteStudent();
                else if (sub == 3) admin.searchStudent();
                else if (sub == 4) admin.displayAllStudents();
                waitForInput();
                break;
            }
            case 2:
                menuMgr.runAdminMenu(); // Module 2 (Already fixed)
                break;
            case 3: {
                string reg;
                cout << "Enter Reg No to Clear Dues: "; getline(cin, reg);
                unordered_map<string, Student>& db = admin.getStudentDB();
                if (db.find(reg) != db.end()) {
                    billSys.clearDues(db[reg]);
                } else {
                    cout << "Student not found.\n";
                }
                waitForInput();
                break;
            }
            case 4: {
                string d;
                cout << "Enter New Date (DD-MM-YYYY): "; 
                getline(cin, d); // Changed to getline for safety
                if(menuMgr.isValidDate(d)) {
                    billSys.setSystemDate(d);
                    cout << "Date updated.\n";
                } else {
                    cout << "Invalid Date Format.\n";
                }
                waitForInput();
                break;
            }
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n"; waitForInput();
        }
    } while (choice != 5);
}

// ==========================================
//            Student Panel Logic
// ==========================================
void runStudentPanel(Student& student, MenuManager& menuMgr, BillingSystem& billSys) {
    int choice;
    string today = billSys.getSystemDate();
    
    do {
        system("cls");
        cout << "\n==== WELCOME, " << student.getName() << " ====\n";
        cout << "Date: " << today << "\n";
        
        // Dynamic Status Display
        bool status = billSys.isCheckedIn(student);
        cout << "Status: " << (status ? "[CHECKED-IN]" : "[NOT CHECKED-IN]") << "\n";
        cout << "--------------------------------\n";
        cout << "1. View Today's Menu & Rate\n";
        cout << "2. " << (status ? "Check-OUT (Cancel Meal)" : "Check-IN (Book Meal)") << " for Today\n";
        cout << "3. View My Transaction History\n";
        cout << "4. Generate/View Bill Invoice\n";
        cout << "5. Logout\n";
        cout << "Choice: ";
        
        // FIX: Use robust input helper
        choice = getIntegerInput();

        switch (choice) {
            case 1:
                menuMgr.displayDayMenu(today);
                waitForInput();
                break;
            case 2: {
                // Get Price from Module 2
                float price = menuMgr.getDailyPrice(today);
                if (price <= 0) {
                    cout << "Error: Menu/Price not set for today by Admin yet.\n";
                } else {
                    // Confirm Action
                    cout << "\nDaily Rate for " << today << " is Rs. " << price << endl;
                    cout << "Proceed? (y/n): ";
                    char confirm; 
                    cin >> confirm; 
                    cin.ignore(10000, '\n'); // Clear buffer immediately after char input

                    if (confirm == 'y' || confirm == 'Y') {
                        // Call Module 3 Toggle
                        string msg = billSys.toggleCheckIn(student, price);
                        cout << msg << endl;
                    } else {
                        cout << "Action cancelled.\n";
                    }
                }
                waitForInput();
                break;
            }
            case 3:
                billSys.displayTransactionHistory(student);
                waitForInput();
                break;
            case 4:
                billSys.generateStudentBill(student);
                waitForInput();
                break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n"; waitForInput();
        }
    } while (choice != 5);
}
//.\MessSystem.exe    g++ main.cpp module_1.cpp module_2.cpp module_3.cpp -o MessSystem