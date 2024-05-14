#pragma once

#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>

class ArgumentParser {
public:
    bool Parse(int argc, char** argv);

    std::unordered_map<std::string, std::size_t> readConfig(const std::filesystem::path& filename);

    std::string getInputTape();
    std::string getOutputTape();
    std::vector<std::string> getTempTapeFilenames();

private:
    std::string inputTape;
    std::string outputTape;
    std::vector<std::string> tempTapeFilenames;
};
