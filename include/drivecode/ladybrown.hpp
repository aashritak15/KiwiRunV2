#ifndef LADYBROWN_HPP
#define LADYBROWN_HPP

#include "main.h"
#include "globals.hpp"

inline pros::Motor ladyBrown(-17, pros::MotorGearset::red);

void ladyBrownInit();
void runLB();

#endif