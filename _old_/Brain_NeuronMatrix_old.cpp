#include <iostream>
#include "Brain.h"

Brain::Brain(int nnneurons, int nnlayers){
  #ifdef DEBUG
    std::cout << "Constructing Brain...\n";
    std::cout << "with "<<nneurons<<" neuron(s) and "<<nlayers<<" layer(s).\n";
  #endif
  
  nneurons = nnneurons;
  nlayers  = nnlayers;
  
  //This creates an neuronMatrix with indices [neuron][layer]
  neuronLayer.reserve(nlayers);
  for (int n=0; n<nlayers; n++) {
    neuronLayer.push_back( Neuron(3,1) );
  }
  for (int l=0; l<nneurons; l++) {
    neuronMatrix.push_back(neuronLayer);
  }
  
  //TODO: connections[][] = 0
}

void Brain::initBrain(){
  //connects output of neuron [0][0] with 0th input of neuron [0][1]
  //connectNeurons(neuronNumber[0][0], neuronNumber[0][1], 0);
  
  std::vector<float> weights;
  // TODO: beliebige Anzahl an Weights hinzufügen!
  weights.push_back(0.5);
  weights.push_back(0.3);
  weights.push_back(0.2);
  
  //set weights and threshold of Neuron 0 in layer 0
  neuronMatrix[0][0].setWeights(weights);
  neuronMatrix[0][0].setThreshold(1.0);
  //set weights and threshold of Neuron 1 in layer 0
  neuronMatrix[1][0].setWeights(weights);
  neuronMatrix[1][0].setThreshold(1.0);
}

void Brain::connectNeurons(int n1, int n2, int input) {
  //connections[n1][n2][input].assign(1,1);
}

int Brain::getNeuronNumber(int ineuron, int ilayer) {
  std::cout<<"ineuron: "<<ineuron<<"\n";
  std::cout<<"ilayer:  "<<ilayer <<"\n";
  if (ineuron <= nneurons && ilayer <= nlayers
      && ineuron*ilayer <= nneurons*nlayers) {
    return 2*ineuron+ilayer;
  }
  return 0;
}

std::vector<float> Brain::output(const std::vector<float>& x) {
  std::vector<float> temp;
  //Test
  temp.push_back(neuronMatrix[0][0].calculateOutput(x).at(0)
                 + neuronMatrix[1][0].calculateOutput(x).at(0));
  return temp;
}

Brain::~Brain() {
  #ifdef DEBUG
    std::cout << "Destructing Brain...\n";
  #endif
}