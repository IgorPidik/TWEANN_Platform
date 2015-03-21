#ifndef NeuralNetwork_H
#define NeuralNetwork_H

#include <QObject>
#include <QDebug>
#include "connections.h"
#include "iscapeinterface.h"

typedef QList<Neuron*> Layer;

class NeuralNetwork : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief The Mutation enum
     *
     * Possible mutation actions
     */
    enum Mutation
    {
        ADD_NEURON,
        ADD_CONNECTION,
        MUTATE_WEIGHT,
        MUTATE_ACTIVATION_FUNCTION  ,
        LAST // @see randomMutation, otherwise useless
    };

    /**
     * @brief The Mode enum
     *
     * Possible modes of neural network
     */
    enum Mode
    {
        FEEDFORWARD,
        RECURRENT
    };

    explicit NeuralNetwork(QObject *parent = 0);
    ~NeuralNetwork();

    /**
     * @brief construct neural network
     * @param topology - the topology of the neural network
     *
     * Constructs neural network
     */
    void construct(QVector<int> &topology);

    /**
     * @brief connectionManager
     * @return pointer to Connections class
     *
     * Returns a pointer to Connection class
     */
    Connections *connectionManager();

    Mutation randomMutation();

    /**
     * @brief mutate
     * @param mutation - perform certain mutation
     *
     * Mutates neural network
     */
    void mutate(Mutation mutation);

    /**
     * @brief feed
     * @param input
     *
     * Feeds input from Controller class
     */
    void feed(QList<double> &input);

    /**
     * @brief neurons
     * @return list of neurons
     *
     * Returns list of neurons(mNeurons)
     */
    QList<Neuron*> neurons();

    /**
     * @brief copy
     * @return pointer to new neural network
     *
     * Creates new neural network with same topology and connections and returns it's pointer
     */
    NeuralNetwork* copy();

    /**
     * @brief addNeuron
     * @param neuron
     *
     * Adds neuron to mNeurons, used when copying neural network
     */
    void addNeuron(Neuron *neuron);

    /**
     * @brief addLayer
     * @param layer
     *
     * Adds layer to mLayersm used when coping neural network
     */
    void addLayer(Layer layer);

    /**
     * @brief copyConnections
     * @param connections
     *
     * Makes connections in copied neural network
     */
    void copyConnections(QList<Connection> &connections);

    /**
     * @brief setMode
     * @param mode
     * Sets mode of neural network
     */
    void setMode(Mode mode);

    /**
     * @brief mode
     * @return mode
     * Returns neural network's mode
     */
    Mode mode();

    /**
     * @brief setScore
     * @param score
     * Sets neural network's score
     * @bug useless
     */
    void setScore(double score);

    /**
     * @brief score
     * @return
     * Returns neural network's score
     */
    double score();

    /**
     * @brief setScape
     * @param scape
     * Sets train scope for neural network
     */
    void setScape(IScapeInterface *scape);

    IScapeInterface *scape();

    /**
     * @brief scoreNN
     * @param attempts
     * Scores Neural network, setScape(IScapeInterface *scape) must be called first
     */
    void scoreNN(int attempts);

    /**
     * @brief output
     * @return list of doubles
     *
     * Returns neural network's last output
     */
    QList<double> output();

    void setLastId(int id) {mIdForNeuron = id;}


signals:
    /**
     * @brief trainingDone
     * @see scoreNN
     *
     * Emitted when scoring is done
     */
    void trainingDone();

    /**
     * @brief newNeuralNetworkOutput
     * @param outputs
     * Emitted when network gets outputs from all neurons in last layer
     */
    void newNeuralNetworkOutput(QList<double> outputs);

public slots:

    /**
     * @brief newNeuronOutput
     * @param output
     *
     * Called from neurons in last layer when processing is done
     */
    void newNeuronOutput(double output);

private slots:

    /**
     * @brief neuralNetworkOutput
     * @param outputs
     * Called when processing is done
     */
    void neuralNetworkOutput(QList<double> outputs);


private:

    /**
     * @brief mutateActivationFunction
     *
     *
     */
    void mutateActivationFunction();

    /**
     * @brief testNN
     *
     * Feeds neural network with input mAttempts-times
     */
    void testNN();

    /**
     * @brief initANN
     *
     * Inits recurrent connections in neural network
     */
    void initANN();

    /**
     * @brief resetANN
     * Resets neural network
     */
    void resetANN();

    /**
     * @brief addConnection
     * @see mutate(Mutation mutation)
     *
     * Adds connection in neural network
     */
    void addConnection();

    /**
     * @brief addConnection
     * @param from
     * @param to
     * @see Connection class -> addConnection
     * Adds connection from neuron to neuron
     * equivalent to mConnectionsManager->addConnection(Neuron *from, Neuron *to);
     */
    void addConnection(Neuron *from , Neuron *to);

    /**
     * @brief addConnectionFrom
     * @param from
     * Adds connection from passed neuron to random neuron
     */
    void addConnectionFrom(Neuron *from);

    /**
     * @brief addConnectionTo
     * @param to
     * Adds connection from random neuron to passed neuron
     */
    void addConnectionTo(Neuron *to);

    /**
     * @brief addNeuron
     * @see mutate(Mutation mutation)
     *
     * Adds neuron to neural network
     */
    void addNeuron();

    /**
     * @brief mutateWeight
     * @see mutate(Mutation mutation)
     *
     * Generates new weight for connection
     */
    void mutateWeight();

    /**
     * @brief connectNeuralNewtork
     *
     * Connects initial neurons
     */
    void connectNeuralNewtork();

    /**
     * @brief getIdForNeuron
     * @return id for next neuron
     *
     * Returns increased id for next neuron
     */
    int getIdForNeuron();


    int mIdForNeuron;

    Connections *mConnectionsManager;
    QList<Layer> mLayers;
    QList<Neuron*> mNeurons;
    Mode mMode;
    double mScore;

    IScapeInterface *mScape;

    QList<double> mTrainingInputs;

    /**
     * @brief mTraining
     * @see scoreNN(int atempts)
     *
     * Used when scoing neural network
     */
    bool mTraining;

    /**
     * @brief mInit
     * Needed in case initing neurons feeds neural network
     */
    bool mInit;

    int mBaseAttempts;

    int mAttempts;

    /**
     * @brief mOutput
     * Stores outputs from last layer's neurons
     */
    QList<double> mOutput;

};

#endif // NeuralNetwork_H
