#include "main.h"
#include "op_control.hpp"
#include "setup.hpp"

void handleDriveMode(bool driveMode) {
    driveMode ? handleArcade() : handleTank();
}

void handleArcade() {
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    chassis.arcade(leftY, rightX);
}

void handleTank() {
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    chassis.tank(leftY, rightY);
}



void handleArm() {

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        arm.move(-127);
    }

    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        arm.move(127);
    }

    else {
        arm.move_voltage(0); 
    }
}