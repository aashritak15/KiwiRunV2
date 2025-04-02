#include "kiwirun/includes.hpp"
#include "globals.hpp"
#include <fstream>

namespace kiwi {

    void Config::write() {
        nlohmann::json master; //Declare master JSON object

        int index = 0; //Declare index and segments
        int segment = 0;

        std::ofstream file; //Declare file
        file.open("std/path.json"); //TODO: TESTING PATH! CHANGE BACK TO THIS->PATHID

        pros::Task writeTask([&]() {
            while(true) {
                nlohmann::json pointData; //pointData: JSON object for a single point

                lemlib::Pose pose = chassis.getPose(); //Retrieve pose information
                float leftRPM = leftMotors.get_actual_velocity();
                float rightRPM = rightMotors.get_actual_velocity();

                pointData["segment"] = segment;

                pointData["x pos"] = pose.x; //Write pose
                pointData["y pos"] = pose.y;
                pointData["theta"] = pose.theta;
                
                pointData["linear vel"] = getChassisLinearVel(leftRPM, rightRPM); //Write velocities
                pointData["angular vel"] = getChassisAngularVel(leftRPM, rightRPM);
                
                for(int i = 0; i < subsysStates.size(); i++) { //Write all subsystems
                    float state = subsysStates[i];

                    std::string subsysName = subsysNames[i];

                    pointData[subsysName] = state;
                }

                std::cout << pointData.dump(4) << std::endl; //TODO: DEBUG COUT!

                master[std::to_string(index)] = pointData; //Point data written to master data

                index++; //Increment index

                if(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
                    break;
                }

                if(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
                    segment++;
                }

                pros::delay(10);
            }

            file << master.dump(3);

            file.flush();
            file.close();
        });
    }

    float Config::getWheelLinearVel(float rpm) {
        float linear = rpm / 60 * 2 * M_PI * drivetrain.wheelDiameter;
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

        return (rightLinear - leftLinear) / drivetrain.trackWidth;
    }
};