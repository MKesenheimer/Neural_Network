#include <iostream>
#include <fstream>
#include <vector>
#include "Main.h"
#include "Brain.h"
#include "Trainer.h"
#include "Neuron.h"
#include "Helper.h"
#include "Interface.h"

using namespace::std;

int main()
{
  cout << "==============================================\n";
  cout << "           Simple Neuronal Network            \n";
  cout << "==============================================\n\n";
  
  //write all data to a file
  //ofstream ofile;
  //ofile.open("brain.dat");
  //ofile << "Writing this to a file.\n";
  //ofile.close();
  
  Interface fileAcces("brain.dat");

  //the number of active neurons
  int nneurons = fileAcces.readInput("nneurons");
  //the number of input values
  int ninputs = fileAcces.readInput("ninputs");
  //the number of output values
  int noutputs = fileAcces.readInput("noutputs");
  
  //accuracy of the desired output
  long double accuracy = 0.001;
  //learning rate of the brain
  long double learningRate = 10;
  
  //constructing and initializing the brain
  Brain Jarvis(nneurons,ninputs,noutputs);
  //constructing and initializing the trainer
  Trainer TonyStark(learningRate,accuracy);
  
  //store the demanded input and the desired output
  std::vector< std::vector<long double> > dataSets;
  int nsets = fileAcces.readInput("nsets");
  dataSets = fileAcces.readDataSets();
  
  //set up the datasets
  for (int i = 0; i < 2*nsets; i += 2) {
    TonyStark.addDataSet(dataSets[i],dataSets[i+1]);
  }
  
  //now all the data is provided, train the brain
  TonyStark.train(&Jarvis);
  
  
  cout << "\n\n=> Testing the brain:\n";
  
  //init in- and output vectors
  vector<long double> input;
  vector<long double> output;
  
  //we can now test the brain
  for (int j=0; j<2*nsets; j += 2) {
    input = dataSets[j];
    for (int i=0; i<input.size(); i++) {
      cout << "\tJarvis input "<<i<<":\t\t"<< input[i] << "\n";
    }
    //calculate the output
    output = Jarvis.output(input);
    for (int i=0; i<output.size(); i++) {
      cout << "\tJarvis output "<<i<<":\t"<< output[i] << "\n";
    }
    cout<<endl;
  }
  
  return 0;
}