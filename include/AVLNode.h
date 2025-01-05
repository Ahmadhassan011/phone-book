#ifndef AVLNODE_H
#define AVLNODE_H

#include "Contact.h"

class AVLNode
{
public:
    Contact contact;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(const Contact &contact);
};

#endif // AVLNODE_H