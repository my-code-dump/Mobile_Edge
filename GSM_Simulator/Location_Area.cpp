#include "Location_Area.h"

Location_Area::Location_Area(int ID, float min_X, float min_Y, float max_X, float max_Y, int las) {
    LA_ID = ID;
    minX = min_X;
    minY = min_Y;
    maxX = max_X;
    maxY = max_Y;
    totalLA = las;
}

int Location_Area::returnID() {
    return LA_ID;
}

float Location_Area::returnMinX() {
    return minX;
}

float Location_Area::returnMaxX() {
    return maxX;
}

float Location_Area::returnMinY() {
    return minY;
}

float Location_Area::returnMaxY() {
    return maxY;
}

void Location_Area::setTotalLA (int la) {
    totalLA = la;
}

int Location_Area::returnTotalLA () {
    return totalLA;
}

void Location_Area::printLAData() {
    std::cout << "LA[" << LA_ID << "]: {x:[" << minX << "," 
    << maxX << "], y:[" << minY << "]," << maxY 
    << " Total LA's = " << totalLA << "]}" << std::endl;
}