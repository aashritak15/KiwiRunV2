#include <cmath>
#include "kiwirun/includes.hpp"

namespace kiwi {

float Path::findLateralError(float targetX, float targetY) { //lateral error

    lemlib::Pose currentPose = this->config.chassis.getPose(true);

    float deltaX = targetX - currentPose.x;
    float deltaY = targetY - currentPose.y;

    return -deltaX * sin(currentPose.theta) + deltaY * cos(currentPose.theta);
}

float Path::findLongitudinalError(float targetX, float targetY) { //longitudinal error

    lemlib::Pose currentPose = this->config.chassis.getPose(true);

    float deltaX = targetX - currentPose.x;
    float deltaY = targetY - currentPose.y;

    return deltaX * cos(currentPose.theta) + deltaY * sin(currentPose.theta);
}

float Path::findThetaError(float targetTheta) { //theta error with angle jump sanitation
    float currentTheta = this->config.chassis.getPose(true).theta;
    float delta = fmod((targetTheta - currentTheta + M_PI), 2*M_PI) - M_PI;
    return (delta == -M_PI) ? M_PI : delta;
}

int Path::findClosestPoint(lemlib::Pose pose, int prevIndex) {
    int closestIndex = prevIndex + 1;
    float closestDist = infinity();

    for (int i = prevIndex + 1; i < this->pathRecordings.size(); i++) { 
        //loop starting at ONE FORWARD THE PREVIOUSLY CLOSEST POINT! SKIP / STOP TOLERANCE!
        //and ending at the end of the path

        const float dist = std::abs(pose.distance(pathRecordings[i])); //distance to pose

        if (dist < closestDist) { //if tested point is closer, record it and move on
            closestDist = dist;
            closestIndex = i;
        } else if (dist > closestDist) {
            prevIndex = closestIndex;
            return closestIndex; //if distance increases because you're past the closest point, return the closest pose
        }
    }

    return -1; //you're screwed
}

void Path::updateSubsystems(int index) {
    for (int i = 0; i < this->subsysRecordings.size(); i++) {
        this->config.subsysStates[i] = this->subsysRecordings[index][i]; //loop through all subsystem states and update
    }
}

void Path::ramseteStep(int index) {

    float beta = this->config.beta; //beta and zeta values fetched
    float zeta = this->config.zeta;

    lemlib::Pose target = this->pathRecordings[index]; //target pose fetched

    float targetX = target.x; //target pose data fetched
    float targetY = target.y;
    float targetTheta = target.theta;

    float linearVelTarget = this->velRecordings[index][0]; //target velocities fetched
    float angularVelTarget = this->velRecordings[index][1];

    float errorLateral = findLateralError(targetX, targetY); //lateral or crosstrack error calculated
    float errorLongitudinal = findLongitudinalError(targetX, targetY); //longitudinal or front/back error calculated
    float errorTheta = findThetaError(targetTheta); //angular error calculated

    float gain = 2 * zeta * std::sqrt(
        std::pow(angularVelTarget, 2) + (beta * std::pow(linearVelTarget, 2))
    ); //master gain calculated with: sqrt[(angular velocity)^2 + beta * (linear velocity)^2]

    float linearVelCommand = (linearVelTarget * std::cos(errorTheta)) + (gain * errorLongitudinal);
        //linear command calculated with: [linear velocity target * cos(angle error)] + (gain * longitudinal error)
    float angularVelCommand = angularVelTarget + (gain * errorTheta) + (
        beta * linearVelTarget * std::sin(errorTheta) * errorLateral / errorTheta
    );
        //angular command calculated with: beta * linear velocity target * sin(angle error) * lateral error / angle error

    this->config.leftMotors.move_velocity(linearVelCommand + angularVelCommand); //velocity sent to motors 
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