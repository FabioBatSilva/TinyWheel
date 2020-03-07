#if DEVICE_UI == DEVICE_UI_TFT
    #include <TFT_eSPI.h>
    #define UI_DISPLAY TFT_eSPI
#else
    #error "Unknown DEVICE_UI."
#endif

#include <unity.h>
#include <View.h>

namespace ViewTest
{
    template <class D>
    class ViewPageMock : public ViewPage<D>
    {
        public:
            uint8_t const mId;

            unsigned long drawCalls;

            unsigned long updateCalls;

            bool drawCallReturn = true;

            bool updateCallReturn = true;

            const std::vector<std::string> mCharacteristics;

            ViewPageMock(uint8_t const id, std::vector<std::string> characteristics):
                mId(id),
                drawCalls(0),
                updateCalls(0),
                drawCallReturn(true),
                updateCallReturn(true),
                mCharacteristics(characteristics) {}

            uint8_t const id() override {
                return mId;
            };

            bool draw(D* display) {
                this->drawCalls ++;

                return this->drawCallReturn;
            }

            bool update(D* display, Wheel::Values* values) {
                this->updateCalls ++;

                return this->updateCallReturn;
            }

            const std::vector<std::string> characteristics() {
                return mCharacteristics;
            };
    };

    void test_draw(void)
    {
        UI_DISPLAY display;

        View<UI_DISPLAY> view(&display);

        ViewPageMock<UI_DISPLAY> viewPageMock1(1, {});
        ViewPageMock<UI_DISPLAY> viewPageMock2(2, {});

        TEST_ASSERT_EQUAL(0, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.updateCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.lastChange);

        TEST_ASSERT_EQUAL(0, viewPageMock2.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock2.updateCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.lastChange);

        // start on page 1
        view.draw(&viewPageMock1);

        TEST_ASSERT_EQUAL(1, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.updateCalls);
        TEST_ASSERT_GREATER_THAN_INT(0, viewPageMock1.lastChange);

        long previusChange = viewPageMock1.lastChange;

        // continue on page 1
        view.draw(&viewPageMock1);

        TEST_ASSERT_EQUAL(2, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.updateCalls);
        TEST_ASSERT_GREATER_OR_EQUAL(previusChange, viewPageMock1.lastChange);

        previusChange = viewPageMock1.lastChange;

        // move to page 2
        view.draw(&viewPageMock2);

        TEST_ASSERT_EQUAL(1, viewPageMock2.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock2.updateCalls);
        TEST_ASSERT_GREATER_THAN(0, viewPageMock2.lastChange);

        // back to page 1
        view.draw(&viewPageMock1);

        TEST_ASSERT_EQUAL(3, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.updateCalls);
        TEST_ASSERT_GREATER_OR_EQUAL(previusChange, viewPageMock1.lastChange);
    }

