#include "NumberWithUnits.hpp"
using namespace std;
using namespace ariel;

int main () {


    ifstream file_stream{"filename.txt"};

    try {
        NumberWithUnits::read_units(file_stream);

    } catch (exception& e) {
        cout << e.what() << endl;
    }



    NumberWithUnits a = {3, "r_m"};
    NumberWithUnits b = {3.0000001, "r_m"};
    NumberWithUnits c = {3, "r_cm"};


    cout << "a = " << a << ", b = " << b << ", c = " << c << endl;

    cout << boolalpha;

    cout << "a > b = " << (a > b) << endl;
    cout << "b > a = " << (b > a) << endl;



    return 0;
}