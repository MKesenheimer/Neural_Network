#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <fstream>
#include <vector>

class Interface {
  public:
    Interface(std::string strname); //default constructor
    ~Interface();

    //read data from file
    long double readInput(std::string search);
    //read the data set from file
    std::vector<long double> readData(std::string set);
    std::vector< std::vector<long double> >  readDataSets();
  
  private:
    std::string filename;

};

#endif