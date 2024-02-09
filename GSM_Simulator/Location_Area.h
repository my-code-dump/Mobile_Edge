#ifndef LA_H
#define LA_H

#include "Base_Station.h"
#include <iostream>
#include <vector>

class Location_Area {
    private:
        int LASize;
        int totalSize;
        std::vector<Base_Station> LA_BSTs;
        std::vector<Base_Station> ALL_BSTs;
    
    public:
        Location_Area(int, int, std::vector<Base_Station> &All_BST);

        void setLASize(int);
        int getLASize(int);

        void setTotalSize(int);

        void determineOwnership ();
};

#endif