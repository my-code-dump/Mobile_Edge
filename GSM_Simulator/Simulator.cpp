#include "Base_Station.h"
#include "User.h"
#include <cmath>
#include <random>

// All sizes are in KMs
struct GSM_Conditions {
    int totalBST = 100;
    int totalUsers = 5000;
    int totalHours = 1;
    float sizeBST = 1;
    float sizeLA = 2;
    float areaSize = 10;
} GSM;

// ----- Initialize base stations -----
void printBTSData (std::vector<Base_Station> &BST_List) {
    for (int i = 0; i < BST_List.size(); i++) {
       BST_List[i].printBTSData(); 
    }
}

void initializeBTS (std::vector<Base_Station> &BST_List) {
    int maxSize = GSM.areaSize;
    int id = 1;
    for(int i = 0; i < maxSize; i++) {
        for (int j = 0; j < maxSize; j++) {
            Base_Station bst (id,j,i,j+1,i+1);
            BST_List.push_back(bst);
            id++;
        }
    }
}

// ----- Initialize user location -----
void printUserData (std::vector<User> &User_List) {
    for (int i = 0; i < User_List.size(); i++) {
       User_List[i].printUserData(); 
    }
}

float pickANumber (float min, float max) {
    std::random_device rd;
    std::mt19937 seed(rd());

    std::uniform_real_distribution<float> dis(min, max);
    float output = dis(seed);

    return output;
}

void initializeUsers (std::vector<User> &User_List) {
    int userID = 1;
    int distributedUsers = sqrt(GSM.totalUsers);
    int remainders = GSM.totalUsers - pow(distributedUsers,2);
    float increment = GSM.areaSize / (distributedUsers - 1);
    float yIncrement = 0;

    for (int i = 0; i < distributedUsers; i++) {
        float xIncrement = 0;
        for (int j = 0; j < distributedUsers; j++) {
            User usr(userID, xIncrement, yIncrement);        
            xIncrement += increment;
            User_List.push_back(usr);
            userID++;
        }
        yIncrement += increment;
    }
    
    if (remainders != 0) {
        // Randomly distribute if the total # of users cannot be evenly distributed
        int remainderID = GSM.totalUsers - remainders + 1;
        for (int i = 0; i < remainders; i++) {
            User usr (remainderID,pickANumber(0,GSM.areaSize),pickANumber(0,GSM.areaSize));
            User_List.push_back(usr);
            remainderID++;
        }
    }
}

// ----- Experiment -----
void runExperiment (std::vector<Base_Station> BST_List, std::vector<User> User_List) { 

}

int main () {
    std::vector<Base_Station> BST_List;
    initializeBTS(BST_List);
    printBTSData(BST_List);

    std::vector<User> User_List;
    initializeUsers(User_List);    
    //printUserData(User_List);

    return 0;
}
