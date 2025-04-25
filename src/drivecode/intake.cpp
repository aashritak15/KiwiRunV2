#include "drivecode/intake.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include <queue>

bool l1Pressed = false;
bool l2Pressed = false;
bool downPressed = false;
bool upPressed = false;

//color sort states
bool throwNextAfterThis = false;
bool throwNext = false;
bool throwNextNext = false;
bool ringDetected = false;
int sortState = 0;

float intakeState = 0;

std::queue<bool> throwRings;
bool throwPushed = false;

void intakeInit() {
    firstStage.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    secondStage.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    optical.set_led_pwm(100);

    pros::Task intakeTask(runIntake, "intake");
    pros::Task colorTask(colorSort, "color sort");
}

void updateIntake() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (!l1Pressed) {
            l1Pressed = true;
            if (intakeState == 0 || intakeState == 2 || intakeState == 3) {
                intakeState = 1;
            } else if (intakeState == 1) {
                intakeState = 0;
            }
        }
    } else {
        l1Pressed = false;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (!l2Pressed) {
            l2Pressed = true;
            if (intakeState == 0 || intakeState == 1 || intakeState == 3) {
                intakeState = 2;
            } else if (intakeState == 2) {
                intakeState = 0;
            }
        }
    } else {
        l2Pressed = false;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
        if (!downPressed) {
            downPressed = true;
            if (intakeState == 0 || intakeState == 1 || intakeState == 2) {
                intakeState = 3;
            } else if (intakeState == 3) {
                intakeState = 0;
            }
        }
    } else {
        downPressed = false;
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
        if(!upPressed) {
            upPressed = true;
            sortState++;
            if(sortState == 3) {
                sortState = 0;
            }
        }
    } else {
        upPressed = false;
    }
}

void runIntake() {
    while(true) {
        if (intakeState == 0) {
            firstStage.move_voltage(0);
            secondStage.move_voltage(0);
        } else if (intakeState == 1) {
            firstStage.move_voltage(12000);
            secondStage.move_voltage(12000);
        } else if (intakeState == 2) {
            firstStage.move_voltage(-12000);
            secondStage.move_voltage(-12000);
            throwNext = false;
            throwNextNext = false;
        } else if(intakeState == 3) {
            firstStage.move_voltage(12000);
        }

        pros::delay(10);
    }
}

void colorSort() {
    while(true) {
        //no throw logic
        if(sortState == 0) {
            controller.set_text(0, 0, "no sort   ");
            throwNext = false;
            ringDetected = false;
        }

        //throw red logic
        else if(sortState == 1) {
            controller.set_text(0, 0, "sort red   ");

            if(0 < optical.get_hue() && optical.get_hue() < 30) { //if a newly detected ring is red,
                if(!ringDetected) { //and a ring hasn't been detected yet,
                    throwNext = true; //throw the next ring
                    ringDetected = true;

                    // std::cout<<"I SAW RED "<<"\n";
                }
            } else {
                ringDetected = false;
            }

            /*
            // if(150 < optical.get_hue() && optical.get_hue() < 220) { //if blue detected
            //     if(!throwPushed) {
            //         throwRings.push(true);
            //         throwPushed = false;
            //     }
            // } else if (0 < optical.get_hue() && optical.get_hue() < 30) {
            //     if(!throwPushed) {
            //         throwRings.push(false);
            //         throwPushed = false;
            //     }
            // } else {
            //     throwPushed = false;
            // }

            // //throw logic
            // if(throwRings.front()) {
            //     float prevIntake = intakeState;

            //     intakeState = 2; //throw
            //     pros::delay(100);
            //     intakeState = prevIntake;
            // }
            // if(!throwRings.empty())
            //     throwRings.pop();
            */
        }

        //throw blue logic
        else if(sortState == 2) {
            controller.set_text(0, 0, "sort blue   ");

            if(200 < optical.get_hue() && optical.get_hue() < 240) { //if a newly detected ring is red,
                if(!ringDetected) { //and a ring hasn't been detected yet
                    throwNext = true; //throw the next ring
                    ringDetected = true;

                    // std::cout<<"I SAW BLUE"<<"\n";
                }
            } else {
                ringDetected = false;
            }

        }

        //actual throw logic
        if(throwNext && distance.get() < 30) { //if throw next and a ring is about to be scored,
            float prevIntake = intakeState;

            pros::delay(165);

            intakeState = 2; //throw
            pros::delay(100);
            intakeState = prevIntake;

            // std::cout<<"I THREW IT"<<"\n";

            throwNext = false;
        }

        pros::delay(10);
    }
}