#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Contact.h"
#include "AVLNode.h"

using namespace std;

class Phonebook
{
public:
    AVLNode *root;

private:
    int getHeight(AVLNode *node);
    int getBalanceFactor(AVLNode *node);
    AVLNode *rotateLeft(AVLNode *node);
    AVLNode *rotateRight(AVLNode *node);
    AVLNode *insertNode(AVLNode *node, const Contact &contact);
    AVLNode *findMinNode(AVLNode *node);
    AVLNode *deleteNode(AVLNode *node, const string &name);
    void prefixSearchNode(AVLNode *node, const string &prefix);
    bool isDuplicateNode(AVLNode *node, const string &name, const string &phoneNumber);
    void displayInOrder(AVLNode *node);
    bool searchContactNode(AVLNode *node, const string &name);
    bool searchPhoneNumberNode(AVLNode *node, const string &phoneNumber);
    void saveToCSV(AVLNode *node, ofstream &file);
    void loadFromCSV();

public:
    Phonebook();
    void addContact(const string &name, const string &phoneNumber, const string &email = "");
    bool searchContactbyName(const string &name);
    bool searchContactbyNumber(const string &num);
    void deleteContact(const string &name);
    void displayAllContacts();
    void prefixSearch(const string &prefix);
    bool isDuplicate(const string &name, const string &phoneNumber);
    void saveContacts();
    void loadContacts();
    string getPhoneNumberByName(const string &name);
    string getNameByPhoneNumber(const string &phoneNumber);
    void inorderTraversal(AVLNode *node, vector<AVLNode *> &nodes);
};

#endif // PHONEBOOK_H