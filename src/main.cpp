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


void autonomous() {
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // kiwi::Path filePath(config, "/usd/path.json");
    // filePath.follow();

    chassis.moveToPose(5, 30, 90, 4000); 
}

void opcontrol() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    while(true) {

        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))  {
            //chassis.turnToHeading(45, 2000);
            chassis.moveToPoint(0, 24, 10000);
            std::cout<<"SDGDSFSd";
        }
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
            chassis.turnToHeading(90, 2000);}

         if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))  {
            chassis.turnToHeading(0, 2000);
            std::cout<<"SDGDSFSd";

         }
       


        //kiwiRunControl();

        //matchControl();

        // firstPath.follow();
    }
}
