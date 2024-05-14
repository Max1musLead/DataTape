#include "tape.h"

///// TAPE /////
void FileTape::changeMode() {
    if (writeMode) {
        writeMode = false;
    } else {
        writeMode = true;
    }
    
    file.close();

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

///// SORT /////
void TapeSorter::sort(FileTape& inputTape, FileTape& outputTape, const std::vector<std::string>& tempFilenames) {
    std::vector<std::string> tempFiles;
    std::vector<int> buffer;
    int tempFileIndex = 0;

    while (true) {
        buffer.clear();
        try {
            for (int i = 0; i < memoryLimit / sizeof(int); ++i) {
                buffer.push_back(inputTape.read());
                inputTape.forward();
            }
        } catch (const std::runtime_error&) {
            if (!buffer.empty()) {
                std::sort(buffer.begin(), buffer.end());
                std::string tempFilename = tempFilenames[tempFileIndex++];
                saveToFile(buffer, tempFilename);
                tempFiles.push_back(tempFilename);
            }
            break;
        }
        std::sort(buffer.begin(), buffer.end());
        std::string tempFilename = tempFilenames[tempFileIndex++];
        saveToFile(buffer, tempFilename);
        tempFiles.push_back(tempFilename);
    }

    mergeFiles(tempFiles, outputTape);
}

///// SORT /////
void TapeSorter::saveToFile(const std::vector<int>& data, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Cannot open temp file: " + filename);
    }
    for (int value : data) {
        outFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
}

///// SORT /////
void TapeSorter::mergeFiles(const std::vector<std::string>& tempFiles, FileTape& outputTape) {
    std::vector<std::ifstream> streams(tempFiles.size());
    std::vector<int> buffer(tempFiles.size());
    std::vector<bool> hasData(tempFiles.size(), true);

    for (size_t i = 0; i < tempFiles.size(); ++i) {
        streams[i].open(tempFiles[i], std::ios::binary);
        if (!streams[i].is_open()) {
            throw std::runtime_error("Cannot open temp file: " + tempFiles[i]);
        }
        if (!streams[i].read(reinterpret_cast<char*>(&buffer[i]), sizeof(buffer[i]))) {
            hasData[i] = false;
        }
    }

    while (true) {
        int minIndex = -1;
        for (size_t i = 0; i < buffer.size(); ++i) {
            if (hasData[i] && (minIndex == -1 || buffer[i] < buffer[minIndex])) {
                minIndex = i;
            }
        }
        if (minIndex == -1) {
            break;
        }
        outputTape.write(buffer[minIndex]);
        outputTape.forward();
        if (!streams[minIndex].read(reinterpret_cast<char*>(&buffer[minIndex]), sizeof(buffer[minIndex]))) {
            hasData[minIndex] = false;
        }
    }

    for (auto& stream : streams) {
        stream.close();
    }
}
