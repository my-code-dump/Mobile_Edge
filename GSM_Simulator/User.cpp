#include "User.h"

User::User(int id, float user_X, float user_Y) {
    USER_ID = id;
    userX = user_X;
    userY = user_Y;
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
