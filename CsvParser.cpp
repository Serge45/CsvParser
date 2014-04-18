#include "CsvParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

CsvParser::CsvParser()
    : splitMark(',') {

}

CsvParser::~CsvParser() {
    
}

bool CsvParser::read(const std::string &filePath) {
    std::ifstream fileReader(filePath);

    if (!fileReader.is_open()) {
        return false;
    }

    readBuffer.clear();

    CsvReadStatus readStatus;

    bool isSuccessful = false;

    while (true) {
        readStatus = readNextRow(fileReader);
        if (readStatus == CsvReadStatus::CSV_END_OF_FILE) {
            isSuccessful = true;
            break;
        } else if ((readStatus == CsvReadStatus::CSV_READ_FAIL) ||
                   (readStatus == CsvReadStatus::CSV_READ_BAD)) {
            isSuccessful = false;
            break;
        }
    }

    fileReader.close();
    return isSuccessful;
}

/*Check rdstate on each line only, not each cell.*/
bool CsvParser::write(const std::string &filePath) {
    std::ofstream outputFileStream(filePath);

    if (!outputFileStream.is_open()) {
        return false;
    }

    std::ofstream::iostate writeState = std::ofstream::goodbit;

    for (auto line = readBuffer.cbegin(); line != readBuffer.cend(); ++line) {
        for (auto cell = line->cbegin(); cell != line->cend(); ++cell) {
            outputFileStream << *cell;
            if (cell != line->cend() - 1) {
                outputFileStream << splitMark;
            }
        }
        writeState = (outputFileStream << std::endl).rdstate();

        if ((writeState | std::ofstream::badbit) ||
            (writeState | std::ofstream::failbit)) {
            outputFileStream.close();
            return false;
        }
    }
    outputFileStream.close();
    return true;
}

std::vector<std::vector<std::string> > &CsvParser::rawData() {
    return readBuffer;
}

CsvReadStatus CsvParser::readNextRow(std::istream &inputFileStream) {
    std::string line;

    if (!std::getline(inputFileStream, line)) {
        std::ios::iostate readStatus = inputFileStream.rdstate();
        
        if (readStatus & std::istream::eofbit) {
            return CsvReadStatus::CSV_END_OF_FILE; 
        }

        if (readStatus & std::istream::failbit) {
            return CsvReadStatus::CSV_READ_FAIL;
        }

        if (readStatus & std::istream::badbit) {
            return CsvReadStatus::CSV_READ_BAD;
        }
        return CsvReadStatus::CSV_READ_FAIL;
    }

    std::stringstream lineStream(line);
    
    std::string cell;
    std::vector<std::string> result;

    while (getline(lineStream, cell, splitMark)) {
        result.push_back(cell);
    }

    readBuffer.push_back(result);

    return CsvReadStatus::CSV_READ_SUCESS;
}

void CsvParser::parse() {
    std::cerr << "Abstract method called!" << std::endl;
    assert(false);
}