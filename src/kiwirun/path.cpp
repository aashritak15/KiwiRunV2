#include "kiwirun/path.hpp"
#include <fstream>

namespace kiwi {

    Path::Path(kiwi::Config& config, std::string fileID):
    config(config), fileID(fileID){
        this->fetchInformation();
    };

    void Path::fetchInformation() {
        nlohmann::json json_master;

        std::ifstream file;
        file.open(this->fileID);
        
        file >> json_master;

        file.close();

        std::vector<std::vector<nlohmann::json>> json_segments = json_master["data"];
        //first vector: segments
        //second vector: data points of each segment
        //nlohmann json: single data points


        for (int i = 0; i < json_segments.size(); i++) { //loop through segments
            for(int j = 0; i < json_segments[i].size(); j++) { //loop through data points
                lemlib::Pose point(0, 0, 0); //find position information

                point.x = json_segments[i][j]["x pos"]; //TODO: check
                point.y = json_segments[i][j]["y pos"];
                point.theta = json_segments[i][j]["theta"];

                this->pathRecordings.push_back(point); //write position information

                std::vector<float> subsysInfoPoint; //find subsystem information

                for (int k = 0; k < this->config.subsysStates.size(); k++) {
                    subsysInfoPoint.push_back(json_segments[i][j][this->config.subsysNames[k]]);
                }

                this->subsysRecordings.push_back(subsysInfoPoint); //write subsystem information

                std::vector<float> velRecordingsPoint; //find velocity information

                velRecordingsPoint.push_back(json_segments[i][j]["linear vel"]);
                velRecordingsPoint.push_back(json_segments[i][j]["angular vel"]);

                velRecordings.push_back(velRecordingsPoint); //write velocity information
            } 
        }
    };

    void getSubsystems() {

    };

    void getVelocities() {

    };
    
}