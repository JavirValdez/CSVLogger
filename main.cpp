#include "CSVLogger.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    string file("../npidata_pfile_20050523-20190512.csv");
    CSVLogger firstReader(file, 38);
    firstReader.AddStateFilters({"CA", "OK", "VA"});
    firstReader.ReadAndStorageCSV();

    return 0;

}
