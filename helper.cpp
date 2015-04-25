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