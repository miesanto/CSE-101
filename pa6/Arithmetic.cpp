//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023
// Arithmetic.cpp
// pa6
// Top-level client for BigInteger ADT
//-----------------------------------------------------------------------------

#include<iostream>
#include<fstream>
#include<string>
#include"BigInteger.h"

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

    // read in strings
    string str_A, str_B;
    in >> str_A;
    in >> str_B;

    // make BigInteger objects
    BigInteger A = BigInteger(str_A);
    BigInteger B = BigInteger(str_B);
    
    // write lines to out
    // A
    out << A << endl << endl;

    // B
    out << B << endl << endl;

    // A + B
    out << A + B << endl << endl;

    // A - B
    out << A - B << endl << endl;

    // A - A
    out << A - A << endl << endl;

    // 3*A - 2*B
    out << A.mult(BigInteger("3")) - B.mult(BigInteger("2")) << endl << endl;

    // A*B
    out << A * B << endl << endl;

    // A*A
    out << A * A << endl << endl;

    // B*B
    out << B * B << endl << endl;

    // 9*(A^4) + 16*(B^5)
    out << (A * A * A * A).mult(BigInteger("9")) + (B * B * B * B * B).mult(BigInteger("16")) << endl << endl;

    // close files
    in.close();
    out.close();

    return 0;
}