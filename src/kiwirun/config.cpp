#include "config.hpp"

namespace kiwi {

    Config::Config(float beta, float zeta,
                   const std::vector<float*>& subsysStates,
                   const std::vector<std::string>& subsysNames,
                   lemlib::Chassis* chassis,
                   const lemlib::Drivetrain& drivetrain,
                   pros::MotorGroup* leftMotors,
                   pros::MotorGroup* rightMotors,
                   const pros::Controller& controller)
        : beta(beta),
          zeta(zeta),
          chassis(chassis),
          drivetrain(drivetrain),
          leftMotors(leftMotors),
          rightMotors(rightMotors),
          controller(controller),
          subsysStates(subsysStates),
          subsysNames(subsysNames) {}

}
