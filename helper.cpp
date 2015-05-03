#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>
#include "helper.h"

float getRandomNumber(){
  float a = 1.0;
  float x = (float)rand()/(float)(RAND_MAX/a);
  #ifdef DEBUGQ
    std::cout<<"Random number: "<<x<<"\n";
  #endif
  //return 1;
  return x;
}

float norm(const std::vector<float>& a) {
  float accum = 0;
    for (int i = 0; i < a.size(); i++) {
        accum += a[i] * a[i];
    }
    return accum;
}

std::vector<float> subtract(const std::vector<float>& a, const std::vector<float>& b) {
  std::vector<float> temp;
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

float vecsum(const std::vector<float>& a) {
  float temp = 0;
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