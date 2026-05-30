#pragma once

#include "api.h"
#include "lemlib/api.hpp"
#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/motors.hpp"


// controller
inline pros::Controller controller(pros::E_CONTROLLER_MASTER); //controller

// drivetrain
inline pros::Imu imu(1);
inline pros::Rotation verticalEnc(11);

inline pros::MotorGroup right_dt({5,6,7}, pros::MotorGearset::blue);    // right side
inline pros::MotorGroup left_dt({15,16,17}, pros::MotorGearset::blue);  // left side

// drivetrain config for movement functions
inline lemlib::Drivetrain drivetrain(&left_dt,
                &right_dt,
                10.25,                                // Track width (distance between left and right wheels)
                lemlib::Omniwheel::NEW_275,           
                450,                                  // Drivetrain RPM
                2                                     // Chase power
);

// EXAMPLES
//inline pros::Motor scoringRoller(-15);
//inline pros::adi::DigitalOut scoringGate('A');


//configure tracking wheels
inline lemlib::TrackingWheel verticalTrackingWheel(&verticalEnc, lemlib::Omniwheel::NEW_275, -0.5);

// configure entire odom setup
inline lemlib::OdomSensors sensors(&verticalTrackingWheel, // Vertical tracking wheel 1
              nullptr,                                    // Vertical tracking wheel 2 (none)
              nullptr,                                    // Horizontal tracking wheel 1 (none)
              nullptr,                                    // Horizontal tracking wheel 2 (none)
              &imu                                        // Inertial Sensor
);

// lateral pid
inline lemlib::ControllerSettings lateral_controller(7, //proportional gain (kP) //5.5
                                            0, // integral gain (kI) //0.5
                                        25, // derivative gain (kD) //18
                                         1,//windup
                                           0.5, // small error range, in inches
                                          100, // small error range timeout, in milliseconds
                                           1, // large error1range, in inches
                                         750, // large error range timeout, in milliseconds
                                              5 // maximum acceleration (slew)
);

// angular pid
inline lemlib::ControllerSettings angular_controller(5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              43.5, // derivative gain (kD)
                                              3.5, // anti windup
                                              0.5, // small error range, in degrees
                                                50, // small error range timeout, in milliseconds
                                              1.5, // large error range, in degrees
                                              200, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);


// input curve for throttle input during driver control
inline lemlib::ExpoDriveCurve throttle_curve(
    3,    // deadband
    0,    // min output
    1.01  
);

// input curve for steer input during driver control
inline lemlib::ExpoDriveCurve steer_curve(
    3,    // deadband 
    0,    // min output
    1.01   
);


inline lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
                        &throttle_curve,
                        &steer_curve
);