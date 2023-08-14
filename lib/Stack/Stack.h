#ifndef STACK_H_
#define STACK_H_
#include <iostream>
#include <cstdlib>
using namespace std;

class Stack
{
    string *arr;
    int top;
    int capacity;
 
public:
    Stack();
    Stack(int size);
    ~Stack(); 
 
    void push(string);
    string pop();
    string peek();
 
    int size();
    bool isEmpty();
    bool isFull();
};

#endif /* STACK_H_*/