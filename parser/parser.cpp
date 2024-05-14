#include "parser.h"

bool ArgumentParser::Parse(int argc, char** argv) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }
    
    for (int i = 0; i < args.size(); i++) {
        if (args[i] == "-i" || args[i] == "--input") {
            inputTape = args[++i];
        } else if (args[i] == "-o" || args[i] == "--output") {
            outputTape = args[++i];
        } else {
            tempTapeFilenames.push_back(args[i]);
        }
    }

    return true;
}

std::unordered_map<std::string, std::size_t> ArgumentParser::readConfig(const std::filesystem::path& filename) {
    std::unordered_map<std::string, std::size_t> config;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filename.string());
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('=');
        std::string key = line.substr(0, delimiterPos);
        std::size_t value = std::stoi(line.substr(delimiterPos + 1));
        config[key] = value;
    }

    return config;
}

std::string ArgumentParser::getInputTape() {
    return inputTape;
}

std::string ArgumentParser::getOutputTape() {
    return outputTape;
}

std::vector<std::string> ArgumentParser::getTempTapeFilenames() {
    return tempTapeFilenames;
}
