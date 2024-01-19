//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023
// BigInteger.cpp
// pa6
// Implementation file for BigInteger ADT. The BigInteger ADT will represent
// a signed integer by encapsulating two pieces of data: an int (which will
// be either 1, -1, or 0) giving its sign, and a List of non-negative longs
// representing its magnitude. Each List element will be a single digit in
// the base 'b' positional numbering system, where 'b' is a power of 10.
// The reason we chose 'b' to be a power of 10 is to facilitate the conversion
// between base 10 and base 'b'.
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include<stdexcept>
#include<cstdlib>
#include"BigInteger.h"

// Global constants -----------------------------------------------------------

const ListElement BASE = 1000000000;                // 1 billion
const int POWER = 9;

using namespace std;

// Helper functions -----------------------------------------------------------

void negateList(List& L);
void sumList(List& S, List A, List B, int sgn);
int normalizeList(List &L);
void shiftList(List& L, int p);
void scalarMultList(List& L, ListElement m);

// Class Constructors & Destructors -------------------------------------------

// BigInteger()
// Constructor that creates a new BigInteger in the zero state:
// signum=0, digits=().
BigInteger::BigInteger() {
    signum = 0;
    digits = List();
}

// BigInteger()
// Constructor that creates a new BigInteger from the long value x.
BigInteger::BigInteger(long x) {
    if( x > 0 ) {
        signum = 1;                             // positive
    }
    else if( x < 0 ) {
        signum = -1;                            // negative
        x = -x;                                 // make x positive for digit extraction
    }
    else {
        signum = 0;                             // zero
    }
    
    // get digits from x and insert them into the List 'digits'
    while (x > 0) {
        digits.insertBefore(x % BASE);          // extract the last digit of x in base BASE
        x /= BASE;                              // move to next digit
    }
}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s) {
    // error message
    if( s.length()==0 ) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }

    // determine the sign of the BigInteger
    if( s[0]=='+' || s[0]=='-' ) {
        signum = 1;                             // default to positive sign
        
        if( s[0]=='-' ) {
            signum = -1;                        // set to negative if '-' is at s[0]
        }

        s = s.substr(1, s.length() - 1);        // remove the sign prefix from the string
    }
    else {
        signum = 1;                             // default to positive sign if no sign prefix
    }

    // check that all characters in the string are numeric
    for (long unsigned int i = 0; i < s.length();) {
        // error message if not
        if( !std::isdigit(s[i]) ) {
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        }
        i++;
    }

    bool my_flag = false;

    // extract digits from the string in groups of POWER
    // insert them into the List 'digits'
    for (int i = s.length() - POWER; ; i -= POWER) {

        // if i is less than 0, insert the last group of digits
        if( i < 0 ) {
            digits.insertAfter(stol(s.substr(0, i + POWER)));
            my_flag = true;                     // set to break out of loop
        }

        // if i is 0 or greater than 0, insert the current group of digits
        else if( i==0 || i > 0 ) {
            digits.insertAfter(stol(s.substr(i, POWER)));
            
            if( i==0 ) {
                my_flag = true;                 // set to break out of loop
            }
        }

        // break once the digits are inserted
        if( my_flag ) {
            break;
        }
    }
}

// BigInteger()
// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger& N) {
    digits = N.digits;
    signum = N.signum;
}


// Access functions -----------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is positive, 
// negative or 0, respectively.
int BigInteger::sign() const {
    return signum;
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger& N) const {
    // compare signs
    if( signum!=N.signum ) {
        // return -1 if this length is less, 1 if greater
        return (signum < N.signum) ? -1 : 1;
    }
    
    // if both have the same sign
    else {
        // negative numbers
        if( signum==-1 ) {
            // compare lengths in reverse order
            if( digits.length() != N.digits.length() ) {
                // return 1 if this length is less, -1 if greater
                return (digits.length() < N.digits.length()) ? 1 : -1;
            }
        }

        // positive numbers
        else if( signum==1 ) {
            // compare lengths
            if( digits.length() != N.digits.length() ) {
                // return 1 if this length is greater, -1 if less
                return (digits.length() > N.digits.length()) ? 1 : -1;
            }
        }

        // check for equality or non-equality based on digit values
        if( digits==N.digits || (digits.length()==0 && N.digits.length()==0) ) {
            return 0;
        }
        
        // compare individual digits
        List A  = digits;
        List B = N.digits;

        A.moveFront();
        B.moveFront();

        while (true) {
            // if reached the end of A
            if( A.position()==A.length() ) {
                // if also reached the end of B, both are equal
                if( B.position()==B.length() ) {
                    return 0;
                }
                // else, A is shorter
                else {
                    return -1;
                }
            }
            
            // if reached the end of B, B is shorter
            else if( B.position()==B.length() ) {
                return 1;
            }

            // A's current digit is greater
            else if( A.peekNext() > B.peekNext() ) {
                return 1;
            }

            // B's current digit is greater
            else if( A.peekNext() < B.peekNext() ) {
                return -1;
            } 
            else {
                A.moveNext();
                B.moveNext();
            }
        }
    }

    return 0;
}


// Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero() {
    signum = 0;
    digits.clear();
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate() {
    if( signum!=0 ) {
        signum *= -1;
    }
}


// Helper functions implementation -----------------------------------------

// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List& L) {
    L.moveFront();

    // iterate through each element
    while (L.position() < L.length()) {
        L.setAfter(-1 * L.peekNext());          // set the current element to its negation
        L.moveNext();
    }
}

// sumList()
// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List& S, List A, List B, int sgn) {
    // clear S
    S.clear();

    // multiply List B by the scalar multiplier sgn
    scalarMultList(B, sgn);

    A.moveBack();
    B.moveBack();
    
    // iterate through A and B
    while (true) {
        // if both Lists A and B have more elements
        if( A.position()!=0 && B.position()!=0 ) {
            int res = A.peekPrev() + B.peekPrev();
            S.insertAfter(res);                 // insert sum of corresponding elements into S

            B.movePrev();
            A.movePrev();
        }

        // if only B has more elements
        else if( B.position()!=0 ) {
            S.insertAfter(B.peekPrev());        // insert remaining B elements into S
            B.movePrev();
        } 

        // if only A has more elements
        else if( A.position()!=0 ) {
            S.insertAfter(A.peekPrev());        // insert remaining A elements into S
            A.movePrev();
        } 

        // if no more elements in either list, break
        else {
            break;
        }
    }
}

// normalizeList()
// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used
// by add(), sub() and mult().
int normalizeList(List &L) {
    long carry = 0;
    L.moveBack();

    // iterate from right to left
    for (; L.position() > 0 || carry != 0; L.movePrev()) {
        // if reached the leftmost end of L
        if(L.position()==0 ) {
            L.insertAfter(carry);               // insert any remaining carry as a new element
            carry = 0;                          // reset carry to 0
            break;
        }

        long sum = L.peekPrev() + carry;        // sum of current element and the carry
        carry = sum / BASE;                     // new carry
        L.setBefore(sum % BASE);                // set current element to the remainder after division by BASE

        // if the current element is negative and not leftmost
        if( L.peekPrev() < 0 && L.position()!=1 ) {
            carry -= 1;                         // adjust the carry
            L.setBefore(L.peekPrev() + BASE);   // add BASE to the current element to make it positive
        }
    }

    L.moveFront();
    
    // remove leading zeros from the list
    while (L.peekNext() == 0 && L.length() > 2) {
        L.eraseAfter();
    }
    
    L.moveFront();

    // determine the sign of the resulting integer
    if( L.peekNext() > 0 ) {
        return 1;                               // positive
    }
    else if( L.peekNext() < 0 ) {
        negateList(L);                          // change the sign to make it positive
        carry = 0;                              // reset carry to 0

        // iterate from right to left
        for (int pos = L.position(); pos > 0; L.movePrev(), pos--) {
            L.setBefore(L.peekPrev() + carry);  // add any remaining carry to the current element
            
            // if the current element is negative, adjust the carry and add BASE to make positive
            if( L.peekPrev() < 0 ) {
                carry = -1;
                L.setBefore(L.peekPrev() + BASE);
            }
            else {
                carry = 0;
            }
        }

        return -1;
    }
    else {
        return 0;
    }
}

// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List& L, int p) {
    int i = 0;

    while (i < p) {
        L.insertAfter(0);
        i++;
    }
}

// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, ListElement m) {
    L.moveFront();

    while (L.position() < L.length()) {
        L.setAfter(L.peekNext() * m);           // set the current element to its product with m
        L.moveNext();
    }
}


