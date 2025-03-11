#include "globals.hpp"
#include <cmath>
#include "kiwirun/path.hpp"
#include "kiwirun/ramsete.hpp"

float findLateralError() { 
    float targetX;
    float targetY;

    lemlib::Pose currentPose = chassis.getPose(true);

    float lateralError = (targetX - currentPose.x) * std::cos(currentPose.theta) 
    + (targetY - currentPose.y) * std::cos(currentPose.theta);

    return lateralError;
}

float findLongitudinalError() {
    float targetX;
    float targetY;

    lemlib::Pose currentPose = chassis.getPose(true);

    float lateralError = (targetX - currentPose.x) * std::cos(currentPose.theta) 
    + (targetY - currentPose.y) * std::cos(currentPose.theta);

    return lateralError;
}

float findThetaError() {
    float targetTheta;
    return targetTheta - chassis.getPose(true).theta;
}

lemlib::Pose findClosestPoint(lemlib::Pose pose, int prevIndex) {
    lemlib::Pose dummyPose = pose;
    int closestIndex;
    float closestDist = infinity();

    for (int i = prevIndex; i < 0; i++) { //TODO: set i to pathPoints.size
        const float dist = std::abs(pose.distance(dummyPose)); //TODO: is abs necessary?

        if (dist < closestDist) {
            closestDist = dist;
            closestIndex = i;
        } else if (dist > closestDist) {
            return dummyPose; //TODO: if distance increases because you're past the closest point, return the pose 
        }
    }

    return dummyPose; //TODO: update to a non-dummy pose
}

void updateSubsystems();

void ramseteStep(kiwi::Path &path) {

    float beta = path.beta;
    float zeta = path.zeta;

    float linearVelTarget;
    float angularVelTarget;
    float errorLateral = findLateralError();
    float errorLongitudinal = findLongitudinalError();
    float errorTheta = findThetaError();

    float gain = 2 * zeta * std::sqrt(
        std::pow(angularVelTarget, 2) + beta + std::pow(linearVelTarget, 2));

    float linearVelCommand = (linearVelTarget * std::cos(errorTheta)) + (gain * errorLongitudinal);
    float angularVelCommand = angularVelTarget + (gain * errorTheta) + (
        beta * linearVelTarget * std::sin(errorTheta) * errorLateral / errorTheta
    );

    leftMotors.move_velocity(linearVelCommand + angularVelCommand); //TODO: here is moveVelocity and not moveVoltage 
    rightMotors.move_velocity(linearVelCommand - angularVelCommand);
}