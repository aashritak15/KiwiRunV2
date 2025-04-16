#include "drivecode/intake.hpp"
#include "pros/motors.h"

bool l1Pressed = false;
bool l2Pressed = false;

float intakeState = 0;

void intakeInit() {
    firstStage.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    secondStage.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    pros::Task intakeTask(runIntake, "intake");
}

void updateIntake() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (!l1Pressed) {
            l1Pressed = true;
            if (intakeState == 0 || intakeState == 2) {
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
            if (intakeState == 0 || intakeState == 1) {
                intakeState = 2;
            } else if (intakeState == 2) {
                intakeState = 0;
            }
        }
    } else {
        l2Pressed = false;
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
        }

        pros::delay(10);
    }
}