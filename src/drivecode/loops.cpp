#include "drivecode/loops.hpp"

void matchControl() {
    updateIntake();
    updatePistons();

    int throttle = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    chassis.arcade(throttle, turn); 
}