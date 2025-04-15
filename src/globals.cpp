#include "kiwirun/includes.hpp"
#include "drivecode/intake.hpp"
#include "globals.hpp"
#include <functional>

//TODO: all of this is wrong btw lmao

pros::Controller controller(pros::E_CONTROLLER_MASTER);


pros::MotorGroup leftMotors({-1, -4, -2}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({14, 15, 12}, pros::MotorGearset::blue);


pros::Imu imu(3);


lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 12.75,
                              lemlib::Omniwheel::NEW_325, 450, 2);


lemlib::ControllerSettings linearController(10, 0, 3, 3, 1, 100, 3, 500, 20);
lemlib::ControllerSettings angularController(2, 0, 10, 3, 1, 100, 3, 500, 0);


lemlib::OdomSensors sensors(nullptr, nullptr, nullptr, nullptr, &imu);


lemlib::ExpoDriveCurve throttleCurve(3, 10, 1.019);
lemlib::ExpoDriveCurve steerCurve(3, 10, 1.019);


lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);


