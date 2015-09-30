#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include "Brain.h"
#include "Helper.h"

Brain::Brain(int nneur, int ninp, int noutp){
  #ifdef DEBUGV2
    std::cout << "Constructing Brain ";
    std::cout << "with "<<nneur<<" active neuron(s).\n";
  #endif

  nneurons = nneur;
  ninputs = ninp;
  noutputs = noutp;
  
  neuron.reserve(nneurons);
  inputLayer.reserve(ninputs);
  outputLayer.reserve(noutputs);
  
  initBrain();
}

//if you want to change the design of the network, you should only
//make changes in this function
void Brain::initBrain(){
  int identifier = 0;
  std::vector<long double> weights;
  
  //fill inputLayer with neurons
  for (int i=0; i<ninputs; i++) {
    addInputLayerNeuron(identifier,nneurons);
    weights.clear();
    for (int j=0; j<1; j++) {
      weights.push_back(getRandomNumber());
    }
    setParamInputLayer(i,weights,getRandomNumber());
    identifier++;
  }
  
  //fill brain with active neurons
  for (int i=0; i<nneurons; i++) {
    addActiveNeuron(identifier,ninputs,noutputs);
    weights.clear();
    for (int j=0; j<ninputs; j++) {
      weights.push_back(getRandomNumber());
    }
    setParamActiveNeuron(i,weights,getRandomNumber());
    identifier++;
  }
  
  //fill outputLayer with neurons
  for (int i = 0; i<noutputs; i++) {
    addOutputLayerNeuron(identifier,nneurons);
    weights.clear();
    for (int j=0; j<nneurons; j++) {
      weights.push_back(getRandomNumber());
    }
    setParamOutputLayer(i,weights,getRandomNumber());
    identifier++;
  }
  
  
  //connect the neurons
  //generate a neural network with 1 hidden layer:
  
  //connect inputLayer neurons with active neurons
  for (int m=0; m<ninputs; m++) {
    for (int n=0; n<nneurons; n++) {
      connectNeurons(&inputLayer[m],n,&neuron[n],m);
    }
  }
  
  //connect active neurons with outputLayer
  for (int m=0; m<nneurons; m++) {
    for (int n=0; n<noutputs; n++) {
      connectNeurons(&neuron[m],n,&outputLayer[n],m);
    }
  }
}


void Brain::connectNeurons(Neuron *neur1, int output, Neuron *neur2, int input) {
  int nneur1;
  nneur1 = neur1->getIdentifier();
  neur2->establishConnection(input, nneur1, output);
  
  #ifdef DEBUGV2
  std::cout<<"Neuron n"<<neur2->getIdentifier()<<" is connected on input i"<<input<<" with n";
  std::cout<<neur2->getConnectedNeuron(input)<<" at output o"<<neur2->getConnectedOutput(input)<<"\n";
  #endif
}

