#include "module_3.hpp"
#include "module_1.hpp"
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
bool BillingSystem::isCheckedIn(Student& student) {
    if (currentDate == "N/A") return false;

    TransactionNode* temp = student.head;
    while (temp != NULL) {
        if (temp->date == currentDate) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}
string BillingSystem::toggleCheckIn(Student& student, float dailyPrice) {
    if (currentDate == "N/A") return "Error: System Date not set by Admin.";

    if (isCheckedIn(student)) {
        TransactionNode* current = student.head;
        TransactionNode* previous = NULL;

        while (current != NULL) {
            if (current->date == currentDate) {
                if (previous == NULL) {
                    student.head = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                
                logToMasterFile(student.getRegNo(), "REFUND   ", dailyPrice);
                return "Check-Out Successful! Charges removed for " + currentDate;
            }
            previous = current;
            current = current->next;
        }
        return "Error: synchronization failed.";
    } 
    else {
        TransactionNode* newNode = new TransactionNode(currentDate, dailyPrice);
        
        if (student.head == NULL) {
            student.head = newNode;
        } else {
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
