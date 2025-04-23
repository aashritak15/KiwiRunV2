#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "main.h"
#include "globals.hpp"
#include "pros/distance.hpp"
#include <cctype>

inline pros::Motor firstStage(-11, pros::MotorGearset::blue);
inline pros::Motor secondStage(12, pros::MotorGearset::blue);

inline pros::Optical optical(4); //TODO: add port
inline pros::Distance distance(21); //TODO: add port

void intakeInit();

void updateIntake();
void runIntake();
void colorSort();

extern float intakeState;

#endif