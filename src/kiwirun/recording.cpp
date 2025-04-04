#include "kiwirun/includes.hpp"
#include "globals.hpp"
#include <fstream>
#include <stdexcept>

namespace kiwi {

    void Config::write() {
        nlohmann::json master = nlohmann::json::array(); //Declare master JSON object

        std::ofstream file; //Declare file
        file.open("std/path.json"); //TODO: TESTING PATH! CHANGE BACK TO THIS->PATHID

        pros::Task writeTask([&]() {
            int jklmfun = 0;

            while(true) {
                nlohmann::json pointData; //pointData: JSON object for a single point

                lemlib::Pose pose = this->chassis.getPose(); //Retrieve pose information
                float leftRPM = leftMotors.get_actual_velocity();
                float rightRPM = rightMotors.get_actual_velocity();

                float xPos = std::round(pose.x * 1000) / 1000;
                float yPos = std::round(pose.y * 1000) / 1000;
                float thetaPos = std::round(pose.theta * 1000) / 1000;

                // std::cout<<xPos<<"\n";
                // std::cout<<yPos<<"\n";
                // std::cout<<thetaPos<<"\n";
                // std::cout<<getChassisLinearVel(leftRPM, rightRPM)<<"\n";
                // std::cout<<getChassisAngularVel(leftRPM, rightRPM)<<"\n";
                // std::cout<<leftRPM<<"\n";
                // std::cout<<rightRPM<<"\n";

                // std::cout<<this->drivetrain.trackWidth<<"\n";
                // std::cout<<this->drivetrain.wheelDiameter<<"\n";
                
                pointData["index"] = jklmfun;

                pointData["x pos"] = xPos; //Write pose
                pointData["y pos"] = yPos;
                pointData["theta"] = thetaPos;
                
                pointData["linear vel"] = getChassisLinearVel(leftRPM, rightRPM); //Write velocities
                pointData["angular vel"] = getChassisAngularVel(leftRPM, rightRPM);
                
                for(int i = 0; i < subsysStates.size(); i++) { //Write all subsystems
                    float state = this->subsysStates[i];

                    std::string subsysName = subsysNames[i];

                    // std::cout<<subsysName<<"\n";
                    // std::cout<<state<<"\n\n";

                    pointData[subsysName] = state;
                }

                jklmfun++;
                // std::cout<<jklmfun<<"\n";

                // master[std::to_string(jklmfun)] = pointData;

                file<<pointData.dump(3)<<"\n";

                // if (std::isnan(pose.x) || std::isnan(pose.y) || std::isnan(pose.theta)) {
                //     std::cerr << "Warning: Invalid pose values detected!" << std::endl;
                // }
                // if (std::isnan(leftRPM) || std::isnan(rightRPM)) {
                //     std::cerr << "Warning: Invalid motor RPM values!" << std::endl;
                // }

                if(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
                    break;
                }

                pros::delay(10);
            }

            file.flush();
            file.close();
        });
    }

    float Config::getWheelLinearVel(float rpm) {
        float linear = rpm / 60 * 2 * M_PI * 3.25; //TODO: NOT WHEEL DIAMETER
        return linear;
    }

    float Config::getChassisLinearVel(float leftRPM, float rightRPM) {
        float leftLinear = getWheelLinearVel(leftRPM);
        float rightLinear = getWheelLinearVel(rightRPM);

        return (leftLinear + rightLinear) / 2;
    }

    float Config::getChassisAngularVel(float leftRPM, float rightRPM) {
        float leftLinear = getWheelLinearVel(leftRPM);
        float rightLinear = getWheelLinearVel(rightRPM);

        float angularVel = (rightLinear - leftLinear) / 12.75; //TODO: NOT TRACKWIDTH

        return angularVel;
    }
};