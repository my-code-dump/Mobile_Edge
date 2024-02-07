#include <iostream>
#include <vector>

class User {
    private:
        int USER_ID;
        float userX;
        float userY;
    
    public:
        std::vector<int> BTS;
        
        User(int, float, float);
        
        void setID (int);

        int returnID ();

        void setUserX (float);

        float returnUserX ();

        void setUserY (float);

        float returnUserY ();
};