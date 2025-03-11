#include "kiwirun/path.hpp"

namespace kiwi {

    Path::Path(kiwi::Config& config, std::string fileID):
    config(config), fileID(fileID){
        getPathPoints();
        getSubsystems();
        getVelocities();
    };

    void getPathPoints() { //TODO: complete

    };

    void getSubsystems() {

    };

    void getVelocities() {

    };
    
}