#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

#include "Trainer.h"
#include "helper.h"

void Trainer::train(Brain* brain) {
  
  std::cout<<"Training the brain...\n";
  
  std::vector<float> input;
  std::vector<float> output;
  std::vector<float> desiredOutput;
  
  for (int i=0; i<(brain->numberOfInputs()); i++) {
    input.push_back(getRandomNumber());
  }
  
  //brain->setParamInputLayer(0,input,1);
  
  //Try first optimizing the brain so that output = Sum(input)
  for (int i=0; i<(brain->numberOfOutputs()); i++) {
    desiredOutput.push_back(vecsum(input));
    std::cout << "desired output: "<< desiredOutput[i] << "\n";
  }
  
  output = brain->output(input);
  for (int i=0; i<(brain->numberOfOutputs()); i++) {
    std::cout << "brain output: "<<output[i]<<"\n";
  }
  std::cout<<"Delta: "<<norm(subtract(output,desiredOutput))<<"\n";
}