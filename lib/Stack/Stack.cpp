#include <iostream>
#include <cstdlib>
using namespace std;
#include "Stack.h"

/*
 * Function:  Stack
 * --------------------
 * Stack default constructor.
 */
Stack::Stack(){

}

/*
 * Function:  Stack
 * --------------------
 * Stack constructor to initialise object.   
 */
Stack::Stack(int size)
{
    arr = new string[size];
    capacity = size;
    top = -1;
}
 
/*
 * Function: ~Stack
 * --------------------
 * Screen Deconstructor.
 */
Stack::~Stack() {
    delete[] arr;
}
 
/*
 * Function:  push
 * --------------------
 * Adds string to top of stack.   
 */
void Stack::push(string x)
{
    if (isFull())
    {
        cout << "Overflow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }
 
    cout << "Inserting " << x << endl;
    arr[++top] = x;
}
 
/*
 * Function:  pop
 * --------------------
 * Takes top element off stack.   
 */
string Stack::pop()
{
    if (isEmpty())
    {
        cout << "Underflow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }
 
    cout << "Removing " << peek() << endl;
    return arr[top--];
}
 
/*
 * Function:  peek
 * --------------------
 * Looks at the top element on the stack.
 */
string Stack::peek()
{
    if (!isEmpty()) {
        return arr[top];
    }
    else {
        return "";
    }
}
 
/*
 * Function:  size
 * --------------------
 * Returns the size of the stack.   
 */
int Stack::size() {
    return top + 1;
}
 
/*
 * Function:  isEmpty
 * --------------------
 * Returns bool to see if stack is empty.
 */
bool Stack::isEmpty() {
    return top == -1;
}
 
/*
 * Function:  isFull
 * --------------------
 * Returns bool to see if stack is full.   
 */
bool Stack::isFull() {
    return top == capacity - 1;
}