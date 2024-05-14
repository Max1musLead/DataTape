#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "lib/tape.h"
#include "parser/parser.h"

const std::string inputFilename = "C:\\Users\\Maksim\\Desktop\\DataTape\\tmp\\input.bin";
const std::string outputFilename = "C:\\Users\\Maksim\\Desktop\\DataTape\\tmp\\output.bin";
const std::vector<std::string> tempFilenames = {"C:\\Users\\Maksim\\Desktop\\DataTape\\tmp\\tapetmp1.bin", "C:\\Users\\Maksim\\Desktop\\DataTape\\tmp\\tapetmp2.bin"};

const std::unordered_map<std::string, std::size_t> testConfig = {
    {"read_delay", 0},
    {"write_delay", 0},
    {"forward_delay", 0},
    {"backward_delay", 0}
};

void createTestFile(const std::string& filename, const std::vector<int>& data) {
    std::ofstream file(filename, std::ios::binary);
    for (int value : data) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    file.close();
}

TEST(FileTapeTest, WriteReadTest) {
    {    
        FileTape writeTape(inputFilename, testConfig, true);
        writeTape.write(10);
        writeTape.forward();
        writeTape.write(20);
        writeTape.forward();
        writeTape.write(30);
        writeTape.rewind();
    }

    FileTape readTape(inputFilename, testConfig);
    EXPECT_EQ(readTape.read(), 10);
    readTape.forward();
    EXPECT_EQ(readTape.read(), 20);
    readTape.forward();
    EXPECT_EQ(readTape.read(), 30);
}

TEST(TapeSorterTest, SortTest) {
    int MaxMemory = 8;
    createTestFile(inputFilename, {3, 1, 2});
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    sorter.sort(inputTape, outputTape, tempFilenames);

    outputTape.changeMode();
    outputTape.rewind();
    EXPECT_EQ(outputTape.read(), 1);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), 2);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), 3);
}

TEST(TapeSorterTest, SortIdenticalNumbersTest) {
    int MaxMemory = 8;
    createTestFile(inputFilename, {5, 5, 5});
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    sorter.sort(inputTape, outputTape, tempFilenames);

    outputTape.changeMode();
    outputTape.rewind();
    EXPECT_EQ(outputTape.read(), 5);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), 5);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), 5);
}

TEST(TapeSorterTest, SortNegativeNumbersTest) {
    int MaxMemory = 8;
    createTestFile(inputFilename, {-3, -1, -2});
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    sorter.sort(inputTape, outputTape, tempFilenames);

    outputTape.changeMode();
    outputTape.rewind();
    EXPECT_EQ(outputTape.read(), -3);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), -2);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), -1);
}

TEST(TapeSorterTest, SortSingleElementTest) {
    int MaxMemory = 8;
    createTestFile(inputFilename, {42});
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    sorter.sort(inputTape, outputTape, tempFilenames);

    outputTape.changeMode();
    outputTape.rewind();
    EXPECT_EQ(outputTape.read(), 42);
}

TEST(TapeSorterTest, SortRandomNumbersTest) {
    int MaxMemory = 20;
    std::vector<int> data = {10, 3, 5, 8, 2, 7, 6, 1, 9, 4};
    createTestFile(inputFilename, data);
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    sorter.sort(inputTape, outputTape, tempFilenames);

    outputTape.changeMode();
    outputTape.rewind();
    std::sort(data.begin(), data.end());
    for (int i = 0; i < data.size(); ++i) {
        EXPECT_EQ(outputTape.read(), data[i]);
        outputTape.forward();
    }
}

TEST(TapeSorterTest, SortLargeNumbersTest) {
    int MaxMemory = 10;
    createTestFile(inputFilename, {INT_MAX, INT_MIN, INT_MAX - 1, INT_MIN + 1});
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    sorter.sort(inputTape, outputTape, tempFilenames);

    outputTape.changeMode();
    outputTape.rewind();
    EXPECT_EQ(outputTape.read(), INT_MIN);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), INT_MIN + 1);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), INT_MAX - 1);
    outputTape.forward();
    EXPECT_EQ(outputTape.read(), INT_MAX);
}

TEST(TapeSorterTest, SortAlreadySortedTest) {
    int MaxMemory = 20;
    createTestFile(inputFilename, {1, 2, 3, 4, 5});
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    sorter.sort(inputTape, outputTape, tempFilenames);

    outputTape.changeMode();
    outputTape.rewind();
    for (int i = 1; i <= 5; ++i) {
        EXPECT_EQ(outputTape.read(), i);
        outputTape.forward();
    }
}

TEST(TapeSorterErrorTest, SortEmptyFileTest) {
    int MaxMemory = 8;
    createTestFile(inputFilename, {});
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    sorter.sort(inputTape, outputTape, tempFilenames);

    outputTape.changeMode();
    outputTape.rewind();
    EXPECT_THROW(outputTape.read(), std::runtime_error);
}

TEST(TapeSorterErrorTest, MemoryErrorTest) {
/// У нас доступно 2 диска по 15, а нужно 40 -> Bad_alloc ///
    int MaxMemory = 15;
    std::vector<int> data = {10, 3, 5, 8, 2, 7, 6, 1, 9, 4};
    createTestFile(inputFilename, data);
    
    FileTape inputTape(inputFilename, testConfig);
    FileTape outputTape(outputFilename, testConfig, true);
    
    TapeSorter sorter(MaxMemory);
    EXPECT_THROW(sorter.sort(inputTape, outputTape, tempFilenames), std::bad_alloc);
}