std::vector<long double> Brain::output(const std::vector<long double>& x) {
  /* Ablauf:
   * - Loop n über die Indices aller Neuronen (0 <= n < ninputs + nneurons + noutputs)
   * - berechne den Output eines Neurons, wenn alle Neuronen, die am Eingang des Neurons
   *   verbunden sind, schon berechnet wurden (finishedNeurons Liste). Die inputLayer Neuronen
   *   stehen schon von Beginn an auf der finishedNeurons Liste.
   * - nach der Berechnung, setze das soeben berechnete Neuron auf die finishedNeurons Liste
   * - Speichere den Output des i.ten Neurons nach dessen Berechnung im vector out
   * - die nachfolgenden Neuronen, die vom i.ten Neuron abhängen, können vom vector out
   *   ihre Eingabewerte lesen. Dazu schauen sie in der Liste finishedNeurons nach an welcher
   *   Stelle die Neuronen auftauchen, mit denen sie verbunden sind. Diese Zahl verrät
   *   an welchem Speicherplatz im vector out die Eingabewerte liegen.
   */
   
  //TODO: prüfe ob die Größe des übergebenen vectors x der Anzahl der Neuronen im inputLayer entspricht
  //überprüfe außerdem ob alle neuronen im inputLayer und outputLayer verbunden sind
  
  //in matrix out[i][output] are stored vectors of outputs of calculated neurons with index i
  std::vector< std::vector<long double> > out;
  //this is the vector which is returned from calculation
  std::vector<long double> totalOut;
  //to store the output of current neuron
  std::vector<long double> outtemp;
  //to calculate and store the input of current neuron
  std::vector<long double> intemp;
  std::vector<int> finishedNeurons; //store here the processed neurons
  finishedNeurons.clear();
  int numberOfFinishedNeurons = 0;

  //first go through the neurons in the inputLayer...
  for (int n=0; n<ninputs; n++) {
    //generate the input for the n-th neuron
    //inputLayer neurons have only one input
    std::vector<long double> in(&x[n],&x[n+1]);
    //calculate the output of the n-th neuron
    outtemp = inputLayer[n].calculateOutput(in);
    //store the number of the calculated neuron in string finishedNeurons
    finishedNeurons.push_back(inputLayer[n].getIdentifier());
    numberOfFinishedNeurons++;
    //push the output of the n-th neuron into vector out
    out.push_back(outtemp);
  }
  
  bool finished = false;
  
  //... then calculate the outputs of the active neurons:
  while(!finished) {
    for (int n=0; n<nneurons; n++) {
      bool calculateCNeur = true;
      //name of current neuron
      int nCNeur = neuron[n].getIdentifier();
      #ifdef DEBUGV3
        std::cout<<"current neuron "<<nCNeur<<"\n";
      #endif
      //is current neuron in the finishedNeurons list?
      //if so, next neuron
      bool foundFinished = false;
      for(int m=0; m<numberOfFinishedNeurons; m++) {
        if(finishedNeurons[m]==nCNeur) {
          foundFinished = true;
          m = numberOfFinishedNeurons; // escape the for-loop
        }
      }
      if (foundFinished) {
        #ifdef DEBUGV3
          std::cout<<" was already processed.\n";
        #endif
        calculateCNeur = false;
      } else {
        #ifdef DEBUGV3
          std::cout<<" was not processed yet";
        #endif
        //which neurons are connected to the inputs of current neuron?
        //are they in the finished list?
        //if one or more are not in the finished list, skip the current neuron and calculate it later
        for (int j=0; j<neuron[n].numberOfInputs(); j++) {
          int nConnected = getConnectedNeuron(&neuron[n],j);
          //std::size_t posConnected = finishedNeurons.find(strConnected);
          bool foundConnected = false;
          for(int m=0; m<numberOfFinishedNeurons; m++) {
            if(finishedNeurons[m]==nConnected) {
              foundConnected = true;
              m = numberOfFinishedNeurons; // escape the for-loop
            }
          }
          
          if (!foundConnected) {
            #ifdef DEBUGV3
              std::cout<<"neuron "<<nConnected<<", to which current neuron "<<nCNeur<<" is connected, was not processed yet. Skip current neuron.\n";
            #endif
            //skip the following calculation and...
            calculateCNeur = false;
            //... skip the loop
            j=neuron[n].numberOfInputs()+1;
          }
        }
        
        if (calculateCNeur) {
          #ifdef DEBUGV3
            std::cout<<"now we can finally calculate the output of neuron "<<nCNeur<<"\n";
          #endif
            
          //erase the temporary input vector
          intemp.clear();
          //erase the temporary output vector
          outtemp.clear();
            
          //where does the connected neuron occur in the finished list?
          // -> this number tells us where to find the output of the connected neuron in vector out
          for (int k=0; k<neuron[n].numberOfInputs(); k++) {
            int posInFinished;
            bool foundConnected;
            for(int m=0; m<numberOfFinishedNeurons; m++) {
              if(finishedNeurons[m]==getConnectedNeuron(&neuron[n],k)) {
                posInFinished = m;
                foundConnected = true;
                int indexOfOutput = getConnectedOutput(&neuron[n],k);
                //build the input vector that is later returned to current neuron:
                intemp.push_back(out[posInFinished][indexOfOutput]);
                m = numberOfFinishedNeurons; // escape the for-loop
              }
            }
          }
          
          //calculate te output of current neuron and store it in outtemp
          #ifdef DEBUGV2
            for (int l=0; l<neuron[n].numberOfInputs(); l++) {
              std::cout<<"input of current neuron "<<nCNeur<<": "<<intemp[l]<<"\n";
            }
          #endif
          outtemp = neuron[n].calculateOutput(intemp);
          #ifdef DEBUGV2
            for (int l=0; l<neuron[n].numberOfOutputs(); l++) {
              std::cout<<"output of current neuron: "<<outtemp[l]<<"\n";
            }
          #endif
          out.push_back(outtemp);
         
          //put the calculated neuron on the finished list
          finishedNeurons.push_back(neuron[n].getIdentifier());
          numberOfFinishedNeurons++;
        }
      }
    }
    
    //if all neurons were processed:
    if (numberOfFinishedNeurons>=nneurons+ninputs) {
      finished = true;
    } else {
      finished = false;
    }
  }

  //this is more or less the same code as for the active neurons
  //TODO: simplify and shorten this!
  //at least, go through the neurons in the outputlayer:
  for (int n=0; n<noutputs; n++) {
    int nCNeur = outputLayer[n].getIdentifier();
    #ifdef DEBUGV2
      std::cout<<"calculate the output of neurons in the outputLayer o"<<nCNeur<<"\n";
    #endif
    
    //erase the temporary input vector
    intemp.clear();
    //erase the temporary output vector
    outtemp.clear();
    
    //erase the temporary input vector
    intemp.clear();
    //erase the temporary output vector
    outtemp.clear();
    
    //where does the connected neuron occur in the finished list?
    // -> this number tells us where to find the output of the connected neuron in vector out
    for (int k=0; k<outputLayer[n].numberOfInputs(); k++) {
      int posInFinished;
      bool foundConnected;
      for(int m=0; m<numberOfFinishedNeurons; m++) {
        if(finishedNeurons[m]==getConnectedNeuron(&outputLayer[n],k)) {
          posInFinished = m;
          foundConnected = true;
          int indexOfOutput = getConnectedOutput(&outputLayer[n],k);
          //build the input vector that is later returned to current neuron:
          intemp.push_back(out[posInFinished][indexOfOutput]);
          m = numberOfFinishedNeurons; // escape the for-loop
        }
      }
    }
    
    //calculate te output of current neuron and store it in outtemp
    #ifdef DEBUGV2
    for (int l=0; l<outputLayer[n].numberOfInputs(); l++) {
      std::cout<<"input of current neuron "<<nCNeur<<": "<<intemp[l]<<"\n";
    }
    #endif
    outtemp = outputLayer[n].calculateOutput(intemp);
    #ifdef DEBUGV2
    for (int l=0; l<outputLayer[n].numberOfOutputs(); l++) {
      std::cout<<"output of current neuron: "<<outtemp[l]<<"\n";
    }
    #endif
    totalOut.push_back(outtemp[0]);
    
    //put the calculated neuron on the finished list
    finishedNeurons.push_back(outputLayer[n].getIdentifier());
    numberOfFinishedNeurons++;
  }
  
  #ifdef DEBUGV2
    std::cout<<"total number of neurons: "<<nneurons+ninputs<<"\n";
    std::cout<<"number of finished neurons: "<<numberOfFinishedNeurons<<"\n";
    std::cout<<"finished neurons:\n";
    for (int m=0; m<numberOfFinishedNeurons; m++) {
      std::cout<<finishedNeurons[m]<<"\n";
    }
  #endif
  
  return totalOut;
}

