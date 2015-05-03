#ifndef BRAIN_H
#define BRAIN_H

#include <vector>
#include "Neuron.h"

class Brain {
  public:
    //construct Brain with number of neurons and number of layers
    Brain(int nneur, int ninputs, int noutputs);
    void initBrain();
    ~Brain();
    
    //calculates the output
    std::vector<float> output(const std::vector<float>& x);
    //connects output of neur1 with input of neuron neur2
    void connectNeurons(Neuron *neur1, int output, Neuron *neur2, int input);
    //extract the neuron that is connected on input iinput of neuron neur
    std::string getConnectedNeuron(Neuron *neur, int iinput);
    //extract the output of the neuron that is connected on input iinput of neuron neur
    std::string getConnectedOutput(Neuron *neur, int iinput);
    //takes neuron name, input or output name and returns integer (example: n0 -> 0, i1 -> 1, o2 -> 2)
    int nameToInt(std::string name);
    
    //return the number of inputs, putputs and neurons
    int numberOfInputs();
    int numberOfOutputs();
    int numberOfNeurons();
    
    //add neurons to the brain
    void addInputLayerNeuron(int identifier, int nouts);
    void addOutputLayerNeuron(int identifier, int nins);
    void addActiveNeuron(int identifier, int nins, int nouts);
    
    //set the weight and the theta function of neuron with identifier n
    void setParamInputLayer(int n, const std::vector<float>& weights, float theta);
    void setParamOutputLayer(int n, const std::vector<float>& weights, float theta);
    void setParamActiveNeuron(int n, const std::vector<float>& weights, float theta);
    
    //the Trainer can request and set all the weights and thresholds
    std::vector<float> getAllParameters();
    //the vector params must have the same layout as the vector that is returned by getAllParameters()
    void setAllParameters(const std::vector<float>& params);
    
  private:
    int nneurons;
    int ninputs;
    int noutputs;
    std::vector<Neuron> neuron;
    std::vector<Neuron> inputLayer;
    std::vector<Neuron> outputLayer;
    std::string connections; //vector of connections, whose entries points to the next input iinput of neuron ineur
};

#endif