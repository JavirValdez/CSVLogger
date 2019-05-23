#pragma once
#include <string>
#include <vector>
#include <iostream>


class CSVLogger
{
public:
    CSVLogger(std::string& csvPath, unsigned int numberOfFields = 0);
    virtual ~CSVLogger();
    //Number of fields that are going to be saved into the database
    //Current Min number is 38
    void SetNumberOfFields(unsigned int numberOfFields);
    //Number of fields that are going to be saved into the database
    int GetNumberOfFields() const;
    //reads the passed file and writes into the redis database
    void ReadAndStorageCSV();
    //creates a filter to save only the selected states on StatesFilters
    void AddStateFilters(std::vector <std::string> stateFilter);

protected:

private:
    void FilterAndLogUserData(std::string& line);
    std::string NPIHeaderName(int headerNumber);

    std::string& CSVPath;
    unsigned int NumberOfFields;
    std::ifstream* CSVStream;
    std::vector <std::string> StatesFilter;
};
