#include "NumberWithUnits.hpp"
using namespace std;
using namespace ariel;

int main () {


    ifstream file_stream{"filename.txt"};

    try {
        NumberWithUnits::read_units(file_stream);
        cout << "============================" << endl;

        NumberWithUnits cm(1, "r_cm");

        cout << cm << endl;
        NumberWithUnits m(2, "r_m");
        NumberWithUnits km(3, "r_km");
        NumberWithUnits mm(4, "r_mm");
        NumberWithUnits nm(5, "r_nm");

        cout << cm << " + " << nm << " = " << (cm + nm) << endl;
        cout << cm << " + " << mm << " = " << (cm + mm) << endl;
        cout << cm << " + " << m << " = " << (cm + m) << endl;
        cout << cm << " + " << km << " = " << (cm + km) << endl;

        cout << m << " + " << nm << " = " << (m + nm) << endl;
        cout << m << " + " << mm << " = " << (m + nm) << endl;
        cout << m << " + " << cm << " = " << (m + nm) << endl;
        cout << m << " + " << km << " = " << (m + nm) << endl;

        // cout << cm << " + " << nm << " = " << (m + nm) << endl;
        // cout << cm << " + " << mm << " = " << (m + nm) << endl;
        // cout << cm << " + " << cm << " = " << (m + nm) << endl;
        // cout << cm << " + " << km << " = " << (m + nm) << endl;
        
        

    } catch (exception& e) {
        cout << e.what() << endl;
    }   


    








    // while (1) {
    //     cout << "enter format: " << endl;
    //     NumberWithUnits a(3, "r_km");
    //     cin >> a;

    //     cout << a << endl;
    // }


    // NumberWithUnits a = {3, "r_m"};
    // NumberWithUnits b = {3.0000001, "r_m"};
    // NumberWithUnits c = {3, "r_cm"};


    // cout << "a = " << a << ", b = " << b << ", c = " << c << endl;

    // cout << boolalpha;

    // cout << "a > b = " << (a > b) << endl;
    // cout << "b > a = " << (b > a) << endl;



    return 0;
}