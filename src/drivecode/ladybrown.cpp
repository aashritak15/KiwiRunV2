#include "drivecode/ladybrown.hpp"
#include "pros/misc.h"
#include "pros/motors.h"

bool pidActive = false;
float lbTarget = 0;
bool pidToggle = false;

void ladyBrownInit() {
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    ladyBrown.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

    pros::Task lbTask(runLB, "ladybrown");
}

void updateLB() {
    if(pidActive)
        controller.set_text(0,0,"pid on    ");
    else
        controller.set_text(0,0,"pid off     ");


    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        pidActive = true;
        lbTarget = 30;
    }
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        pidActive = true;
        lbTarget = 0;
    }
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        pidActive = true;
        lbTarget = 150;
    }


}

void runLB() {

    float kP = 0.1;
    float kD = 0;
    float prevError = 0;

    int count = 1;

    while (true) {
        if(!pidActive) { //if pid inactive, enable right stick control 
            float command = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127.0 * 100.0 * -0.9; 
            ladyBrown.move_velocity(command);

            pros::delay(10);

        } else { //if pid active, run pid
            float error = lbTarget - lbRotation.get_angle() / 100.0;
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

        std::cout<<lbRotation.get_position() / 100 <<"\n";

        pros::delay(10);
    }
}