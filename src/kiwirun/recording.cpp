#include "kiwirun/includes.hpp"
#include "globals.hpp"
#include <fstream>
#include <stdexcept>

#include "drivecode/intake.hpp" //TODO: REMOVE LATER PPLS
#include "pros/misc.hpp"

namespace kiwi {

    void Config::write() {
        pros::Task writeTask([&]() {
            nlohmann::json master = nlohmann::json::array();  // Holds all points

            int index = 0;

            int count = 1;

            while (true) {
                nlohmann::json pointData;

                lemlib::Pose pose = this->chassis.getPose();
                float leftRPM = leftMotors.get_actual_velocity();
                float rightRPM = rightMotors.get_actual_velocity();

                float xPos = std::round(pose.x * 1000) / 1000;
                float yPos = std::round(pose.y * 1000) / 1000;
                float thetaPos = std::round(pose.theta * 1000) / 1000;

                pointData["index"] = index++;
                pointData["x pos"] = xPos;
                pointData["y pos"] = yPos;
                pointData["theta"] = thetaPos;

                pointData["linear vel"] = getChassisLinearVel(leftRPM, rightRPM);
                pointData["angular vel"] = getChassisAngularVel(leftRPM, rightRPM);

                for (size_t i = 0; i < subsysStates.size(); i++) {
                    pointData[subsysNames[i]] = subsysStates[i].get();

                }

                master.push_back(pointData);  // Collect this point into master array

                if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
                    break;
                }

                pros::delay(10);
            }

            intakeState = 1; //* the break logic works

            pros::delay(500);

            std::ofstream file("/usd/path.json", std::ios::out | std::ios::trunc);

            pros::delay(500);

            if(!pros::usd::is_installed()) {
                throw std::runtime_error("there's no sd card ya friggin bum\n");
            }
            
            if(!file.is_open()) {
                throw std::runtime_error("Failed to open file.");
            }

            file << master.dump(3);  // Write all at once
            file.close();
        });
    }

    float Config::getWheelLinearVel(float rpm) {
        return rpm / 60 * M_PI * this->drivetrain.wheelDiameter * this->driven / this->driving; //TODO: check if wheel diameter reference works
    }

    float Config::getChassisLinearVel(float leftRPM, float rightRPM) {
        return (getWheelLinearVel(leftRPM) + getWheelLinearVel(rightRPM)) / 2;
    }

    float Config::getChassisAngularVel(float leftRPM, float rightRPM) {
        return ( getWheelLinearVel(leftRPM) - getWheelLinearVel(rightRPM) ) / this->drivetrain.trackWidth ; //TODO: check if trackwidth reference works 

        //TODO: multiply/divide by two????
    }

};
