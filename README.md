The project is to develop a C++ program that will read the provider csv files, extract a specified number of fields, and load the data into a Redis database.

It is required the redox library to interact with the redis database:
https://github.com/hmartiro/redox

Once that is installed you can compile it with:
g++ main.cpp CSVLogger.cpp -std=c++11 -lredox -lev -lhiredis

You can also add the -D DEBUG mode when compiling to display the data that is being saved into the redis database:
g++ main.cpp CSVLogger.cpp -std=c++11 -lredox -lev -lhiredis -D DEBUG

Once the project is compiled you can run it with the command:
./a.out ../npidata_pfile_20050523-20190512.csv

Note: The program requires the path of the csv files to storage on the database, it can be 1 or multiple files, you will also need to limit the number of fields that you want to save into the database, that is done inside the function main in the constructor of the CSVLogger, and you will find there the function to filter the states to save.

