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
  
  //the number of neurons of the brain
  int nneurons = 5;
  int ninputs = 4;
  int noutputs = 2;
  
  //constructing and init the brain
  Brain Jarvis(nneurons,ninputs,noutputs);
  //constructing the trainer and telling him which brain he should train
  Trainer TonyStark(&Jarvis);
  
  //store the demanded input and the desired output
  std::vector<float> demandedInput;
  std::vector<float> desiredOutput;
  
  //set up the dataset
  //Set 1
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(1);
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  desiredOutput.push_back(0);
  desiredOutput.push_back(0);
  TonyStark.addDataSet(demandedInput,desiredOutput);
  //Set 2
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(0);
  demandedInput.push_back(1);
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  desiredOutput.push_back(1);
  desiredOutput.push_back(0);
  TonyStark.addDataSet(demandedInput,desiredOutput);
  //Set 3
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  demandedInput.push_back(1);
  demandedInput.push_back(0);
  desiredOutput.push_back(0);
  desiredOutput.push_back(1);
  TonyStark.addDataSet(demandedInput,desiredOutput);
  //Set 4
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  demandedInput.push_back(1);
  desiredOutput.push_back(1);
  desiredOutput.push_back(1);
  TonyStark.addDataSet(demandedInput,desiredOutput);
  
  //now all the data is provided, train the brain
  TonyStark.train();
  
  cout << "\n\n=> Testing the brain:\n";
  
  //init in- and output vectors
  vector<float> input;
  vector<float> output;
  
  //Test 1
  input.clear();
  input.push_back(1);
  input.push_back(0);
  input.push_back(0);
  input.push_back(0);
  for (int i=0; i<input.size(); i++) {
    cout << "\tJarvis input "<<i<<":\t\t"<< input[i] << "\n";
  }
  //calculate the output
  output = Jarvis.output(input);
  for (int i=0; i<output.size(); i++) {
    cout << "\tJarvis says  "<<i<<":\t\t"<<output[i] << "\n";
  }
  cout<<endl;
  
  //Test 2
  input.clear();
  input.push_back(0);
  input.push_back(1);
  input.push_back(0);
  input.push_back(0);
  for (int i=0; i<input.size(); i++) {
    cout << "\tJarvis input "<<i<<":\t\t"<< input[i] << "\n";
  }
  //calculate the output
  output = Jarvis.output(input);
  for (int i=0; i<output.size(); i++) {
    cout << "\tJarvis says  "<<i<<":\t\t"<<output[i] << "\n";
  }
  cout<<endl;
  
  //Test 3
  input.clear();
  input.push_back(0);
  input.push_back(0);
  input.push_back(1);
  input.push_back(0);
  for (int i=0; i<input.size(); i++) {
    cout << "\tJarvis input "<<i<<":\t\t"<< input[i] << "\n";
  }
  //calculate the output
  output = Jarvis.output(input);
  for (int i=0; i<output.size(); i++) {
    cout << "\tJarvis says  "<<i<<":\t\t"<<output[i] << "\n";
  }
  cout<<endl;
  
  //Test 4
  input.clear();
  input.push_back(0);
  input.push_back(0);
  input.push_back(0);
  input.push_back(1);
  for (int i=0; i<input.size(); i++) {
    cout << "\tJarvis input "<<i<<":\t\t"<< input[i] << "\n";
  }
  //calculate the output
  output = Jarvis.output(input);
  for (int i=0; i<output.size(); i++) {
    cout << "\tJarvis says  "<<i<<":\t\t"<<output[i] << "\n";
  }
  cout<<endl;
  
  return 0;
}