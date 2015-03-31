#include "ExampleController.h"
#include <QTime>
#include <QtAlgorithms>
#include "ExampleScape.h"
#include <iostream>

Controller::Controller(QObject *parent) : QObject(parent)
{
    qDebug() << "starting conntroller";
    mPopulation = 20;
    mMutationAttempts = 0;
    mNetworksDone = mPopulation;
    NeuralNetwork *net = new NeuralNetwork(this);

    net->setScore(0);
    connect(net, &NeuralNetwork::trainingDone, this, &Controller::trainingDone, Qt::QueuedConnection);

    //set base topology for neural network
    QVector<int> vec;
    vec.append(2);
    vec.append(1);
    vec.append(1);
    net->construct(vec);

    //set network's training scape
    Scape *scape = new Scape(net);
    net->setScape(scape);

    // var will store pointer to best neural network
    bestNetwork = net->copy();

    nets.append(net);

    //fill population to mPopulation
    fillPopulation();

    //start with training
    train();
}

Controller::~Controller()
{
    qDebug() << "stopping conroller";
}

bool Controller::compareNeuralNetwork(NeuralNetwork *net1, NeuralNetwork *net2)
{
    return (net1->score() > net2->score());
}

void Controller::trainingDone()
{
    qDebug() << "done" << --mNetworksDone;

    if(mNetworksDone == 0)
    {
        qDebug() << "all done";
        qSort(nets.begin(), nets.end(), compareNeuralNetwork);
        if(nets.first()->score() > 0.95 )
        {
            qDebug() << "score > 0.9" << "score" << nets.first()->score();

        }
        else
        {
            mNetworksDone = mPopulation;
            if(nets.first()->score() < bestNetwork->score() || nets.first()->score() == bestNetwork->score())
            {
                qDebug() << "######################################reset population";

                resetPopulation(bestNetwork);
            }
            else
            {
                qDebug() << "bestNet score" << nets.first()->score() << bestNetwork->score();
                bestNetwork = nets.first()->copy();
                bestNetwork->setScore(nets.first()->score());

            }
            train();
        }
    }

}

void Controller::get3champions()
{
    qSort(nets.begin(), nets.end(), compareNeuralNetwork);
    while(nets.size() != 3)
    {
        nets.removeLast();
    }
}

void Controller::fillPopulation()
{
    while(nets.size() != mPopulation)
    {
        NeuralNetwork *net = nets.first()->copy();
        net->setScore(0);
        connect(net, &NeuralNetwork::trainingDone, this, &Controller::trainingDone, Qt::QueuedConnection);
        nets.append(net);

    }
}

void Controller::resetPopulation(NeuralNetwork *net)
{
    nets.clear();
    nets.append(net->copy());
    fillPopulation();
}

void Controller::train()
{
    mMutationAttempts++;
    get3champions();
    fillPopulation();
    foreach (NeuralNetwork *net, nets)
    {
        mutateNN(net, (round(net->neurons().length()/3)));
        net->scoreNN(10);
    }


}


void Controller::mutateNN(NeuralNetwork *net, int index)
{
    for(int i = 0; i < index; i++)
    {
        if((qrand() % 2) == 0)
            net->mutate(NeuralNetwork::MUTATE_WEIGHT);
        else
            net->mutate(net->randomMutation());
    }
}