int Brain::getConnectedNeuron(Neuron *neur, int iinput){
  
  //check if iinput was set correctly
  if (iinput >= (neur->numberOfInputs())) {
    std::cout<<"\nError: iinput is to large: "<<iinput<<"\n";
    exit(1);
  }

  return neur->getConnectedNeuron(iinput);
}

int Brain::getConnectedOutput(Neuron *neur, int iinput) {
  return neur->getConnectedOutput(iinput);
}

int Brain::numberOfInputs() {
  return ninputs;
}

int Brain::numberOfOutputs() {
  return noutputs;
}

int Brain::numberOfNeurons() {
  return nneurons;
}

void Brain::setParamInputLayer(int n, const std::vector<long double>& weights, long double theta) {
    inputLayer[n].setWeights(weights);
    inputLayer[n].setThreshold(theta);
}

void Brain::setParamOutputLayer(int n, const std::vector<long double>& weights, long double theta) {
    outputLayer[n].setWeights(weights);
    outputLayer[n].setThreshold(theta);
}

void Brain::setParamActiveNeuron(int n, const std::vector<long double>& weights, long double theta) {
    neuron[n].setWeights(weights);
    neuron[n].setThreshold(theta);
}

void Brain::addInputLayerNeuron(int identifier, int nouts) {
  inputLayer.push_back( Neuron(identifier,1,nouts) );
}

