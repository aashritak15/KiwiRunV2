#include "drivecode/pistons.hpp"

float clampState = 0;
float leftDoinkerState = 0;
float rightDoinkerState = 0;

bool yPressed = false;
bool LEFT_DOINK_BUTTON = false;
bool RIGHT_DOINK_BUTTON = false;

void pistonInit() {
    mogoClamp.set_value(false);
    leftDoinker.set_value(false);
    rightDoinker.set_value(false);
    
    pros::Task pistonTask (runPistons, "pistons");
}

void updatePistons() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {  //mogo clamp state update
        if (!yPressed) {                                                //toggle on y
            yPressed = true;
            if (clampState == 0) {
                clampState = 1;
            } else {
                clampState = 0;
            }
        }
    } else {
        yPressed = false;
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {  //left doinker state update
        if (!LEFT_DOINK_BUTTON) {                                       //toggle on x
            LEFT_DOINK_BUTTON = true;
            if (leftDoinkerState == 0) {
                leftDoinkerState = 1;
            } else {
                leftDoinkerState = 0;
            }
        }
    } else {
        LEFT_DOINK_BUTTON = false;
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {  //right doinker state update
        if (!RIGHT_DOINK_BUTTON) {                                      //toggle on a
            RIGHT_DOINK_BUTTON = true;
            if (rightDoinkerState == 0) {
                rightDoinkerState = 1;
            } else {
                rightDoinkerState = 0;
            }
        }
    } else {
        RIGHT_DOINK_BUTTON = false;
    }

}

void runPistons() {
    while (true) {
        if(clampState == 1) { //mogo clamp control
            mogoClamp.set_value(true);
        } else {
            mogoClamp.set_value(false);
        }

        if (leftDoinkerState == 1) { //left doinker control
            leftDoinker.set_value(true);
        } else {
            leftDoinker.set_value(false);
        }

        if (rightDoinkerState == 1) { //right doinker control
            rightDoinker.set_value(true);
        } else {
            rightDoinker.set_value(false);
        } 

        pros::delay(10);
    }
}