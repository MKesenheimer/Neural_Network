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
  
  //new
  neur2->establishConnection(input, strneur1, output);
  #ifdef DEBUGV2
    std::cout<<"Neuron "<<strneur2<<" is connected on input i"<<input<<" with ";
    std::cout<<neur2->getConnectedNeuron(input)<<" at output "<<neur2->getConnectedOutput(input)<<"\n";
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
   
  //TODO: prüfe ob die größe des übergebenen vectors x der Anzahl der Neuronen im inputLayer entspricht
  //überprüfe außerdem ob alle neuronen im inputLayer und outputLayer verbunden sind
  
  //in matrix out[i][output] are stored vectors of outputs of calculated neurons with index i
  std::vector< std::vector<long double> > out;
  //this is the vector which is returned from calculation
  std::vector<long double> totalOut;
  //to store the output of current neuron
  std::vector<long double> outtemp;
  //to calculate and store the input of current neuron
  std::vector<long double> intemp;
  std::string finishedNeurons = ""; //store here the processed neurons
  int numberOfFinishedNeurons = 0;
  
  //there are two methods to determine wheter the neuron was calculated:
  //1.) put the calculated neuron on the finishedNeurons list
  //2.) set the finished-flag of each neuron
  //you can choose which methode you like better (one is mandatory, don't use both):
  #define USELIST
  //#define USEFLAG
  
  
  #ifdef USEFLAG
  //reset the "is-finished"-index of each neuron
  for (int n=0; n<ninputs; n++) {
    inputLayer[n].unsetfinished();
  }
  for (int n=0; n<nneurons; n++) {
    neuron[n].unsetfinished();
  }
  for (int n=0; n<noutputs; n++) {
    outputLayer[n].unsetfinished();
  }
  #endif

  //first go through the neurons in the inputLayer...
  for (int n=0; n<ninputs; n++) {
    //generate the input for the n-th neuron
    //inputLayer neurons have only one input
    std::vector<long double> in(&x[n],&x[n+1]);
    //calculate the output of the n-th neuron
    outtemp = inputLayer[n].calculateOutput(in);
    //store the number of the calculated neuron in string finishedNeurons
    #ifdef USELIST
    finishedNeurons += inputLayer[n].getNeuronName() + ", ";
    #endif
    #ifdef USEFLAG
    //DEBUG
    finishedNeurons += inputLayer[n].getNeuronName() + ", ";
    inputLayer[n].setfinished();
    #endif
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
      //keep track if the current neuron must be calculated
      bool calculateCNeur = true;
      //name of current neuron
      std::string strCNeur = neuron[n].getNeuronName();
      #ifdef DEBUGV3
        std::cout<<"current neuron "<<strCNeur<<"\n";
      #endif
      //is current neuron in the finishedNeurons list?
      //if so, next neuron
      #ifdef USELIST
      std::size_t found = finishedNeurons.find(strCNeur);
      if (found!=std::string::npos) {
      #endif
      #ifdef USEFLAG
      if (neuron[n].isfinished()) {
      #endif
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
        std::size_t posCNeur = 0;
        //commented to gained a better performance
        //connections.find(strCNeur+",i");
        if (posCNeur==std::string::npos) {
          #ifdef DEBUGV1
            std::cout<<" and could not be found as target in string connections. Skip.\n";
          #endif
          calculateCNeur = false;
        } else {
          #ifdef DEBUGV3
            std::cout<<" and is at position "<<posCNeur<<" in string connections.\n";
          #endif
          //which neurons are connected to the inputs of current neuron?
          #ifdef DEBUGV2
            std::cout<<"the inputs of neuron "<<strCNeur<<" are connected to "<<getConnectedNeuronName(&neuron[n],-1)<<"\n";
          #endif
          //are they in the finished list?
          //if one or more are not in the finished list, skip the current neuron and calculate it later
          for (int k=0; k<neuron[n].numberOfInputs(); k++) {
            #ifdef USELIST
            std::size_t posConnected = finishedNeurons.find(getConnectedNeuronName(&neuron[n],k));
            if (posConnected==std::string::npos) {
            #endif
            #ifdef USEFLAG
            if (!getConnectedNeuron(&neuron[n],k)->isfinished()) {
            #endif
              #ifdef DEBUGV3
                std::cout<<"neuron "<<strConnected<<", to which current neuron "<<strCNeur<<", is connected was not processed yet. Skip current neuron.\n";
              #endif
              //skip the following calculation and...
              calculateCNeur = false;
              //... skip the loop
              k=neuron[n].numberOfInputs()+1;
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
              #ifdef USELIST
              std::size_t posInFinished = finishedNeurons.find(getConnectedNeuronName(&neuron[n],k));
              if (posInFinished!=std::string::npos) {
                //count the colons that occur until connected neuron occurs
                std::string strSubFinished;
                strSubFinished = finishedNeurons.substr(0,posInFinished);
                #ifdef DEBUGV3
                  std::cout<<"string until connected neuron "<<getConnectedNeuronName(&neuron[n],k)<<": "<<strSubFinished<<"\n";
                #endif
                std::size_t nOccur = std::count(strSubFinished.begin(), strSubFinished.end(), ',');
                #ifdef DEBUGV3
                  std::cout<<"colon occurs "<<nOccur<<" times.\n";
                #endif
                int indexOfOutput = nameToInt(getConnectedOutput(&neuron[n],k));
                //build the input vector that is later returned to current neuron:
                intemp.push_back(out[nOccur][indexOfOutput]);
              }
              #endif
              #ifdef USEFLAG
              std::size_t posInFinished = finishedNeurons.find(getConnectedNeuronName(&neuron[n],k));
              if (getConnectedNeuron(&neuron[n],k)->isfinished()) {
                //count the colons that occur until connected neuron occurs
                std::string strSubFinished;
                strSubFinished = finishedNeurons.substr(0,posInFinished);
                #ifdef DEBUGV3
                  std::cout<<"string until connected neuron "<<getConnectedNeuronName(&neuron[n],k)<<": "<<strSubFinished<<"\n";
                #endif
                std::size_t nOccur = std::count(strSubFinished.begin(), strSubFinished.end(), ',');
                #ifdef DEBUGV3
                  std::cout<<"colon occurs "<<nOccur<<" times.\n";
                #endif
                int indexOfOutput = nameToInt(getConnectedOutput(&neuron[n],k));
                //build the input vector that is later returned to current neuron:
                intemp.push_back(out[nOccur][indexOfOutput]);
              }
              #endif
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
            #ifdef USELIST
            finishedNeurons += neuron[n].getNeuronName() + ", ";
            #endif
            #ifdef USEFLAG
            //DEBUG
            finishedNeurons += neuron[n].getNeuronName() + ", ";
            neuron[n].setfinished();
            #endif
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
      #ifdef USELIST
      std::size_t posInFinished = finishedNeurons.find(getConnectedNeuronName(&outputLayer[n],k));
      if (posInFinished!=std::string::npos) {
        //count the colons that occur until connected neuron occurs
        std::string strSubFinished;
        strSubFinished = finishedNeurons.substr(0,posInFinished);
        #ifdef DEBUGV3
          std::cout<<"string until connected neuron "<<getConnectedNeuronName(&outputLayer[n],k)<<": "<<strSubFinished<<"\n";
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
      #endif
      #ifdef USEFLAG
      std::size_t posInFinished = finishedNeurons.find(getConnectedNeuronName(&outputLayer[n],k));
      if (getConnectedNeuron(&outputLayer[n],k)->isfinished()) {
        //count the colons that occur until connected neuron occurs
        std::string strSubFinished;
        strSubFinished = finishedNeurons.substr(0,posInFinished);
        #ifdef DEBUGV3
          std::cout<<"string until connected neuron "<<getConnectedNeuronName(&outputLayer[n],k)<<": "<<strSubFinished<<"\n";
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
      #endif
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
    #ifdef USELIST
    finishedNeurons += outputLayer[n].getNeuronName() + ", ";
    #endif
    #ifdef USEFLAG
    //DEBUG
    finishedNeurons += outputLayer[n].getNeuronName() + ", ";
    outputLayer[n].setfinished();
    #endif
    numberOfFinishedNeurons++;
  }
  
  #ifdef DEBUGV2
    std::cout<<"total number of neurons: "<<nneurons+ninputs+noutputs<<"\n";
    std::cout<<"number of finished neurons: "<<numberOfFinishedNeurons<<"\n";
    std::cout<<"finished neurons: "<<finishedNeurons<<"\n";
  #endif
  
  return totalOut;
}

std::string Brain::getConnectedNeuronName(Neuron *neur, int iinput){
  
  //check if iinput was set correctly
  if (iinput >= (neur->numberOfInputs())) {
    std::cout<<"\nError: iinput is to large: "<<iinput<<"\n";
    exit(1);
  }
  
  //if iinput is negative, return all neurons that are conected to this neuron
  if (iinput<0) {
    std::string strNeurons = "";
    for (int i=0; i<neur->numberOfInputs(); i++) {
    strNeurons += neur->getConnectedNeuron(i);
    }
    return strNeurons;
  }

  return neur->getConnectedNeuron(iinput);
}

Neuron* Brain::getConnectedNeuron(Neuron *neur, int iinput){
  
  //check if iinput was set correctly
  if (iinput >= (neur->numberOfInputs())) {
    std::cout<<"\nError: iinput is to large: "<<iinput<<"\n";
    exit(1);
  }
  
  return nameToNeuron(neur->getConnectedNeuron(iinput));
}

std::string Brain::getConnectedOutput(Neuron *neur, int iinput) {
  return neur->getConnectedOutput(iinput);
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

Neuron* Brain::nameToNeuron(std::string name) {
  int ident = nameToInt(name);
  Neuron* temp;

  if (ident<ninputs) {
    temp = &inputLayer[ident];
  } else if (ident<ninputs+nneurons) {
    temp = &neuron[ident-ninputs];
  } else if (ident<ninputs+nneurons+noutputs) {
    temp = &outputLayer[ident-ninputs-nneurons];
  }
  
  if (temp->getIdentifier() != ident) {
    std::cout<<"error in nameToNeuron: wrong identifier:\n";
    std::cout<<"ident = "<<ident<<"\n";
    std::cout<<"temp  = "<<temp->getIdentifier()<<"\n";
    #ifdef DEBUGV1
      std::cout<<"name        = "<<name<<"\n";
      std::cout<<"neuron name = "<<temp->getNeuronName()<<"\n";
    #endif
    exit(1);
    return NULL;
  }
  
  return temp;
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