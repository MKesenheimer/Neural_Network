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
    //return the parameters of this neuron (weights, threshold)
    std::vector<long double> getParams();
    //establish connection to another neuron:
    //tell this function which input of current neuron
    //should be connected with output of neuron with name "neuronName"
    void establishConnection(int input, int neuronName, int output);
    //tell at which position of the vectors connectedN and connectedI
    //the dataset with index i could be found
    int reversePointer(int i);
    //return connected neuron which is connected to input
    int getConnectedNeuron(int input);
    //return connected output which is connected to input
    int getConnectedOutput(int input);

  private:
    int identifier;         //"name" of neuron
    int ninputs;            //number of inputs
    int noutputs;           //number of outputs
    long double theta;            //threshold
    std::vector<long double> i;   //inputs
    std::vector<long double> o;   //output
    std::vector<long double> w;   //weights
    std::vector<int> pointerToIndex;     //this vector points to the entries of connectedN and connectedI
    std::vector<int> connectedN; //the input with index i is connected to which neuron?
    std::vector<int> connectedO; //the input with index i is connected to which output?
};

#endif