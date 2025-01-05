#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <sstream>

#include "Phonebook.h"
#include "callHistory.h"

using namespace std;

void wait_msg() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printTableHeader() {
    cout << left << setw(20) << "Name" << setw(15) << "Phone Number" << setw(30) << "Email" << endl;
    cout << "-------------------------------------------------------------\n";
}

void printTableFooter() {
    cout << "-------------------------------------------------------------\n";
}

int main() {
    Phonebook phonebook;
    CallHistory callHistory;
    callHistory.loadCallHistory();
    int choice;
    string name, phoneNumber, email, prefix;

    while (true) {
        system("cls"); // Clear screen for Windows
        cout << "==============================\n";
        cout << "        Phonebook Menu        \n";
        cout << "==============================\n";
        cout << "1. Add a Contact\n";
        cout << "2. Delete a Contact\n";
        cout << "3. Search for a Contact\n";
        cout << "4. Prefix Search for Contacts\n";
        cout << "5. Display All Contacts\n";
        cout << "6. Display Most Calls\n";
        cout << "7. Display Recent Calls\n";
        cout << "8. Display Longest Calls\n";
        cout << "9. Exit\n";
        cout << "==============================\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter contact name: ";
                getline(cin, name);
                cout << "Enter contact number: ";
                getline(cin, phoneNumber);
                cout << "Enter contact email (optional): ";
                getline(cin, email);
                if (phonebook.isDuplicate(name, phoneNumber)) {
                    cout << "Contact already exists!\n";
                } else {
                    phonebook.addContact(name, phoneNumber, email);
                    cout << "Contact added successfully!\n";
                }
                wait_msg();
                break;

            case 2:
                cout << "Enter contact name to delete: ";
                getline(cin, name);
                phonebook.deleteContact(name);
                wait_msg();
                break;

            case 3:
                int searchChoice;
                cout << "1. Search by name\n";
                cout << "2. Search by phone number\n";
                cout << "Enter your choice: ";
                cin >> searchChoice;
                cin.ignore();
                if (searchChoice == 1) {
                    cout << "Enter name: ";
                    getline(cin, name);
                    if (phonebook.searchContactbyName(name)) {
                        cout << "Contact found!\n";
                        cout << "Name: " << name << endl;
                        cout << "Phone Number: " << phonebook.getPhoneNumberByName(name) << endl;
                    } else {
                        cout << "Contact not found!\n";
                    }
                } else if (searchChoice == 2) {
                    cout << "Enter phone number: ";
                    getline(cin, phoneNumber);
                    if (phonebook.searchContactbyNumber(phoneNumber)) {
                        cout << "Contact found!\n";
                        cout << "Name: " << phonebook.getNameByPhoneNumber(phoneNumber) << endl;
                        cout << "Phone Number: " << phoneNumber << endl;
                    } else {
                        cout << "Contact not found!\n";
                    }
                } else {
                    cout << "Invalid choice!\n";
                }
                wait_msg();
                break;

            case 4:
                cout << "Enter prefix to search: ";
                getline(cin, prefix);
                phonebook.prefixSearch(prefix);
                wait_msg();
                break;

            case 5:
                cout << "All Contacts:\n";
                printTableHeader();
                phonebook.displayAllContacts();
                printTableFooter();
                wait_msg();
                break;

            case 6:
                callHistory.displayMostCalls();
                wait_msg();
                break;

            case 7:
                callHistory.displayRecentCalls();
                wait_msg();
                break;

            case 8:
                callHistory.displayLongestCalls();
                wait_msg();
                break;

            case 9:
                cout << "Exiting Phonebook...\n";
                return 0;

            default:
                cout << "Invalid choice! Please try again.\n";
                wait_msg();
                break;
        }
    }

    return 0;
}