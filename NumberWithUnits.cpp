#include "NumberWithUnits.hpp"
#include <map>
using namespace std;

const double EPSILON = 0.000001;
map<string, map<string, double>> units_map; // Self implementation of units conversion table.


bool char_is_a_number(char c) {
    return c >= 43 && c <= 57; // (*). (+), (-), (/), (.) AND (0-9)
}

bool char_is_a_lower_letter(char c) {
    return c >= 97 && c <= 122; // 'a'-'z'
}

bool char_is_a_capital_letter(char c) {
    return (c >= 64 && c <= 90) || c == 95; // 'A'-'Z' OR (_)
}


double get_conversion_rate(const ariel::NumberWithUnits &n1, const ariel::NumberWithUnits &n2){
    double conversion_rate;
    bool are_convertable = false;
    string u1 = n1.getUnitType();
    string u2 = n2.getUnitType();

    for (const auto& pair : units_map[u2]) {
        
        if(pair.first == u1) { 
            // If found a matching unit
            // to convert in 'u2's conversion table - insert conversion rate into a var.
            conversion_rate = units_map[u2][u1];
            are_convertable = true;
        }
    }

    if(are_convertable) { 
        return conversion_rate;
    }
    // If units are not convertable code will reach here: 
    string err = "Cannot perform operation between " + u1 + " and " + u2;
    throw invalid_argument(err);
}


namespace ariel{ 

    /**
        Constructor: 
         @param num(double): A double value to present measures of a specified unit type.
         @param unit_type(string): A string value to specify a unit type. Unit types
                           are set after the call to NumberWithUnits::read_units(ifstream).
    */
    NumberWithUnits::NumberWithUnits(const double& num, const string& unit_type) {
        if(units_map.count(unit_type) == 0) {
            string err = "No valid unit type named '" +unit_type + "'";
            throw invalid_argument(err);
        } 

        setNumber(num);
        setUnitType(unit_type);
    }

    /**
        A method that matches trasitive units that are recieved from
        input file stream.
        Example: KM -> M
                 CM -> M => M ? -> KM != CM 
                         => ATTACH KM -> CM
                                   CM -> KM
         @param from_unit(string): A double value to present measures of a specified unit type.
         @param to_unit(string):   A string value to specify a unit type. Unit types
                           are set after the call to NumberWithUnits::read_units(ifstream).
         @param flag(bool): A flag that reruns the method with 
                            opposite string values if it is set to 1.
                            else - does nothing.
    */
    void add_all_valid_units(const string &from_unit, const string &to_unit, bool bidirectional) {
        
        string mid_unit;
        double conversion_rate;
        double conv_rate_mid_unit;

        for (const auto &pair : units_map[from_unit]) {
            mid_unit = pair.first; // The "middle man" that helps converting unit types.
                                   // Example: convert from KM to CM using mid_unit = M.
                                   // So calc: 1 KM = 1000 M => 1 KM = X = [M->KM] * (1/[M->CM])

            if(pair.first != to_unit){
                conv_rate_mid_unit = units_map[from_unit][mid_unit]; // Get conversion rate

                conversion_rate = conv_rate_mid_unit * (1/units_map[from_unit][to_unit]);
                units_map[mid_unit][to_unit] = 1/conversion_rate; // Ex.  KM->CM
                units_map[to_unit][mid_unit] = conversion_rate;   // AND  CM->KM
        }
      }

      if (bidirectional) { // if bidirectional flag was set to 1, 
                           // rerun this method with opposite string values.
          add_all_valid_units(to_unit, from_unit, false);
      }
    }


    /** 
         @param num (double): A double value to present measures of a specified unit type.
         @param unit_type(string): A string value to specify a unit type. Unit types
                           are set after the call to NumberWithUnits::read_units(ifstream).
    */
    void NumberWithUnits::read_units(ifstream& ifstream) {

        string l_unit, r_unit, oper;
        double num1,num2;
        const int bidirectional = 1;

        // Read from input stream seperated by ' ' (white spaces).
        while (ifstream >> num1 >> l_unit >> oper >> num2 >> r_unit) {
            if (num1 == 1 && oper == "=" && num2 > 0) {
                units_map[l_unit][r_unit] = num2; 
                units_map[r_unit][l_unit] = 1/num2;

                add_all_valid_units(l_unit, r_unit, bidirectional); // Iterate over l_unit's "neighbors" to match with r_unit's.
                                                                    // AND the other way around. 
            } 
        }
    }

    /*====================*/
    //   Unary Operators  //
    /*====================*/

