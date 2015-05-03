#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Neuron {
  public:
    Neuron(int identifier = -999, int ninputs = 3, int noutputs = 1); //default constructor
    ~Neuron();
    void setWeights(const std::vector<float>& w);
    void setThreshold(float theta);
    std::vector<float> calculateOutput(const std::vector<float>& x);
    int numberOfInputs();
    int numberOfOutputs();
    int getIdentifier();
    std::string getNeuronName();
    //return the parameters of this neuron (weights, threshold)
    std::vector<float> getParams();

  private:
    int identifier;         //"name" of neuron
    int ninputs;            //number of inputs
    int noutputs;           //number of outputs
    float theta;            //threshold
    std::vector<float> i;   //inputs
    std::vector<float> o;   //output
    std::vector<float> w;   //weights
};

#endif