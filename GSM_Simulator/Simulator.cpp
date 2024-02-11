#include "Base_Station.h"
#include "User.h"
#include "Location_Area.h"
#include <cmath>
#include <random>
#include <ctime>
#include <climits>

// All sizes are in KMs
struct GSM_Conditions {
    int totalBST = 100;
    int totalUsers = 100;
    int totalHours = 1;
    float sizeBST = 1;
    float sizeLA = 3;
    float areaSize = 10;
    float metersPerSecond = 0.10;
    std::vector<int> network_traffic;
    std::vector<int> paging_traffic;
} GSM;

// ----- Initialize base stations -----
void printBTSData (std::vector<Base_Station> &BST_List) {
    for (int i = 0; i < BST_List.size(); i++) {
       BST_List[i].printBTSData(); 
    }
}

void initializeBTS (std::vector<Base_Station> &BST_List) {
    int maxSize = GSM.areaSize;
    int id = 0;
    for(int i = 0; i <= maxSize; i++) {
        for (int j = 0; j <= maxSize; j++) {
            Base_Station bst (id,j,i,j+1,i+1);
            BST_List.push_back(bst);
            id++;
        }
    }
    //printBTSData(BST_List);
}

void initializeLA (std::vector<Base_Station> &BST_List, std::vector<Location_Area> &LA_List) {
    int size = GSM.areaSize + 1;
    int iterate = GSM.sizeLA;
    int index = 0;
    float minX = 0;
    float maxX = 0;
    float minY = 0;
    float maxY = 0;
    std::vector<std::vector<int>> test_list;
    std::vector<std::vector<float>> coor_list;

    if (iterate != 0) {
        int jump = 0;
        int bstListSize = BST_List.size();
        while (jump < bstListSize) {
            int limit = jump + size;
            for (int i = jump; i < (limit - 1); i+=iterate) {
                //std::vector<int> block;
                //std::vector<float> blockFloat;
                
                int leftCornerTop = i;
                //block.push_back(leftCornerTop);
                //block.push_back(leftCornerTop);

                minX = BST_List[leftCornerTop].returnMinX();
                minY = BST_List[leftCornerTop].returnMinY();
                //blockFloat.push_back(minX);
                //blockFloat.push_back(minY);

                int rightCornerTop = i + iterate;
                if (rightCornerTop >= limit) {
                    rightCornerTop = limit - 1;
                }
                maxX = BST_List[rightCornerTop].returnMaxX();
                //blockFloat.push_back(maxX);
                //block.push_back(rightCornerTop);
                
                int leftCornerBot = i + (size * iterate);
                int tempIterate = iterate;
                while (leftCornerBot >= bstListSize) {
                    tempIterate--;
                    leftCornerBot = i + (size * tempIterate);
                }
                maxY = BST_List[leftCornerBot].returnMaxY();
                //blockFloat.push_back(maxY);
                //block.push_back(leftCornerBot);
                
                //test_list.push_back(block);
                //coor_list.push_back(blockFloat);
                Location_Area LA (index, minX, minY, maxX, maxY);
                LA_List.push_back(LA);
                index++;
            }
            jump += (iterate + 1) * size;
            //std::cout << jump << std::endl;
        }
        /*
        for (int i = 0; i < test_list.size(); i++) {
            std::cout << "[ ";
            for (int j = 0; j < test_list[i].size(); j++) {
                std::cout << test_list[i][j] << "=" << coor_list[i][j] << " ";
            }
            std::cout << "]";
            std::cout << std::endl;
        }
        */
    }
    else {
        throw std::invalid_argument("Locational area is 0!");
    }
}

// ----- Initialize user location -----
void printUserData (std::vector<User> &User_List) {
    for (int i = 0; i < User_List.size(); i++) {
       User_List[i].printUserData(); 
    }
}

float pickANumber (float min, float max, float isClone) {
    std::random_device rd;
    std::mt19937 seed(rd());

    std::uniform_real_distribution<float> dis(min, max);
    float output = dis(seed);
    if (isClone == output) {
        pickANumber (min, max, isClone);
    }
    return output;
}

void initializeUsers (std::vector<Base_Station> &BST_List, std::vector<User> &User_List) {
    int userID = 0;
    int distributedUsers = sqrt(GSM.totalUsers);
    int remainders = GSM.totalUsers - pow(distributedUsers,2);
    float increment = (GSM.areaSize) / (distributedUsers - 1);
    float yIncrement = 0;

    for (int i = 0; i < distributedUsers; i++) {
        float xIncrement = 0;
        for (int j = 0; j < distributedUsers; j++) {
            User usr(userID, xIncrement, yIncrement, 
                    pickANumber(0,(GSM.areaSize), -1), pickANumber(0,(GSM.areaSize), -1));        
            
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
            User usr (remainderID, pickANumber(0,(GSM.areaSize), -1), pickANumber(0, (GSM.areaSize), -1), 
                    pickANumber(0,(GSM.areaSize), -1), pickANumber(0,(GSM.areaSize), -1));
            
            User_List.push_back(usr);
            remainderID++;
        }
    }
    //printUserData(User_List);
}

