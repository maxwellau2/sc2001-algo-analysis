#include "DoublyLinkedList.h"
#include <cstdlib> // for malloc and free

DoublyLinkedList::DoublyLinkedList() {
    this->head = NULL;
    this->tail = NULL;
    this->length = 0;
}

void DoublyLinkedList::append(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    if (this->length == 0) {
        this->head = node;
        this->tail = node;
    } else {
        node->prev = this->tail;
        this->tail->next = node;
        this->tail = node;
    }
    this->length++;
}

bool DoublyLinkedList::isEmpty() {
    return this->length == 0;
}

void DoublyLinkedList::pop() {
    if (this->isEmpty()) {
        return;
    } else if (this->length > 1) {
        Node* node = this->tail;
        this->length--;
        Node* prevNode = this->tail->prev;
        prevNode->next = NULL;
        free(node);
        this->tail = prevNode;
        return;
    } else {
        Node* head = this->head;
        free(head);
        this->head = NULL;
        this->tail = NULL;
        this->length--;
        return;
    }
}

Node* DoublyLinkedList::peekLast() {
    return this->tail;
}

Node* DoublyLinkedList::peekFirst() {
    return this->head;
}

Node* DoublyLinkedList::getFromIdx(int n) {
    if (n >= this->length) {
        return NULL;
    } else if (n == this->length - 1) {
        return this->peekLast();
    } else if (n == 0) {
        return this->peekFirst();
    }
    Node* temp = this->head;
    for (int i = 0; i < n; i++) {
        temp = temp->next;
    }
    return temp;
}

void DoublyLinkedList::printList() {
    Node* node = this->head;
    while (node) {
        std::cout << node->value << "\n";
        node = node->next;
    }
    std::cout << "That's it\n";
}
