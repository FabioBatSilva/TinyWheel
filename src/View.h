#pragma once

#include <Arduino.h>

#include <Wheel.h>
#include <WheelValue.h>
#include <WheelCharacteristic.h>

#define VIEW_PAGE_HOME 1
#define VIEW_PAGE_SPEED 2
#define VIEW_PAGE_SLEEP 3
#define VIEW_PAGE_BATTERY 3
#define VIEW_PAGE_CONNECTING 4

template <class D>
class ViewPage
{
    public:
        virtual ~ViewPage() {}

        unsigned long lastChange = 0;

        virtual const uint8_t id() = 0;

        virtual void draw(D* display) = 0;

        virtual void update(D* display, Wheel::Values* values) = 0;

        virtual const std::vector<std::string> characteristics() = 0;
};

template <class D>
class ViewPageHome : public ViewPage<D>
{
    public:
        ~ViewPageHome() {}

        uint8_t const id() override {
            return VIEW_PAGE_HOME;
        };

        void draw(D* display);

        void update(D* display, Wheel::Values* values) {};

        const std::vector<std::string> characteristics() {
            return {};
        };
};

template <class D>
class ViewPageSleep : public ViewPage<D>
{
    public:
        ~ViewPageSleep() {}

        uint8_t const id() override {
            return VIEW_PAGE_SLEEP;
        };

        void draw(D* display);

        void update(D* display, Wheel::Values* values) {};

        const std::vector<std::string> characteristics() {
            return {};
        };
};

template <class D>
class ViewPageSpeed : public ViewPage<D>
{
    public:
        ~ViewPageSpeed() {}

        uint8_t const id() override {
            return VIEW_PAGE_SPEED;
        };

        void draw(D* display);

        void update(D* display, Wheel::Values* values);

        const std::vector<std::string> characteristics() {
            return {
                WHEEL_CHARACTERISTIC_SPEED_RPM_UUID
            };
        };
};

template <class D>
class ViewPageBattery : public ViewPage<D>
{
    public:
        ~ViewPageBattery() {}

        uint8_t const id() override {
            return VIEW_PAGE_BATTERY;
        };

        void draw(D* display);

        void update(D* display, Wheel::Values* values);

        const std::vector<std::string> characteristics() {
            return {
                WHEEL_CHARACTERISTIC_BATTERY_REMAINING_UUID
            };
        };
};

template <class D>
class ViewPageConnecting : public ViewPage<D>
{
    public:
        ~ViewPageConnecting() {}

        uint8_t const id() override {
            return VIEW_PAGE_CONNECTING;
        };

        void draw(D* display);

        void update(D* display, Wheel::Values* values) {}

        const std::vector<std::string> characteristics() {
            return {};
        };
};

template <class D>
class View
{
    protected:
        uint8_t currentPage = -1;

        D* display;

    public:
        View(D* display) {
            this->display = display;
        }

        void draw(ViewPage<D>* page) {
            if (this->currentPage != page->id()) {
                // force pages to draw.
                page->lastChange = 0;
            }

            page->draw(this->display);

            this->currentPage = page->id();
            page->lastChange = millis();
        };

        void update(ViewPage<D>* page, Wheel::Values* values) {
            if (this->currentPage != page->id()) {
                // force pages to draw.
                page->lastChange = 0;

                page->draw(this->display);

                this->currentPage = page->id();
            }

            // update if there are no characteristics to check.
            bool changed = page->characteristics().empty();
            long timestamp = millis();

            for (std::string uuid : page->characteristics()) {
                if (values->changed(uuid, page->lastChange)) {
                    changed = true;
                    break;
                }
            }

            // update if any value changed or if is the first time in this page
            if (changed || page->lastChange == 0) {
                page->update(this->display, values);

                page->lastChange = timestamp;
            }
        };
};
