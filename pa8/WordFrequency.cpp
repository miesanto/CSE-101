//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023 pa8
// WordFrequency.cpp
// Top-level client for Dictionary ADT. Used FileIO.cpp as reference.
// It will read in each line of a file, parse the individual words on
// each line, convert each word to all lower case characters, then
// place it (as key) in a Dictionary.
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include<fstream>
#include"Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    // error message
    if( argc!=3 ){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    ifstream in;
    in.open(argv[1]);
    if( !in.is_open() ){
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    ofstream out;
    out.open(argv[2]);
    if( !out.is_open() ){
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }

    // read dictionary D from in
    Dictionary D;
    size_t begin, end, len;
    string line;
    string key;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";

    while( getline(in, line) ) {
        len = line.length();

        // get first key (TAKEN FROM C++ EXAMPLE ON WEBSITE)
        begin = min(line.find_first_not_of(delim, 0), len);
        end   = min(line.find_first_of(delim, begin), len);
        key   = line.substr(begin, end-begin);

        while( key!="" ) {
        // convert key to all lower case characters
        for( size_t i = 0; i < key.length(); i++ ) {
            key[i] = tolower(key[i]);
        }

        // add to dictionary
        if( D.contains(key) ) {
            D.getValue(key)++;
        }
        else {
            D.setValue(key, 1);
        }

        // get next key (TAKEN FROM C++ EXAMPLE ON WEBSITE)
        begin = min(line.find_first_not_of(delim, end+1), len);
        end   = min(line.find_first_of(delim, begin), len);
        key   = line.substr(begin, end-begin);
      }
    }

    // write D to out
    out << D << endl;

    // close files 
    D.clear();
    in.close();
    out.close();

    return 0;
}