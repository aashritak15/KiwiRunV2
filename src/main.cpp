#include "main.h"
#include "drivecode/ladybrown.hpp"
#include "drivecode/intake.hpp"
#include "drivecode/pistons.hpp"
#include "drivecode/loops.hpp"
#include "globals.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "kiwirun/includes.hpp"
#include "pros/screen.h"

void initialize() {
    chassis.calibrate();
    chassis.setPose(0, 0, 0);

    pros::screen::print(pros::E_TEXT_MEDIUM, 1, "hello world");
    pros::screen::print(pros::E_TEXT_MEDIUM, 2, "hello world");

    intakeInit();
    pistonInit();
    ladyBrownInit();
}

void disabled() {}

void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy


void autonomous() {}

void opcontrol() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    while(true) {
        matchControl();
    }
}
