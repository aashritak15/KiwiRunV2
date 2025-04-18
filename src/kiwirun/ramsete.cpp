#include <cmath>
#include "kiwirun/includes.hpp"
#include "globals.hpp"

std::string debugLine;
std::ofstream debugFile("usd/debug.txt");

namespace kiwi {

float Path::findLateralError(float targetX, float targetY) { // lateral error

    lemlib::Pose currentPose = this->config.chassis.getPose(true);

    float deltaX = targetX - currentPose.x;
    float deltaY = targetY - currentPose.y;

    return -deltaX * sin(currentPose.theta) + deltaY * cos(currentPose.theta);
}

float Path::findLongitudinalError(float targetX, float targetY) { // longitudinal error

    lemlib::Pose currentPose = this->config.chassis.getPose(true);

    float deltaX = targetX - currentPose.x;
    float deltaY = targetY - currentPose.y;

    return deltaX * cos(currentPose.theta) + deltaY * sin(currentPose.theta);
}

float Path::findThetaError(float targetTheta) { // theta error with angle jump sanitation
    float currentTheta = this->config.chassis.getPose(true).theta;
    float delta = fmod((targetTheta - currentTheta + M_PI), 2 * M_PI) - M_PI;
    return (delta == -M_PI) ? M_PI : delta;
}

int Path::findClosestPoint(lemlib::Pose pose, int prevIndex) {
    int closestIndex = prevIndex + 1;
    float closestDist = infinity();

    for (int i = closestIndex; i < this->pathRecordings.size(); i++) {
        // loop starting at ONE FORWARD THE PREVIOUSLY CLOSEST POINT! SKIP / STOP TOLERANCE!
        // and ending at the end of the path

        const float dist = std::abs(pose.distance(pathRecordings[i])); // distance to pose

        if (dist < closestDist) { // if tested point is closer, record it and move on
            closestDist = dist;
            closestIndex = i;
        } else if (dist > closestDist) {
            prevIndex = closestIndex;
            return closestIndex; // if distance increases because you're past the closest point, return the closest pose
        }
    }

    return -1; // you're screwed OR end of path reached
}

void Path::updateSubsystems(int index) { //TODO: check
    for (int i = 0; i < this->subsysRecordings[index].size(); i++) {
        this->config.subsysStates[i] = this->subsysRecordings[index][i]; // loop through all subsystem states and update
    }
}

float Path::toRPM(float linearVel) {
    float corrected = linearVel / (M_PI * this->config.drivetrain.wheelDiameter) * 60 / this->config.driven * this->config.driving;
    return corrected;
}

void Path::ramseteStep(int index) {

    float beta = this->config.beta; // beta and zeta values fetched
    float zeta = this->config.zeta;

    lemlib::Pose target = this->pathRecordings[index]; // target pose fetched

    float targetX = target.x; // target pose data fetched
    float targetY = target.y;
    float targetTheta = target.theta;

    float linearVelTarget = this->velRecordings[index][0]; // target velocities fetched
    float angularVelTarget = this->velRecordings[index][1];

    float errorLateral = findLateralError(targetX, targetY); // lateral or crosstrack error calculated
    float errorLongitudinal = findLongitudinalError(targetX, targetY); // longitudinal or front/back error calculated
    float errorTheta = findThetaError(targetTheta); // angular error calculated

    float gain = 2 * zeta * std::sqrt(
        std::pow(angularVelTarget, 2) + (beta * std::pow(linearVelTarget, 2))
    ); // master gain calculated with: sqrt[(angular velocity)^2 + beta * (linear velocity)^2]

    float linearVelCommand = 
        (linearVelTarget * std::cos(errorTheta)) 
        + (gain * errorLongitudinal
    ); // linear command calculated with: [linear velocity target * cos(angle error)] + (gain * longitudinal error)

    if(std::abs(errorTheta) < 0.01) { //*divide by zero protection
        errorTheta = 0.01;
    }

    float angularVelCommand = 
        angularVelTarget + (gain * errorTheta) + 
        (beta * linearVelTarget * std::sin(errorTheta)
        * errorLateral / errorTheta
    ); // angular command calculated with: beta * linear velocity target * sin(angle error) * lateral error / angle error

    float tangentialVelCommand = angularVelCommand * this->config.drivetrain.trackWidth / 2; //convert angular vel command from rad/s to in/s
    //TODO: multiply or divide by two?

    float leftRPMCommand = toRPM(linearVelCommand + tangentialVelCommand); //convert in/s of wheels to rpm
    float rightRPMCommand = toRPM(linearVelCommand - tangentialVelCommand);

    float maxRPM = std::max(std::abs(leftRPMCommand), std::abs(rightRPMCommand));

    if(maxRPM > 600) {
        float scale = 600.0 / maxRPM;
        leftRPMCommand *= scale;
        rightRPMCommand *= scale;
    }

    this->config.leftMotors.move_velocity(leftRPMCommand); // velocity sent to motors
    this->config.rightMotors.move_velocity(rightRPMCommand);

    leftBack.move_velocity(leftRPMCommand / 3); //*: sketch 55w implementation lol but it'll work
    rightBack.move_velocity(rightRPMCommand / 3);

    lemlib::Pose pose = this->config.chassis.getPose();

    debugLine.append("current x: " + std::to_string(pose.x) + "\n"); //*DEBUG LINES FOR CURRENT XYTHETA
    debugLine.append("current y: " + std::to_string(pose.y) + "\n");
    debugLine.append("current theta: " + std::to_string(pose.theta) + "\n\n");

    debugLine.append("x target: " + std::to_string(targetX) + "\n"); //*DEBUG LINES FOR TARGET XYTHETA
    debugLine.append("y target: " + std::to_string(targetY) + "\n");
    debugLine.append("theta target: " + std::to_string(targetTheta) + "\n\n");

    debugLine.append("current left rpm: " + std::to_string(this->config.leftMotors.get_actual_velocity()) + "\n"); //*DEBUG LINES FOR CURRENT RPMS
    debugLine.append("current right rpm: " + std::to_string(this->config.rightMotors.get_actual_velocity()) + "\n\n");

    debugLine.append("lateral error: " + std::to_string(errorLateral) + "\n"); //*DEBUG LINES FOR ERRORS
    debugLine.append("longitudinal error: " + std::to_string(errorLongitudinal) + "\n");
    debugLine.append("theta error: " + std::to_string(errorTheta) + "\n\n");

    debugLine.append("gain: " + std::to_string(gain) + "\n"); //*DEBUG LINES FOR GAIN

    debugLine.append("linear vel target: " + std::to_string(linearVelTarget) + "\n"); //*DEBUG LINES FOR VEL TARGETS
    debugLine.append("angular vel target: " + std::to_string(angularVelTarget) + "\n\n");

    debugLine.append("linear vel command: " + std::to_string(linearVelCommand) + "\n");//*DEBUG LINES FOR VEL COMMANDS
    debugLine.append("angular vel command: " + std::to_string(angularVelCommand) + "\n");
    debugLine.append("tangential vel command: " + std::to_string(tangentialVelCommand) + "\n\n");

    debugLine.append("left rpm command: " + std::to_string(linearVelCommand) + "\n"); //*DEBUG LINE FOR LEFT AND RIGHT RPM
    debugLine.append("right rpm command: " + std::to_string(angularVelTarget) + "\n\n\n\n"); //*LAST DEBUG LINE

    //19 debug lines wow

}

void Path::follow() {
    int index = 0;

    this->fetchData(); //fetch data

    while (true) {

        debugLine.append("NEW TICK\n\n");

        index = findClosestPoint(this->config.chassis.getPose(), index);

        debugLine.append("closest index: " + std::to_string(index) + "\n\n");

        if (index == -1) {
            break;
        }

        ramseteStep(index);
        updateSubsystems(index);

        debugFile << debugLine;
        debugLine.clear();

        pros::delay(10);

    }

    return;

}

}
