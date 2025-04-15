#include "main.h"
#include "drivecode/ladybrown.hpp"
#include "drivecode/intake.hpp"
#include "drivecode/pistons.hpp"
#include "drivecode/loops.hpp"
#include "globals.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "kiwirun/includes.hpp"

void initialize() {

    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    intakeInit();
    ladyBrownInit();
    pistonInit();

    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading

            pros::lcd::print(3, "Intake state: %f", intakeState);
            pros::lcd::print(4, "Piston state: %f", clampState);

            // std::cout<<"X: "<<std::to_string(chassis.getPose().x)<<"\n";
            // std::cout<<"Y: "<<std::to_string(chassis.getPose().y)<<"\n";
            // std::cout<<"Theta: "<<std::to_string(chassis.getPose().theta)<<"\n\n";

            // log position telemetry
            // lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}

void disabled() {}

void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy


void autonomous() {}

void opcontrol() {
    while (true) {
        
        matchControl();

        pros::delay(10);
        
    }
}