// Define the xorshift96 state variables
static unsigned long x = 123456789, y = 362436069, z = 521288629;

// xorshift96 pseudorandom number generator function
unsigned long xorshf96() {
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

// Function to generate random float within a range [min, max]
float getRandomFloatInRange(float min, float max) {
    // Calculate the range
    float range = max - min;
    
    // Scale the random number and add min to ensure it falls within the range
    float randomNumber = (float)xorshf96() / (float)ULONG_MAX * range + min;
    
    return randomNumber;
}

// ----- Experiment -----
void moveUser (User &usr) {
    float goHereX = usr.returnMoveToX();
    float goHereY = usr.returnMoveToY();
    float currentX = usr.returnUserX();
    float currentY = usr.returnUserY();
    float distanceToTravel = getRandomFloatInRange(0.0,0.10001);

    float angle = 0;
    if (goHereX != currentX && goHereY != currentY) {
        angle = atan2(goHereY - currentY, goHereX - currentX);
    } else if (goHereY == currentY) {
        angle = (goHereX > currentX) ? 0 : M_PI; // 0 or 180 degrees
    } else { // goHereX == currentX
        angle = (goHereY > currentY) ? M_PI / 2.0 : -M_PI / 2.0; // 90 or -90 degrees
    }
    
    // Update current coordinates
        currentX += distanceToTravel * cos(angle);
        if (usr.returnIsMoveToXGreaterUserX()) {
            if (currentX > goHereX) {
                currentX = goHereX;
            }
        }
        else {
            if (currentX < goHereX) {
                currentX = goHereX;
            }
        }
    //if ((currentY + distanceToTravel) )
    currentY += distanceToTravel * sin(angle);
    if (usr.returnIsMoveToYGreaterUserY()) {
        if (currentY > goHereY) {
            currentY = goHereY;
        }
    }
    else {
        if (currentY < goHereY) {
            currentY = goHereY;
        }
    }
    usr.setUserX(currentX);
    usr.setUserY(currentY);
}

void runExperiment (std::vector<Base_Station> BST_List, std::vector<User> User_List) { 
    int simulationLimit = GSM.totalHours * 60 * 60;
    int totalUsers = User_List.size();
    for (int s = 0; s < simulationLimit; s++) {
        std::cout << "s = " << s << std::endl;
        for (int u = 0; u < totalUsers; u++) {
            //std::cout << "u = " << u << std::endl;
            moveUser(User_List[u]);
            if ((User_List[u].returnUserX() == User_List[u].returnMoveToX()) && (User_List[u].returnUserY() == User_List[u].returnMoveToY())) {
                //std::cout << "User[" << u << "] Finished" << std::endl;
                float tempEndX = pickANumber(0,(GSM.areaSize), User_List[u].returnMoveToX());
                float tempEndY = pickANumber(0,(GSM.areaSize), User_List[u].returnMoveToY());
                User_List[u].setMoveToX(tempEndX);
                User_List[u].setMoveToY(tempEndY);
            }
        }
    }
    /*
    float curX = 10;
    float curY = 10;
    float endX = 0;
    float endY = 0;
    User test (0,curX,curY,endX,endY);
    for (int i = 0; i < 3600; i++) {
        moveUser(test);
        std::cout << "At x:" << test.returnUserX() << " y:" << test.returnUserY() << 
        " | Final Destination x:" << test.returnMoveToX() << " y:" << test.returnMoveToY() << std::endl;
        if ((test.returnUserX() == test.returnMoveToX()) && (test.returnUserY() == test.returnMoveToY())) {
                std::cout << "User[" << "test" << "] Finished" << std::endl;
            std::cout << "finished i=" << i << std::endl;
            break;
            float tempEndX = pickANumber(0,(GSM.areaSize), test.returnMoveToX());
            float tempEndY = pickANumber(0,(GSM.areaSize), test.returnMoveToY());
            test.setMoveToX(tempEndX);
            test.setMoveToY(tempEndY);
        }
    }
    */
}

int main () {
    std::vector<Base_Station> BST_List;
    initializeBTS(BST_List);
    //printBTSData(BST_List);
    std::vector<Location_Area> LA_LIST;
    initializeLA(BST_List, LA_LIST);

    std::vector<User> User_List;
    initializeUsers(BST_List, User_List);    
    printUserData(User_List);

    //runExperiment(BST_List, User_List);
    return 0;
}
