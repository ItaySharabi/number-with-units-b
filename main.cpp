#include "NumberWithUnits.hpp"
using namespace std;
using namespace ariel;

int main () {


    ifstream file_stream{"filename.txt"};

    try {
        NumberWithUnits::read_units(file_stream);
        // NumberWithUnits::printMap();

    } catch (exception& e) {
        cout << e.what() << endl;
    }



    NumberWithUnits a = {3, "r_m"};
    NumberWithUnits b = {3, "r_m"};
    NumberWithUnits c = {3, "r_m"};
    cout << "a = " << a << endl;

    istringstream iss{" -16 [r_m]   -7 [r_hour ]  8.8 [r_min ]"};

    iss >> a >> b >> c;

    cout << "a = " << a << ", b = " << b << ", c = " << c << endl;

    // NumberWithUnits a{41.0035, "r_m"};
    // NumberWithUnits b(0.0035, "r_m");
    // NumberWithUnits c{4.1, "r_km"};

    // a -= b;
    // cout << (a == c) << endl;



    // cout << "a - b  = " << (a -= b) << endl;

    // cout << (a > c) << endl;

    // cout << (c > a) << endl;

    // cout << (a <= c) << endl;


    // if(a > c) {
    //     cout << a << " > " << c << endl;
    // } else {
    //     cout << a << "<=" << c << endl;
    // }

    return 0;
}