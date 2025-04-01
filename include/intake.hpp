#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "main.h"

inline pros::Motor intakeUpper(14, pros::MotorGearset::blue);
inline pros::Motor intakeLower(-13, pros::MotorGearset::green);

void intakeInit();
void updateIntake();
void runIntake();

extern float intakeState;

#endif