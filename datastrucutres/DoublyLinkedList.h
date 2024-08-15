#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>

struct Node{
    int value;
    Node* next = NULL;
    Node* prev = NULL;
};

class DoublyLinkedList {
    Node* head;
    Node* tail;
    public:
        int length;
        DoublyLinkedList();
        void append(int value);
        bool isEmpty();
        void pop();
        Node* peekLast();
        Node* peekFirst();
        Node* getFromIdx(int n);
        void printList();
};

#endif // DOUBLYLINKEDLIST_H
