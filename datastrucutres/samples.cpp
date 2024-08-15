#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <string>
#include <list>
#include "DoublyLinkedList.h"

void testQueue(){
    std::queue<int> q;
    for(int i=0; i<10; i++){
        q.push(i);
    }
    while (!q.empty()){
        int res = q.front();
        q.pop();
        std::cout << res << std::endl;
    }
}

void testStack(){
    std::stack<int> s;
     for(int i=0; i<10; i++){
        s.push(i);
    }
    while (!s.empty()){
        int res = s.top();
        s.pop();
        std::cout << res << std::endl;
    }
}

void testUnorderedMap(){
    // tldr, unordered map is basically a c++ implementation of hashtable/hashmap
    std::unordered_map<int,std::string> map;
    map[1] = "one";
    map[3] = "three";
    std::cout << map[1] << std::endl << map[3] << std::endl;
    return;
}

void testDoublyLinkedList(){
    DoublyLinkedList list = DoublyLinkedList();
    for (int i=0; i<10; i++){
        list.append(i);
    }
    list.printList();

    std::cout << "First element is: " << list.peekFirst()->value << std::endl;
    std::cout << "Last element is: " << list.peekLast()->value << std::endl;
    std::cout << "trying to remove last using pop" << std::endl;
    std::cout << "initial: " << list.length << std::endl;
    list.pop();
    std::cout << "final: " << list.length << std::endl;
    std::cout << "Last element is: " << list.peekLast()->value << std::endl;
}

int main(){
    // testUnorderedMap();
    // std::cout << __cplusplus / 100 % 100 << '\n';
    // std::cout << list.isEmpty() << std::endl;
}