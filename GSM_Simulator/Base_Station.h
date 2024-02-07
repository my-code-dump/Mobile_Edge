class Base_Station {
    private:
        int BTS_ID;
        float minX;
        float minY;
        float maxX;
        float maxY;

    public:
        Base_Station(int, float, float, float, float);

        void setID (int);

        int returnID ();

        void setMinX (float);

        float returnMinX ();

        void setMinY (float);

        float returnMinY ();

        void setMaxX (float);

        float returnMaxX ();

        void setMaxY (float);

        float returnMaxY ();
};
