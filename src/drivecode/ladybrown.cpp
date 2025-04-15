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
}