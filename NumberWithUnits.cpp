#include "NumberWithUnits.hpp"
#include <map>
#include <vector>

const double EPSILON = 0.000001;
map<string, map<string, double>> units_map;

// bool are_convertible(const ariel::NumberWithUnits& n1, const ariel::NumberWithUnits& n2) {
//     for (const auto& pair : units_map[n2.getUnitType()]) {

//         if(pair.first == u1) {
//             cout << "Conversion can be made!" << endl;
//             return true;
//         }
//     }
//     return false;
// }

double get_conversion_rate(const ariel::NumberWithUnits &n1, const ariel::NumberWithUnits &n2){
    double conversion_rate;
    bool are_convertable = false;
    string u1 = n1.getUnitType();
    string u2 = n2.getUnitType();

    for (const auto& pair : units_map[u2]) {
        
        if(pair.first == u1) {
            cout << "Conversion can be made!" << endl;
            conversion_rate = units_map[u2][u1];
            are_convertable = true;
        }
    }

    if(are_convertable) {
        return conversion_rate;
    }

    string err = "Cannot perform operation between " + u1 + " and " + u2;
    throw invalid_argument(err);
}


namespace ariel{ 

    NumberWithUnits::NumberWithUnits(const double& num, const string& unit_type) {
        if(units_map.count(unit_type) == 0) {
            string err = "No valid unit type named '" +unit_type + "'";
            throw invalid_argument(err);
        } 

        setNumber(num);
        setUnitType(unit_type);
    }

    void add_all_valid_units(const string &from_unit, const string &to_unit) {

        string mid_unit;
        double conversion_rate;
        double conv_rate_mid_unit;
        for (const auto &pair : units_map[from_unit]) {
            mid_unit = pair.first; // The "middle man" that helps converting unit types.
                                   // Example: convert from KM to CM using mid_unit = M.
                                   // So calc: 1 KM = 1000 M => 1 KM =  [M->KM] * (1/[M->CM])

            if(pair.first != to_unit){
                conv_rate_mid_unit = units_map[from_unit][mid_unit]; // = pair.second

                conversion_rate = conv_rate_mid_unit * (1/units_map[from_unit][to_unit]);
                units_map[mid_unit][to_unit] = 1/conversion_rate; // KM->CM
                units_map[to_unit][mid_unit] = conversion_rate;   // CM->KM
        }
      }
    }

    void NumberWithUnits::read_units(ifstream& ifstream) {
        cout << "read_units()" << endl;


        // map<string, map<string, double>>::iterator it;
        string l_unit, r_unit, oper;
        double num1,num2;
        double x,y;

        while (ifstream >> num1 >> l_unit >> oper >> num2 >> r_unit) {
            if (num1 == 1 && oper == "=" && num2 > 0) {
                units_map[l_unit][r_unit] = num2; 
                units_map[r_unit][l_unit] = 1/num2;


                add_all_valid_units(l_unit, r_unit); // Iterate over l_unit's "neighbors" to match with r_unit. 
                add_all_valid_units(r_unit, l_unit); // Iterate over r_unit's "neighbors" to match with l_unit. 

            } 
        }
    }

    /*--------------------*/
    //   Unary Operators  //
    /*--------------------*/

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

    NumberWithUnits NumberWithUnits::operator++ (int a) { // post-fix inc (a++)
        NumberWithUnits copy = this->operator++();
        copy.setNumber(copy.getNumber()-1);
        return copy;
    }

    NumberWithUnits NumberWithUnits::operator-- (int a) { // post-fix decrement (a--)
        NumberWithUnits copy = this->operator--();
        copy.setNumber(copy.getNumber()+1);
        return copy;
    }


    /*====================*/
    //  Binary Operators  //
    /*====================*/


    NumberWithUnits operator+ (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        cout << "Binary (+)" << endl;
        string u1 = n1.getUnitType();
        string u2 = n2.getUnitType();
        double a = n1.getNumber();
        double b = n2.getNumber();

        if(u1 != u2) { // if n1 and n2 are of same unit type
            
            double x;
            x = get_conversion_rate(n1, n2); // should crash if units are not convertable. 
            return NumberWithUnits(a + (b*x), u1);
        }

        // if unit types are the same:
        cout << "same unit types! converting...." << endl;
        return NumberWithUnits(a + b, u1);
    }

    NumberWithUnits operator-(const NumberWithUnits& n1, const NumberWithUnits& n2) {
        cout << "Binary (-)" << endl;
        string u1 = n1.getUnitType();
        string u2 = n2.getUnitType();
        double a = n1.getNumber();
        double b = n2.getNumber();

        if(u1 != u2) { // if n1 and n2 are of same unit type
            
            double x;
            x = get_conversion_rate(n1, n2); // should crash if units are not convertable. 
            return NumberWithUnits(a - (b*x), u1);
        }

        cout << "same unit types! converting...." << endl;
        return NumberWithUnits(a - b, u1);
    }

