#include "module_1.hpp"
#include <fstream>
#include <sstream>

using namespace std;

Student::Student() {}
Student::Student(string n, string reg, string room) 
    : name(n), regNo(reg), roomNo(room) {}

string Student::getRegNo() const { return regNo; }
string Student::getName() const { return name; }
string Student::getRoomNo() const { return roomNo; }

void Student::displayStudent() const {
    cout << "Reg No: " << regNo << " | Name: " << name << " | Room: " << roomNo << endl;
}

Admin::Admin() {}

string Admin::getSafeString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty() && input.find(DELIMITER) == string::npos) return input;
        cout << "Invalid input. Try again.\n";
    }
}

void Admin::addStudent() {
    cout << "\n--- Add New Student ---\n";
    string reg = getSafeString("Enter Reg No (Unique ID): ");
    
    if (studentDB.find(reg) != studentDB.end()) {
        cout << "Error: Student already exists!\n";
        return;
    }

    string name = getSafeString("Enter Name: ");
    string room = getSafeString("Enter Room No: ");

    Student s(name, reg, room);
    studentDB[reg] = s;
    
    cout << "Student Added Successfully.\n";
    saveToFile(); 
}

void Admin::displayAllStudents() {
    cout << "\n--- All Registered Students ---\n";
    if (studentDB.empty()) {
        cout << "Database is empty.\n";
        return;
    }
    for (auto it = studentDB.begin(); it != studentDB.end(); ++it) {
        it->second.displayStudent();
    }
}

void Admin::saveToFile() {
    ofstream file(DB_FILE.c_str());
    if (file.is_open()) {
        for (auto it = studentDB.begin(); it != studentDB.end(); ++it) {
            file << it->second.getRegNo() << DELIMITER 
                 << it->second.getName() << DELIMITER 
                 << it->second.getRoomNo() << "\n";
        }
        file.close();
    }
}

void Admin::loadFromFile() {
    ifstream file(DB_FILE.c_str());
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string reg, name, room;
            if (getline(ss, reg, DELIMITER) && getline(ss, name, DELIMITER) && getline(ss, room)) {
                studentDB[reg] = Student(name, reg, room);
            }
        }
        file.close();
    }
}