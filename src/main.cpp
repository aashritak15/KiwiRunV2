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
    sortState = 2;

    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    //alliance
    // chassis.moveToPose(-7, 3.4, -50, 1500);
    chassis.turnToHeading(-50, 500);
    chassis.moveToPoint(-7, 3.4, 1000); //-6.3, 2.7
    chassis.waitUntilDone();
    pidActive = true;
    lbTarget = 195;

    pros::delay(500);//750
    pidActive = true;
    lbTarget = 2;

    //mogo
    // chassis.moveToPoint(5.3, -7.2, 750, {.forwards = false});
    // chassis.turnToHeading(0, 500);
    chassis.moveToPose(4, -24, 0, 2400, {.forwards = false, .minSpeed = 40});
    chassis.moveToPoint(4, -32, 1000, {.forwards = false, .maxSpeed = 40});
    // chassis.moveToPose(6.9, -30.5, 0, 2400, {.forwards = false, .lead = 0.2, .minSpeed = 50}); //try changing to move to pose
    chassis.waitUntilDone();
    pros::delay(100);
    clampState = 1;
    pros::delay(100);
    intakeState = 2;

    //middle rings
    chassis.turnToHeading(132.1,3000, {.minSpeed = 70});
    chassis.waitUntilDone();
    intakeState = 1;
    chassis.moveToPose(35, -39.5, 90, 1000, {.minSpeed = 70});
    chassis.moveToPoint(41.5, -39.5, 1000, {.maxSpeed = 60}); //TODO: prev 43.25
    chassis.waitUntilDone();
    pros::delay(250);

    // chassis.moveToPose(15.04, -31.95, 130.5, 2000, {.forwards = false});
    // chassis.turnToHeading(61, 1000);
    // chassis.waitUntilDone();
    // pros::delay(200);
    // // chassis.moveToPoint(29.16, -26.94, 1000);
    // chassis.moveToPoint(30, -28, 1000);
    // chassis.moveToPose(51.23, 5.85, 41.8, 3000);

    //1 ring
    chassis.moveToPose(7.6, -22.7, 138, 1500, {.forwards = false, .minSpeed = 90});
    chassis.waitUntilDone();
    chassis.turnToHeading(84, 500);
    chassis.moveToPoint(29.7, -23.6, 1000, {.minSpeed = 100});
    chassis.waitUntilDone();
    pros::delay(100);

    chassis.turnToHeading(-90, 1000);
    chassis.moveToPoint(-17, -23.6, 5000, {.maxSpeed = 80});
    // //ring stack
    // chassis.moveToPose(43, 8, 42.4, 3000, {.minSpeed = 60}); //60
    // chassis.waitUntilDone();

    // intakeState = 2;
    // chassis.moveToPoint(45.47, 12.44, 750, {.minSpeed = 100}); //100
    // chassis.waitUntilDone();
    // pros::delay(350);
    // intakeState = 1;
    // pros::delay(500);

    // //fifth ring
    // chassis.moveToPoint(32.9, -4.3, 1000, {.forwards = false, .maxSpeed = 50});
    // chassis.turnToHeading(230, 1200);

    // chassis.moveToPoint(0, -27.3, 3000);
    // chassis.waitUntilDone();

    return;
}

void blueRingSide() {
    sortState = 1;

    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    //alliance
   
    chassis.turnToHeading(50, 500);
    chassis.moveToPoint(7, 3.4, 1000); //-6.3, 2.7
    chassis.waitUntilDone();
    pidActive = true;
    lbTarget = 195;

    pros::delay(500);//750
    pidActive = true;
    lbTarget = 2;

    //mogo
   
    chassis.moveToPose(-4.5, -24, 0, 2400, {.forwards = false,  .minSpeed = 40});
    chassis.moveToPoint(-4.5, -34, 1000, {.forwards = false, .maxSpeed = 40});
    
    chassis.waitUntilDone();
    pros::delay(100);
    clampState = 1;
    pros::delay(100);
    intakeState = 2;

    // //middle rings
    chassis.turnToHeading(-150.7,3000, {.minSpeed = 70});
    chassis.waitUntilDone();
    intakeState = 1;
    chassis.moveToPose(-35, -56.5, -90, 1000, {.minSpeed = 70});
    chassis.moveToPoint(-39.5, -56.5, 1000, {.maxSpeed = 60}); //TODO: prev 43.25
    chassis.waitUntilDone();
    pros::delay(250);

   

    // // //1 ring
    chassis.moveToPose(-1, -30.2, -136, 1500, {.forwards = false, .minSpeed = 90});
    chassis.waitUntilDone();
    chassis.turnToHeading(-105.5, 900);
     chassis.moveToPoint(-23.9, -42.2, 1000);
    // chassis.waitUntilDone();
    // pros::delay(100);
    chassis.turnToHeading(90, 1000);
    chassis.moveToPoint(16.1, -39.1 , 5000, {.maxSpeed = 80});
    

    return;
}

void redMogo() {
    //going to clamp 
    chassis.moveToPoint(0, -31, 1000, {.forwards = false});
    chassis.waitUntilDone();
    pros::delay(200 ); 

    //clamp onto the goal 
    clampState = 1;
    
    pros::delay(250 ); 



    //turn to face ring stack 

    chassis.turnToHeading(90, 1000);

    intakeState = 1;

    //intake the ring # 1 

    chassis.moveToPoint(28,-31, 1000);

    //turn to ring # 2 

    chassis.turnToHeading(-17, 1000);

    


}


void autonomous() {
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // kiwi::Path filePath(config, "/usd/path.json");
    // filePath.follow();
  
    //redRingSide();
    redMogo(); 
}

void opcontrol() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    //kiwiRunControl();

    // matchControl();
    

     autonControl();

    // firstPath.follow();
}
