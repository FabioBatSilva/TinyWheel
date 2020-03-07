#pragma once

#include <string>
#include <vector>

#include <View.h>
#include <WheelDevice.h>

#ifndef WHEEL_MAX_SCANS
    #define WHEEL_MAX_SCANS 10
#endif

template<class D>
class ScannerApp
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

        static ScannerApp<D>::UI ui;

        static unsigned long scanCount;

        static WheelDeviceScanner scanner;

        static void onScanResult(std::vector<WheelInfo> result);

        static void startDeepSleep();

        static void preLoop();

    public:
        static void setup();

        static void init();

        static void sleep();

        static void loop();
};
