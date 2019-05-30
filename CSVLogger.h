//#*******************************************************************************#
//#                                                                               #
//# FILE: CSVLogger.h                                                             #
//#                                                                               #
//# DESCRIPTION: This is a header file for the CSV logger app.                    #
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

#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace redox
{
    class Redox;
}

class CSVLogger
{
public:
    enum class RedisOpEnum
    {
        SET,
	DEL
    };

    CSVLogger(std::string& csvPath, unsigned int numberOfFields = 0, RedisOpEnum redisOperation = RedisOpEnum::SET);
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
    std::string Date;
    unsigned int NumberOfFields;
    std::ifstream* CSVStream;
    std::vector <std::string> StatesFilter;
    redox::Redox* Rdx;
    RedisOpEnum RedisOperation;
};
