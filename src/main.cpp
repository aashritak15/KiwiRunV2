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

void blueMogo() {
    sortState = 1;
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

    chassis.moveToPoint(20,-31, 1000);

    //turn and go to ring # 2 

    chassis.turnToHeading(0, 1000);

    chassis.moveToPoint(20, -0.5, 2000);

    pros::delay(500);

    //turn to face corner and go

    chassis.turnToHeading(45, 1000);
    chassis.waitUntilDone();

    intakeState = 2;

    chassis.moveToPoint(29.7, 13.3, 2000);
    chassis.waitUntilDone();
    intakeState = 1;

    pros::delay(1500);

    //option numero uno: get ring by alliance and touch ladder
    //go to rings by alliance stake
    chassis.moveToPoint(16.5, 1, 1000, {.forwards = false});
    chassis.turnToHeading(-90, 1000);
    chassis.moveToPose(-50, 0, -90, 2000, {.minSpeed = 80});
    chassis.waitUntilDone();

    //ladder
    chassis.turnToHeading(170, 500, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
    chassis.moveToPoint(-22.22, -20, 1000);
    //end of option 1


    // //option number dos: straight to ladder
    // chassis.moveToPoint(16.5, 1, 1000, {.forwards = false});
    // chassis.moveToPose(-12.6, -26.7, 227, 2000, {.minSpeed = 80});
    // chassis.waitUntilDone();
    // pidActive = true;
    // lbTarget = 35;
    // //end of option 2


    // //option number tres: drop and go to center mogo
    // chassis.moveToPoint(16.5, 1, 1000, {.forwards = false});
    // pros::delay(1000);
    // chassis.turnToHeading(-136.5, 500);
    // chassis.waitUntilDone();
    // clampState = 0;
    // chassis.moveToPose(19.8, -23, 0, 2000, {.forwards = false});
    // //end of option 3




    //go back to the positio0n before corner
    
    //chassis.moveToPoint(16,-4, 2000, {.forwards = false});
    // //ring 2 (wrong color)
    // intakeState = 2;
    // chassis.moveToPoint(29.7, 13.3, 2000);
    // chassis.waitUntilDone();
    // intakeState = 1;
    // pros::delay(1500);

    // //ring 3
    // chassis.moveToPoint(16,-4, 2000, {.forwards = false});
    // intakeState = 2;
    // chassis.moveToPoint(29.7, 13.3, 2000);
    // chassis.waitUntilDone();
    // intakeState = 1;
    // pros::delay(1500);



    /* extra stuff if time permits


    //turn to face rings in middle ladder

    chassis.turnToHeading(226.5, 1000);

    chassis.moveToPose(-14.3, -35, 200, 3000, {.lead = 0.3, .minSpeed = 80});

    chassis.waitUntilDone();

    leftDoinkerState = 1;

    pros::delay(100);

    return;

    //move backwards

    chassis.moveToPoint(-11, -26.8, 2000, {.forwards = false});

    //turn to intake the ring

    chassis.turnToHeading(-155, 1000);

    return;
*/



}

void redMogo() {
    sortState = 2;
    //going to clamp 
    chassis.moveToPoint(0, -31, 1000, {.forwards = false});
    chassis.waitUntilDone();
    pros::delay(200 ); 

    //clamp onto the goal 
    clampState = 1;
    
    pros::delay(250 ); 



    //turn to face ring stack 

    chassis.turnToHeading(-90, 1000);

    intakeState = 1;

    //intake the ring # 1 

    chassis.moveToPoint(-20,-31, 1000);

    //turn and go to ring # 2 

    chassis.turnToHeading(0, 1000);

    chassis.moveToPoint(-20, -0.5, 2000);

    pros::delay(500);

    //turn to face corner and go

    chassis.turnToHeading(-45, 1000);
    chassis.waitUntilDone();

    intakeState = 2;

    chassis.moveToPoint(-29.7, 13.3, 2000);
    chassis.waitUntilDone();
    intakeState = 1;

    pros::delay(1500);

    //option numero uno: get ring by alliance and touch ladder
    //go to rings by alliance stake
    chassis.moveToPoint(-16.5, 1, 1000, {.forwards = false});
    chassis.turnToHeading(90, 1000);
    chassis.moveToPose(50, 0, -90, 2000, {.minSpeed = 80});
    chassis.waitUntilDone();

    //ladder
    chassis.turnToHeading(-170, 500, {.direction = AngularDirection::CW_CLOCKWISE});
    chassis.moveToPoint(22.22, -20, 1000);
    //end of option 1


    // //option number dos: straight to ladder
    // chassis.moveToPoint(-16.5, 1, 1000, {.forwards = false});
    // chassis.moveToPose(12.6, -26.7, 227, 2000, {.minSpeed = 80});
    // chassis.waitUntilDone();
    // pidActive = true;
    // lbTarget = 35;
    // //end of option 2


    // //option number tres: drop and go to center mogo
    // chassis.moveToPoint(-16.5, 1, 1000, {.forwards = false});
    // pros::delay(1000);
    // chassis.turnToHeading(136.5, 500);
    // chassis.waitUntilDone();
    // clampState = 0;
    // chassis.moveToPose(-19.8, -23, 0, 2000, {.forwards = false});
    // //end of option 3

}


void autonomous() {
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // kiwi::Path filePath(config, "/usd/path.json");
    // filePath.follow();
  
    //redRingSide();
    //blueMogo(); 
    redMogo();
}

void opcontrol() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    //kiwiRunControl();

    // matchControl();
    

     autonControl();

    // firstPath.follow();
}
