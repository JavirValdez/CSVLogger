#pragma once
#include <string>
#include <vector>
#include <iostream>


class CSVLogger
{
public:
    CSVLogger(std::string& csvPath, int numberOfFields = 0);
    virtual ~CSVLogger();
    void SetNumberOfFields(int numberOfFields);
    int GetNumberOfFields();
    void ReadAndStorageCSV();
    void AddStateFilters(std::vector <std::string> stateFilter);

protected:

private:
    void FilterAndLogUserData(std::vector <std::string>& userData);
    std::string NPIHeaderName(int headerNumber);


    std::string& CSVPath;
    int NumberOfFields;
    std::ifstream* CSVStream;
    std::vector <std::string> StatesFilter;
};
