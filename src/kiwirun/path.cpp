#include "main.h"

namespace kiwi {

class path {
    public:
        std::string pathFileID;
        
        float beta;
        float zeta;

        lemlib::Chassis chassis;
        pros::MotorGroup leftMotors;
        pros::MotorGroup rightMotors;

        std::vector<int*> subsysStates; //points to all subsys
};

}