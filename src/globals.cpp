#include "kiwirun/includes.hpp"
#include "drivecode/intake.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.hpp"
#include "globals.hpp"
#include <functional>

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({-1, -2}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({12, 15}, pros::MotorGearset::blue);

pros::Motor leftBack(-7, pros::MotorGearset::green);
pros::Motor rightBack(13, pros::MotorGearset::green);

// pros::Imu imu(3);

lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 12.75,
                              lemlib::Omniwheel::NEW_275, 450, 2);


lemlib::ControllerSettings linearController(0, 0, 0, 3, 1, 100, 3, 500, 20);
lemlib::ControllerSettings angularController(0, 0, 0, 3, 1, 100, 3, 500, 0);


lemlib::OdomSensors sensors(nullptr, nullptr, nullptr, nullptr, nullptr);


lemlib::ExpoDriveCurve throttleCurve(3, 10, 1.019);
lemlib::ExpoDriveCurve steerCurve(3, 10, 1.019);


lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);


