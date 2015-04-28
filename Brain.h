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
      //extract from string the iinput-th connection which neurons are connected with neur on the inputs
      std::string getConnectedNeuron(Neuron *neur, int iinput);
      //extract the output of neuron2 that is connected to neuron1 on input iinput
      std::string getConnectedOutput(Neuron *neuron1, int iinput);
      //takes neuron name, input or output name and returns integer (example: n0 -> 0, i1 -> 1, o2 -> 2)
      int nameToInt(std::string name);
      //train the brain (set input and desired output)
      void train(const std::vector<float>& input, const std::vector<float>& output);
      
      //return the number of inputs, putputs and neurons
      int numberOfInputs();
      int numberOfOutputs();
      int numberOfNeurons();
      
      //add neurons to the brain
      void addInputLayerNeuron(int identifier, int nouts);
      void addOutputLayerNeuron(int identifier, int nins);
      void addActiveNeuron(int identifier, int nins, int nouts);
      
      //set the weight and the theta function of neuron with identifier id
      void setParamInputLayer(int n, const std::vector<float>& weights, float theta);
      void setParamOutputLayer(int n, const std::vector<float>& weights, float theta);
      void setParamActiveNeuron(int n, const std::vector<float>& weights, float theta);
      
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