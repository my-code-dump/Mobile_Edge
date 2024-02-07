//#include <iostream>
//#include <vector>
#include "Base_Station.h"
#include "User.h"

// All sizes are in KMs
struct GSM_Conditions {
    float areaSize = 10;
    int totalBST = 100;
    float sizeBST = 1;
    float sizeLA = 2;
    int totalUsers = 5000;
} GSM;

int main () {
    std::vector<Base_Station> BST_List;
    int maxSize = GSM.areaSize;
    int id = 1;
    for(int i = 0; i < maxSize; i++) {
        for (int j = 0; j < maxSize; j++) {
            Base_Station bst (id,j,i,j+1,i+1);
            BST_List.push_back(bst);
            id++;
        }
    }

    for (int i = 0; i < BST_List.size(); i++) {
       BST_List[i].printBTS(); 
    }

    User tester (1,1,1);
    std::cout << tester.returnID() << std::endl;

    return 0;
}
