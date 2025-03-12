#include <cmath>
#include "kiwirun/includes.hpp"

namespace kiwi {

float Path::findLateralError(float targetX, float targetY) { 

    lemlib::Pose currentPose = this->config.chassis.getPose(true);

    float lateralError = (targetX - currentPose.x) * std::cos(currentPose.theta) 
    + (targetY - currentPose.y) * std::cos(currentPose.theta);

    return lateralError;
}

float Path::findLongitudinalError(float targetX, float targetY) {

    lemlib::Pose currentPose = this->config.chassis.getPose(true);

    float lateralError = (targetX - currentPose.x) * std::cos(currentPose.theta) 
    + (targetY - currentPose.y) * std::cos(currentPose.theta);

    return lateralError;
}

float Path::findThetaError(float targetTheta) {
    return targetTheta - this->config.chassis.getPose(true).theta;
}

lemlib::Pose Path::findClosestPoint(lemlib::Pose pose, int& prevIndex) {
    int closestIndex;
    float closestDist = infinity();

    for (int i = prevIndex + 1; i < this->pathRecordings.size(); i++) {
        const float dist = std::abs(pose.distance(pathRecordings[i]));

        if (dist < closestDist) {
            closestDist = dist;
            closestIndex = i;
        } else if (dist > closestDist) {
            prevIndex = closestIndex;
            return pathRecordings[i - 1]; //if distance increases because you're past the closest point, return the pose 
        }
    }

    prevIndex = -1; //signal that end of path is reached
    return pathRecordings[prevIndex + 1]; //TODO: probably not a good handler
}

void Path::updateSubsystems(int index) {
    for (int i = 0; i < this->subsysRecordings.size(); i++) {
        this->config.subsysStates[i] = this->subsysRecordings[index][i];
    }
}

void Path::ramseteStep(lemlib::Pose target) {

    float beta = this->config.beta;
    float zeta = this->config.zeta;

    float targetX = target.x;
    float targetY = target.y;
    float targetTheta = target.theta;

    float linearVelTarget;
    float angularVelTarget;
    float errorLateral = findLateralError(targetX, targetY);
    float errorLongitudinal = findLongitudinalError(targetX, targetY);
    float errorTheta = findThetaError(targetTheta);

    float gain = 2 * zeta * std::sqrt(
        std::pow(angularVelTarget, 2) + beta + std::pow(linearVelTarget, 2));

    float linearVelCommand = (linearVelTarget * std::cos(errorTheta)) + (gain * errorLongitudinal);
    float angularVelCommand = angularVelTarget + (gain * errorTheta) + (
        beta * linearVelTarget * std::sin(errorTheta) * errorLateral / errorTheta
    );

    this->config.leftMotors.move_velocity(linearVelCommand + angularVelCommand); //TODO: here is moveVelocity and not moveVoltage 
    this->config.rightMotors.move_velocity(linearVelCommand - angularVelCommand);
}

void Path::follow() {
    int index;

    while(true) {

        lemlib::Pose target = findClosestPoint(this->config.chassis.getPose(), index);

        if(index == -1) {
            return;
        }

        ramseteStep(target);
        updateSubsystems(index);

        pros::delay(10);
    }

}

}