#include "kiwirun/path.hpp"
#include <fstream>

namespace kiwi {

    Path::Path(kiwi::Config& config, std::string fileID):
    config(config), fileID(fileID){
        this->fetchData();
    };

    void Path::fetchData() {
        nlohmann::json master;

        std::ifstream file(this->fileID);
        if (!file.is_open()) throw std::runtime_error("Unable to open file: " + this->fileID);

        file >> master;
        file.close();

        for (const auto& jsonPoint : master) {
            lemlib::Pose point(0, 0, 0);
            point.x = jsonPoint["x pos"];
            point.y = jsonPoint["y pos"];
            point.theta = jsonPoint["theta"];
            this->pathRecordings.push_back(point);

            std::vector<float> subsysInfoPoint;
            for (size_t k = 0; k < this->config.subsysStates.size(); k++) {
                subsysInfoPoint.push_back(jsonPoint[this->config.subsysNames[k]]);
            }
            this->subsysRecordings.push_back(subsysInfoPoint);

            std::vector<float> velRecordingsPoint;
            velRecordingsPoint.push_back(jsonPoint["linear vel"]);
            velRecordingsPoint.push_back(jsonPoint["angular vel"]);
            velRecordings.push_back(velRecordingsPoint);
        }
    };
}