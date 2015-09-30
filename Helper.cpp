#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <limits>
#include "Helper.h"

long double getRandomNumber(){
  long double a = 1.0;
  long double x = (long double)rand()/(long double)(RAND_MAX/a);
  #ifdef DEBUGQ
    std::cout<<"Random number: "<<x<<"\n";
  #endif
  //return 1;
  return x;
}

long double norm(const std::vector<long double>& a) {
  long double accum = 0;
    for (int i = 0; i < a.size(); i++) {
        accum += a[i] * a[i];
    }
    return accum;
}

std::vector<long double> subtract(const std::vector<long double>& a, const std::vector<long double>& b) {
  std::vector<long double> temp;
  if (a.size() != b.size()) {
    std::cout<<"error in function subtract. Vector a and b are unequal in size.";
    exit(1);
  } else {
    for (int i=0; i<a.size(); i++) {
      temp.push_back(a[i]-b[i]);
    }
  }
  return temp;
}

long double vecsum(const std::vector<long double>& a) {
  long double temp = 0;
  for (int j=0; j<a.size(); j++) {
    temp += a[j];
  }
  return temp;
}

int deltaFct(int i, int j) {
  int temp;
  if (i==j) {
    temp = 1;
  } else {
    temp = 0;
  }
  return temp;
}
