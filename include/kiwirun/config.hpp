#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "main.h"

namespace kiwi {

    class Config {
        public:
            float beta;
            float zeta;

            lemlib::Chassis* chassis;
            lemlib::Drivetrain drivetrain;
            pros::MotorGroup* leftMotors;
            pros::MotorGroup* rightMotors;

            pros::Controller controller;

            std::vector<float*> subsysStates; // safer than reference_wrapper
            std::vector<std::string> subsysNames;
    
            Config(float beta, float zeta, 
                const std::vector<float*>& subsysStates,
                const std::vector<std::string>& subsysNames,
                lemlib::Chassis* chassis,
                const lemlib::Drivetrain& drivetrain,
                pros::MotorGroup* leftMotors,
                pros::MotorGroup* rightMotors,
                const pros::Controller& controller);
            
            void write();
            float getWheelLinearVel(float rpm);
            float getChassisLinearVel(float leftRPM, float rightRPM);
            float getChassisAngularVel(float leftRPM, float rightRPM);
    };
    
}

#endif
