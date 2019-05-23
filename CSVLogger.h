#pragma once
#include <string>
#include <vector>
#include <iostream>


class CSVLogger
{
public:
    CSVLogger(std::string& csvPath, int numberOfFields = 0);
    virtual ~CSVLogger();
    //Number of fields that are going to be saved into the database
    //Current Min number is 38
    void SetNumberOfFields(int numberOfFields);
    //Number of fields that are going to be saved into the database
    int GetNumberOfFields() const;
    //reads the passed file and writes into the redis database
    void ReadAndStorageCSV();
    //creates a filter to save only the selected states on StatesFilters
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
