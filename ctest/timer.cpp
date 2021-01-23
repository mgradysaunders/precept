#include "doctest.h"
#include <thread> 
#include <pre/timer>

TEST_CASE("timer") {
    using namespace std::chrono_literals;
    pre::SystemTimer timer;
    std::this_thread::sleep_for(1s);
    CHECK(timer.nanoseconds() > 999000000);
    CHECK(timer.microseconds() > 999000);
    CHECK(timer.milliseconds() > 999);
    CHECK(timer.seconds() > 0.999);
}

