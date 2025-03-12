#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "main.h"

namespace kiwi {

    class Config {
        public:
            float beta;
            float zeta;

            lemlib::Chassis& chassis;
            lemlib::Drivetrain& drivetrain;
            pros::MotorGroup& leftMotors;
            pros::MotorGroup& rightMotors;

            std::vector<std::reference_wrapper<float>> subsysStates; //points to all subsys
            std::vector<std::string> subsysNames;
    
            Config(float beta, float zeta, 
                std::vector<std::reference_wrapper<float>> subsysStates,
                std::vector<std::string> subsysNames,
                lemlib::Chassis& chassis,
                lemlib::Drivetrain& drivetrain,
                pros::MotorGroup& leftMotors,
                pros::MotorGroup& rightMotors);
            
            void write();
            float getWheelLinearVel(float rpm);
            float getChassisLinearVel(float leftRPM, float rightRPM);
            float getChassisAngularVel(float leftRPM, float rightRPM);
    };
    
}

#endif