    // This overload is inside the class scope (not a friend func) 
    // and returns a reference to *this, after calculation.
    // By doing so, gain the ability to invoke ((n1 += n2) += n3)
    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& num) {

        string u1 = getUnitType();
        string u2 = num.getUnitType();
        double a = getNumber();
        double b = num.getNumber();

        if(u1 != u2) { // if n1 and n2 are of same unit type
            
            double x;
            NumberWithUnits temp(getNumber(), getUnitType());
            x = get_conversion_rate(temp, num); // should crash if units are not convertable. 
            setNumber(a + (b*x));
            // setUnitType(u1);
            return *this;
        }

        cout << "same unit types! converting...." << endl;
        setNumber(a+b);
        return *this;


        // setNumber(num.number + getNumber());
        // setUnitType(num.unit_type);
        // return *this;
    }


    NumberWithUnits& NumberWithUnits::operator-= (const NumberWithUnits& num) {
        string u1 = getUnitType();
        string u2 = num.getUnitType();
        double a = getNumber();
        double b = num.getNumber();

        if(u1 != u2) { // if n1 and n2 are of same unit type
            
            double x;
            NumberWithUnits temp(getNumber(), getUnitType());
            x = get_conversion_rate(temp, num); // should crash if units are not convertable. 
            setNumber(a - (b*x));
            // setUnitType(u1);
            return *this;
        }

        cout << "same unit types! converting...." << endl;
        setNumber(a-b);
        return *this;
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

            cout << "a-(b*x) = " << a << "-(" << b << "*" << x << ")" <<  " = " << (a-(b*x)) << endl;
            return (a-(b*x)) > EPSILON ? true : false;
            // return NumberWithUnits(a + (b*x), u1);
        }

        // if unit types are the same:
        cout << "same unit types! converting...." << endl;
        // 3m > 4 km ? 
        return ((a-b) > EPSILON) ? true : false;
    }

    bool operator>= (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return ((n1 > n2) || (n1 == n2));
    }

    bool operator< (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return !(n1 >= n2);
    }

    bool operator<= (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return !(n1>n2); 
    }

    bool operator== (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        string u1 = n1.getUnitType();
        string u2 = n2.getUnitType();
        double x;


        double a = n1.getNumber();
        double b = n2.getNumber();

        cout << "n1: " << n1 << ", n2: " << n2 << endl;

        if(u1 != u2) { // if n1 and n2 are of same unit type
            
            x = get_conversion_rate(n1, n2); // should crash if units are not convertable. 

            cout << "a-(b*x) = " << a << "-(" << b << "*" << x << ")" <<  " = " << (a-(b*x)) << endl;
            return (abs((a-(b*x))) < EPSILON || (a-(b*x)) == 0);
        }

        // if unit types are the same:
        // cout << "a-b = " << a << "-" << b  <<  " = " << (a-b) << endl;

        cout << "same unit types! converting...." << endl;
        return (abs(a-b) < EPSILON || abs(a-b) == 0);
    }

    bool operator!= (const NumberWithUnits& n1, const NumberWithUnits& n2) {
        return !(n1 == n2);
    }


    // Has to be defined as a friend function inorder to get parameters that are not
    // of class type.
    ostream& operator<< (ostream& cout, const NumberWithUnits& a) {
        // cout << a.getNumber() << "[" << a.getUnitType() << "]";
        cout << a.number << "[" << a.unit_type << "]";
        return cout;
    }

    static istream& getAndCheckNextCharIs(istream& input, char expectedChar) {
        char actualChar;
        input >> actualChar;
        if (!input) {return input;}

        if (actualChar!=expectedChar) {
            // failbit is for format error
            input.setstate(ios::failbit);
        }
        return input;
    }
    

    bool char_is_a_number(char c) {
        return c >= 43 && c <= 57; // (*). (+), (-), (/), (.) AND (0-9)
    }

    bool char_is_a_lower_letter(char c) {
        return c >= 97 && c <= 122; // 'a'-'z'
    }

    bool char_is_a_capital_letter(char c) {
        return (c >= 64 && c <= 90) || c == 95; // 'A'-'Z' OR (_)
    }


        istream &operator>>(istream &istream, NumberWithUnits &num)
    {
        string input, number, unit;

        getline(istream, input, ']'); // Empty the input stream until reaches char ']'.

        char c;
        unsigned int i = 0;

        while (i < input.length())
        {
            c = input[i++];
            if (c == ' ' || c == '[') {continue;} // Skip white-spaces

            if (char_is_a_number(c)){
                number += c; // Append to number as strindg
            }

            if (char_is_a_lower_letter(c) || char_is_a_capital_letter(c)){
                unit += c; // Append to unit_type
            }
        } 

            double value = stod(number); // Convert a string to double

            num = NumberWithUnits(value, unit);
            return istream;
    }



