#ifndef ADIVIC_CSV_PARSER_H
#define ADIVIC_CSV_PARSER_H
#include <string>
#include <vector>
#include <iosfwd>

//Strong-typed enum, now is support for VC11+
enum CsvReadStatus : unsigned int {
    CSV_READ_SUCESS,
    CSV_READ_FAIL,
    CSV_READ_BAD,
    CSV_END_OF_FILE
};

class CsvParser
{
public:
    CsvParser();
    virtual ~CsvParser();

public:
    bool read(const std::string &filePath);
    bool write(const std::string &filePath);
    std::vector<std::vector<std::string> > &rawData();
    virtual void parse();

private:
    CsvReadStatus readNextRow(std::istream &inputFileStream);

protected:
    std::vector<std::vector<std::string> > readBuffer;

private:
    char splitMark;
};

#endif