#include "Base_Station.h"

Base_Station::Base_Station(int id, 
                     float min_X, float min_Y, 
                     float max_X, float max_Y) {

            BTS_ID = id;
            minX = min_X;
            minY = min_Y;
            maxX = max_X;
            maxY = max_Y;
}

void Base_Station::setID (int id) {
    BTS_ID = id;
}

int Base_Station::returnID () {
    return BTS_ID;
}

void Base_Station::setMinX (float x) {
    minX = x;
}

float Base_Station::returnMinX () {
    return minX;
}

void Base_Station::setMinY (float y) {
    minY = y;
}

float Base_Station::returnMinY () {
    return minY;
}

void Base_Station::setMaxX (float x) {
    maxX = x;
}

float Base_Station::returnMaxX () {
    return minX;
}

void Base_Station::setMaxY (float y) {
    maxY = y;
}

float Base_Station::returnMaxY () {
    return minY;
}
