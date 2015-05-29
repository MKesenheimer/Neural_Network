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
    //establish connection to another neuron:
    //tell this function which input of current neuron
    //should be connected with output of neuron with name "neuronName"
    void establishConnection(int input, std::string neuronName, int output);
    //tell at which position of the vectors connectedN and connectedI
    //the dataset with index i could be found
    int reversePointer(int i);
    //return connected neuron which is connected to input
    std::string getConnectedNeuron(int input);
    //return connected output which is connected to input
    std::string getConnectedOutput(int input);
    
    //were the outputs of this neuron already calculated (in Brain::output)
    bool isfinished();
    //set if the outputs were calculated
    void setfinished();
    //unset if the outputs were not calculated yet
    void unsetfinished();

  private:
    //"name" of neuron
    int identifier;
    //number of inputs
    int ninputs;
    //number of outputs
    int noutputs;
    //threshold
    long double theta;
    //inputs
    std::vector<long double> i;
    //output
    std::vector<long double> o;
    //weights
    std::vector<long double> w;
    //this vector points to the entries of connectedN and connectedI
    std::vector<int> pointerToIndex;
    //the input with index i is connected to which neuron?
    std::vector<std::string> connectedN;
    //the input with index i is connected to which output?
    std::vector<std::string> connectedO;
    //were the outputs of this neuron already calculated (in Brain::output)
    bool finished;
};

#endif