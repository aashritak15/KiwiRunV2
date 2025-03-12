#include "main.h"
#include "pros/imu.hpp"
#include "kiwirun/includes.hpp"

extern pros::Controller controller;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern lemlib::TrackingWheel horizontal;
extern lemlib::TrackingWheel vertical;

extern lemlib::Chassis chassis;
extern lemlib::Drivetrain drivetrain;

extern pros::Imu imu;

extern kiwi::Config configuration;
extern kiwi::Path path;