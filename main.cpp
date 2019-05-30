//#*******************************************************************************#
//#                                                                               #
//# FILE: main.cpp                                                                #
//#                                                                               #
//# USAGE: main.cpp [-h]                                                          #
//#                                                                               #
//# DESCRIPTION: The project is to develop a C++ program that will read the       #
//# provider csv file, extract a specified number of fields, and load the data    #
//# into a Redis database.                                                        #
//#                                                                               #
//# OPTIONS: Each parameter will be used as a CSV file path to storage into the   #
//# database.				                                          #
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

#include "CSVLogger.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
       if("-d" == argv[1])
       {
           for(int numberOfFiles = 2; numberOfFiles < argc; numberOfFiles++)
           { 
               std::string file = argv[numberOfFiles];
               CSVLogger firstReader(file, 38, CSVLogger::RedisOpEnum::DEL);
               firstReader.AddStateFilters({"SC"});
               firstReader.ReadAndStorageCSV();
           }
       }
       else
       {
           //it reads and storage each CSV file passed as parameter
           for(int numberOfFiles = 1; numberOfFiles < argc; numberOfFiles++)
           { 
               std::string file = argv[numberOfFiles];
               CSVLogger firstReader(file, 38);
               firstReader.AddStateFilters({"SC"});
               firstReader.ReadAndStorageCSV();
           }
       }
    }
    else
    {
       std::cout << "Error: please add the CSV files as arguments" << std::endl;
    }

    return 0;
}
