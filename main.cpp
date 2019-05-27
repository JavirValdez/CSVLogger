#include "CSVLogger.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc == 3)
    {
       std::string file = {argv[1]};
       std::string date = {argv[2]};
       CSVLogger firstReader(file, date, 38);
       firstReader.AddStateFilters({"SC"});
       firstReader.ReadAndStorageCSV();
    }
    else
    {
       std::cout << "Error: please add the CSV file and the date as an arguments" << std::endl;
    }

    return 0;
}
