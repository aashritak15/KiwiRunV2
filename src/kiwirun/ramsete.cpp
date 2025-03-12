#include <cmath>
#include "kiwirun/includes.hpp"

namespace kiwi {

float Path::findLateralError(float targetX, float targetY) { 

    lemlib::Pose currentPose = this->config.chassis.getPose(true);

    float deltaX = targetX - currentPose.x;
    float deltaY = targetY - currentPose.y;

    return -deltaX * sin(currentPose.theta) + deltaY * cos(currentPose.theta);
}

float Path::findLongitudinalError(float targetX, float targetY) {

    lemlib::Pose currentPose = this->config.chassis.getPose(true);

    float deltaX = targetX - currentPose.x;
    float deltaY = targetY - currentPose.y;

    return deltaX * cos(currentPose.theta) + deltaY * sin(currentPose.theta);
}

float Path::findThetaError(float targetTheta) {
    float currentTheta = this->config.chassis.getPose(true).theta;
    float delta = fmod((targetTheta - currentTheta + M_PI), 2*M_PI) - M_PI;
    return (delta == -M_PI) ? M_PI : delta;
}

int Path::findClosestPoint(lemlib::Pose pose, int prevIndex) {
    int closestIndex = prevIndex + 1;
    float closestDist = infinity();

    for (int i = prevIndex + 1; i < this->pathRecordings.size(); i++) {
        const float dist = std::abs(pose.distance(pathRecordings[i]));

        if (dist < closestDist) {
            closestDist = dist;
            closestIndex = i;
        } else if (dist > closestDist) {
            prevIndex = closestIndex;
            return closestIndex; //if distance increases because you're past the closest point, return the pose 
        }
    }

    return -1;
}

void Path::updateSubsystems(int index) {
    for (int i = 0; i < this->subsysRecordings.size(); i++) {
        this->config.subsysStates[i] = this->subsysRecordings[index][i];
    }
}

void Path::ramseteStep(int index) {

    float beta = this->config.beta;
    float zeta = this->config.zeta;

    lemlib::Pose target = this->pathRecordings[index]; 

    float targetX = target.x;
    float targetY = target.y;
    float targetTheta = target.theta;

    float linearVelTarget = this->velRecordings[index][0];
    float angularVelTarget = this->velRecordings[index][1];
    float errorLateral = findLateralError(targetX, targetY);
    float errorLongitudinal = findLongitudinalError(targetX, targetY);
    float errorTheta = findThetaError(targetTheta);

    float gain = 2 * zeta * std::sqrt(
        std::pow(angularVelTarget, 2) + (beta * std::pow(linearVelTarget, 2)));

    float linearVelCommand = (linearVelTarget * std::cos(errorTheta)) + (gain * errorLongitudinal);
    float angularVelCommand = angularVelTarget + (gain * errorTheta) + (
        beta * linearVelTarget * std::sin(errorTheta) * errorLateral / errorTheta
    );

    this->config.leftMotors.move_velocity(linearVelCommand + angularVelCommand); //TODO: here is moveVelocity and not moveVoltage 
    this->config.rightMotors.move_velocity(linearVelCommand - angularVelCommand);
}

void Path::follow() {
    int index = 0;

    while(true) {

        index = findClosestPoint(this->config.chassis.getPose(), index);

        if(index == -1) {
            break;
        }

        ramseteStep(index);
        updateSubsystems(index);

        pros::delay(10);
    }

    return;

}

}