// BigInteger Arithmetic operations -------------------------------------------

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) const {
    List A = N.digits;                          // copy of digits of BigInteger N
    List B = digits;                            // copy of the digits of this BigInteger

    BigInteger S;                               // resulting sum
    
    // cases based on the signs of this BigInteger and N
    if( N.signum==-1 ) {
        if( signum==1 ) {
            sumList(S.digits, B, A, -1);        // subtract A from B and store in S
        }
        else if( signum==-1 ) {
            scalarMultList(B, -1);              // multiply B by -1 to make it positive
            sumList(S.digits, B, A, -1);        // subtract A from B and store in S
        }
        else if( signum==0 ) {
            S.digits = A;                       // copy the digits of N to S
            S.signum = N.signum;                // set the sign of S to be the sign of N

            return S;
        }
    }
    else if( N.signum==1 ) {
        if( signum==-1 ) {
            sumList(S.digits, A, B, -1);        // subtract B from A and store in S
        }
        else if( signum==1 ) {
            sumList(S.digits, B, A, 1);         // add A and B and store in  S
        }
        else if( signum==0 ) {
            S.digits = A;                       // copy the digits of N to S
            S.signum = N.signum;                // set the sign of S to be the sign of N

            return S;
        }
    }
    else if( N.signum==0 ) {
        S.digits = digits;                      // copy the digits of this BigInteger to S
        S.signum = signum;                      // set the sign of S to be the sign of this BigInteger

        return S;
    }

    S.signum = normalizeList(S.digits);         // normalize the result and set its sign

    return S;
}

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) const {
    List A = N.digits;                          // copy of digits of BigInteger N
    List B = digits;                            // copy of the digits of this BigInteger

    BigInteger D;                               // resulting difference

    // cases based on the sign of this BigInteger
    if( signum==1 ) {
        // cases based on the sign of N
        if( N.signum==1 ) {
            sumList(D.digits, B, A, -1);        // subtract A from B and store in D
        }
        else {
            sumList(D.digits, B, A, 1);         // add A and B and store in D
        }
    }
    else {
        // cases based on the sign of N
        if( N.signum==1 ) {
            scalarMultList(B, -1);              // multiply B by -1 to make it positive
            sumList(D.digits, B, A, -1);        // subtract A from B and store in D
        }
        else {
            scalarMultList(B, -1);              // multiply B by -1 to make it positive
            sumList(D.digits, B, A, 1);         // add A and B and store in D
        }
    }

    D.signum = normalizeList(D.digits);         // normalize the result and set its sign

    return D;
}

// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N) const {
    List curr_long = digits;                    // a list with the digits of this BigInteger
    List long_N = N.digits;                     // a list with the digits of N
    List product = List();                      // a list to store intermediate products
    BigInteger P = BigInteger();                // resulting product

    long_N.moveBack();

    // iterate through the digits of N
    while (long_N.position() > 0) {
        // multiply curr_long by the last digit of N, then add the intermediate product to the running total
        scalarMultList(curr_long, long_N.peekPrev());
        sumList(product, product, curr_long, 1);
        normalizeList(product);

        product.moveBack();

        // add the shifted product to the final result
        shiftList(product, N.digits.length() - long_N.position());
        sumList(P.digits, P.digits, product, 1);
        normalizeList(P.digits);

        product.clear();                        // clear for next iteration

        curr_long = digits;                     // reset curr_long

        long_N.movePrev();
    }

    P.signum = signum * N.signum;               // set the sign of the resulting product

    return P;
}

// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string() {
    // if the BigInteger is 0
    if( signum==0 ) {
        return "0";
    }

    std::string output = "";

    // if the BigInteger is negative, add a negative sign
    if( signum==-1 ) {
        output += "-";
    }

    // iterate through the digits
    for (digits.moveFront(); digits.position() < digits.length(); digits.moveNext()) {
        // get the current digit value and convert to string
        int value = digits.peekNext();
        std::string value_output = std::to_string(std::abs(value));

        // add the digit to the output string with leading zeros if necessary
        if( digits.position()==0 && value!=0 ) {
            output += value_output;             // add the first non-zero digit without leading zeros
        }

        // else add leading zeros if necessary
        else {
            int add_zeros = POWER - value_output.length();
            output += std::string(add_zeros, '0') + value_output;
        }
    }

    if( output[0]=='-' ) {
        // remove leading zeros after the negative sign
        while (output[1] == '0') {
            output.erase(1, 1);
        }
    }

    return output;
}


// Overriden Operators --------------------------------------------------------

// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<(std::ostream& stream, BigInteger N) {
    return stream << N.BigInteger::to_string();
}

// operator==()
// Returns true if and only if A equals B.
bool operator==(const BigInteger& A, const BigInteger& B) {
    if (A.compare(B) == 0) {
        return true;
    }
    return false;
}

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == -1) {
        return true;
    }
    return false;
}

// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) <= 0) {
        return true;
    }
    return false;
}

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == 1) {
        return true;
    }
    return false;
}

// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) >= 0) {
        return true;
    }
    return false;
}

// operator+()
// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ) {
    return A.BigInteger::add(B);
}

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ) {
    A = A.BigInteger::add(B);
    return A;
}

// operator-()
// Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ) {
    return A.BigInteger::sub(B);
}

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ) {
    A = A.BigInteger::sub(B);
    return A;
}

// operator*()
// Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ) {
    return A.BigInteger::mult(B);
}

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ) {
    A = A.BigInteger::mult(B);
    return A;
}