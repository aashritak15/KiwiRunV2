#include "main.h"
#include "globals.hpp"

namespace kiwi {

    class Config {
        public:
            float beta;
            float zeta;
    
            int subsysNumber;

            lemlib::Chassis& chassis;
            lemlib::Drivetrain& drivetrain;
            pros::MotorGroup& leftMotors;
            pros::MotorGroup& rightMotors;

            std::vector<std::reference_wrapper<int>> subsysStates; //points to all subsys
    
            Config (float beta, float zeta, 
                std::vector<std::reference_wrapper<int>> subsysStates,
                lemlib::Chassis& chassis,
                lemlib::Drivetrain& drivetrain,
                pros::MotorGroup& leftMotors,
                pros::MotorGroup& rightMotors);
    };
    
}