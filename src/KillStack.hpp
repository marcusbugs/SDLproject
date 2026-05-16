/*
 * Author:   Marcus Cassell
 * Date:     May 14, 2026
 * Purpose:  CSCI 221 Final Project
 *
 * Linked list stack
 */

#ifndef _KILLSTACK_H_
#define _KILLSTACK_H_

#include "Node.hpp"

class KillStack {
private:
    Node* head;
    int       count;

public:
    KillStack();
    ~KillStack();


    void   push(int killNumber, Uint32 killTime);

    bool   pop();

    int    peekNumber() const;
    Uint32 peekTime()   const;

    int    size() const;

    void   clear();
};

#endif // _KILLSTACK_H_