//     istream& operator>> (istream& input, NumberWithUnits& num) {

//     //---------------------------------------------
//     // Does not check format
//     //---------------------------------------------
//     // char ch;
//     // return (input >> c._re >> ch >> c._im >> ch);
//     //---------------------------------------------

//     //---------------------------------------------
//     // Checks format, with rewind on failure.
//     //---------------------------------------------
//     double value = 0;
//     string unit, unit_;
//     char c;

//     // remember place for rewinding
//     ios::pos_type startPosition = input.tellg();

//     if ( (!(input >> value))                  ||
//          (!getAndCheckNextCharIs(input,'['))) {
        
//         // rewind on error
//         auto errorState = input.rdstate(); // remember error state
//         input.clear(); // clear error so seekg will work
//         input.seekg(startPosition); // rewind
//         input.clear(errorState); // set back the error flag

//         cout << "BAD INPUT" << endl;
//         cout << "val: " << value << ", unit: " << unit << endl;
//     } else {
//         cout << "GOOD INPUT" << endl;
//         cout << "val: " << value << ", unit: " << unit << endl;


//         // char c;  

//         // while ((c = getchar())) {
//         //     if (c != ' ') {
//         //         break;
//         //     }
//         // }
//         // for(unsigned int i = 0; i < unit.size(); i++) {
//         //     c = unit.at(i);
//         //     if (c != ']') {
//         //         unit_.push_back(c);
//         //     }
//         //     cout << c;
//         // }
//         num = NumberWithUnits(value, unit);

//     }

//     return input;
//     //---------------------------------------------
// }


    // istream& operator>> (istream& input, NumberWithUnits& num){

    //     // string in;

    //     // input >> in;



    //     // cout << "in = " << in << endl;

    //     // int index = 0;

    //     // while(index < in.size()) {
    //     //     cout << in.at(index++) << endl;;
    //     // }

    //     double value = 0;
    //     string unit;

    //     // remember place for rewinding
    //     ios::pos_type startPosition = input.tellg();
    //     cout << "===========" << num << "==================================" << endl;

    //     if ((!(input >> skipws >> value))                   ||
    //         (!getAndCheckNextCharIs(input,'['))   
    //         /*!(input >> unit)*/ 
    //         /*!getAndCheckNextCharIs(input,']'))*/) {

    //         // rewind on error
    //         auto errorState = input.rdstate(); // remember error state
    //         input.clear(); // clear error so seekg will work
    //         input.seekg(startPosition); // rewind
    //         input.clear(errorState); // set back the error flag
    //         cout << "===========" << num << "========BAD INPUT================" << endl;



    //     } else {


    //         char c = 'a';

    //         cout << "c = " << c << endl;

    //         while (c != '\r' && c != '\0' && c != '\e') {

    //             if (((c >= 64 || c <= 125) || c == ' ')) {

    //                 if (c != ']') { 
    //                     input >> skipws >> c;

    //                     cout << c << endl;
    //                 }
    //             } else {
    //                 cout << "HERE" << endl;
    //                 break;
    //             }
                
    //         }

    //         cout << c;




    //         // if(input >> skipws >> unit) {
    //         //     cout << "unit = " << unit << endl;
                

    //         //     string new_unit;
                
    //         //     if (unit.at(unit.size()-1) == ']') {
    //         //         char c;
    //         //         for(unsigned int i = 0; i < unit.size()-1 ; i++) {
    //         //             c = unit.at(i);
    //         //             new_unit.push_back(c);
    //         //         }
    //         //         num = NumberWithUnits(value, new_unit);

    //         //     }

    //         // } else {
    //         //     // cout << unit << endl;
    //         //     string err = "Wrong input format!";
    //         //     throw invalid_argument(err);
    //         // }


    //         // // if (unit.at(unit.size()-1) == ']') {
    //         //     char c = ']';
    //         //     while (input >> unit && unit.at(unit.size()-1) == ']') {
    //         //         cout << "unit = " << unit << endl;
    //         //         if(unit.at(unit.size()-1) == ']'){
    //         //             num.setNumber(value);
    //         //             // cout << unit << endl;
    //         //             num.setUnitType(unit);
    //         //             cout << "===========" << num << "========GOOD INPUT================" << endl;
    //         //             break;
    //         //         }

    //         //     }
                
    //         // }

    //         // num.number = value;
    //         // num.unit_type = unit;
    //         // c._re = new_re;
    //         // c._im = new_im;
    //     }

    //     return cin;
    // }  

    
};
