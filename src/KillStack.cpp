/*
 * Author:   Marcus Cassell
 * Date:     May 14, 2026
 * Purpose:  CSCI 221 Final Project -- implementation of KillStack.
 *
 *This stores kill time data to keep track in HUD
 * Linked list stack
 */

#include "KillStack.hpp"

KillStack::KillStack() {
    head  = nullptr;
    count = 0;
}

KillStack::~KillStack() {
    clear();
}

void KillStack::push(int killNumber, Uint32 killTime) {
    Node* node = new Node(killNumber, killTime);
    node->setNext(head);
    head = node;
    count++;
}

bool KillStack::pop() {
    if (head == nullptr) {
        return false;
    }
    Node* old = head;
    head = head->getNext();
    delete old;
    count--;
    return true;
}

int KillStack::peekNumber() const {
    if (head == nullptr) {
        return 0;
    }
    return head->getKillNumber();
}

Uint32 KillStack::peekTime() const {
    if (head == nullptr) {
        return 0;
    }
    return head->getKillTime();
}

int KillStack::size() const {
    return count;
}

void KillStack::clear() {
    while (head != nullptr) {
        Node* old = head;
        head = head->getNext();
        delete old;
    }
    count = 0;
}
