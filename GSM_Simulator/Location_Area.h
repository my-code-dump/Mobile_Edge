#ifndef LA_H
#define LA_H

#include "Base_Station.h"
#include <iostream>

class Location_Area {
    private:
        int LA_ID;
        float minX;
        float maxX;
        float minY;
        float maxY;
    
    public:
        Location_Area(int, float, float, float, float);

        int returnID ();
        
        float returnMinX();
        
        float returnMaxX();
        
        float returnMinY();
        
        float returnMaxY();

        void printLAData();
};

#endif