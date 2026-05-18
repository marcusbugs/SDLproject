/*
 * Author:   Marcus Cassell
 * Date:     May 14, 2026
 * Purpose:  CSCI 221 Final Project implementation of KillNode.
 *
 */

#include "Node.hpp"

Node::Node(int n, Uint32 t) {
    killNumber = n;
    killTime   = t;
    next       = nullptr;
}

int Node::getKillNumber() const {
    return killNumber;
}

Uint32 Node::getKillTime() const {
    return killTime;
}

Node* Node::getNext() const {
    return next;
}

void Node::setNext(Node* n) {
    next = n;
}
