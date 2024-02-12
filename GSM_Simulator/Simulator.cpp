#include "Base_Station.h"
#include "User.h"
#include "Location_Area.h"
#include <cmath>
#include <random>
#include <ctime>
#include <climits>
#include <getopt.h>
#include <errno.h>

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

// ----- Randomizer functions needed for downstairs -----
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
    float range = max - min;
    float randomNumber = (float)xorshf96() / (float)ULONG_MAX * range + min;
    
    return randomNumber;
}
// ----- Initialize Base Stations -----
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

// ----- Initializing the Location Area -----
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
            for (int i = jump; i < limit; i+=iterate) {
                int xAxis = GSM.sizeLA;
                int yAxis = GSM.sizeLA;
                
                int leftCornerTop = i;
                minX = BST_List[leftCornerTop].returnMinX();
                minY = BST_List[leftCornerTop].returnMinY();

                int rightCornerTop = i + (iterate - 1);
                if (rightCornerTop >= limit) {
                    rightCornerTop = limit - 1;
                    xAxis = abs(rightCornerTop - i) + 1;
                }
                maxX = BST_List[rightCornerTop].returnMaxX();
                
                int tempIterate = iterate;
                int leftCornerBot = i + (size * (iterate-1));
                int tempLeftCornerBot = leftCornerBot;
                while (leftCornerBot >= bstListSize) {
                    tempIterate--;
                    leftCornerBot = i + (size * tempIterate);
                    yAxis = (abs(leftCornerBot - leftCornerTop)/size) + 1;
                }
                maxY = BST_List[leftCornerBot].returnMaxY();
                
                int totalLAs = xAxis * yAxis;
                Location_Area LA (index, minX, minY, maxX, maxY, totalLAs);
                LA_List.push_back(LA);
                index++;
            }
            jump += iterate * size;
        }
    }
    else {
        throw std::invalid_argument("Locational area is 0!");
    }
}

// ----- Initialize User Location -----
void printUserData (std::vector<User> &User_List) {
    for (int i = 0; i < User_List.size(); i++) {
       User_List[i].printUserData(); 
    }
}


void initializeUsers (std::vector<Location_Area> &LA_List, std::vector<User> &User_List) {
    int userID = 0;
    int maxUsers = GSM.totalUsers;

    for (int i = 0; i < maxUsers; i++) {
        User usr (userID, pickANumber(0,(GSM.areaSize), -1), pickANumber(0, (GSM.areaSize), -1), 
                pickANumber(0,(GSM.areaSize), -1), pickANumber(0,(GSM.areaSize), -1));
        
        User_List.push_back(usr);
        userID++;
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
    }
}

// ----- Experiment Section -----

// This function is for checking if the user has moved out of the location area.
int checkLA (std::vector<Location_Area> &LA_List, User &usr, float currentX, float currentY) {
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

// This function is to get the direction of the user
int moveUserAndLocationUpdate (std::vector<Location_Area> &LA_List, User &usr) {
    float goHereX = usr.returnMoveToX();
    float goHereY = usr.returnMoveToY();
    float currentX = usr.returnUserX();
    float currentY = usr.returnUserY();
    // get a distance in meters between 0 to 10
    float distanceToTravel = getRandomFloatInRange(0.0,0.10001);

    // IDK this is crazy math I don't understand it just works I guess
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

    usr.setUserX(currentX);
    usr.setUserY(currentY);

    int result = checkLA(LA_List, usr, currentX, currentY);
    return result;
}

// This funciton is needed to check for the paging proccess where the user 
// can receive a call every 0.0125 sec. Based on poisson.
int getPoisson () {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<> poisson(0.0125);

    return poisson(gen);
}

int pagingProcess(std::vector<Location_Area> &LA_List, User &user) {
    int paging = 0;
    int pois = getPoisson();
    //std::cout << "poisson = " << pois << std::endl;
    if (pois == 1) {
        int index = user.getLAID();
        paging = 60 * LA_List[index].returnTotalLA();
    }

    return paging;
}

// Main experiment function. It does both the location updates and paging request and prints 
// data is csv where a python script will be doing the graphing
void runExperiment (std::vector<Location_Area> LA_List, std::vector<User> User_List) { 
    // File 1 - location update 
    char buffer[100];
    sprintf(buffer, "%dLAsizeLocationUpdate.csv", GSM.sizeLA);
    char *filename = buffer;
    FILE *fp;
    fp = fopen(filename, "w+");
    fprintf(fp,"Seconds,Location_Update\n");
    
    // File 2 - paging proccess
    sprintf(buffer, "%dLAsizePagingProcess.csv", GSM.sizeLA);
    char *filename2 = buffer;
    FILE *fp2;
    fp2 = fopen(filename2, "w+");
    fprintf(fp2,"Seconds,Paging_Process\n");

    int simulationLimit = GSM.totalHours * 60 * 60;
    int totalUsers = User_List.size();

    for (int s = 0; s < simulationLimit; s++) {
        int locationUpdate = 0;
        int pagingCalls = 0;
        for (int u = 0; u < GSM.totalUsers; u++) {
            locationUpdate += moveUserAndLocationUpdate(LA_List, User_List[u]);
            pagingCalls += pagingProcess(LA_List, User_List[u]);
            if ((User_List[u].returnUserX() == User_List[u].returnMoveToX()) && (User_List[u].returnUserY() == User_List[u].returnMoveToY())) {
                float tempEndX = pickANumber(0,(GSM.areaSize), User_List[u].returnMoveToX());
                float tempEndY = pickANumber(0,(GSM.areaSize), User_List[u].returnMoveToY());
                User_List[u].setMoveToX(tempEndX);
                User_List[u].setMoveToY(tempEndY);
            }
        }
        fprintf(fp,"%d,%d\n", s, locationUpdate);
        fprintf(fp2,"%d,%d\n", s, pagingCalls);
        std::cout << "s = " << s << std::endl;
    }
}

int main (int argc, char* argv[]) {
    int n = 0;
    int m = 0;
    int opt = 0;
    while ((opt = getopt(argc,argv,"N:")) != -1) {
        switch (opt) {
            case 'N':
                n = atoi(optarg);
                break;
            default:
                printf("Invalid Inputs\n");
                break;
        }
    }

    GSM.sizeLA = n;

    std::vector<Base_Station> BST_List;
    initializeBTS(BST_List);
    std::vector<Location_Area> LA_List;
    initializeLA(BST_List, LA_List);

    std::vector<User> User_List;
    initializeUsers(LA_List, User_List);    

    runExperiment(LA_List, User_List);
    return 0;
}
