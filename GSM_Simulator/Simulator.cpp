#include "Base_Station.h"
#include "User.h"
#include "Location_Area.h"
#include <cmath>
#include <random>

// All sizes are in KMs
struct GSM_Conditions {
    int totalBST = 100;
    int totalUsers = 5000;
    int totalHours = 1;
    float sizeBST = 1;
    float sizeLA = 3;
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
    for(int i = 0; i <= maxSize; i++) {
        for (int j = 0; j <= maxSize; j++) {
            Base_Station bst (id,j,i,j+1,i+1);
            BST_List.push_back(bst);
            id++;
        }
    }
    //printBTSData(BST_List);
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

float findMax (std::vector<float> numbers) {
    return 0;
}

void initializeLA (std::vector<Base_Station> &BST_List) {
    int size = GSM.areaSize + 1;
    int iterate = GSM.sizeLA;
    int index = 0;
    float minX = 0;
    float maxX = 0;
    float minY = 0;
    float maxY = 0;
    std::vector<Location_Area> LS_LIST;
    std::vector<std::vector<int>> test_list;
    std::vector<std::vector<float>> coor_list;

    if (iterate != 0) {
        int jump = 0;
        int bstListSize = BST_List.size();
        while (jump < bstListSize) {
            int limit = jump + size;
            for (int i = jump; i < (limit - 1); i+=iterate) {
                int tempIterate = iterate;
                std::vector<int> block;
                std::vector<float> blockFloat;
                
                int leftCornerTop = i;
                block.push_back(leftCornerTop);
                block.push_back(leftCornerTop);

                minX = BST_List[leftCornerTop].returnMinX();
                minY = BST_List[leftCornerTop].returnMinY();
                blockFloat.push_back(minX);
                blockFloat.push_back(minY);

                int rightCornerTop = i + tempIterate;
                if (rightCornerTop >= limit) {
                    rightCornerTop = limit - 1;
                }
                maxX = BST_List[rightCornerTop].returnMaxX();
                blockFloat.push_back(maxX);
                block.push_back(rightCornerTop);
                
                int leftCornerBot = i + (size * (tempIterate));
                while (leftCornerBot >= bstListSize) {
                    tempIterate--;
                    leftCornerBot = i + (size * tempIterate);
                }
                maxY = BST_List[leftCornerBot].returnMaxY();
                blockFloat.push_back(maxY);
                block.push_back(leftCornerBot);
                
                test_list.push_back(block);
                coor_list.push_back(blockFloat);
                index++;
            }
            jump += (iterate + 1) * size;
            //std::cout << jump << std::endl;
        }
        for (int i = 0; i < test_list.size(); i++) {
            std::cout << "[ ";
            for (int j = 0; j < test_list[i].size(); j++) {
                std::cout << test_list[i][j] << "=" << coor_list[i][j] << " ";
            }
            std::cout << "]";
            std::cout << std::endl;
        }
    }
    else {
        throw std::invalid_argument("Locational area is 0!");
    }
}

// ----- Experiment -----
void runExperiment (std::vector<Base_Station> BST_List, std::vector<User> User_List) { 

}

int main () {
    std::vector<Base_Station> BST_List;
    initializeBTS(BST_List);
    //printBTSData(BST_List);

    std::vector<User> User_List;
    initializeUsers(User_List);    
    //printUserData(User_List);
    initializeLA(BST_List);

    return 0;
}
