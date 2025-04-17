#include "kiwiRunObjects.hpp"

std::vector<std::reference_wrapper<float>> subsysStates;
std::vector<std::string> subsysNames;

kiwi::Config standardConfig(0, 0, 3, 4,
    subsysStates,
    subsysNames,
    chassis,
    drivetrain,
    leftMotors,
    rightMotors,
    controller
);

kiwi::Path path(standardConfig, "usd/path");