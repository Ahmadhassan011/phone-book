#ifndef CONTACT_H
#define CONTACT_H

#include <string>
using namespace std;

struct Contact
{
    string name;
    string phoneNumber;
    string email;

    Contact(const string &name, const string &phoneNumber, const string &email = "");
};

#endif // CONTACT_H
