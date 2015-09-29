#include <iostream>
#include <vector>
#include <math.h>
#include "Neuron.h"

Neuron::Neuron(int ident, int nninputs, int nnoutputs) {
  #ifdef DEBUGV2
    std::cout<<"Constructing neuron "<<ident<<" with ";
    std::cout<<nninputs<<" input(s) and "<<nnoutputs<<" output(s)."<<"\n";
  #endif
  
  identifier = ident;
  ninputs = nninputs;
  noutputs = nnoutputs;
  
  w.reserve(ninputs);
  i.reserve(ninputs);
  o.reserve(noutputs);
  
  connectedN.reserve(ninputs);
  connectedO.reserve(ninputs);
  
  //if a Neuron is initialized, it is inactive first
  for (int n = 0; n<ninputs; n++) {
    w[n] = 0;
  }
  for (int n = 0; n<noutputs; n++) {
    o[n] = 0;
  }
  theta = 0;
}

void Neuron::setWeights(const std::vector<long double>& ww) {
  w = ww;
  #ifdef DEBUGV2
    std::cout<<"weights of neuron "<<identifier<<":\n";
    for (int i=0; i<ninputs; i++) {
      std::cout<<"i"<<i<<": "<<w[i]<<"\n";
    }
  #endif
}

void Neuron::setThreshold(long double ttheta) {
  theta = ttheta;
  #ifdef DEBUGV2
    std::cout<<"threshold of neuron "<<identifier<<": "<<theta<<"\n";
  #endif
}

std::vector<long double> Neuron::calculateOutput(const std::vector<long double>& xx) {
  
  //set inputs
  i = xx;
  //transfer function
  long double temp = 0;
  for (int n=0; n<ninputs; n++) {
    temp += w[n]*i[n];
  }

  //why is this necessary? (without segfault)
  o.assign(noutputs,temp);
  
  //perceptron
  //#define PERCEPTRON
  #ifdef PERCEPTRON
    //theta function
    for (int n=0; n<noutputs; n++) {
      if (temp >= theta) {
        o[n] = 1;
      } else {
        o[n] = 0;
      }
    }
  #endif
  
  //sigmoid neuron:
  #define SIGMOID
  #ifdef SIGMOID
    for (int n=0; n<noutputs; n++) {
      o[n] = 1/(1+exp(theta-temp));
    }
  #endif
  
  return o;
}

int Neuron::numberOfInputs(){
  return ninputs;
}

int Neuron::numberOfOutputs(){
  return noutputs;
}

int Neuron::getIdentifier(){
  return identifier;
}

std::vector<long double> Neuron::getParams() {
  std::vector<long double> params;
  //the return value consists of "ninputs" weights and 1 threshold
  params.reserve(ninputs+1);
  
  for (int i=0; i<ninputs; i++) {
    params.push_back(w[i]);
  }
  
  params.push_back(theta);
  
  return params;
}

void Neuron::establishConnection(int input, int neuronName, int output) {
  connectedO.push_back(output);
  connectedN.push_back(neuronName);
  pointerToIndex.push_back(input); //the vector pointerToIndex grows in the same way as the connectedO/N vectors
}

int Neuron::getConnectedNeuron(int input) {
  return connectedN[reversePointer(input)];
}

int Neuron::getConnectedOutput(int input) {
  return connectedO[reversePointer(input)];
}

int Neuron::reversePointer(int i) {
  int position;
  for (position = 0; position<pointerToIndex.size(); position++) {
    if(pointerToIndex[position]==i) {
      return position; //the rest is never reached
    }
  }
  if (position>=pointerToIndex.size()) {
    std::cout<<"error in Neuron: input i"<<i<<" could not be found in vector pointerToIndex.\n";
    for (int k = 0; k<pointerToIndex.size(); k++) {
      std::cout<<"pointerToIndex["<<k<<"]: "<<pointerToIndex[k]<<"\n";
    }
    exit(1);
  }
  return -999;
}

Neuron::~Neuron() {
  #ifdef DEBUGV3
    std::cout<<"Destructing neuron "<<identifier<<"\n";
  #endif  
}