    NumberWithUnits NumberWithUnits::operator+ () {
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator- () {
        return NumberWithUnits(-1 * this->getNumber(), this->getUnitType());
    }

    NumberWithUnits& NumberWithUnits::operator++ () { // Pre-fix increment
        setNumber(getNumber()+1);
        return *this;
    }

    NumberWithUnits& NumberWithUnits::operator-- () { // Pre-fix Decrement
        setNumber(getNumber()-1);
        return *this;
    }

    /** 
        Create a copy of (this), while increasing it's value
        and then return the copy.
        @param a - This is a value not to use. It is being passed by itself.
    */
    NumberWithUnits NumberWithUnits::operator++ (int a) { // post-fix inc (a++)
        NumberWithUnits copy = this->operator++();
        copy.setNumber(copy.getNumber()-1);
        return copy;
    }

    /** 
        Create a copy of (this), while increasing it's value
        and then return the copy.
        @param a - This is a value not to use. It is being passed by itself.
    */
    NumberWithUnits NumberWithUnits::operator-- (int a) { // post-fix decrement (a--)
        NumberWithUnits copy = this->operator--();
        copy.setNumber(copy.getNumber()+1);
        return copy;
    }


    /*====================*/
    //  Binary Operators  //
    /*====================*/


    NumberWithUnits operator+ (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        string u1 = n1.getUnitType();
        string u2 = n2.getUnitType();
        double a = n1.getNumber();
        double b = n2.getNumber();

        if(u1 != u2) { // if n1 and n2 are of same unit type
            
            double x;
            x = get_conversion_rate(n1, n2); // should throw if units are not convertable. 
            return NumberWithUnits(a + (b*x), u1);
        }

        // only if unit types are the same code will reach here:
        return NumberWithUnits(a + b, u1);
    }

    NumberWithUnits operator-(const NumberWithUnits& n1, const NumberWithUnits& n2) {
        NumberWithUnits negative_n2 = (n2);
        return (n1 + (-negative_n2));
    }

    // This overload is inside the class scope (not a friend func) 
    // and returns a reference to *this, after calculation.
    // By doing so, gain the ability to invoke ((n1 += n2) += n3)
    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& num) {

        string u1 = this->unit_type;
        string u2 = num.unit_type;
        double a = this->number;
        double b = num.number;

        if(u1 != u2) { // if n1 and n2 are of same unit type
            
            double x;
            x = get_conversion_rate((*this), num); // should crash if units are not convertable. 
            setNumber(a + (b*x));
            // this->number = (a + (b*x));
            return *this;
        }

        // only if unit types are the same code will reach here:
        setNumber(a+b);
        // this->number = (a + b);
        return *this;
    }


    NumberWithUnits& NumberWithUnits::operator-= (const NumberWithUnits& num) {
        NumberWithUnits neg = num;
        return (*this).operator+=((-neg));
    }


    // Defined as a friend function to enable 2 arguments which are not of class type.
    // allows operations such as 3*a (while 'a' is of type 'NumberWithUnits').
    NumberWithUnits operator* (double x, const NumberWithUnits& num) {
        NumberWithUnits mult(x * num.getNumber(), num.getUnitType());
        return mult;
    }

    NumberWithUnits operator* (const NumberWithUnits& num, double x) {
        NumberWithUnits mult(x * num.getNumber(), num.getUnitType());
        return mult;
    }

    bool operator> (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        string u1 = n1.getUnitType();
        string u2 = n2.getUnitType();


        double a = n1.getNumber();
        double b = n2.getNumber();

        if(u1 != u2) { // if n1 and n2 are of same unit type
            
            double x;
            x = get_conversion_rate(n1, n2); // should crash if units are not convertable. 
            return (a-(b*x)) > EPSILON ? true : false;
            // return NumberWithUnits(a + (b*x), u1);
        }

        // only if unit types are the same code will reach here:
        return ((a-b) > EPSILON) ? true : false;
    }

    bool operator>= (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return ((n1 > n2) || (n1 == n2));
    }

    bool operator< (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return (n2 > n1);
    }

    bool operator<= (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return !(n1>n2); 
    }

    bool operator== (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return !((n1 > n2) || (n2 > n1));
        // string u1 = n1.getUnitType();
        // string u2 = n2.getUnitType();
        // double x;


        // double a = n1.getNumber();
        // double b = n2.getNumber();

        // cout << "n1: " << n1 << ", n2: " << n2 << endl;

        // if(u1 != u2) { // if n1 and n2 are of same unit type
            
        //     x = get_conversion_rate(n1, n2); // should crash if units are not convertable. 

        //     cout << "a-(b*x) = " << a << "-(" << b << "*" << x << ")" <<  " = " << (a-(b*x)) << endl;
        //     return (abs((a-(b*x))) < EPSILON || (a-(b*x)) == 0);
        // }

        // // if unit types are the same:
        // // cout << "a-b = " << a << "-" << b  <<  " = " << (a-b) << endl;

        // cout << "same unit types! converting...." << endl;
        // return (abs(a-b) < EPSILON || abs(a-b) == 0);
    }

    bool operator!= (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return !(n1 == n2);
    }


    ostream& operator<< (ostream& cout, const NumberWithUnits& a) {
        cout << a.number << "[" << a.unit_type << "]";
        return cout;
    }

    istream &operator>>(istream &istream, NumberWithUnits &num)
    {   
        string input,
        number, unit;
        getline(istream, input, ']'); // Empty the input stream until reaches char ']'.
        bool format_validation = false;
        int count = 0;
        char c;
        unsigned int i = 0;
        

        while (i < input.length())
        {
            c = input[i++];
            if (c == ' ') {continue;} // Skip white-spaces

            if(c == '[') {
                if(count == 0) {
                    count++;
                    format_validation = true;
                } else {
                    throw invalid_argument("Too many '['");
                }
            }
            if (char_is_a_number(c) && !format_validation){
                number += c; // Append to number as string
            }
            

            if ((char_is_a_lower_letter(c) || char_is_a_capital_letter(c)) && format_validation){
                unit += c; // Append to unit_type
            }
        } 

            double value = stod(number); // Convert a string to double

            num = NumberWithUnits(value, unit);
            return istream;
    }
};
