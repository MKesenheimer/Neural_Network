#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include "Brain.h"
#include "helper.h"

Brain::Brain(int nneur, int ninp, int noutp){
  #ifdef DEBUGV2
    std::cout << "Constructing Brain ";
    std::cout << "with "<<nneur<<" active neuron(s).\n";
  #endif

  nneurons = nneur;
  ninputs = ninp;
  noutputs = noutp;
  connections = "";
  
  neuron.reserve(nneurons);
  inputLayer.reserve(ninputs);
  outputLayer.reserve(noutputs);
  
  initBrain();
}

//if you want to change the design of the network, you should only
//make changes in this function
void Brain::initBrain(){
  int identifier = 0;
  std::vector<float> weights;
  
  //debug
  //int debint = 0;
  
  //Fill inputLayer with neurons
  for (int i=0; i<ninputs; i++) {
    addInputLayerNeuron(identifier,nneurons);
    weights.clear();
    for (int j=0; j<1; j++) {
      weights.push_back(getRandomNumber());
      //weights.push_back(debint++);
    }
    setParamInputLayer(i,weights,getRandomNumber());
    //setParamInputLayer(i,weights,debint++);
    identifier++;
  }
  
  //Fill brain with active neurons
  for (int i=0; i<nneurons; i++) {
    addActiveNeuron(identifier,ninputs,noutputs);
    weights.clear();
    for (int j=0; j<ninputs; j++) {
      weights.push_back(getRandomNumber());
      //weights.push_back(debint++);
    }
    setParamActiveNeuron(i,weights,getRandomNumber());
    //setParamActiveNeuron(i,weights,debint++);
    identifier++;
  }
  
  //Fill outputLayer with neurons
  for (int i = 0; i<noutputs; i++) {
    addOutputLayerNeuron(identifier,nneurons);
    weights.clear();
    for (int j=0; j<nneurons; j++) {
      weights.push_back(getRandomNumber());
      //weights.push_back(debint++);
    }
    setParamOutputLayer(i,weights,getRandomNumber());
    //setParamOutputLayer(i,weights,debint++);
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
  std::string strneur1, strneur2;
  strneur1 = neur1->getNeuronName();
  strneur2 = neur2->getNeuronName();
  
  #ifdef DEBUGV2
    std::cout<<"Connecting output "<<output<<" of Neuron "<<strneur1;
    std::cout<<" with input "<<input<<" of Neuron "<<strneur2<<".\n";
  #endif
  
  //TODO: Überprüfe ob auch die Aus- und Eingänge vorhanden sind
  // die miteinander verbunden werden sollen (output<=numberofOutputs usw.)
  //außerdem ob auch die Neuronen da sind, die verbunden werden sollen

  //int to string
  std::string strfromtuple, strtotuple, strconnection;
  strfromtuple  = "("+ strneur1 + ",o" + std::to_string(output) + ")";
  strtotuple    = "("+ strneur2 + ",i" + std::to_string(input) + ")";
  strconnection = strtotuple + " <- " + strfromtuple + ", ";
  
  //Check if the connection is already in use (every tuple must occur only once)
  //if so, throw error and ignore connection.
  bool connectionused = false;
  std::size_t found1 = connections.find(strfromtuple);
  std::size_t found2 = connections.find(strtotuple);
  if (found1!=std::string::npos) {
    std::cout << "Bad connection. Connection from "<< strfromtuple <<" already used.\n";
    connectionused = true;
  } else if(found2!=std::string::npos) {
    std::cout << "Bad connection. Connection to "<< strtotuple <<" already used.\n";
    connectionused = true;
  }
  if (!connectionused) {
    connections += strconnection;
  }
  
  #ifdef DEBUGV3
    std::cout<<connections<<"\n";
  #endif
}

std::vector<float> Brain::output(const std::vector<float>& x) {
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
   
  //TODO: prüfe ob die größe des übergebenen vectors x der Anzahl der Neuronen im inputLayer entspricht
  //überprüfe außerdem ob alle neuronen im inputLayer und outputLayer verbunden sind
  
  //in matrix out[i][output] are stored vectors of outputs of calculated neurons with index i
  std::vector< std::vector<float> > out;
  //this is the vector which is returned from calculation
  std::vector<float> totalOut;
  //to store the output of current neuron
  std::vector<float> outtemp;
  //to calculate and store the input of current neuron
  std::vector<float> intemp;
  std::string finishedNeurons = ""; //store here the processed neurons
  int numberOfFinishedNeurons = 0;

  //first go through the neurons in the inputLayer...
  for (int n=0; n<ninputs; n++) {
    //generate the input for the n-th neuron
    //inputLayer neurons have only one input
    std::vector<float> in(&x[n],&x[n+1]);
    //calculate the output of the n-th neuron
    outtemp = inputLayer[n].calculateOutput(in);
    //store the number of the calculated neuron in string finishedNeurons
    finishedNeurons += inputLayer[n].getNeuronName() + ", ";
    numberOfFinishedNeurons++;
    #ifdef DEBUGV3
      std::cout<<"finished neurons: "<<finishedNeurons<<"\n";
    #endif
    //push the output of the n-th neuron into vector out
    out.push_back(outtemp);
  }

  
  bool finished = false;
  
  //... then calculate the outputs of the active neurons:
  while(!finished) {
    for (int n=0; n<nneurons; n++) {
      bool calculateCNeur = true;
      //name of current neuron
      std::string strCNeur = neuron[n].getNeuronName();
      #ifdef DEBUGV3
        std::cout<<"current neuron "<<strCNeur<<"\n";
      #endif
      //is current neuron in the finishedNeurons list?
      //if so, next neuron
      std::size_t found = finishedNeurons.find(strCNeur);
      if (found!=std::string::npos) {
        #ifdef DEBUGV3
          std::cout<<" was already processed.\n";
        #endif
        calculateCNeur = false;
      } else {
        #ifdef DEBUGV3
          std::cout<<" was not processed yet";
        #endif
        //look if we can find the current neuron as target neuron in the connections list
        //(the next character is ",i")
        std::size_t posCNeur = connections.find(strCNeur+",i");
        if (posCNeur==std::string::npos) {
          #ifdef DEBUGV3
            std::cout<<" and could not be found as target in string connections. Skip.\n";
          #endif
          calculateCNeur = false;
        } else {
          #ifdef DEBUGV3
            std::cout<<" and is at position "<<posCNeur<<" in string connections.\n";
          #endif
          //which neurons are connected to the inputs of current neuron?
          #ifdef DEBUGV2
            std::cout<<"the inputs of neuron "<<strCNeur<<" are connected to "<<getConnectedNeuron(&neuron[n],-1)<<"\n";
          #endif
          //are they in the finished list?
          //if one or more are not in the finished list, skip the current neuron and calculate it later
          for (int j=0; j<neuron[n].numberOfInputs(); j++) {
            std::string strConnected = getConnectedNeuron(&neuron[n],j);
            std::size_t posConnected = finishedNeurons.find(strConnected);
            if (posConnected==std::string::npos) {
              #ifdef DEBUGV3
                std::cout<<"neuron "<<strConnected<<", to which current neuron "<<strCNeur<<", is connected was not processed yet. Skip current neuron.\n";
              #endif
              //skip the following calculation and...
              calculateCNeur = false;
              //... skip the loop
              j=neuron[n].numberOfInputs()+1;
            }
          }
          if (calculateCNeur) {
            #ifdef DEBUGV3
              std::cout<<"now we can finally calculate the output of neuron "<<strCNeur<<"\n";
            #endif
            
            //erase the temporary input vector
            intemp.clear();
            //erase the temporary output vector
            outtemp.clear();
            
            //where does the connected neuron occur in the finished list?
            // -> this number tells us where to find the output of the connected neuron in vector out
            for (int k=0; k<neuron[n].numberOfInputs(); k++) {
              std::size_t posInFinished = finishedNeurons.find(getConnectedNeuron(&neuron[n],k));
              if (posInFinished!=std::string::npos) {
                //count the colons that occur till connected neuron occurs
                std::string strSubFinished;
                strSubFinished = finishedNeurons.substr(0,posInFinished);
                #ifdef DEBUGV3
                  std::cout<<"string till connected neuron "<<getConnectedNeuron(&neuron[n],k)<<": "<<strSubFinished<<"\n";
                #endif
                std::size_t nOccur = std::count(strSubFinished.begin(), strSubFinished.end(), ',');
                #ifdef DEBUGV3
                  std::cout<<"colon occurs "<<nOccur<<" times.\n";
                #endif
                int indexOfOutput = nameToInt(getConnectedOutput(&neuron[n],k));
                //build the input vector that is later returned to current neuron:
                intemp.push_back(out[nOccur][indexOfOutput]);
              }
            }
            //calculate te output of current neuron and store it in outtemp
            #ifdef DEBUGV2
              for (int l=0; l<neuron[n].numberOfInputs(); l++) {
                std::cout<<"input of current neuron "<<strCNeur<<": "<<intemp[l]<<"\n";
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
            finishedNeurons += neuron[n].getNeuronName() + ", ";
            numberOfFinishedNeurons++;
          }
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
  //at last, go through the neurons in the outputlayer:
  for (int n=0; n<noutputs; n++) {
    std::string strCNeur = outputLayer[n].getNeuronName();
    #ifdef DEBUGV2
      std::cout<<"calculate the output of neurons in the outputLayer "<<strCNeur<<"\n";
    #endif
    
    //erase the temporary input vector
    intemp.clear();
    //erase the temporary output vector
    outtemp.clear();
    
    //where does the connected neuron occur in the finished list?
    // -> this number tells us where to find the output of the connected neuron in vector out
    for (int k=0; k<outputLayer[n].numberOfInputs(); k++) {
      std::size_t posInFinished = finishedNeurons.find(getConnectedNeuron(&outputLayer[n],k));
      if (posInFinished!=std::string::npos) {
        //count the colons that occur till connected neuron occurs
        std::string strSubFinished;
        strSubFinished = finishedNeurons.substr(0,posInFinished);
        #ifdef DEBUGV3
          std::cout<<"string till connected neuron "<<getConnectedNeuron(&outputLayer[n],k)<<": "<<strSubFinished<<"\n";
        #endif
        std::size_t nOccur = std::count(strSubFinished.begin(), strSubFinished.end(), ',');
        #ifdef DEBUGV3
          std::cout<<"colon occurs "<<nOccur<<" times.\n";
        #endif
        int indexOfOutput = nameToInt(getConnectedOutput(&outputLayer[n],k));
        #ifdef DEBUGV3
          std::cout<<"indices where to look in matrix out: ("<<nOccur<<","<<indexOfOutput<<")\n";
          std::cout<<"matrix out: "<<out[nOccur][indexOfOutput]<<"\n";
        #endif
        //build the input vector that is later returned to current neuron:
        intemp.push_back(out[nOccur][indexOfOutput]);
      }
    }
    //calculate the output of current neuron and store it in outtemp
    #ifdef DEBUGV2
      for (int l=0; l<outputLayer[n].numberOfInputs(); l++) {
        std::cout<<"input of current neuron "<<strCNeur<<": "<<intemp[l]<<"\n";
      }
    #endif
    outtemp = outputLayer[n].calculateOutput(intemp);
    #ifdef DEBUGV2
      for (int l=0; l<outputLayer[n].numberOfOutputs(); l++) {
        std::cout<<"output of current neuron "<<strCNeur<<": "<<outtemp[l]<<"\n";
      }
    #endif
    totalOut.push_back(outtemp[0]);
    
    //put the calculated neuron on the finished list
    finishedNeurons += outputLayer[n].getNeuronName() + ", ";
    numberOfFinishedNeurons++;
  }
  
  #ifdef DEBUGV2
    std::cout<<"total number of neurons: "<<nneurons+ninputs<<"\n";
    std::cout<<"number of finished neurons: "<<numberOfFinishedNeurons<<"\n";
    std::cout<<"finished neurons: "<<finishedNeurons<<"\n";
  #endif
  
  return totalOut;
}

std::string Brain::getConnectedNeuron(Neuron *neur, int iinput){
  
  //check if iinput was set correctly
  if (iinput >= (neur->numberOfInputs())) {
    std::cout<<"\nError: iinput is to large: "<<iinput<<"\n";
    exit(1);
  }
  
  //definitions
  std::string targetname = neur->getNeuronName();
  std::string strConnected = "";
  std::string strcc = ""; // current connection
  int counter = 0;
  
  //search for the connected neurons and store them in string strConnected
  //if 0<=iinput<numberOfInputs return only the neuron that is connected
  //to the iinput-th input
  std::size_t lastpos = 1;
  for (int j=0; j<(neur->numberOfInputs()); j++) {
    std::size_t targetpos = connections.find(targetname+",i", lastpos);
    std::size_t pointerpos1 = connections.find("<- (", targetpos);
    std::size_t pointerpos2 = connections.find(",o", targetpos);
    lastpos = pointerpos2;
    #ifdef DEBUGV3
      std::cout<<"targetpos "<<targetpos<<"\n";
      std::cout<<"pointerpos1 "<<pointerpos1<<"\n";
      std::cout<<"pointerpos2 "<<pointerpos2<<"\n";
    #endif
    if(!(targetpos==std::string::npos
         || pointerpos1==std::string::npos
         || pointerpos2==std::string::npos)){
      strcc = connections.substr(pointerpos1+4,pointerpos2-pointerpos1-4);
      strConnected += strcc  + ", ";
      //return the iinput-th connection of current neuron
      if (iinput == counter) {
        return strcc;
      }
      counter++;
    }
  }
  
  if (counter<(neur->numberOfInputs())) {
    std::cout<<"\nError: neuron "<<targetname<<" is not fully connected."<<"\n";
    exit(1);
  }
  
  //if iinput is negativ, return the whole string:
  return strConnected;
}

std::string Brain::getConnectedOutput(Neuron *neur, int iinput){
  std::string strName1 = neur->getNeuronName();
  std::string strName2 = getConnectedNeuron(neur, iinput);
  std::string strInput = "i" + std::to_string(iinput);
  std::string strFind = "(" + strName1 + "," + strInput + ") <- (" + strName2 + ",";
  std::string strOutput = "";
  
  #ifdef DEBUGV3
    std::cout<<"name of neuron 1 (target): "<<strName1<<"\n";
    std::cout<<"name of input of neuron 1: "<<strInput<<"\n";
    std::cout<<"name of neuron 2: "<<strName2<<"\n";
    std::cout<<"connections list: "<<connections<<"\n";
    std::cout<<"string to find: "<<strFind<<"\n";
  #endif
  
  std::size_t pos1 = connections.find(strFind);
  std::size_t pos2 = connections.find("),",pos1);
  if (pos1==std::string::npos) {
    std::cout<<"Error in getConnectedOutput: string not found: "<<strFind<<"\n";
    exit(1);
  } else {
    pos1 += strFind.length();
    strOutput = connections.substr(pos1,pos2-pos1);
    #ifdef DEBUGV3
      std::cout<<"output "<<strOutput<<" of neuron "<<strName2<<" is connected to input "<<strInput<<" of neuron "<<strName1<<".\n";
    #endif
  }
  
  return strOutput;
}

int Brain::nameToInt(std::string name){
  std::string temp;
  int a;
  
  temp = name;
  temp.erase(0,1);

  std::istringstream(temp) >> a;
 
  #ifdef DEBUGV3
    std::cout<<"a: "<<a<<" temp: "<<temp<<"\n";
  #endif
  return a;
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

void Brain::setParamInputLayer(int n, const std::vector<float>& weights, float theta) {
    inputLayer[n].setWeights(weights);
    inputLayer[n].setThreshold(theta);
}

void Brain::setParamOutputLayer(int n, const std::vector<float>& weights, float theta) {
    outputLayer[n].setWeights(weights);
    outputLayer[n].setThreshold(theta);
}

void Brain::setParamActiveNeuron(int n, const std::vector<float>& weights, float theta) {
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

std::vector<float> Brain::getAllParameters() {
  std::vector<float> params;
  std::vector<float> temp;
  
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

void Brain::setAllParameters(const std::vector<float>& params) {
  std::vector<float> weights;
  float theta;
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