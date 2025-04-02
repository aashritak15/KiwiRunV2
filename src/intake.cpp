#include "main.h"
#include "intake.hpp"
#include "globals.hpp"

float intakeState = 0;

void intakeInit() {
    intakeUpper.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    intakeLower.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    pros::Task intakeTask(runIntake, "intake");
}

/**
 *@brief update intake global state based on button inputs
 */
void updateIntake() {
    static bool buttonl1Pressed = false;
    static bool buttonl2Pressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (!buttonl1Pressed) {
            buttonl1Pressed = true;
            if (intakeState == 0 || intakeState == 2) {
                intakeState = 1;
            } else if (intakeState == 1) {
                intakeState = 0;
            }
        }
    } else {
        buttonl1Pressed = false;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (!buttonl2Pressed) {
            buttonl2Pressed = true;
            if (intakeState == 0 || intakeState == 1) {
                intakeState = 2;
            } else if (intakeState == 2) {
                intakeState = 0;
            }
        }
    } else {
        buttonl2Pressed = false;
    }
}

/**
 *@brief run intake motors based on global state
 */
void runIntake() {

    while (true) {
        if (intakeState == 0) {
            intakeUpper.move_voltage(0);
            intakeLower.move_voltage(0);
        } else if (intakeState == 1) {
            intakeUpper.move_voltage(12000);
            intakeLower.move_voltage(-12000);
        } else if (intakeState == 2) {
            intakeUpper.move_voltage(-12000);
            intakeLower.move_voltage(12000);
        }

        pros::delay(10);
    }
}