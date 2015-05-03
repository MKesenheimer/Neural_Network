#include <iostream>
#include <vector>
#include "Main.h"
#include "Brain.h"
#include "Trainer.h"
#include "Neuron.h"
#include "helper.h"

using namespace::std;

int main()
{
  cout << "==============================================\n";
  cout << "           Simple Neuronal Network            \n";
  cout << "==============================================\n\n";
  
  //contructing and init the Brain
  int nneurons = 5;
  int ninputs = 3;
  int noutputs = 1;
  
  Brain Jarvis(nneurons,ninputs,noutputs);
  
  Trainer TonyStark(&Jarvis);
  TonyStark.train();
  
  cout << "\n\n=> Testing the brain:\n";
  
  //init in- and output vectos
  vector<float> input;
  vector<float> output;
  
  //Test 1
  input.clear();
  input.push_back(1);
  input.push_back(0);
  input.push_back(0);
  for (int i=0; i<input.size(); i++) {
    cout << "\tJarvis input "<<i<<":\t\t"<< input[i] << "\n";
  }
  //calculate the output
  output = Jarvis.output(input);
  for (int i=0; i<output.size(); i++) {
    cout << "\tJarvis says  "<<i<<":\t\t"<<output[i] << "\n\n";
  }
  
  //Test 2
  input.clear();
  input.push_back(0);
  input.push_back(1);
  input.push_back(0);
  for (int i=0; i<input.size(); i++) {
    cout << "\tJarvis input "<<i<<":\t\t"<< input[i] << "\n";
  }
  //calculate the output
  output = Jarvis.output(input);
  for (int i=0; i<output.size(); i++) {
    cout << "\tJarvis says  "<<i<<":\t\t"<<output[i] << "\n\n";
  }
  
  //Test 3
  input.clear();
  input.push_back(0);
  input.push_back(0);
  input.push_back(1);
  for (int i=0; i<input.size(); i++) {
    cout << "\tJarvis input "<<i<<":\t\t"<< input[i] << "\n";
  }
  //calculate the output
  output = Jarvis.output(input);
  for (int i=0; i<output.size(); i++) {
    cout << "\tJarvis says  "<<i<<":\t\t"<<output[i] << "\n\n";
  }
  
  return 0;
}