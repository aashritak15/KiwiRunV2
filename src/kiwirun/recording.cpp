#include "kiwirun/recording.hpp"
#include "globals.hpp"

namespace kiwi {

class Config {
    
float getWheelLinearVel(float rpm) {
    float linear = rpm / 60 * 2 * M_PI * drivetrain.wheelDiameter;
    return linear;
}

float getChassisLinearVel(float leftRPM, float rightRPM) {
    float leftLinear = getWheelLinearVel(leftRPM);
    float rightLinear = getWheelLinearVel(rightRPM);

    return (leftLinear + rightLinear) / 2;
}

float getChassisAngularVel(float leftRPM, float rightRPM) {
    float leftLinear = getWheelLinearVel(leftRPM);
    float rightLinear = getWheelLinearVel(rightRPM);

    return (rightLinear - leftLinear) / drivetrain.trackWidth;
}

void write() {
    nlohmann::json json;
    int index = 1;

    while(true) {
        lemlib::Pose pose = chassis.getPose();
        float leftRPM = leftMotors.get_actual_velocity();
        float rightRPM = rightMotors.get_actual_velocity();

        json["index"] = index;
        json["linear vel"] = getChassisLinearVel(leftRPM, rightRPM);
        json["angular vel"] = getChassisAngularVel(leftRPM, rightRPM);
        
        for(int i = 0; i < this->; i++)

        pros::delay(10);
    }
}

};

}