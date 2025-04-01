#include "kiwirun/includes.hpp"
#include "globals.hpp"
#include <fstream>

namespace kiwi {

    void Config::write() {
        nlohmann::json json_master;
        std::vector<nlohmann::json> json_segments;
        std::vector<std::vector<nlohmann::json>> json_pointData;

        int index = 0;
        int segment = 0;

        std::ofstream file;
        file.open("std/path.json");

        pros::Task writeTask([&]() {
            while(true) {
                nlohmann::json json_entry;

                lemlib::Pose pose = chassis.getPose();
                float leftRPM = leftMotors.get_actual_velocity();
                float rightRPM = rightMotors.get_actual_velocity();

                json_entry["index"] = index;
                json_entry["segment"] = segment;
                json_entry["x pos"] = pose.x;
                json_entry["y pos"] = pose.y;
                
                json_entry["linear vel"] = getChassisLinearVel(leftRPM, rightRPM);
                json_entry["angular vel"] = getChassisAngularVel(leftRPM, rightRPM);
                
                for(int i = 0; i < subsysStates.size(); i++) {
                    int state = subsysStates[i];

                    json_entry[subsysNames[i]] = state;
                }

                json_pointData.push_back(json_entry);

                index++;

                if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) { //TODO: ADD CONTROLLER OBJECT
                    break;
                }

                if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) { //TODO: ADD CONTROLLER OBJECT
                    json_segments.push_back(json_pointData);
                    json_pointData.clear();

                    segment++;
                }

                pros::delay(10);
            }

            json_master["data"] = json_segments;

            file << json_master.dump(5);
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