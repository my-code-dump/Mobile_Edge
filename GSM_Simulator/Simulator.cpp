#include <iostream>
#include <vector>
#include "Base_Station.h"

// All sizes are in kilometers
struct GSM_Conditions {
    float areaSize = 10;
    int totalBTS = 100;
    float sizeBTS = 1;
    float sizeLA = 2;
    int totalUsers = 5000;
} GSM;
class User {

};

int main () {
    Base_Station x (10,10.5,0,5,6);
    return 0;
}
