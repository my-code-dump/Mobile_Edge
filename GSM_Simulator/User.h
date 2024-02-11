#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include "Base_Station.h"
#include "Location_Area.h"

class User {
    private:
        int USER_ID;
        int BS_ID;
        bool isMoveToXGreaterUserX;
        bool isMoveToYGreaterUserY;
        float userX;
        float userY;
        float moveToX;
        float moveToY;
    
    public:
        std::vector<int> BTS;
        
        User(int id, float user_x, float user_y, float move_to_x, float move_to_y);
        
        void setID (int);

        int returnID ();

        void setUserX (float);

        float returnUserX ();

        void setUserY (float);

        float returnUserY ();
        
        void setMoveToX (float);

        float returnMoveToX ();

        void setMoveToY (float);

        float returnMoveToY ();

        void setIsMoveToXGreaterUserX (bool);

        bool returnIsMoveToXGreaterUserX ();

        void setIsMoveToYGreaterUserY (bool);

        bool returnIsMoveToYGreaterUserY ();

        void printUserData (); 
};

#endif