#include "lib/tape.h"
#include "parser/parser.h"

int main(int argc, char** argv) {
    const int MaxMemory = 8;
    const std::filesystem::path ConfigPath = "C:\\Users\\Maksim\\Desktop\\DataTape\\config.txt";

    ArgumentParser parser;
    if (!parser.Parse(argc, argv)){
        exit(EXIT_FAILURE);
    }

    auto config = parser.readConfig(ConfigPath);
    
    try {
        FileTape inputTape(parser.getInputTape(), config);
        FileTape outputTape(parser.getOutputTape(), config, true);

        TapeSorter sorter(MaxMemory);
        sorter.sort(inputTape, outputTape, parser.getTempTapeFilenames());
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
}