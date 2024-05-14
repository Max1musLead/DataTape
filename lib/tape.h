#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>
#include <unordered_map>
#include <queue>

class FileTape {
public:
    FileTape(const std::string& filename, const std::unordered_map<std::string, std::size_t>& config, bool writeMode = false)
        : filename(filename), current(0), writeMode(writeMode), readDelay(config.at("read_delay")), writeDelay(config.at("write_delay")),
          forwardDelay(config.at("forward_delay")), backwardDelay(config.at("backward_delay")) {
        
        if (writeMode) {
            file.open(filename, std::ios::out | std::ios::binary);
            if (!file) {
                throw std::runtime_error("Cannot open file: " + filename);
            }
        } else {
            file.open(filename, std::ios::in | std::ios::binary);
            if (!file) {
                throw std::runtime_error("Cannot open file: " + filename);
            }
        }
    }

    int read() {
        Delay(readDelay);
        int value;
        file.seekg(current * sizeof(int), std::ios::beg);
        if (!file.read(reinterpret_cast<char*>(&value), sizeof(int))) {
            throw std::runtime_error("End of tape reached");
        }
        return value;
    }

    void write(int value) {
        if (!writeMode) {
            throw std::runtime_error("Tape is not in write mode");
        }
        Delay(writeDelay);
        file.seekp(current * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<const char*>(&value), sizeof(int));
    }

    void forward() {
        Delay(forwardDelay);
        ++current;
    }

    void backward() {
        Delay(backwardDelay);
        if (current > 0) {
            --current;
        }
    }

    void changeMode();

    void rewind() {
        current = 0;
    }

    ~FileTape() {
        file.close();
    }

private:
    std::fstream file;
    std::string filename;
    int current;
    bool writeMode;
    const std::size_t readDelay;
    const std::size_t writeDelay;
    const std::size_t forwardDelay;
    const std::size_t backwardDelay;

    void Delay(std::size_t delay) const {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
};

class TapeSorter {
public:
    TapeSorter(int memoryLimit) : memoryLimit(memoryLimit) {}

    void sort(FileTape& inputTape, FileTape& outputTape, const std::vector<std::string>& tempFilenames);

private:
    int memoryLimit;

    void saveToFile(const std::vector<int>& data, const std::string& filename);

    void mergeFiles(const std::vector<std::string>& tempFiles, FileTape& outputTape);
};
