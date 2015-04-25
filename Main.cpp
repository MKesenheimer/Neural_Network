#include <iostream>
#include <vector>
#include "Brain.h"
#include "Trainer.h"
#include "helper.h"

using namespace::std;

int main()
{
  cout << "====================================\n";
  cout << "=     Simple Neuronal Network      =\n";
  cout << "====================================\n\n";
  
  //contructing and init the Brain
  int nneurons = 2;
  int ninputs = 3;
  int noutputs = 1;
  
  Brain Jarvis(nneurons,ninputs,noutputs);
  Jarvis.initBrain();
  
  Trainer TonyStark;
  TonyStark.train(&Jarvis);
  
  vector<float> input;
  for (int i=0; i<ninputs; i++) {
    input.push_back(getRandomNumber());
  }

  for (int i=0; i<input.size(); i++) {
    cout << "Jarvis input "<<i<<": "<< input[i] << "\n";
  }

  //calculate the output
  vector<float> output;
  output = Jarvis.output(input);
  
  #ifdef DEBUG
    cout << "Output size: "<< output.size() << "\n";
  #endif
  for (int i=0; i<output.size(); i++) {
    cout << "Jarvis says: " << output[i] << "\n";
  }
  
  return 0;
}
