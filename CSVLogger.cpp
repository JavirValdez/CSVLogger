#include "CSVLogger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace FieldNames
{
    const int BUSINESS_STATE = 31;
    const int LAST_UPDATE_DATE = 37;
}
namespace
{
    const int NUMBER_OF_ALPHABET_LETTERS = 26;
}

CSVLogger::CSVLogger(std::string& csvPath, int numberOfFields):
    CSVPath(csvPath),
    NumberOfFields(numberOfFields),
    CSVStream(new std::ifstream(CSVPath))
{
}

CSVLogger::~CSVLogger()
{
}

void CSVLogger::SetNumberOfFields(int numberOfFields)
{
    NumberOfFields = numberOfFields;
}

int CSVLogger::GetNumberOfFields()
{
    return NumberOfFields;
}

void CSVLogger::ReadAndStorageCSV()
{
    if(!CSVStream->is_open())
    {
        std::cout << "ERROR: Unable to open " << CSVPath << std::endl;
    }
    else
    {
        std::cout << "Success opening file"  << std::endl;
        std::string line;
        std::getline(*CSVStream, line);
        int counter = 0;
        while(std::getline(*CSVStream, line) && counter < 100)
        {
            std::stringstream lineStream(line);
            std::string field;
            std::vector <std::string> fields;
            for(int currentField = 0; currentField < NumberOfFields && getline(lineStream, field, ','); currentField++)
            {
                field.erase(field.begin());
                field.erase(field.end()-1);
                fields.push_back(field);
            }
            FilterAndLogUserData(fields);
            counter++;
        }
        CSVStream->close();
    }

}

void CSVLogger::AddStateFilters(std::vector <std::string> stateFilter)
{
    StatesFilter = stateFilter;
}


void CSVLogger::FilterAndLogUserData(std::vector <std::string>& userData)
{
    for(const std::string& state: StatesFilter)
    {
        if(userData.at(FieldNames::BUSINESS_STATE) == state)
        {
            std::string nppes("NPPES:");
            std::string lastUpdateDate = userData.at(FieldNames::LAST_UPDATE_DATE);
            std::replace(lastUpdateDate.begin(), lastUpdateDate.end(), '/', ':');
            for(const std::string& field: userData)
            {
                std::string redisKey = nppes + lastUpdateDate + NPIHeaderName(&field - &userData.at(0)) + userData.at(0);

                std::cout << redisKey << ":    " << field << std::endl;
            }
        }
    }
}


std::string CSVLogger::NPIHeaderName(int headerNumber)
{
    std::string headerName(":COL");
    int overloaded = headerNumber / NUMBER_OF_ALPHABET_LETTERS;
    if(overloaded)
    {
        headerName += char('A' + overloaded - 1);
        headerName += char('A' + (headerNumber - (NUMBER_OF_ALPHABET_LETTERS * overloaded)));
    }
    else
    {
        headerName += char('A' + headerNumber);
    }
    headerName += ":";
    return headerName;
}
