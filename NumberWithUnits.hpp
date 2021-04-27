#pragma once
#ifndef NUMWITHUNITS
#define NUMWITHUNITS

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <map>

// #define a 97
// #define z 122
// #define A 65
// #define Z 90

using namespace std;

namespace ariel {

    //read_units(units_file);

    class NumberWithUnits {
        private:
            double number;
            string unit_type;
        public:
            static void printMap();
            // NumberWithUnits() {
            //     // cout << "constructor1()" << endl;
            //     number = -1;
            //     unit_type = "NaN";
            // }; // Empty Constructor
            NumberWithUnits(const double& num, const string& u_type);
            ~NumberWithUnits(){}; // Destructor
            
                
            // Format for units file:

            /* Command starts with "1"
            // Followed by <unit_name>
            // Followed by "="
            // Followed by <(double)value>
            // Followed by <unit_name> */ 
            static void read_units(ifstream& ifstream);


            double getNumber() const {return this->number;}
            string getUnitType() const{return this->unit_type;}
            void setNumber(double n) {this->number = n;}
            void setUnitType(string unit_type) {this->unit_type = unit_type;}
            
            // bool valid_operation(const NumberWithUnits&, const NumberWithUnits&);
        

            /*========================*/
            //    Unary Operators     //
            /*========================*/
            NumberWithUnits& operator+ ();
            NumberWithUnits& operator- ();

            NumberWithUnits& operator++ (); // Pre-fix-Increment (++a) inc a and return
            NumberWithUnits& operator-- ();
            NumberWithUnits  operator++ (int); // Post-fix-Increment (a++) return a and inc
            NumberWithUnits  operator-- (int);

            /*========================*/
            //    Binary Operators    //
            /*========================*/

            NumberWithUnits& operator+=(const NumberWithUnits&);
            NumberWithUnits& operator-= (const NumberWithUnits&);

            friend NumberWithUnits operator+ (const NumberWithUnits&, const NumberWithUnits&);
            friend NumberWithUnits operator- (const NumberWithUnits&, const NumberWithUnits&);
            
            // friend NumberWithUnits operator+= (NumberWithUnits, NumberWithUnits);            
            // friend NumberWithUnits operator-= (NumberWithUnits, NumberWithUnits);

            friend NumberWithUnits operator* (double, const NumberWithUnits&);
            friend NumberWithUnits operator* (const NumberWithUnits&, double);

            friend bool operator> (const NumberWithUnits&, const NumberWithUnits&);
            friend bool operator>= (const NumberWithUnits&, const NumberWithUnits&);

            friend bool operator< (const NumberWithUnits&, const NumberWithUnits&);
            friend bool operator<= (const NumberWithUnits&, const NumberWithUnits&);

            friend bool operator== (const NumberWithUnits&, const NumberWithUnits&);
            friend bool operator!= (const NumberWithUnits&, const NumberWithUnits&);

            /*========================*/
            //  friend I/O operators  //
            /*========================*/
            friend ostream& operator<< (ostream&, const NumberWithUnits&);
            friend istream& operator>> (istream&, NumberWithUnits&);
    };
};

#endif
