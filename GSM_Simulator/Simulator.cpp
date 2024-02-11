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
    int totalUsers = 5000;
    int totalHours = 1;
    int sizeLA = 5;
    float sizeBST = 1;
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
    for(int i = 0; i < maxSize; i++) {
        for (int j = 0; j < maxSize; j++) {
            Base_Station bst (id,j,i,j+1,i+1);
            BST_List.push_back(bst);
            id++;
        }
    }
}

void printLA (std::vector<Location_Area> &LA_List) {
    for (int i = 0; i < LA_List.size(); i++) {
       LA_List[i].printLAData(); 
    }
}
void initializeLA (std::vector<Base_Station> &BST_List, std::vector<Location_Area> &LA_List) {
    int size = GSM.areaSize;
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
            //td::cout << "limit = " << limit << std::endl;
            for (int i = jump; i < limit; i+=iterate) {
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

                int rightCornerTop = i + (iterate - 1);
                if (rightCornerTop >= limit) {
                    rightCornerTop = limit;
                }
                maxX = BST_List[rightCornerTop].returnMaxX();
                blockFloat.push_back(maxX);
                block.push_back(rightCornerTop);
                
                int leftCornerBot = i + (size * (iterate-1));
                while (leftCornerBot >= bstListSize) {
                    tempIterate--;
                    leftCornerBot = i + (size * iterate);
                }
                maxY = BST_List[leftCornerBot].returnMaxY();
                blockFloat.push_back(maxY);
                block.push_back(leftCornerBot);
                
                Location_Area LA (index, minX, minY, maxX, maxY);
                LA_List.push_back(LA);
                test_list.push_back(block);
                coor_list.push_back(blockFloat);
                index++;
            }
            jump += iterate * size;
            //std::cout << "jump = " << jump << std::endl;
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
    printLA(LA_List);
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

void initializeUsers (std::vector<Location_Area> &LA_List, std::vector<User> &User_List) {
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
    
    int userLimit = GSM.totalUsers;
    int LAs = LA_List.size();
    for (int u = 0; u < userLimit; u++) { 
        float userX = User_List[u].returnUserX();
        float userY = User_List[u].returnUserY();
        for (int l = 0; l < LAs; l++) {
            float minX = LA_List[l].returnMinX();
            float minY = LA_List[l].returnMinY();
            float maxX = LA_List[l].returnMaxX();
            float maxY = LA_List[l].returnMaxY();
            if (((minX <= userX) && (maxX >= userX)) 
                && (((minY <= userY) && (maxY >= userY)))) {
                User_List[u].setLAID(LA_List[l].returnID());
                break;
            }
        }
        //User_List[u].printUserData();
    }

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

float getPoisson () {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<> poisson(0.0125);

    return poisson(gen);
}

int checkBST (std::vector<Location_Area> &LA_List, User &usr, float currentX, float currentY) {
    int updated = 0;
    int laid = usr.getLAID();
    int LAs = LA_List.size();
    float minX = LA_List[laid].returnMinX();
    float minY = LA_List[laid].returnMinY();
    float maxX = LA_List[laid].returnMaxX();
    float maxY = LA_List[laid].returnMaxY();
    if (((minX > currentX) || (maxX < currentX)) 
        || (((minY > currentY) || (maxY < currentY)))) {
        for (int l = 0; l < LAs; l++) {
            float minX = LA_List[l].returnMinX();
            float minY = LA_List[l].returnMinY();
            float maxX = LA_List[l].returnMaxX();
            float maxY = LA_List[l].returnMaxY();
            if (((minX <= currentX) && (maxX >= currentX)) 
                && (((minY <= currentY) && (maxY >= currentY)))) {
                usr.setLAID(LA_List[l].returnID());
                break;
            }
        }
        updated = 800;
    }

    return updated;
}

// ----- Experiment -----
int moveUser (std::vector<Location_Area> &LA_List, User &usr) {
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
    /*
    std::cout << "Usr[" << usr.returnID() << "] | Current X = " << currentX << " Current Y = " 
    << currentY << " | Goal x:" << goHereX << " Goal y:" << goHereY << std::endl; 
    */
    usr.setUserX(currentX);
    usr.setUserY(currentY);

    int result = checkBST(LA_List, usr, currentX, currentY);
    return result;
}

void runExperiment (std::vector<Location_Area> LA_List, std::vector<User> User_List) { 
    char buffer[100];
    sprintf(buffer, "%dLAsizeLocationUpdate.csv", GSM.sizeLA);
    char *filename = buffer;
    FILE *fp;
    fp = fopen(filename, "w+");
    int simulationLimit = GSM.totalHours * 60 * 60;
    int totalUsers = User_List.size();
    User_List[0].printUserData();
    for (int s = 0; s < simulationLimit; s++) {
        int locationUpdate = 0;
        for (int u = 0; u < GSM.totalUsers; u++) {
            locationUpdate += moveUser(LA_List, User_List[u]);
            //float test = getPoisson();
            if ((User_List[u].returnUserX() == User_List[u].returnMoveToX()) && (User_List[u].returnUserY() == User_List[u].returnMoveToY())) {
                //std::cout << "User[" << u << "] Finished" << std::endl;
                float tempEndX = pickANumber(0,(GSM.areaSize), User_List[u].returnMoveToX());
                float tempEndY = pickANumber(0,(GSM.areaSize), User_List[u].returnMoveToY());
                User_List[u].setMoveToX(tempEndX);
                User_List[u].setMoveToY(tempEndY);
                //std::cout << "=== GOAL ACHIEVED! renew! ===" << std::endl;
            }
        }
        fprintf(fp,"%d,%d\n",s,locationUpdate);
        std::cout << "s = " << s << std::endl;
        /*
        if (locationUpdate != 0){
            std::cout << "+Location Update KB:" << locationUpdate << std::endl; 
        }
        */
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
    std::vector<Location_Area> LA_List;
    initializeLA(BST_List, LA_List);

    std::vector<User> User_List;
    initializeUsers(LA_List, User_List);    
    //printUserData(User_List);

    runExperiment(LA_List, User_List);
    return 0;
}
