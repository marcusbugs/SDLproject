/*
 * Author:   Marcus Cassell
 * Date:     May 14, 2026
 * Purpose:  CSCI 221 Final Project -- single-link node holding one kill
 *           record. Each node stores the kill number (1st, 2nd, ...) and
 *           the time of the kill in ms since SDL init, plus a pointer
 *           to the next-older kill node. Used by KillStack.
 *
 */

#ifndef _KILLNODE_H_
#define _KILLNODE_H_

#include <SDL2/SDL.h>   // for Uint32

class Node {
private:
    int       killNumber;   // 1 for first kill, 2 for second, ...
    Uint32    killTime;     // SDL_GetTicks() value at the moment of the kill
    Node* next;         // points at the previous (older) kill, or nullptr

public:
    Node(int n, Uint32 t);

    int       getKillNumber() const;
    Uint32    getKillTime()   const;
    Node* getNext()       const;

    void      setNext(Node* n);
};

#endif // _KILLNODE_H_
