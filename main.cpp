#include <iostream>
#include <limits>
#include "module_1.hpp"

using namespace std;

int main() {
    Admin admin;
    admin.loadFromFile(); // Load data on startup

    int choice;
    do {
        cout << "\n==== STUDENT MANAGEMENT ====\n";
        cout << "1. Add Student\n";
        cout << "2. View All Students\n";
        cout << "3. Exit\n";
        cout << "Choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(); // Clear newline

        switch (choice) {
            case 1:
                admin.addStudent();
                break;
            case 2:
                admin.displayAllStudents();
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 3);

    return 0;
}