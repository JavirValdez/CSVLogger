//#*******************************************************************************#
//#                                                                               #
//# FILE: CSVLogger.cpp                                                           #
//#                                                                               #
//# DESCRIPTION: This is a source file for the CSV logger app, it contains the    #
//# the logic to read an decript the CSV files and log them into a redis          #
//# database.
//#                                                                               #
//# DEVELOPER: Javir Valdez                                                       #
//# DEVELOPER PHONE: +52 (644) 451-1441                                           #
//# DEVELOPER EMAIL: josejavirvaldez@gmail.com                                    #
//#                                                                               #
//# VERSION: 1.0                                                                  #
//# CREATED DATE-TIME: 20190529-07:00 Central Time Zone USA                       #
//#                                                                               #
//# VERSION: 1.1                                                                  #
//# REVISION DATE-TIME: YYYYMMDD-HH:MM                                            #
//# DEVELOPER MAKING CHANGE: First_name Last_name                                 #
//# DEVELOPER MAKING CHANGE: PHONE: +1 (XXX) XXX-XXXX                             #
//# DEVELOPER MAKING CHANGE: EMAIL: first.last@email.com                          #
//#                                                                               #
//#/* Copyright (C) EOS BlockSmith, LLC   - All Rights Reserved                   #
//# * Unauthorized copying of this file, via any medium is strictly prohibited    #
//# * Proprietary and confidential.                                               #
//# */                                                                            #
//#*******************************************************************************#

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


CSVLogger::CSVLogger(std::string& csvPath, unsigned int numberOfFields, RedisOpEnum redisOperation):
    CSVPath(csvPath),
    Date(csvPath.substr(csvPath.find("-") + 1, 8)), //extracts the date from the file name
    NumberOfFields(numberOfFields),
    CSVStream(new std::ifstream(CSVPath)),
    Rdx(new redox::Redox()),
    RedisOperation(redisOperation)
{
}

CSVLogger::~CSVLogger()
{
    delete CSVStream;
    delete Rdx;
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
    if(!CSVStream->is_open() || !Rdx->connect("localhost", 6379))
    {
        std::cout << "ERROR: Unable to open " << CSVPath << "or unable to open redis database" << std::endl;
    }
    else
    {
        std::cout << "Success opening file " << CSVPath << std::endl;
        std::string line;
        //This getline gets rid of the header title names
        std::getline(*CSVStream, line);
	
        //While there are still lines in the CSV file
        while(std::getline(*CSVStream, line))
        {
            FilterAndLogUserData(line);
        }
        Rdx->disconnect();
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
                //Gets the key and the value for each field
    	        for(const std::string& field: userData)
                {
		    //loads only the fields with data
		    if(field.length())
		    {
                        std::string redisKey = nppes + Date + NPIHeaderName(&field - &userData.at(0)) + userData.at(0);

                        if(RedisOpEnum::SET == RedisOperation)
			{
		        //    Rdx->set(redisKey, field);
			}
			else if(RedisOpEnum::DEL == RedisOperation)
			{
			//    Rdx->del(redisKey);
			}
			DEBUG_MSG(redisKey << ' ' << field);
		    }
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

