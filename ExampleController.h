#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "neuralnetwork.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    ~Controller();

    /**
     * @brief compareNeuralNetwork
     * @param net1
     * @param net2
     * @return (net1 > net2)
     * Fuction is used as qSort argument for comparing
     */
    static bool compareNeuralNetwork(NeuralNetwork *net1, NeuralNetwork *net2);

public slots:

    void trainingDone();

private:
    /**
     * @brief get3champions
     * @param nets
     *
     * Removes all nets in QList but 3 best scored nets
     */
    void get3champions();

    /**
     * @brief fillPopulation
     * @param nets
     *
     * Fills QList with best neural network's copies
     */
    void fillPopulation();

    /**
     * @brief resetPopulation
     * @param net
     * Reset whole population
     */
    void resetPopulation(NeuralNetwork *net);

    /**
     * @brief train
     * train network
     */
    void train();

    int mNetworksDone;
    int mAttempts;

    int mPopulation;


    QList<NeuralNetwork*> nets;
    NeuralNetwork *bestNetwork;

    /**
     * @brief mutateNN
     * @param net
     * @param index
     * Mutate neural network index-times
     */
    void mutateNN(NeuralNetwork *net, int index);

};

#endif // CONTROLLER_H
