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

    screenInit();
    intakeInit();
    pistonInit();
    ladyBrownInit();
}

void disabled() {}

void competition_initialize() {}

//temp, move to separate file later
void redRingSide() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    //alliance
    chassis.turnToHeading(-50, 500);
    chassis.moveToPoint(-7, 3.4, 1000); //-6.3, 2.7
    chassis.waitUntilDone();
    pidActive = true;
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    lbTarget = 177;
    pros::delay(750);
    //lbTarget = 2;

    // mogo
    chassis.moveToPoint(5.3, -7.2, 750, {.forwards = false}); //6.4, -7.6
    chassis.turnToHeading(0, 500);
    chassis.moveToPoint(6.9, -29.5, 1000, {.forwards = false}); //try changing to move to pose
    chassis.waitUntilDone();
    pidActive = true;
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    lbTarget = 2;
    clampState = 1;
    pros::delay(100);
    intakeState = 2;

    //middle rings
    chassis.turnToHeading(132.1, 500);
    chassis.waitUntilDone();
    intakeState = 1;
    chassis.moveToPose(36, -40, 90, 1000, {.lead = 0.4, .minSpeed = 80});
    chassis.moveToPoint(45, -37.7, 1000, {.maxSpeed = 50});
    pros::delay(1000);

    //1 ring
    chassis.moveToPoint(32, -33, 1000, {.forwards = false});
    chassis.moveToPoint(15, -15, 1000, {.forwards = false});
    chassis.turnToHeading(97.8, 500);
    chassis.moveToPoint(34.5, -18.2, 500);
    pros::delay(1000);

    //corner rings
    chassis.turnToHeading(23.7, 500);
}


void autonomous() {
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // kiwi::Path filePath(config, "/usd/path.json");
    // filePath.follow();
  
    redRingSide();
}

void opcontrol() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    while(true) {
        //kiwiRunControl();

        matchControl();

        //autonControl();

        // firstPath.follow();
    }
}
