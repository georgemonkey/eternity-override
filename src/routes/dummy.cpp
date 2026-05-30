#include "dummy.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "setup.hpp"

// Example autonomous route — copy this file when adding new routes.
// Create a matching header in include/ and register the function in main.cpp's selector.
void dummy() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 4, 1000);
}
