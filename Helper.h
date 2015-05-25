#ifndef HELPER_H
#define HELPER_H

  //get random number between 0 and 1
  long double getRandomNumber();
  //calculate the norm of vector a
  long double norm(const std::vector<long double>& a);
  //subtract two vectos and return a-b
  std::vector<long double> subtract(const std::vector<long double>& a, const std::vector<long double>& b);
  //sum of vector entries:
  long double vecsum(const std::vector<long double>& a);
  //Kronecker delta function
  int deltaFct(int i, int j);

#endif