void Brain::addOutputLayerNeuron(int identifier, int nins) {
  outputLayer.push_back( Neuron(identifier,nins,1) );
}

void Brain::addActiveNeuron(int identifier, int nins, int nouts) {
  neuron.push_back( Neuron(identifier,nins,nouts) );
}

std::vector<long double> Brain::getAllParameters() {
  std::vector<long double> params;
  std::vector<long double> temp;
  
  //first loop through the inputLayer neurons
  for (int i=0; i<ninputs; i++) {
    for (int j=0; j<inputLayer[i].numberOfInputs()+1; j++) {
      //the data format is: weight1, weight2, ..., weightn, theta
      temp = inputLayer[i].getParams();
      params.push_back(temp[j]);
    }
  }
  
  //then through the active neurons
  for (int i=0; i<nneurons; i++) {
    for (int j=0; j<neuron[i].numberOfInputs()+1; j++) {
      //the data format is: weight1, weight2, ..., weightn, theta
      temp = neuron[i].getParams();
      params.push_back(temp[j]);
    }
  }
  
  for (int i=0; i<noutputs; i++) {
    for (int j=0; j<outputLayer[i].numberOfInputs()+1; j++) {
      //the data format is: weight1, weight2, ..., weightn, theta
      temp = outputLayer[i].getParams();
      params.push_back(temp[j]);
    }
  }
  
  return params;
}

void Brain::setAllParameters(const std::vector<long double>& params) {
  std::vector<long double> weights;
  long double theta;
  int iter = 0;
  
  //restore the parameters for each neuron from params string
  for (int i=0; i<ninputs; i++) {
    weights.clear();
    for (int j=0; j<inputLayer[i].numberOfInputs(); j++) {
      weights.push_back(params[iter++]);
    }
    theta = params[iter++];
    setParamInputLayer(i,weights,theta);
  }
  
  for (int i=0; i<nneurons; i++) {
    weights.clear();
    for (int j=0; j<neuron[i].numberOfInputs(); j++) {
      weights.push_back(params[iter++]);
    }
    theta = params[iter++];
    setParamActiveNeuron(i,weights,theta);
  }
  
  for (int i=0; i<noutputs; i++) {
    weights.clear();
    for (int j=0; j<outputLayer[i].numberOfInputs(); j++) {
      weights.push_back(params[iter++]);
    }
    theta = params[iter++];
    setParamOutputLayer(i,weights,theta);
  }
}

Brain::~Brain() {
  #ifdef DEBUGV3
    std::cout<<"Destructing brain.\n";
  #endif
}