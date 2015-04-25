#include <iostream>
#include <vector>
#include "Brain.h"

using namespace::std;

int main()
{
  cout << "====================================\n";
  cout << "=   Very simple Neuronal Network   =\n";
  cout << "====================================\n\n";
  
  //init Brain with 2 neuron(s), 6 inputs and 2 output
  //Brain jarvis(2, 6, 2);
  
  Brain jarvis(3,4,2);
  jarvis.initBrain();
  
  vector<float> input;
  input.push_back(1);
  input.push_back(2);
  input.push_back(3);
  input.push_back(4);

  for (int i=0; i<input.size(); i++) {
    cout << "Brain input " << input[i] << "\n";
  }

  //calculate the output
  vector<float> output;
  output = jarvis.output(input);
  
  #ifdef DEBUG
    cout << "Output size: "<< output.size() << "\n";
  #endif
  for (int i=0; i<output.size(); i++) {
    cout << "Jarvis says: " << output[i] << "\n";
  }
  
  return 0;
}
