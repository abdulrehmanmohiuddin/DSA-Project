#include <iostream>
#include <limits>
#include "module_1.hpp"

using namespace std;

int main() {
    Admin admin;
    admin.loadFromFile(); 

    int choice;
    do {
        cout << "\n==== STUDENT MANAGEMENT SYSTEM ====\n";
        cout << "1. Add Student\n";
        cout << "2. View All Students\n";
        cout << "3. Search Student\n"; // New
        cout << "4. Delete Student\n"; // New
        cout << "5. Exit\n";
        cout << "Choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(); 

        switch (choice) {
            case 1: admin.addStudent(); break;
            case 2: admin.displayAllStudents(); break;
            case 3: admin.searchStudent(); break; // New
            case 4: admin.deleteStudent(); break; // New
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);

    return 0;
}
//g++ main.cpp module_1.cpp -o MessSystem