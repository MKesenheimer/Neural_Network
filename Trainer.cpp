#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <time.h>

#include "Trainer.h"
#include "Helper.h"

Trainer::Trainer(long double rate, long double accuracy) {
  //set the learning rate
  learningRate = rate;
  //set the small number for numerical derivatives
  h = 0.0001;
  //accuracy
  eps = accuracy;
  
  //reset the global variables
  allDemandedInputs.clear();
  allDesiredOutputs.clear();
  numberOfSets = 0;
}

void Trainer::setData1() {
  //this dataset works only with a neural network
  //with three inputs and one output, but it is easily
  //extendable to an other configuration
  if ((theBrain->numberOfInputs() != 3) || (theBrain->numberOfOutputs() != 1)) {
    std::cout<<"Error: Wrong trainer for this brain.\n";
    exit(1);
  }

  //store here the input and the desired output
  std::vector<long double> demandedInput;
  std::vector<long double> desiredOutput;
  
  //set up the dataset
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(1);
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  desiredOutput.push_back(0);
  addDataSet(demandedInput,desiredOutput);
  
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(0);
  demandedInput.push_back(1);
  demandedInput.push_back(0);
  desiredOutput.push_back(0.5);
  addDataSet(demandedInput,desiredOutput);
  
  demandedInput.clear();
  desiredOutput.clear();
  demandedInput.push_back(0);
  demandedInput.push_back(0);
  demandedInput.push_back(1);
  desiredOutput.push_back(1);
  addDataSet(demandedInput,desiredOutput);
}

long double Trainer::costFunction(const std::vector<long double>& allParams) {
  long double delta = 0;
  long double costFct = 0;
  std::vector<long double> output;
  output.reserve(theBrain->numberOfOutputs());
  
  //save old parameters of the brain to restore the old brain later:
  std::vector<long double> oldParams;
  oldParams = theBrain->getAllParameters();
  
  //calculate the output of the brain with the given parameters
  theBrain->setAllParameters(allParams);
  
  for (int n=0; n<numberOfSets; n++) {
    //calculate brain output of current dataset n
    output = theBrain->output(allDemandedInputs[n]);
    //sum over the norm squared of the outputs and desired outputs
    delta += norm(subtract(allDesiredOutputs[n],output));
  }
  //costFct = delta/(2*numberOfSets);
  costFct = delta/2;
  
  #ifdef DEBUGV1
    std::cout<<"\tcost function:\t\t"<<costFct<<"\n";
  #endif
  
  //restore the old brain
  theBrain->setAllParameters(oldParams);
  
  return costFct;
}

void Trainer::train(Brain* brain) {
  //which brain should get trained?
  theBrain = brain;
  
  std::cout<<"=> Training the brain:\n";
  time_t start,end;
  time (&start);
  
  //choose here which data the brain should learn
  //the user can provide an other dataset with the function addDataSet(...)
  //if no dataset is present, use the default one:
  if (allDemandedInputs.size() == 0) {
    setData1();
  }
  
  //print out dataset
  std::cout<<"=> Dataset:";
  for (int n=0; n<numberOfSets; n++) {
    std::cout<<"\n";
    for (int i=0; i<allDemandedInputs[n].size(); i++) {
      std::cout << "\tdemanded input "<<i<<":\t"<<allDemandedInputs[n][i] << "\n";
    }
    for (int i=0; i<(theBrain->numberOfOutputs()); i++) {
      std::cout << "\tdesired output "<<i<<":\t"<<allDesiredOutputs[n][i] << "\n";
    }
  }
  
  //does the output meets the requirements?
  bool optimized = false;
  while (!optimized) {
    //load all the parameters of the brain
    std::vector<long double> allParams;
    //the original parameters
    allParams = theBrain->getAllParameters();
    //these are the parameters shifted by h
    std::vector<long double> allParamsh;
    //the new parameters:
    std::vector<long double> newParams;
  
    //if the output is good enough
    if (costFunction(allParams)<eps) {
      optimized = true;
    }
    //if it is not, try to minimize the costFct
    else {
      //calculate the gradient of the costFunction:
      //gradient C = ( (C(v1+h,v2,...,vn)-C(v1,v2,...,vn))/h, (C(v1,v2+h,...,vn)-C(v1,v2,...,vn))/h, ... )
      //           = (  (cfh1-cf)/h, (cfh2-cf)/h, ..., (cfhn-cf)/h) )
      //the costfunction with the original parameters
      long double cf = costFunction(allParams);
      //the costfunction with parameters shifted by h
      long double cfh;
      
      gradientC.clear();
      for (int index = 0; index<allParams.size(); index++) {
        allParamsh.clear();
        for (int n=0; n<allParams.size(); n++) {
          allParamsh.push_back(allParams[n]+h*deltaFct(n,index));
        }
        cfh = costFunction(allParamsh);
        //derivative of costfunction
        long double deriv = (cfh-cf)/h;
        gradientC.push_back(deriv);
      }
  
      #ifdef DEBUGV3
        for (int i = 0; i<gradientC.size(); i++) {
          std::cout<<"gradientC["<<i<<"]: "<<gradientC[i]<<"\n";
        }
      #endif
    
      newParams.clear();
      for (int index = 0; index<allParams.size(); index++) {
        newParams.push_back(allParams[index]-learningRate*gradientC[index]);
      }
      #ifdef DEBUGV3
        for (int i=0; i<allParamsh.size(); i++) {
          std::cout<<"params["<<i<<"]: "<<allParams[i]<<"\n";
          std::cout<<"paramsh["<<i<<"]: "<<allParamsh[i]<<"\n";
          std::cout<<"newparams["<<i<<"]: "<<newParams[i]<<"\n";
        }
      #endif
      
      //consistency check
      if (allParams.size() != allParamsh.size() || allParams.size() != newParams.size()) {
        std::cout<<"Error: parameter vectors unequal in size.\n";
        std::cout<<"allParams.size: "<<allParams.size()<<"\n";
        std::cout<<"allParamsh.size: "<<allParamsh.size()<<"\n";
        std::cout<<"newParams.size: "<<newParams.size()<<"\n";
        exit(1);
      }
      
      theBrain->setAllParameters(newParams);
    }
  }

  time (&end);
  double dif = difftime (end,start);
  std::cout<<"=> Brain is now trained.\n";
  printf ("Elapsed time: %.2lfs.\n", dif );
}

void Trainer::addDataSet(const std::vector<long double>& demandedInputs, const std::vector<long double>& desiredOutputs) {
  allDemandedInputs.push_back(demandedInputs);
  allDesiredOutputs.push_back(desiredOutputs);
  numberOfSets++;
}