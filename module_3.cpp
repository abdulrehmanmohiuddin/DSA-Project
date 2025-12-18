//module_3.cpp

#include "module_3.hpp"
#include "module_1.hpp" // Now we can see Student details
#include <sstream>

// ==========================================
//          Constructor & Setup
// ==========================================

BillingSystem::BillingSystem() {
    currentDate = "N/A";
}

void BillingSystem::setSystemDate(string date) {
    currentDate = date;
}

string BillingSystem::getSystemDate() {
    return currentDate;
}

// ==========================================
//          Core Logic: Check-In/Out
// ==========================================

/**
 * @brief Checks if the student has a transaction node matching 'currentDate'.
 */
bool BillingSystem::isCheckedIn(Student& student) {
    if (currentDate == "N/A") return false;

    TransactionNode* temp = student.head;
    while (temp != NULL) {
        if (temp->date == currentDate) {
            return true; // Found a check-in for today
        }
        temp = temp->next;
    }
    return false;
}

/**
 * @brief Adds or Removes the transaction for the current date.
 * Implements "Hard Core Error Handling" for Linked List deletion.
 */
string BillingSystem::toggleCheckIn(Student& student, float dailyPrice) {
    if (currentDate == "N/A") return "Error: System Date not set by Admin.";

    if (isCheckedIn(student)) {
        // --- LOGIC: CHECK-OUT (Remove Node) ---
        TransactionNode* current = student.head;
        TransactionNode* previous = NULL;

        while (current != NULL) {
            if (current->date == currentDate) {
                // Found the node to delete
                if (previous == NULL) {
                    // It's the head node
                    student.head = current->next;
                } else {
                    // It's a middle or last node
                    previous->next = current->next;
                }
                delete current;
                
                // Log the refund
                logToMasterFile(student.getRegNo(), "REFUND   ", dailyPrice);
                return "Check-Out Successful! Charges removed for " + currentDate;
            }
            previous = current;
            current = current->next;
        }
        return "Error: synchronization failed."; // Should not happen
    } 
    else {
        // --- LOGIC: CHECK-IN (Add Node) ---
        TransactionNode* newNode = new TransactionNode(currentDate, dailyPrice);
        
        if (student.head == NULL) {
            student.head = newNode;
        } else {
            // Append to end
            TransactionNode* temp = student.head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        
        // Log the charge
        logToMasterFile(student.getRegNo(), "CHECK-IN ", dailyPrice);
        return "Check-In Successful! You are set for " + currentDate;
    }
}

// ==========================================
//          Billing & Reporting
// ==========================================

float BillingSystem::calculateTotalBill(Student& student) {
    float total = 0;
    TransactionNode* temp = student.head;
    while (temp != NULL) {
        total += temp->price;
        temp = temp->next;
    }
    return total;
}

void BillingSystem::displayTransactionHistory(Student& student) {
    if (student.head == NULL) {
        cout << "No transactions found.\n";
        return;
    }
    
    cout << "\n--- Meal History for " << student.getName() << " ---\n";
    cout << left << setw(15) << "Date" << " | Price\n";
    cout << "------------------------------\n";
    
    TransactionNode* temp = student.head;
    while (temp != NULL) {
        cout << left << setw(15) << temp->date 
             << " | Rs. " << fixed << setprecision(2) << temp->price << endl;
        temp = temp->next;
    }
    cout << "------------------------------\n";
    cout << "TOTAL OUTSTANDING: Rs. " << calculateTotalBill(student) << endl;
}

void BillingSystem::generateStudentBill(Student& student) {
    // 1. Create Folder (Windows specific command, safe for Dev C++)
    _mkdir("bills"); 
    
    string filename = "bills/" + student.getRegNo() + "_bill.txt";
    ofstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Could not create bill file.\n";
        return;
    }
    
    file << "===== HOSTEL MESS INVOICE =====\n";
    file << "Student: " << student.getName() << "\n";
    file << "Reg No:  " << student.getRegNo() << "\n";
    file << "Date Generated: " << currentDate << "\n\n";
    
    file << "Date\t\tAmount\n";
    file << "----------------------------\n";
    
    TransactionNode* temp = student.head;
    while (temp != NULL) {
        file << temp->date << "\tRs. " << temp->price << "\n";
        temp = temp->next;
    }
    
    file << "----------------------------\n";
    file << "TOTAL DUE:\tRs. " << calculateTotalBill(student) << "\n";
    file << "----------------------------\n";
    file << "(This is a computer generated invoice)\n";
    
    file.close();
    cout << "Bill generated successfully: " << filename << endl;
}

// ==========================================
//          Admin / Master Log
// ==========================================

void BillingSystem::logToMasterFile(string regNo, string action, float amount) {
    // Open in Append Mode (ios::app)
    ofstream file(LOG_FILE.c_str(), ios::app); 
    if (file.is_open()) {
        file << "[" << action << "] " 
             << "Date: " << currentDate << " | "
             << "Reg: " << regNo << " | "
             << "Amt: " << amount << "\n";
        file.close();
    }
}

void BillingSystem::clearDues(Student& student) {
    if (student.head == NULL) {
        cout << "Student has no dues to clear.\n";
        return;
    }
    
    // 1. Delete Linked List
    TransactionNode* current = student.head;
    while (current != NULL) {
        TransactionNode* temp = current;
        current = current->next;
        delete temp;
    }
    student.head = NULL;
    
    // 2. Delete Old Bill File
    string filename = "bills/" + student.getRegNo() + "_bill.txt";
    remove(filename.c_str());
    
    cout << "Dues cleared. History reset for " << student.getName() << ".\n";
    
    // Log this administrative action
    ofstream file(LOG_FILE.c_str(), ios::app);
    if (file.is_open()) {
        file << "[ADMIN-CLEAR] Reg: " << student.getRegNo() 
             << " | Date: " << currentDate << "\n";
        file.close();
    }
}