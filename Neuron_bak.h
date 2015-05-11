#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Neuron {
  public:
    Neuron(int identifier = -999, int ninputs = 3, int noutputs = 1); //default constructor
    ~Neuron();
    void setWeights(const std::vector<long double>& w);
    void setThreshold(long double theta);
    std::vector<long double> calculateOutput(const std::vector<long double>& x);
    int numberOfInputs();
    int numberOfOutputs();
    int getIdentifier();
    std::string getNeuronName();
    //return the parameters of this neuron (weights, threshold)
    std::vector<long double> getParams();

  private:
    int identifier;         //"name" of neuron
    int ninputs;            //number of inputs
    int noutputs;           //number of outputs
    long double theta;            //threshold
    std::vector<long double> i;   //inputs
    std::vector<long double> o;   //output
    std::vector<long double> w;   //weights
};

#endif