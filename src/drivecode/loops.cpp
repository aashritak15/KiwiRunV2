#include "drivecode/loops.hpp"
#include "pros/misc.h"

void matchControl() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    while(true) {

        std::int32_t rawThrottle = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        std::int32_t rawTurn = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

        float throttle = rawThrottle;
        float turn = rawTurn * 0.9;

        float leftCommand = (throttle + turn) / 127.0;
        float rightCommand = (throttle - turn) / 127.0;

        if(leftCommand > 1) {
            float fix = 1 / leftCommand;
            leftCommand = 1;
            rightCommand *= fix;
        }
        
        if(rightCommand > 1) {
            float fix = 1 / rightCommand;
            leftCommand *= fix;
            rightCommand = 1;
        }

        leftCommand *= 12000;
        rightCommand *= 12000;

        // leftMotors.move_voltage(leftCommand);
        // leftBack.move_voltage(leftCommand);

        // rightMotors.move_voltage(rightCommand);
        // rightBack.move_voltage(rightCommand);

        std::cout<<rawThrottle<<"\n";
        std::cout<<rawTurn<<"\n";

        updateIntake();
        updatePistons();

        pros::delay(10);
    }
}

void kiwiRunControl() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // standardConfig.write();

    while(true) {

        int throttle = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) * 0.9;

        double leftCommand = (throttle + turn) / 127.0;
        double rightCommand = (throttle - turn) / 127.0;

        if(leftCommand > 1) {
            float fix = 1 / leftCommand;
            leftCommand = 1;
            rightCommand *= fix;
        }
        
        if(rightCommand > 1) {
            float fix = 1 / rightCommand;
            leftCommand *= fix;
            rightCommand = 1;
        }

        leftCommand *= 12000;
        rightCommand *= 12000;

        leftMotors.move_voltage(leftCommand);
        leftBack.move_voltage(leftCommand);

        rightMotors.move_voltage(rightCommand);
        rightBack.move_voltage(rightCommand);

        updateIntake();
        updatePistons();

        pros::delay(100);
    }
}