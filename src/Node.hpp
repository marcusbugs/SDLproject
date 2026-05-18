/*
 * Author:   Marcus Cassell
 * Date:     May 14, 2026
 * Purpose:  CSCI 221 Final Project
 *           Linked list node
 *
 */

#ifndef _NODE_H_
#define _NODE_H_

#include <SDL2/SDL.h>  // need it for Uint32 sdl time type

class Node {
private:
    int       killNumber;
    Uint32    killTime;
    Node* next;

public:
    Node(int n, Uint32 t);

    int       getKillNumber() const;
    Uint32    getKillTime()   const;
    Node* getNext()       const;

    void      setNext(Node* n);
};

#endif // _NODE_H_
