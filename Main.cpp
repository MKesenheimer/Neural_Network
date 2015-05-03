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
  
  //the number of active neurons
  int nneurons = 5;
  //the number of input values
  int ninputs = 4;
  //the number of output values
  int noutputs = 2;
  
  //accuracy of the desired output
  float accuracy = 0.001;
  //learning rate of the brain
  float learningRate = 10;
  
  //constructing and initializing the brain
  Brain Jarvis(nneurons,ninputs,noutputs);
  //constructing and initializing the trainer
  Trainer TonyStark(learningRate,accuracy);
  
  //store the demanded input and the desired output
  std::vector<float> demandedInput;
  std::vector<float> desiredOutput;
  
  //set up the dataset
  //set 1
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(1);
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  desiredOutput.push_back(0);
  desiredOutput.push_back(0);
  TonyStark.addDataSet(demandedInput,desiredOutput);
  //set 2
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(0);
  demandedInput.push_back(1);
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  desiredOutput.push_back(1);
  desiredOutput.push_back(0);
  TonyStark.addDataSet(demandedInput,desiredOutput);
  //set 3
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  demandedInput.push_back(1);
  demandedInput.push_back(0);
  desiredOutput.push_back(0);
  desiredOutput.push_back(1);
  TonyStark.addDataSet(demandedInput,desiredOutput);
  //set 4
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
  TonyStark.train(&Jarvis);
  
  cout << "\n\n=> Testing the brain:\n";
  
  //init in- and output vectors
  vector<float> input;
  vector<float> output;
  
  //we can now test the brain
  //test 1
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
    cout << "\tJarvis output "<<i<<":\t"<<output[i] << "\n";
  }
  cout<<endl;
  
  //test 2
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
    cout << "\tJarvis output "<<i<<":\t"<<output[i] << "\n";
  }
  cout<<endl;
  
  //test 3
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
    cout << "\tJarvis output "<<i<<":\t"<<output[i] << "\n";
  }
  cout<<endl;
  
  //test 4
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
    cout << "\tJarvis output "<<i<<":\t"<<output[i] << "\n";
  }
  cout<<endl;
  
  return 0;
}