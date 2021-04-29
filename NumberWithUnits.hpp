#pragma once
#ifndef NUMWITHUNITS
#define NUMWITHUNITS

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>


namespace ariel {


    class NumberWithUnits {
        /* Format for units file:
           Command starts with "1"
            Followed by <unit_name>
            Followed by "="
            Followed by <(double)value>
            Followed by <unit_name> */ 
        private:
            double number;
            std::string unit_type;
        public:
            NumberWithUnits(double num, const std::string& unit_type);
            ~NumberWithUnits() {} // Destructor
                        
            static void read_units(std::ifstream&);
            double getNumber() const {return this->number;}
            std::string getUnitType() const{return this->unit_type;}
            void setNumber(double n) {this->number = n;}
            void setUnitType(std::string unit_type) {this->unit_type = unit_type;}        

            /*========================*/
            //    Unary Operators     //
            /*========================*/
            NumberWithUnits operator+ () const;
            NumberWithUnits operator- () const;

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

            friend NumberWithUnits operator* (double, const NumberWithUnits&);
            friend NumberWithUnits operator* (const NumberWithUnits&, double);

            // Implement these operators using only 1 operator ( > ).
            friend bool operator> (const NumberWithUnits&, const NumberWithUnits&);
            friend bool operator>= (const NumberWithUnits&, const NumberWithUnits&);

            friend bool operator< (const NumberWithUnits&, const NumberWithUnits&);
            friend bool operator<= (const NumberWithUnits&, const NumberWithUnits&);

            friend bool operator== (const NumberWithUnits&, const NumberWithUnits&);
            friend bool operator!= (const NumberWithUnits&, const NumberWithUnits&);

            /*========================*/
            //  friend I/O operators  //
            /*========================*/
            friend std::ostream& operator<< (std::ostream&, const NumberWithUnits&);
            friend std::istream& operator>> (std::istream&, NumberWithUnits&);
    };
};

#endif
