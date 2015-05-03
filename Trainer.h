#ifndef TRAIN_H
#define TRAIN_H

#include <vector>
#include "Brain.h"

class Trainer {
  public:
    //the onstructor saves the brain which should get trained
    Trainer(Brain* brain);
    //function to train the brain
    void train();
    //function to determine the training progress
    //input parameters are all weights and the thresholds
    float costFunction(const std::vector<float>& allParams);
    //set the dataset which the brain should learn
    void setData1();
    
  private:
    //this is the dataset we want to teach the brain
    std::vector< std::vector<float> > allDemandedInputs;
    std::vector< std::vector<float> > allDesiredOutputs;
    //number of datasets
    int numberOfSets;
    //the brain which should get trained
    Brain* theBrain;
    //small number for numerical derivatives
    float h;
    //the learning rate -> how fast should the brain learn?
    float learningRate;
    //gradient of the cost function
    std::vector<float> gradientC;
    //accuracy
    float eps;
  
};

#endif