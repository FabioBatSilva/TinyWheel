#pragma once

#include <View.h>

#ifndef WHEEL_MAX_CONNECT_FAILURE
    #define WHEEL_MAX_CONNECT_FAILURE 60
#endif

#ifndef WHEEL_DEVICE_NAME
    #error "WHEEL_DEVICE_NAME not defined."
#endif

#ifndef WHEEL_DEVICE_ADDRESS
    #error "WHEEL_DEVICE_ADDRESS not defined."
#endif

#ifndef WHEEL_DEVICE_KEY
    #error "WHEEL_DEVICE_KEY not defined."
#endif

template<class D>
class MonitorApp
{
    protected:
        struct UI
        {
            static D display;

            static View<D> view;

            static ViewPageHome<D> viewPageHome;

            static ViewPageSleep<D> viewPageSleep;

            static ViewPageSpeed<D> viewPageSpeed;

            static ViewPageBattery<D> viewPageBattery;

            static ViewPageConnecting<D> viewPageConnecting;
        };

        static UI ui;

        static uint8_t page;

    public:
        static void setup();

        static void init();

        static void loop();

        static void sleep();

        static void nextPage() {
            switch (page)
            {
                case VIEW_PAGE_HOME:
                    page = VIEW_PAGE_SPEED;
                    break;
                case VIEW_PAGE_SPEED:
                    page = VIEW_PAGE_BATTERY;
                    break;
                default:
                    page = VIEW_PAGE_HOME;
                    break;
            }
        }
};
