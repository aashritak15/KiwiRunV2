#include "kiwirun/includes.hpp"
#include "globals.hpp"
#include <fstream>
#include <stdexcept>


namespace kiwi {

void Config::write() {
    nlohmann::json master = nlohmann::json::array(); // Declare master JSON object

    std::ofstream file;
    file.open("std/path.json"); // TODO: replace with dynamic path if needed

    pros::Task writeTask([&]() {
        int jklmfun = 0;

        while (true) {
            nlohmann::json pointData;

            lemlib::Pose pose = this->chassis->getPose(); // FIXED: use -> for pointers
            float leftRPM = this->leftMotors->get_actual_velocity(); // FIXED
            float rightRPM = this->rightMotors->get_actual_velocity(); // FIXED

            float xPos = std::round(pose.x * 1000) / 1000;
            float yPos = std::round(pose.y * 1000) / 1000;
            float thetaPos = std::round(pose.theta * 1000) / 1000;

            pointData["index"] = jklmfun;
            pointData["x pos"] = xPos;
            pointData["y pos"] = yPos;
            pointData["theta"] = thetaPos;
            pointData["linear vel"] = getChassisLinearVel(leftRPM, rightRPM);
            pointData["angular vel"] = getChassisAngularVel(leftRPM, rightRPM);

            for (int i = 0; i < subsysStates.size(); i++) {
                float state = *this->subsysStates[i]; // FIXED: dereference float*
                std::string subsysName = subsysNames[i];
                pointData[subsysName] = state;
            }

            jklmfun++;
            file << pointData.dump(3) << "\n";

            if (this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
                break;
            }

            pros::delay(10);
        }

        file.flush();
        file.close();
    });
}

float Config::getWheelLinearVel(float rpm) {
    float linear = rpm / 60 * 2 * M_PI * 3.25; // TODO: Replace with actual wheel diameter
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

    float angularVel = (rightLinear - leftLinear) / 12.75; // TODO: Replace with actual track width
    return angularVel;
}

}
