#ifndef TRAIN_H
#define TRAIN_H

#include <vector>
#include "Brain.h"

class Trainer {
  public:
    //the constructor saves learning rate and the desired accuracy
    Trainer(long double rate, long double accuracy);
    //function to train the brain
    void train(Brain* brain);
    //function to determine the training progress
    //input parameters are all the weights and the thresholds
    long double costFunction(const std::vector<long double>& allParams);
    //default dataset which the brain should learn
    void setData1();
    //add a costum dataset
    void addDataSet(const std::vector<long double>& demandedInputs, const std::vector<long double>& desiredOutputs);
    
  private:
    //this is the dataset we want to teach the brain
    std::vector< std::vector<long double> > allDemandedInputs;
    std::vector< std::vector<long double> > allDesiredOutputs;
    //number of datasets
    int numberOfSets;
    //the brain which should get trained
    Brain* theBrain;
    //small number for numerical derivatives
    long double h;
    //the learning rate -> how fast should the brain learn?
    long double learningRate;
    //gradient of the cost function
    std::vector<long double> gradientC;
    //accuracy
    long double eps;
  
};

#endif