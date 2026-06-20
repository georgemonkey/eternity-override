#include "main.h"
#include "dummy.hpp"
#include "op_control.hpp"
#include "pros/llemu.hpp"
#include "robodash/api.h"
#include "setup.hpp"

bool tuneMode = true;
bool defaultDrive = true; 
int DHoldTime = 0;      

int loadPos = 0;
int p1 = 20;
int p2 = 40;
int p3 = 60; 

void positionTrackerTask() {
    while (true) {
        pros::lcd::print(1, "X: %.2f, Y: %.2f, Theta: %.2f",
                         chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
        pros::delay(10);
    }
}

rd::Selector selector({
    {"dummy", &dummy},
});
    
void initialize() {
    selector.focus();

    if (tuneMode) {
        chassis.setPose(0, 0, 0);
        pros::lcd::initialize();
        pros::Task pos(&positionTrackerTask);
    }

    chassis.calibrate();
    left_dt.set_brake_mode(pros::MotorBrake::coast);
    right_dt.set_brake_mode(pros::MotorBrake::coast);
    diddy.set_brake_mode(pros::MotorBrake::hold);
    
    // Ensure cascade locks in place when power is set to 0
    cascade.set_brake_mode(pros::MotorBrake::hold);

    selector.on_select([](std::optional<rd::Selector::routine_t> routine) {
        if (routine == std::nullopt) {
            std::cout << "No routine selected" << std::endl;
            controller.print(2, 0, "select route");
        } else {
            std::cout << "Selected Routine: " << routine.value().name << std::endl;
            controller.print(2, 0, "run: %s", routine.value().name.c_str());
        }
    });
}

void disabled() {}

void competition_initialize() {
    selector.focus();
}

void opcontrol() {
    while (true) {
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
            DHoldTime += 10;
            if (DHoldTime >= 2000) {
                defaultDrive = !defaultDrive;
                controller.rumble("..");
                DHoldTime = 0;
            }
        } else {
            DHoldTime = 0;
        }

        pros::lcd::print(2, "local encoder: %.2f deg", cascade.get_position() / 100.0);
        
        handleDriveMode(defaultDrive);
        handleDiddy();
        handleCascadeStage();
        handleCascadeScoring();
        
        pros::delay(20);
    }
}


void score(int intendedStage) {
    pros::lcd::print(3, "cascade level: %d", scoringStage);
    if (intendedStage == 0) {
        cascade.move_absolute(loadPos*100, 127);
    }
    else if (intendedStage == 1) {
        cascade.move_absolute(p1*100, 127);
    }
    else if (intendedStage == 2) {
        cascade.move_absolute(p2*100, 127);
    }
    else if (intendedStage == 3) {
        cascade.move_absolute(p3*100, 127);
    }
}