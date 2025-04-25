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
    chassis.moveToPoint(-8.27, 4.23, 500);
    chassis.waitUntilDone();
    pidActive = true;
    lbTarget = 180;
    pros::delay(750);

    //mogo
    //TRASH MOGO MECH ROUTE
    // chassis.moveToPoint(17.6, -15.8, 1000, {.forwards = false});
    // chassis.turnToHeading(40, 750); //36.7
    // chassis.moveToPoint(9, -30.2, 500, {.forwards = false});
    // chassis.waitUntilDone();
    // chassis.moveToPoint(3, -35.8, 500, {.forwards = false, .maxSpeed = 20});
    // chassis.waitUntilDone();
    // clampState = 1;
    // pidActive = true;
    // lbTarget = 2;
    // pros::delay(5000);

    //middle rings
    chassis.turnToHeading(139, 500);
    intakeState = 1;
    chassis.moveToPose(30, -44, 90, 1000, {.lead = 0.5});
    chassis.moveToPoint(40, -44, 1000);
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

        // if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))  {
        //     //chassis.turnToHeading(45, 2000);
        //     chassis.moveToPoint(0, 24, 10000);
        //     std::cout<<"SDGDSFSd";
        // }
        // if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
        //     chassis.turnToHeading(90, 2000);}

        //  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))  {
        //     chassis.turnToHeading(0, 2000);
        //     std::cout<<"SDGDSFSd";

        //  }
       


        //kiwiRunControl();

        matchControl();

        // firstPath.follow();
    }
}
