#include "main.h"
#include "op_control.hpp"
#include "setup.hpp"

int scoringStage = 1;


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



void handleDiddy() {

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        diddy.move(-127);
    }

    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        diddy.move(127);
    }
    else{
        diddy.move(0);
    }
}

void handleCascadeStage() {

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
        scoringStage+=1;
        // handle maximums
        if (scoringStage>3){
        scoringStage=3;
        }
    }

    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
        scoringStage-=1;
        //  handle minimums
        if (scoringStage<0){
        scoringStage=0;
        }
    }
}

void handleCascadeScoring() {
score(scoringStage);
pros::delay(400);
cascade.move_absolute(0, 127);
}