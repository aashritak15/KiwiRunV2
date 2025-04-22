#include "drivecode/intake.hpp"
#include "pros/misc.h"
#include "pros/motors.h"

bool l1Pressed = false;
bool l2Pressed = false;
bool downPressed = false;
bool upPressed = false;

float intakeState = 0;

int sortState = 0;

void intakeInit() {
    firstStage.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    secondStage.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    pros::Task intakeTask(runIntake, "intake");
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

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
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
            if(sortState == 2)
                sortState = 0;
            if(sortState == 0)
                sortState = 1; //score red sort blue
            if(sortState == 1)
                sortState = 2; //score blue sort red
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
        } else if(intakeState == 3) {
            firstStage.move_voltage(12000);
        }

        pros::delay(10);
    }
}

void colorSort() {
    while(true) {
        if(sortState == 0)
            controller.set_text(0, 0, "no sort         ");
        if(sortState == 1)
            controller.set_text(0, 0, "sort red         ");
        if(sortState == 2)
            controller.set_text(0, 0, "sort blue         ");

        


    }
}