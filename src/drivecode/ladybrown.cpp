#include "drivecode/ladybrown.hpp"
#include "pros/motors.h"

void ladyBrownInit() {
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    ladyBrown.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

    pros::Task lbTask(runLB, "ladybrown");
}

void runLB() {
    while (true) {
        double command = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127.0 * 100.0;
        ladyBrown.move_velocity(command);

        pros::delay(10);
    }

    double derivative = 0;
    double prevError;

    // while (true) {
    //     if(std::abs(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)) > 50) { //TODO: tune
    //         pidActive = true;
    //         derivative = 0;
    //     }

    //     if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    //         if(!LB_BUTTON_PRESSED) {
    //             pidActive = false;
    //         } else {
    //             LB_BUTTON_PRESSED = true;
    //         }
    //     }



    //     if(pidActive == true) {
    //         int target; //TODO: chnage
    //         int current;
    //         int kP;
    //         int kD;

    //         double error = target - current;
    //         derivative = error - prevError;

    //         double pTerm = kP * error;
    //         double dTerm = kD * derivative;

    //         ladyBrown.move_velocity(pTerm + dTerm);
    //     } else {

    //     }
        
    //     if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    //         if(!LB_BUTTON_PRESSED) {
    //             pidActive = true;
    //         } else {
    //             LB_BUTTON_PRESSED = true;
    //         }
    //     }
    //     double command = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127.0 * 100.0;
    //     ladyBrown.move_velocity(command);

    //     pros::delay(10);
    // }
}