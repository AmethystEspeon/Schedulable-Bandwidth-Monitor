#include "Includes/nlohmann/json.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>

using json = nlohmann::json;

namespace ns {
    struct bandwidthData {
        std::string time;
        int download;
        int upload;
    };

    void from_json(const json& j, bandwidthData& b) {
        j.at("timestamp").get_to(b.time);
        j.at("download").at("bandwidth").get_to(b.download);
        j.at("upload").at("bandwidth").get_to(b.upload);
    }
} // namespace ns

std::vector<ns::bandwidthData> getNewData(std::string);
void writeCSV(std::vector<ns::bandwidthData>);
void checkIfDelete();
std::string mainFileName = "SpeedTestCSV.csv";
std::string newDataFileName = "SpeedTestResults.json";


int main() {
    //open new data file and grab all stuff as a vector of tuples of <'time', 'download', 'upload'>
    std::vector<ns::bandwidthData> mainData = getNewData(newDataFileName);
    //output data to the CSV
    writeCSV(mainData);
    checkIfDelete();
    return 0;
}

std::vector<ns::bandwidthData> getNewData(std::string fileName) {
    std::vector<ns::bandwidthData> dataVector;
    std::ifstream file(fileName);
    std::string line;
    json jsonSingle;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            jsonSingle = json::parse(line);
            ns::bandwidthData newDataPoint;
            ns::from_json(jsonSingle, newDataPoint);
            dataVector.push_back(newDataPoint);
        }

        file.close();
    }

    return dataVector;
}

void writeCSV(std::vector<ns::bandwidthData> newData) {
    std::ifstream fileCheck(mainFileName, std::ios::in | std::ios::_Nocreate);
    std::fstream file;
    //Check if there's already a CSV. If there isn't, create the headers.
    bool newFile = false;
    if (!fileCheck.is_open()) {
        newFile = true;
    }
    else {
        fileCheck.close();
    }
    if (newFile) {
        file.open(mainFileName, std::ios::out);
        file << "Time, Download, Upload\n";
        file.close();
    }

    file.open(mainFileName, std::ios::out | std::ios::app);
    for (auto it : newData) {
        file << it.time << ", "
            << it.download << ", "
            << it.upload << "\n";
    }
    file.close();

    return;
}

void checkIfDelete() {
    std::string input;
    std::cout << "Do you want to delete the original JSON file? Duplicate entries into the CSV file could be created otherwise. (Y/N)\n";
    std::cin >> input;
    while(input != "Y" && input != "y" && input != "N" && input != "n") {
        std::cout << "Unsupported Answer.\n";
        std::cout << "Do you want to delete the original JSON file? Duplicate entries into the CSV file could be created otherwise. (Y/N)\n";
        std::cin >> input;
    }
    if (input == "Y" || input == "y") {
        std::remove(newDataFileName.c_str());
    }
    return;
}