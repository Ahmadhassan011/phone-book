#include "AVLNode.h"

AVLNode::AVLNode(const Contact &contact)
    : contact(contact), left(nullptr), right(nullptr), height(1) {}