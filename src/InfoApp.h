#pragma once

#include <string>
#include <vector>

#include <View.h>
#include <WheelDevice.h>

template<class D>
class InfoApp
{
    protected:
        struct UI
        {
            static D display;

            static View<D> view;

            static ViewPageHome<D> viewPageHome;

            static ViewPageSleep<D> viewPageSleep;

            static ViewPageConnecting<D> viewPageConnecting;
        };

        static WheelDeviceScanner scanner;

        static InfoApp<D>::UI ui;

        static std::string key;

        static void onScanResult(std::vector<WheelInfo> result);

    public:
        static void setup();

        static void init();

        static void sleep();

        static void loop();
};
