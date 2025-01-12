#include "Phonebook.h"
#include <fstream>
#include <sstream>
#include <iomanip>

Phonebook::Phonebook() : root(nullptr) {
    loadContacts();
}

int Phonebook::getHeight(AVLNode *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

int Phonebook::getBalanceFactor(AVLNode *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode *Phonebook::rotateLeft(AVLNode *node)
{
    AVLNode *newRoot = node->right;
    AVLNode *newRootLeftChild = newRoot->left;

    newRoot->left = node;
    node->right = newRootLeftChild;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

AVLNode *Phonebook::rotateRight(AVLNode *node)
{
    AVLNode *newRoot = node->left;
    AVLNode *newRootRightChild = newRoot->right;

    newRoot->right = node;
    node->left = newRootRightChild;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

AVLNode *Phonebook::insertNode(AVLNode *node, const Contact &contact)
{
    if (node == nullptr)
    {
        return new AVLNode(contact);
    }

    if (contact.name < node->contact.name)
    {
        node->left = insertNode(node->left, contact);
    }
    else if (contact.name > node->contact.name)
    {
        node->right = insertNode(node->right, contact);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1 && contact.name < node->left->contact.name)
    {
        return rotateRight(node);
    }

    if (balanceFactor < -1 && contact.name > node->right->contact.name)
    {
        return rotateLeft(node);
    }

    if (balanceFactor > 1 && contact.name > node->left->contact.name)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balanceFactor < -1 && contact.name < node->right->contact.name)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

AVLNode *Phonebook::findMinNode(AVLNode *node)
{
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return node;
}

AVLNode *Phonebook::deleteNode(AVLNode *node, const string &name)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (name < node->contact.name)
    {
        node->left = deleteNode(node->left, name);
    }
    else if (name > node->contact.name)
    {
        node->right = deleteNode(node->right, name);
    }
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }
        else if (node->left == nullptr)
        {
            AVLNode *temp = node;
            node = node->right;
            delete temp;
        }
        else if (node->right == nullptr)
        {
            AVLNode *temp = node;
            node = node->left;
            delete temp;
        }
        else
        {
            AVLNode *minRight = findMinNode(node->right);
            node->contact = minRight->contact;
            node->right = deleteNode(node->right, minRight->contact.name);
        }
    }

    if (node == nullptr)
    {
        return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1)
    {
        if (getBalanceFactor(node->left) >= 0)
        {
            return rotateRight(node);
        }
        else
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }
    else if (balanceFactor < -1)
    {
        if (getBalanceFactor(node->right) <= 0)
        {
            return rotateLeft(node);
        }
        else
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;
}

void Phonebook::prefixSearchNode(AVLNode *node, const string &prefix)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->contact.name.find(prefix) == 0)
    {
        cout << left << setw(20) << node->contact.name << setw(15) << node->contact.phoneNumber << setw(30) << node->contact.email << endl;
    }

    if (node->contact.name.compare(0, prefix.length(), prefix) >= 0)
    {
        prefixSearchNode(node->left, prefix);
    }

    prefixSearchNode(node->right, prefix);
}

bool Phonebook::isDuplicateNode(AVLNode *node, const string &name, const string &phoneNumber)
{
    if (node == nullptr)
    {
        return false;
    }

    if (node->contact.name == name || node->contact.phoneNumber == phoneNumber)
    {
        return true;
    }

    if (name < node->contact.name)
    {
        return isDuplicateNode(node->left, name, phoneNumber);
    }
    else
    {
        return isDuplicateNode(node->right, name, phoneNumber);
    }
}

void Phonebook::displayInOrder(AVLNode *node)
{
    if (node != nullptr)
    {
        displayInOrder(node->left);
        cout << left << setw(20) << node->contact.name << setw(15) << node->contact.phoneNumber << setw(30) << node->contact.email << endl;
        displayInOrder(node->right);
    }
}

