#include "User.h"

User::User(int id, float user_X, float user_Y, float move_to_X, float move_to_Y) {
    USER_ID = id;
    LA_ID = -1;
    userX = user_X;
    userY = user_Y;
    setMoveToX(move_to_X);
    setMoveToY(move_to_Y);
}

void User::setID (int id) {
    USER_ID = id;
}

int User::returnID () {
    return USER_ID;
}

void User::setUserX (float x) {
    userX = x;
}

float User::returnUserX () {
    return userX;
}

void User::setUserY (float y) {
    userY = y;
}

float User::returnUserY () {
    return userY;
}

void User::setMoveToX (float x) {
    moveToX = x;
    if (moveToX > userX) {
        isMoveToXGreaterUserX = true;
    }
    else {
        isMoveToXGreaterUserX = false;
    }
}

float User::returnMoveToX () {
    return moveToX;
}

void User::setMoveToY (float y) {
    moveToY = y;
    if (moveToY > userY) {
        isMoveToYGreaterUserY = true;
    }
    else {
        isMoveToYGreaterUserY = false;

    }
}

float User::returnMoveToY () {
    return moveToY;
}

void User::setIsMoveToXGreaterUserX (bool cond) {
    isMoveToXGreaterUserX = cond;
}

bool User::returnIsMoveToXGreaterUserX () {
    return isMoveToXGreaterUserX;
}

void User::setIsMoveToYGreaterUserY (bool cond) {
    isMoveToYGreaterUserY = cond;
}

bool User::returnIsMoveToYGreaterUserY () {
    return isMoveToYGreaterUserY;
}

 void User::setLAID (int ID) {
    LA_ID = ID;
 }

int User::getLAID () {
    return LA_ID;
}

void User::printUserData () {
    std::cout << "User[" << USER_ID << "]: {x:" << userX << ", y:" << userY 
    << " Goal x:" << moveToX << " Goal y:" << moveToY << " , LAs:" << LA_ID << "}" << std::endl; 
}


