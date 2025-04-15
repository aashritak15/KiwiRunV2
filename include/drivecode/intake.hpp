#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "main.h"
#include "globals.hpp"

inline pros::Motor firstStage(8, pros::MotorGearset::blue);
inline pros::Motor secondStage(11, pros::MotorGearset::blue);

// inline pros::Optical optical(16);

void intakeInit();

void updateIntake();
void runIntake();

extern float intakeState;

#endif