#ifndef HELPER_H
#define HELPER_H

  //get random number between 0 and 1
  float getRandomNumber();
  //calculate the norm of vector a
  float norm(const std::vector<float>& a);
  //subtract two vectos and return a-b
  std::vector<float> subtract(const std::vector<float>& a, const std::vector<float>& b);
  //sum of vector entries:
  float vecsum(const std::vector<float>& a);
  //Kronecker delta function
  int deltaFct(int i, int j);

#endif