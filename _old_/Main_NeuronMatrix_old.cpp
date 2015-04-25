#include <iostream>
#include <vector>
#include "Brain.h"

using namespace::std;

int main()
{
  cout << "====================================\n";
  cout << "=   Very simple Neuronal Network   =\n";
  cout << "====================================\n\n";
  
  cout << "Initializing Brain...\n";
  //init Brain with 1 layer(s) á 2 neuron(s)
  Brain jarvis(2,1);
  jarvis.initBrain();
  
  cout << "neuron(0,0) with index "<< jarvis.getNeuronNumber(0,0) << endl;
  cout << "neuron(0,1) with index "<< jarvis.getNeuronNumber(0,1) << endl;
  cout << "neuron(1,0) with index "<< jarvis.getNeuronNumber(1,0) << endl;
  cout << "neuron(1,1) with index "<< jarvis.getNeuronNumber(1,1) << endl;
  
  
  std::vector<float> input;
  input.push_back(2.0);
  input.push_back(0.3);
  input.push_back(0.4);

  //calculate the output
  std::vector<float> out;
  out = jarvis.output(input);
      
  cout << "Jarvis says: " << out[0] << "\n";
  
  return 0;
}
