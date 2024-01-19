//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023
// Shuffle.cpp
// Top-level client for List ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include<cmath>
#include<iomanip>
#include "List.h"

void shuffle(List& D) {
    List left_half = List();
    List right_half = List(); 
    int n = D.length();
    // List result = List();

    D.moveFront();

    // split the deck into left and right halves
    // floor accounts for odd length n
    while(D.position() < floor(n/2)) {
        left_half.insertBefore(D.peekNext());
        D.moveNext();
    }

    while(D.position() < n) {
        right_half.insertBefore(D.peekNext());
        D.moveNext();
    }

    D.clear();

    right_half.moveFront();
    left_half.moveFront();

    // perform a perfect shuffle
    for(int i = 0; i < left_half.length(); i++) {
        D.insertBefore(right_half.peekNext());
        D.insertBefore(left_half.peekNext());

        if( i < (left_half.length() - 1) ) {
            left_half.moveNext();
        }

        right_half.moveNext();
    }

    // if n is odd, add the last element from the right half
    if( n%2 != 0 ) {
        D.insertBefore(right_half.peekNext());
    }
}

int main(int argc, char* argv[]) {
    if( argc!=2 ) {
        std::cerr << "Usage: " << argv[0] << " <max_deck_size>" << std::endl;
        return 1;
    }

    int max = std::stoi(argv[1]);

    std::cout << "deck size       shuffle count" << std::endl;
    std::cout << "------------------------------" << std::endl;

    for(int n = 1; n <= max; n++) {
        List deck = List();

        // initialize the deck with values (0, 1, 2, ..., n-1)
        for(int i = 0; i < n; i++) {
            deck.insertBefore(i);
        }

        // perform shuffles and count them until the deck is back to its original order
        int count = 0;
        List my_deck = deck;           // copy the original deck for comparison

        do {
            shuffle(deck);
            count++;
        } while(!deck.equals(my_deck));

        // formatting did not work
        // std::cout << " " << n << "               " << count << std::endl;

        int spaces = 16 + std::to_string(count).length() - std::to_string(n).length();

        std::cout << " " << n << std::setw(spaces) << count << std::endl;
    }

    return 0;
}