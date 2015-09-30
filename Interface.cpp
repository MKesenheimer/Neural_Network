#include <iostream>
#include <fstream>
#include <sstream>
#include "Interface.h"

Interface::Interface(std::string strname) {
  filename = strname;
}

Interface::~Interface() {

}

long double Interface::readInput(std::string search) {
  std::vector<long double> out;
  out = readData(search);
  
  return out[0];
}

std::vector<long double> Interface::readData(std::string set) {
  std::fstream file(filename);
  
  file.clear();
  file.seekg(0, std::ios::beg);
  
  std::string variable;
  std::vector<long double> out;
  out.clear();
  while(file.good()) //loop over lines
  {
    std::string firstline;
    std::getline(file, firstline);
    std::stringstream ss(firstline);
    std::string word;
    //std::cout<<firstline<<std::endl;
    std::getline(ss,word,' ');
    if(word.compare(set) == 0) {
      //std::cout<<word<<std::endl;
      while(std::getline(ss,variable,' ')) {
        std::string::size_type sz;
        long double tmp = std::stod(variable,&sz); //string to double
        out.push_back(tmp);
      }
    }
  }
  
  file.close();
  
#ifdef DEBUGV2
  for (int i=0; i<out.size(); i++) {
    std::cout<<out[i]<<std::endl;
  }
#endif
  
  return out;
}

std::vector< std::vector<long double> > Interface::readDataSets() {
  std::vector< std::vector<long double> > tmp;
  
  int nsets = readInput("nsets");
  for (int i = 1; i <= nsets; i++) {
    std::stringstream ssin, ssout;
    ssin << "set" << i << "_in";
    ssout << "set" << i << "_out";
    tmp.push_back(readData(ssin.str()));
    tmp.push_back(readData(ssout.str()));
  }

  return tmp;
}
