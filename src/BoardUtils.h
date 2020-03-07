#pragma once

class BoardUtils
{
    public:
        template<class D>
        static void setup(D* display);

        template<class D>
        static void deepSleep(D* display);
};