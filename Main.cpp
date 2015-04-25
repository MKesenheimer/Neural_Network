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
  cout << "====================================\n";
  cout << "=     Simple Neuronal Network      =\n";
  cout << "====================================\n\n";
  
  //contructing and init the Brain
  int nneurons = 2;
  int ninputs = 3;
  int noutputs = 1;
  
  Brain Jarvis(nneurons,ninputs,noutputs);
  initBrain(&Jarvis);
  
  Trainer TonyStark;
  TonyStark.train(&Jarvis);
  
  cout << "\nTesting the brain:\n";
  
  vector<float> input;
  for (int i=0; i<ninputs; i++) {
    input.push_back(getRandomNumber());
  }

  for (int i=0; i<input.size(); i++) {
    cout << "Jarvis input "<<i<<": "<< input[i] << "\n";
  }
  
  cout << "sum of inputs: "<<vecsum(input)<<"\n";
  
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


//if you want to change the design of the network, you should only make changes
//in this function
void initBrain(Brain* brain){
  int identifier = 0;
  vector<Neuron> inputLayer;
  vector<Neuron> outputLayer;
  vector<Neuron> neuron;
  
  //Fill inputLayer with neurons
  for (int i=0; i<(brain->numberOfInputs()); i++) {
    inputLayer.push_back( Neuron(identifier,1,brain->numberOfNeurons()) );
    identifier++;
  }
  
  //Fill brain with active neurons
  for (int i=0; i<(brain->numberOfNeurons()); i++) {
    neuron.push_back( Neuron(identifier,brain->numberOfInputs(),brain->numberOfOutputs()) );
    identifier++;
  }
  
  //Fill outputLayer with neurons
  for (int n = 0; n<(brain->numberOfOutputs()); n++) {
    outputLayer.push_back( Neuron(identifier,brain->numberOfNeurons(),1) );
    identifier++;
  }

  //init the inputLayer
  for (int m = 0; m<(brain->numberOfInputs()); m++) {
    std::vector<float> weights;
    for (int n=0; n<inputLayer[m].numberOfInputs(); n++) {
      //DEFAULT:
      weights.push_back(getRandomNumber());
      //DEBUG:
      //weights.push_back(1);
    }
    inputLayer[m].setWeights(weights);
    inputLayer[m].setThreshold(getRandomNumber());
  }
  
   //init the outputLayer
  for (int m = 0; m<(brain->numberOfOutputs()); m++) {
    std::vector<float> weights;
    for (int n=0; n<outputLayer[m].numberOfInputs(); n++) {
      //DEFAULT:
      weights.push_back(getRandomNumber());
      //DEBUG:
      //weights.push_back(1);
    }
    outputLayer[m].setWeights(weights);
    outputLayer[m].setThreshold(getRandomNumber());
  }
  
  //init the neurons
  for (int m=0; m<(brain->numberOfNeurons()); m++) {
    std::vector<float> weights;
    for (int n=0; n<neuron[m].numberOfInputs(); n++) {
      //DEFAULT:
      weights.push_back(getRandomNumber());
      //DEBUG:
      //weights.push_back(1);
    }
    //set weights and threshold of Neuron 0 in layer 0
    neuron[m].setWeights(weights);
    neuron[m].setThreshold(getRandomNumber());
  }
  
  //save the neurons
  brain->setInputLayer(inputLayer);
  brain->setOutputLayer(outputLayer);
  brain->setNeurons(neuron);
  
  //connect the neurons
  //generate a neural network with 1 hidden layer:
  //connect inputLayer neurons with active neurons
  for (int m=0; m<(brain->numberOfInputs()); m++) {
    for (int n=0; n<(brain->numberOfNeurons()); n++) {
      inputLayer = brain->getInputLayer(); //for safety reasons
      neuron = brain->getNeurons();
      brain->connectNeurons(&inputLayer[m],n,&neuron[n],m);
    }
  }
  
  //connect active neurons with outputLayer
  for (int m=0; m<(brain->numberOfNeurons()); m++) {
    for (int n=0; n<(brain->numberOfOutputs()); n++) {
      neuron = brain->getNeurons();
      outputLayer  = brain->getOutputLayer();
      brain->connectNeurons(&neuron[m],n,&outputLayer[n],m);
    }
  }
}