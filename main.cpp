#include "CSVLogger.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
       std::string file = {argv[1]};
       CSVLogger firstReader(file, 38);
       firstReader.AddStateFilters({"CA", "OK", "VA"});
       firstReader.ReadAndStorageCSV();
    }
    else
    {
       std::cout << "No CSV file received, please add it as an argument" << std::endl;
    }

    return 0;
}
