#include "CSVLogger.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace FieldNames
{
    const int BUSINESS_STATE = 31;
    const int LAST_UPDATE_DATE = 37;
    const int NUMBER_OF_ALPHABET_LETTERS = 26;
}

CSVLogger::CSVLogger(std::string& csvPath, unsigned int numberOfFields):
    CSVPath(csvPath),
    NumberOfFields(numberOfFields),
    CSVStream(new std::ifstream(CSVPath))
{
}

CSVLogger::~CSVLogger()
{
	delete CSVStream;
}

void CSVLogger::SetNumberOfFields(unsigned int numberOfFields)
{
    NumberOfFields = numberOfFields;
}

int CSVLogger::GetNumberOfFields() const
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
        //this getline gets rid of the header title names
	    std::getline(*CSVStream, line);
        while(std::getline(*CSVStream, line))
        {
            FilterAndLogUserData(line);
        }
        CSVStream->close();
    }

}

void CSVLogger::AddStateFilters(std::vector <std::string> stateFilter)
{
    StatesFilter = stateFilter;
}

void CSVLogger::FilterAndLogUserData(std::string& line)
{
	std::vector<std::string> userData;

	//Extract the fields from the line

	//First field starts at 1 to avoid the first quote mark
	unsigned int fieldStart = 1;
	//Looks for every iteration of "," to detect each field and saves it on the vector
	for(unsigned int start = 0; start < line.length() && userData.size() < NumberOfFields; start++)
    {
        if(line[start] == '\"' && line[start + 1] == ',' && line[start + 2] == '\"')
        {
            userData.push_back(line.substr(fieldStart, start-fieldStart));
            start += 2;
            fieldStart = start + 1;
        }
    }

    //Check that the userData vector is the correct size otherwise it may have a segmentation fault
    if(userData.size() == NumberOfFields)
    {
        for(const std::string& state: StatesFilter)
        {
	    //Applies the states filter
            if(userData.at(FieldNames::BUSINESS_STATE) == state)
            {
                std::string nppes("NPPES:");
                //std::string lastUpdateDate = userData.at(FieldNames::LAST_UPDATE_DATE);
                //std::replace(lastUpdateDate.begin(), lastUpdateDate.end(), '/', ':');
                //Gets the key and the value for each field
    	        for(const std::string& field: userData)
                {
                    std::string redisKey = nppes + NPIHeaderName(&field - &userData.at(0)) + userData.at(0);

                  //  std::cout << redisKey << ":    " << field << std::endl;
                }
            }
        }
    }
}

std::string CSVLogger::NPIHeaderName(int headerNumber)
{
    //Generates an abstraction of each field name to match with:
    //0 -> :COLA:
    //1 -> :COLB:
    //...
    //25 -> :COLZ:
    //26 -> :COLAA:
    //...
    std::string headerName(":COL");
    int overloaded = headerNumber / FieldNames::NUMBER_OF_ALPHABET_LETTERS;
    if(overloaded)
    {
        headerName += char('A' + overloaded - 1);
        headerName += char('A' + (headerNumber - (FieldNames::NUMBER_OF_ALPHABET_LETTERS * overloaded)));
    }
    else
    {
        headerName += char('A' + headerNumber);
    }
    headerName += ":";
    return headerName;
}