bool Phonebook::searchContactNode(AVLNode *node, const string &name)
{
    if (node == nullptr)
    {
        return false;
    }

    if (node->contact.name == name)
    {
        return true;
    }

    if (name < node->contact.name)
    {
        return searchContactNode(node->left, name);
    }
    else
    {
        return searchContactNode(node->right, name);
    }
}

bool Phonebook::searchPhoneNumberNode(AVLNode *node, const string &phoneNumber)
{
    if (node == nullptr)
    {
        return false;
    }

    if (node->contact.phoneNumber == phoneNumber)
    {
        return true;
    }

    if (phoneNumber < node->contact.phoneNumber)
    {
        return searchPhoneNumberNode(node->left, phoneNumber);
    }
    else
    {
        return searchPhoneNumberNode(node->right, phoneNumber);
    }
}

void Phonebook::inorderTraversal(AVLNode *node, vector<AVLNode *> &nodes)
{
    if (node != nullptr)
    {
        inorderTraversal(node->left, nodes);
        nodes.push_back(node);
        inorderTraversal(node->right, nodes);
    }
}

void Phonebook::addContact(const string &name, const string &phoneNumber, const string &email)
{
    string formattedName = name.substr(0, 20); // Truncate name to 20 characters
    string formattedPhoneNumber = phoneNumber.substr(0, 15); // Truncate phone number to 15 characters

    Contact contact(formattedName, formattedPhoneNumber, email);
    root = insertNode(root, contact);
    saveContacts();
}

bool Phonebook::searchContactbyName(const string &name)
{
    return searchContactNode(root, name);
}

bool Phonebook::searchContactbyNumber(const string &num)
{
    return searchPhoneNumberNode(root, num);
}

void Phonebook::deleteContact(const string &name)
{
    string formattedName = name;
    formattedName.erase(remove(formattedName.begin(), formattedName.end(), ' '), formattedName.end());
    if (searchContactNode(root, name))
    {
        root = deleteNode(root, formattedName);
        cout << "DELETED SUCCESSFULLY!" << endl;
    }
    else
    {
        cout << "CONTACT NOT FOUND!" << endl;
    }
    saveContacts();
}

void Phonebook::displayAllContacts()
{
    displayInOrder(root);
}

void Phonebook::prefixSearch(const string &prefix)
{
    prefixSearchNode(root, prefix);
}

bool Phonebook::isDuplicate(const string &name, const string &phoneNumber)
{
    return isDuplicateNode(root, name, phoneNumber);
}

void Phonebook::saveToCSV(AVLNode *node, ofstream &file)
{
    if (node != nullptr)
    {
        saveToCSV(node->left, file);
        file << node->contact.name << "," << node->contact.phoneNumber << "," << node->contact.email << "\n";
        saveToCSV(node->right, file);
    }
}

void Phonebook::saveContacts()
{
    ofstream file("contacts.csv");
    if (file.is_open())
    {
        saveToCSV(root, file);
        file.close();
    }
}

void Phonebook::loadFromCSV()
{
    ifstream file("contacts.csv");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string name, phoneNumber, email;

            getline(ss, name, ',');
            getline(ss, phoneNumber, ',');
            getline(ss, email, ',');

            addContact(name, phoneNumber, email);
        }
        file.close();
    }
}

void Phonebook::loadContacts()
{
    loadFromCSV();
}

string Phonebook::getPhoneNumberByName(const string &name) {
    AVLNode *node = root;
    while (node != nullptr) {
        if (node->contact.name == name) {
            return node->contact.phoneNumber;
        } else if (name < node->contact.name) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return "";
}

string Phonebook::getNameByPhoneNumber(const string &phoneNumber)
{
    vector<AVLNode *> nodes;
    inorderTraversal(root, nodes);
    
    for (AVLNode *node : nodes)
    {
        if (node->contact.phoneNumber == phoneNumber)
        {
            return node->contact.name;
        }
    }
    return "";
}