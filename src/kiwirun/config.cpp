#include "kiwirun/config.hpp"

namespace kiwi {

    Config::Config(float beta, float zeta, 
        std::vector<std::reference_wrapper<int>> subsysStates,
        lemlib::Chassis& chassis,
        lemlib::Drivetrain& drivetrain,
        pros::MotorGroup& leftMotors,
        pros::MotorGroup& rightMotors):
    beta(beta), zeta(zeta), subsysStates(subsysStates),
    chassis(chassis), drivetrain(drivetrain), 
    leftMotors(leftMotors), rightMotors(rightMotors){
        this->subsysNumber = subsysStates.size();
    }

}