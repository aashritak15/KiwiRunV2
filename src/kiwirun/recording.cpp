#include "kiwirun/includes.hpp"
#include "globals.hpp"
#include <fstream>
#include <stdexcept>
#include <deque> 
#include "pros/misc.hpp"

namespace kiwi {

    void Config::write() {

        pros::Task writeTask([&]() {
            nlohmann::json master = nlohmann::json::array();  // Holds all points

            int index = 0;

            int count = 1;

            std::deque<float> linearVelocityHistory;
            std::deque<float> angularVelocityHistory;

            const int historySize = 5; 

            auto getAverage = [&](std::deque<float>& history, float newValue) {
                 history.push_back(newValue); //add the newest history value to the end 

                    if (history.size() > historySize) {
                        history.pop_front(); //remove the oldest value if past 5 vlaues 
                    }

                    float total = 0;

                    //sum up all the values in the current 

                    for (float value : history) {
                        total += value;
                    }

                    return total / history.size();
                };


            while(leftMotors.get_actual_velocity() < 10 && rightMotors.get_actual_velocity() < 10) {
                pros::delay(10);
            }

            while (true) {
                nlohmann::json pointData;

                lemlib::Pose pose = this->chassis.getPose(true);
                float leftRPM = leftMotors.get_actual_velocity();
                float rightRPM = rightMotors.get_actual_velocity();

                float xPos = std::round(pose.x * 1000) / 1000;
                float yPos = std::round(pose.y * 1000) / 1000;
                float thetaPos = std::round(pose.theta * 1000) / 1000;

                pointData["index"] = index++;
                pointData["x pos"] = xPos;
                pointData["y pos"] = yPos;
                pointData["theta"] = thetaPos;

                float rawLinearVel = getChassisLinearVel(leftRPM, rightRPM);
                float rawAngularVel = getChassisAngularVel(leftRPM, rightRPM);

                // float smoothedLinearVel = getAverage(linearVelocityHistory, rawLinearVel);
                // float smoothedAngularVel = getAverage(angularVelocityHistory, rawAngularVel);

                // pointData["linear vel"] = smoothedLinearVel;
                // pointData["angular vel"] = smoothedAngularVel;

                //TODO: maybe add back velocity smoothing

                pointData["linear vel"] = rawLinearVel;
                pointData["angular vel"] = rawAngularVel;

                for (size_t i = 0; i < subsysStates.size(); i++) {
                    pointData[subsysNames[i]] = subsysStates[i].get();

                }

                master.push_back(pointData);  // Collect this point into master array

                if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) { //break condition //TODO: change button
                    break;
                }

                pros::delay(10);
            }

            pros::delay(500);

            std::ofstream file("/usd/path.json", std::ios::out | std::ios::trunc);

            pros::delay(500);

            if(!pros::usd::is_installed()) {
                throw std::runtime_error("No SD card present.\n");
            }
            
            if(!file.is_open()) {
                throw std::runtime_error("Failed to open file.");
            }

            file << master.dump(3);  // Write all at once
            file.close();
        });
    }

    float Config::getWheelLinearVel(float rpm) {
        return rpm / 60 * M_PI * this->drivetrain.wheelDiameter * this->driving / this->driven; 
    }

    float Config::getChassisLinearVel(float leftRPM, float rightRPM) {
        float left = getWheelLinearVel(leftRPM);    
        float right = getWheelLinearVel(rightRPM);
            
        return (left + right) / 2;
    }

    float Config::getChassisAngularVel(float leftRPM, float rightRPM) {
        float left = getWheelLinearVel(leftRPM);    
        float right = getWheelLinearVel(rightRPM);

        float returned = (left - right) / drivetrain.trackWidth;

        return returned;
    }

};
