#include <iostream>
#include <vector>
#include <sstream>
#include "helper.h"

float getRandomNumber(){
  float a = 1.0;
  float x = (float)rand()/(float)(RAND_MAX/a);
  #ifdef DEBUGQ
    std::cout<<"Random number: "<<x<<"\n";
  #endif
  return x;
}