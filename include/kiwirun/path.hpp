#include "main.h"
#include "config.hpp"

namespace kiwi {

class Path {
    public:

        kiwi::Config& config;
        std::string fileID;

        std::vector<lemlib::Pose> pathRecordings;
        std::vector<std::vector<std::reference_wrapper<int>>> subsysRecordings; //vector (timesteps) of vectors (states at timestep) of reference states
        std::vector<std::vector<float>> velRecordings; //vector (timesteps) of vectors (angular/linear vel at timestep) of velocities

        void getPathPoints();
        void getSubsystems();
        void getVelocities();

        Path(kiwi::Config& config, std::string fileID);


};

}