#include "drivecode/ladybrown.hpp"
#include "pros/misc.h"
#include "pros/motors.h"

bool pidActive = false;
float lbTarget = 0;
bool rightPressed;

void ladyBrownInit() {
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    ladyBrown.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

    pros::Task lbTask(runLB, "ladybrown");
}

void updateLB() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { //TODO: change button
        if (!rightPressed) { //if button right pressed (toggle), activate pid
            rightPressed = true;
            pidActive = true;

            lbTarget = 30; //TODO: change lb target
        }
    } else {
        rightPressed = false; //pidActive deactivated when goal reached
    }
}

void runLB() {

    float kP = 0;
    float kD = 0;
    float prevError = 0;

    int count = 1;

    while (true) {
        if(!pidActive) { //if pid inactive, enable right stick control 
            float command = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127.0 * 100.0; 
            ladyBrown.move_velocity(command);

            pros::delay(10);

        } else { //if pid active, run pid
            float error = lbTarget - lbRotation.get_angle();
            float derivative = prevError - error;

            float pTerm = error * kP;
            float dTerm = derivative * kD;

            float command = pTerm + dTerm; //calculate p and d terms

            if(command > 100) { //clamp command
                float scale = 600.0 / command;
                command *= scale;
            }

            if(error < 2) { //pid timeout to enable manual control: .20 seconds within 2deg to target
                count++;

                if(count == 20) {
                    pidActive = false;
                    prevError = 0;
                }
            } else {
                count = 1;
            }

            ladyBrown.move_velocity(command); //move lb
        }

        pros::delay(10);
    }
}