#ifndef NEURON_H
#define NEURON_H


class NeuralNetwork;

#include <QObject>
#include <QtCore>


class Neuron : public QObject
{
    Q_OBJECT
public:
    explicit Neuron(int id, double layer, QObject *parent = 0);
    ~Neuron();

    /**
     * @brief The ActivationFunction enum
     *
     * Possible activation functions
     */
    enum ActivationFunction
    {
        TANH,
        COS,
        SIN,
        SIGMOID,
    //    GAUSS,
        LAST // used for random
    };

    /**
     * @brief layer
     * @return layerIndex
     *
     * Returns layer's index
     */
    double layer();

    /**
     * @brief numConnections
     * @return count of connections from this neuron
     *
     * Returns count of connections from this neuron
     */
    int numConnections();

    /**
     * @brief setNumConnections
     * @param numConnections
     *
     * Sets count of connections when a connection is added or deleted
     */
    void setNumConnections(int numConnections);

    /**
     * @brief init
     * @param input
     * Feeds input to NN and init neuron if is recurrent
     */
    void init(double input);

    /**
     * @brief id
     * @return neuron's id
     *
     * Returns neurons's id
     */
    int id();

    /**
     * @brief reset
     *
     * Resets neuron
     */
    void reset();

    void mutateActivationFunction();

signals:

    /**
     * @brief proccesed
     * @param output
     * @see input(double input)
     * Emited when neuron got all inputs to sent output to next neurons
     */
    void proccesed(double output);


public slots:

    /**
     * @brief input
     * @param input
     * @see proccesed(double output)
     *
     * Gets input from connected neurons
     */
    void input(double input);

private:
    /**
     * @brief activationFunction
     * @param x
     * @return output after activation function
     *
     * Returns value after processing by activation function
     */
    double activationFunction(double x);

    /**
     * @brief sigmoid
     * @param x
     * @return value of sigmoid activation fucntion
     */
    double sigmoid(double x);

    /**
     * @brief gauss
     * @param x
     * @return value for gauss activation function
     */
    double gauss(double x);

    int mId;
    double mLayer;
    int mNumInputs;
    int mBaseNumInputs;
    double mOutput;

    ActivationFunction mActivationFunction;

    NeuralNetwork *mNetwork;

};

#endif // NEURON_H
