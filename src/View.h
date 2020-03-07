#pragma once

#include <Arduino.h>

#include <Wheel.h>
#include <WheelValue.h>
#include <WheelCharacteristic.h>

#define VIEW_PAGE_HOME 1
#define VIEW_PAGE_SPEED 2
#define VIEW_PAGE_SLEEP 3
#define VIEW_PAGE_BATTERY 4
#define VIEW_PAGE_CONNECTING 5

template <class D>
class ViewPage
{
    public:
        virtual ~ViewPage() {}

        unsigned long lastChange = 0;

        virtual const uint8_t id() = 0;

        virtual bool draw(D* display) = 0;

        virtual bool update(D* display, Wheel::Values* values) = 0;

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

        bool draw(D* display);

        bool update(D* display, Wheel::Values* values) {
            return true;
        }

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

        bool draw(D* display);

        bool update(D* display, Wheel::Values* values) {
            return true;
        }

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

        bool draw(D* display);

        bool update(D* display, Wheel::Values* values);

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

        bool draw(D* display);

        bool update(D* display, Wheel::Values* values);

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

        bool draw(D* display);

        bool update(D* display, Wheel::Values* values) {
            return true;
        }

        const std::vector<std::string> characteristics() {
            return {};
        };
};

template <class D>
class View
{
    protected:
        uint8_t currentPage;

        D* display;

    public:
        View(D* display):
            currentPage(-1),
            display(display) { }

        void draw(ViewPage<D>* page) {
            if (this->currentPage != page->id()) {
                // force pages to draw.
                page->lastChange = 0;
                this->currentPage = page->id();
            }

            if (page->draw(this->display)) {
                page->lastChange = millis();
            }
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
            if (!changed && page->lastChange > 0) {
                return;
            }

            if (page->update(this->display, values)) {
                page->lastChange = timestamp;
            }
        };
};
