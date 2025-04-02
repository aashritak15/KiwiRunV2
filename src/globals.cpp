#include "kiwirun/includes.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include <functional>

std::vector<std::reference_wrapper<float>> subsysStates = {intakeState};
std::vector<std::string> subsysNames = {"intake state"};

kiwi::Config configuration(1,
     1, 
     subsysStates, 
     subsysNames, 
     chassis, 
     drivetrain, 
     leftMotors, 
     rightMotors, 
     controller
);

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({-8, -4, -11}, pros::MotorGearset::blue); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({18, 15, 12}, pros::MotorGearset::blue); // right motor group - ports 6, 7, 9 (reversed)

// Inertial Sensor on port 10
pros::Imu imu(3);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              12.75,
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              450,
                              2
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);