    void test_update(void)
    {
        UI_DISPLAY display;

        View<UI_DISPLAY> view(&display);

        ViewPageMock<UI_DISPLAY> viewPageMock1(1, {"c1"});
        ViewPageMock<UI_DISPLAY> viewPageMock2(2, {"c2"});

        WheelValue c1v1("c1", { 1, 1 });
        WheelValue c1v2("c1", { 1, 2 });
        WheelValue c2v1("c2", { 2, 1 });

        Wheel::values()->put("c1", c1v1, millis());
        Wheel::values()->put("c2", c2v1, millis());

        TEST_ASSERT_EQUAL(0, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.updateCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.lastChange);

        TEST_ASSERT_EQUAL(0, viewPageMock2.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock2.updateCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock1.lastChange);

        // call update firt time
        view.update(&viewPageMock1, Wheel::values());

        long previusChange = viewPageMock1.lastChange;

        TEST_ASSERT_EQUAL(1, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(1, viewPageMock1.updateCalls);
        TEST_ASSERT_GREATER_THAN_INT(0, viewPageMock1.lastChange);

        // force delay to ensure millis() is going to change
        delay(1);

        // call update again time, no changes to the data
        view.update(&viewPageMock1, Wheel::values());

        TEST_ASSERT_EQUAL(1, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(1, viewPageMock1.updateCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock2.drawCalls);
        TEST_ASSERT_EQUAL(0, viewPageMock2.updateCalls);
        // no changes here.
        TEST_ASSERT_EQUAL(previusChange, viewPageMock1.lastChange);

        // Change page
        view.update(&viewPageMock2, Wheel::values());

        TEST_ASSERT_EQUAL(1, viewPageMock2.drawCalls);
        TEST_ASSERT_EQUAL(1, viewPageMock2.updateCalls);

        // back to page 1
        view.update(&viewPageMock1, Wheel::values());

        TEST_ASSERT_EQUAL(2, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(2, viewPageMock1.updateCalls);

        // back to page 1
        view.update(&viewPageMock1, Wheel::values());

        TEST_ASSERT_EQUAL(2, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(2, viewPageMock1.updateCalls);

        previusChange = viewPageMock1.lastChange;

        // force delay
        delay(1);

        // update c1
        Wheel::values()->put("c1", c1v2, millis());

        // call update again time, now there are changes
        view.update(&viewPageMock1, Wheel::values());

        TEST_ASSERT_EQUAL(2, viewPageMock1.drawCalls);
        TEST_ASSERT_EQUAL(3, viewPageMock1.updateCalls);
        TEST_ASSERT_GREATER_THAN(previusChange, viewPageMock1.lastChange);
    }

    void test_draw_last_change(void)
    {
        UI_DISPLAY display;

        View<UI_DISPLAY> view(&display);

        ViewPageMock<UI_DISPLAY> viewPageMock(1, {});

        TEST_ASSERT_EQUAL(0, viewPageMock.drawCalls);
        TEST_ASSERT_TRUE(viewPageMock.drawCallReturn);
        TEST_ASSERT_TRUE(viewPageMock.updateCallReturn);

        // call it firt time
        view.draw(&viewPageMock);

        long previusChange = viewPageMock.lastChange;

        TEST_ASSERT_EQUAL(1, viewPageMock.drawCalls);
        TEST_ASSERT_GREATER_THAN_INT(0, viewPageMock.lastChange);

        // force delay to ensure millis() is going to change
        delay(1);

        // page won't change
        viewPageMock.drawCallReturn = false;

        view.draw(&viewPageMock);

        TEST_ASSERT_EQUAL(2, viewPageMock.drawCalls);
        // no changes here.
        TEST_ASSERT_EQUAL(previusChange, viewPageMock.lastChange);

        // force delay
        delay(1);

        // page shoud change.
        viewPageMock.drawCallReturn = true;

        view.draw(&viewPageMock);

        TEST_ASSERT_EQUAL(3, viewPageMock.drawCalls);
        // should update the last change value.
        TEST_ASSERT_GREATER_THAN(previusChange, viewPageMock.lastChange);
    }

    void test_update_last_change(void)
    {
        UI_DISPLAY display;

        View<UI_DISPLAY> view(&display);

        ViewPageMock<UI_DISPLAY> viewPageMock(1, {});

        TEST_ASSERT_EQUAL(0, viewPageMock.drawCalls);
        TEST_ASSERT_TRUE(viewPageMock.drawCallReturn);
        TEST_ASSERT_TRUE(viewPageMock.updateCallReturn);

        // call update firt time
        view.update(&viewPageMock, Wheel::values());

        long previusChange = viewPageMock.lastChange;

        TEST_ASSERT_EQUAL(1, viewPageMock.drawCalls);
        TEST_ASSERT_EQUAL(1, viewPageMock.updateCalls);
        TEST_ASSERT_GREATER_THAN_INT(0, viewPageMock.lastChange);

        // force delay to ensure millis() is going to change
        delay(1);

        // page won't change
        viewPageMock.updateCallReturn = false;

        view.update(&viewPageMock, Wheel::values());

        TEST_ASSERT_EQUAL(1, viewPageMock.drawCalls);
        TEST_ASSERT_EQUAL(2, viewPageMock.updateCalls);
        // no changes here.
        TEST_ASSERT_EQUAL(previusChange, viewPageMock.lastChange);

        // force delay
        delay(1);

        // page shoud change.
        viewPageMock.updateCallReturn = true;

        view.update(&viewPageMock, Wheel::values());

        TEST_ASSERT_EQUAL(1, viewPageMock.drawCalls);
        TEST_ASSERT_EQUAL(3, viewPageMock.updateCalls);
        // should update the last change value.
        TEST_ASSERT_GREATER_THAN(previusChange, viewPageMock.lastChange);
    }

    void run_tests() {
        RUN_TEST(ViewTest::test_draw);
        RUN_TEST(ViewTest::test_update);
        RUN_TEST(ViewTest::test_draw_last_change);
        RUN_TEST(ViewTest::test_update_last_change);
    }
}