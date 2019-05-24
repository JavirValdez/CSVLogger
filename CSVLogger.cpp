#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#include "CSVLogger.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <redox.hpp>

namespace FieldNames
{
    const int BUSINESS_STATE = 31;
    const int LAST_UPDATE_DATE = 37;
    const int NUMBER_OF_ALPHABET_LETTERS = 26;
}

using namespace redox;

CSVLogger::CSVLogger(std::string& csvPath, std::string& date, unsigned int numberOfFields):
    CSVPath(csvPath),
    Date(date),
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
        //This getline gets rid of the header title names
        std::getline(*CSVStream, line);
        //While there are still lines in the CSV file
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
		Redox rdx;
                if(!rdx.connect("localhost", 6379)) 
		{
                    DEBUG_MSG("Unable to open redis database");
		    break;
		}
		
		std::string nppes("NPPES:");
                //std::string lastUpdateDate = userData.at(FieldNames::LAST_UPDATE_DATE);
                //std::replace(lastUpdateDate.begin(), lastUpdateDate.end(), '/', ':');
                //Gets the key and the value for each field
    	        for(const std::string& field: userData)
                {
                    std::string redisKey = nppes + Date + NPIHeaderName(&field - &userData.at(0)) + userData.at(0);
                    
		    rdx.set(redisKey, field);
		    DEBUG_MSG(redisKey << ' ' << field);
                }
		rdx.disconnect();
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

