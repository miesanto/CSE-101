//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023
// ListTest.cpp
// Test file for List ADT.
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include<stdexcept>
#include"List.h"

using namespace std;

void printListInfo(const List& L) {
    cout << "List: " << L.to_string() << endl;
    cout << "Length: " << L.length() << endl;
    cout << "Cursor Position: " << L.position() << endl;
    cout << endl;
}

int main() {
    try {
        // Test default constructor
        cout << "----- Test default constructor -----" << endl;
        List list_A;
        
        printListInfo(list_A);

        // Test insertion functions
        cout << "----- Test insertion functions -----" << endl;
        cout << "insertBefore(1), insertAfter(2), insertBefore(3), insertAfter(4)" << endl;
        list_A.insertBefore(1);
        list_A.insertAfter(2);
        list_A.insertBefore(3);
        list_A.insertAfter(4);
        cout << endl;

        printListInfo(list_A);

        // Test move functions
        cout << "----- Test move functions -----" << endl;
        cout << "moveFront(), moveNext()" << endl;
        list_A.moveFront();
        list_A.moveNext();
        cout << "Peek Next: " << list_A.peekNext() << endl;
        cout << endl;

        cout << "moveBack(), movePrev()" << endl;
        list_A.moveBack();
        list_A.movePrev();
        cout << "Peek Prev: " << list_A.peekPrev() << endl;
        cout << endl;

        printListInfo(list_A);

        // Test set functions
        cout << "----- Test set functions -----" << endl;
        cout << "setAfter(10), setBefore(20)" << endl;
        list_A.setAfter(10);
        list_A.setBefore(20);
        cout << endl;

        printListInfo(list_A);

        // Test erase functions
        cout << "----- Test erase functions -----" << endl;
        cout << "eraseAfter(), eraseBefore()" << endl;
        list_A.eraseAfter();
        list_A.eraseBefore();
        cout << endl;

        printListInfo(list_A);

        // Test find functions
        cout << "----- Test find functions -----" << endl;
        cout << "moveFront(), findNext(2)" << endl;
        list_A.moveFront();
        int pos = list_A.findNext(2);
        cout << "Find Next: " << pos << endl;
        cout << endl;

        list_A.moveBack();
        pos = list_A.findPrev(1);
        cout << "Find Prev: " << pos << endl;
        cout << endl;

        // Test cleanup function
        cout << "----- Test cleanup function -----" << endl;
        cout << "insertBefore(2), insertBefore(3)" << endl;
        list_A.insertBefore(2);
        list_A.insertBefore(3);
        printListInfo(list_A);

        cout << "cleanup()" << endl;
        list_A.cleanup();
        cout << endl;
        
        printListInfo(list_A);

        // Test concatenation
        cout << "----- Test concatenation -----" << endl;
        List list_B;
        list_B.insertBefore(100);
        list_B.insertBefore(200);
        list_B.insertBefore(300);
        printListInfo(list_B);

        cout << "list_A.concat(list_B)" << endl;
        List list_C = list_A.concat(list_B);
        cout << endl;
        
        printListInfo(list_C);
    }
    
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
