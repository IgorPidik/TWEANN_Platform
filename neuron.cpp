#include "neuron.h"
#include "neuralnetwork.h"

Neuron::Neuron(int id, double layer, QObject *parent) : QObject(parent)
{
    mId = id;
    mLayer = layer;
    mBaseNumInputs = 0;
    mNumInputs = 0;
    mOutput = 0;
    mActivationFunction = TANH;
    mNetwork = qobject_cast<NeuralNetwork*>(parent);
    qDebug() << "[*] New neuron" << "layer" << layer << "id" << id;
}

Neuron::~Neuron()
{
    qDebug() << "[*] Neuron destructor";
}

double Neuron::layer()
{
    return mLayer;
}

int Neuron::numConnections()
{
    return mBaseNumInputs;
}

void Neuron::setNumConnections(int numConnections)
{
    mBaseNumInputs = numConnections;
    mNumInputs = mBaseNumInputs;
}

void Neuron::init(double input)
{
    mNumInputs--;
    mOutput += input;

    qDebug() << "[*] Neuron" << mId << "waiting for " << mNumInputs << "signals from base " << mBaseNumInputs;
    if(mNumInputs == 0)
    {
        mNumInputs = mBaseNumInputs;
        emit proccesed(input);
    }
}

int Neuron::id()
{
    return mId;
}

void Neuron::reset()
{
    mNumInputs = mBaseNumInputs;
    mOutput = 0;
}

void Neuron::mutateActivationFunction()
{
    ActivationFunction function = static_cast<ActivationFunction>(qrand() % LAST);
    mActivationFunction = function;

}

void Neuron::input(double input)
{
     if(mNumInputs != 0)
        mNumInputs--;

    qDebug() << "[*] Neuron "<< mId << "waiting for" << mNumInputs << "signals from base " << mBaseNumInputs << " layer " << layer() << "sender layer";
    float weight = mNetwork->connectionManager()->getConnectionWeight(qobject_cast<Neuron*>(sender()), this);
    qDebug() << "[*] Weight:" << weight;

     mOutput += input*weight;
     if(mNumInputs == 0)
     {
         mNumInputs = mBaseNumInputs;
         double output = activationFunction(mOutput);
         mOutput = 0;
         emit proccesed(output);
     }
}

double Neuron::activationFunction(double x)
{
    switch (mActivationFunction)
    {
        case TANH:
            return tanh(x);
            break;
        case SIGMOID:
            return sigmoid(x);
            break;
        case COS:
            return cos(x);
            break;
        case SIN:
            return sin(x);
            break;
    //    case GAUSS:
    //        return guess(x);

        default:
            qDebug() << "[*] Error in actiovation functions";
            throw "error";
            break;
    }
}

double Neuron::sigmoid(double x)
{
    if(x < -45)
        return 0;
    if(x > 45)
        return 1;
    return (1/(1+exp(-x)));
}

double Neuron::gauss(double x)
{
    qDebug() << "Not implemented yet";
}

