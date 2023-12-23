//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023 pa7
// Order.cpp
// Top-level client for Dictionary ADT.
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

    // open files
    ifstream in(argv[1]);
    ofstream out(argv[2]);

    // read dictionary D from in
    Dictionary D;

    string line;
    int line_num = 1;
    
    while( getline(in, line) ) {
        D.setValue(line, line_num);
        line_num++;
    }

    // write D to out
    out << D.to_string() << endl;
    out << D.pre_string() << endl;

    // close files
    in.close();
    out.close();

    return 0;
}