#include "main.h"
#include "dummy.hpp"
#include "op_control.hpp"
#include "pros/llemu.hpp"
#include "robodash/api.h"
#include "setup.hpp"

// set true for green-screen testing, false for competition selector
bool tuneMode = true;

bool defaultDrive = true; // true = arcade, false = tank
int DHoldTime = 0;        // ms button held for drive mode switch

// only runs when tuneMode is true
void positionTrackerTask() {
    while (true) {
        pros::lcd::print(1, "X: %.2f, Y: %.2f, Theta: %.2f",
                         chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
        pros::delay(10);
    }
}

rd::Selector selector({
    {"dummy", &dummy},
    // add new routes here: {"route name", &yourRouteFunction},
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

void autonomous() {
    if (tuneMode) {
        dummy();
    } else {
        selector.run_auton();
    }
}

void opcontrol() {
    while (true) {
        // drive mode switcher (hold X for 2s)
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

        pros::lcd::print(2, "Enc: %.2f deg", cascadeEnc.get_position() / 100.0);
        handleDriveMode(defaultDrive);
        handleDiddy();
        handleCascadeStage();
        handleCascadeScoring();
        
        pros::delay(20);
    }
}






void score(int intendedStage) {
    cascade.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    if (intendedStage == 0) {
        cascade.move_absolute(30, 127);
        while (!((cascadeEnc.get_position() < 3200) && (cascadeEnc.get_position() > 2800))) { 
            pros::lcd::print(2, "Enc: %.2f deg", cascadeEnc.get_position() / 100.0);
            pros::delay(20);
        }
    }

    else if (intendedStage == 1) {
        cascade.move_absolute(50, 127);
        while (!((cascadeEnc.get_position() < 5200) && (cascadeEnc.get_position() > 4800))) {
            pros::lcd::print(2, "Enc: %.2f deg", cascadeEnc.get_position() / 100.0);
            pros::delay(20);
        }
    }
    else if (intendedStage == 2) {
        cascade.move_absolute(70, 127);
        while (!((cascadeEnc.get_position() < 7200) && (cascadeEnc.get_position() > 6800))) {
            pros::lcd::print(2, "Enc: %.2f deg", cascadeEnc.get_position() / 100.0);
            pros::delay(20);
        }
    }

    cascade.move(0);

    cascade.move_absolute(0, 127);
    while (!((cascadeEnc.get_position() < 200) && (cascadeEnc.get_position() > -200))) {
        pros::lcd::print(2, "Enc: %.2f deg", cascadeEnc.get_position() / 100.0);
        pros::delay(20);
    }
    cascade.move(0);
}