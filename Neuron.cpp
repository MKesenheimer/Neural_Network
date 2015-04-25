#include <iostream>
#include <vector>
#include <math.h>
#include "Neuron.h"

Neuron::Neuron(int ident, int nninputs, int nnoutputs) {
  #ifdef DEBUG
    std::cout<<"Constructing neuron "<<ident<<" with ";
    std::cout<<nninputs<<" input(s) and "<<nnoutputs<<" output(s)."<<"\n";
  #endif
  
  identifier = ident;
  ninputs = nninputs;
  noutputs = nnoutputs;
  
  w.reserve(ninputs);
  i.reserve(ninputs);
  o.reserve(noutputs);
  
  //if a Neuron is initialized, it is inactive first
  for (int n = 0; n<ninputs; n++) {
    w[n] = 0;
  }
  for (int n = 0; n<noutputs; n++) {
    o[n] = 0;
  }
  theta = 0;
}

void Neuron::setWeights(const std::vector<float>& ww) {
  w = ww;
  #ifdef DEBUG
    std::cout<<"weights of neuron "<<identifier<<":\n";
    for (int i=0; i<w.size(); i++) {
      std::cout<<"i"<<i<<": "<<w[i]<<"\n";
    }
  #endif
}

void Neuron::setThreshold(float ttheta) {
  theta = ttheta;
  #ifdef DEBUG
    std::cout<<"threshold of neuron "<<identifier<<": "<<theta<<"\n";
  #endif
}

std::vector<float> Neuron::calculateOutput(const std::vector<float>& xx) {
  
  //set inputs
  i = xx;
  //transfer function
  float temp = 0;
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

std::string Neuron::getNeuronName() {
  std::string temp;
  temp = "n" + std::to_string(getIdentifier());
  return temp;
}

Neuron::~Neuron() {
  #ifdef DEBUG
    std::cout<<"Destructing neuron "<<identifier<<"\n";
  #endif  
}