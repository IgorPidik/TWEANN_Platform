#include "neuralnetwork.h"

NeuralNetwork::NeuralNetwork(QObject *parent) : QObject(parent)
{
    mNeurons.clear();
    mIdForNeuron = 0;
    mConnectionsManager = new Connections(this);
    mMode = FEEDFORWARD;
    mScore = 0;
    mTraining = false;
    mAttempts = 0;

    connect(this, &NeuralNetwork::newNeuralNetworkOutput, this, &NeuralNetwork::neuralNetworkOutput, Qt::QueuedConnection);
}

NeuralNetwork::~NeuralNetwork()
{

}

void NeuralNetwork::construct(QVector<int> &topology)
{
    // i = layer index
    for(int i = 0; i < topology.length(); i++)
    {
        Layer layer;
        for (int j = 0; j < topology.at(i); j++)
        {
            if(!(topology.length() == i+1))
            {
                Neuron *neuron = new Neuron(getIdForNeuron(), (double)i, this);
                layer.append(neuron);
                mNeurons.append(neuron);
            }
            else
            {
                //Last layer neurons , no connections from this neurons only back to neural network
                Neuron *neuron = new Neuron(getIdForNeuron(), (double)i, this);
                layer.append(neuron);
                mNeurons.append(neuron);
            }
        }
        mLayers.append(layer);
    }
    foreach (Neuron *neuron, mLayers.first())
    {
        neuron->setNumConnections(1);
    }
    connectNeuralNewtork();

}

Connections *NeuralNetwork::connectionManager()
{
    return mConnectionsManager;
}

NeuralNetwork::Mutation NeuralNetwork::randomMutation()
{
    return static_cast<Mutation>(qrand() % LAST);
}

void NeuralNetwork::mutate(NeuralNetwork::Mutation mutation)
{
    switch (mutation)
    {
        case ADD_CONNECTION:
            qDebug() << "mutation #### ADD_CONNECTION";
            addConnection();
            break;
        case ADD_NEURON:
            qDebug() << "mutation #### ADD_NEURON";
            addNeuron();
            break;
        case MUTATE_WEIGHT:
            qDebug() << "mutation #### MUTATE_WEIGHT";
            mutateWeight();
            break;

        case MUTATE_ACTIVATION_FUNCTION:
            qDebug() << "mutaion #### MUTATE_ACTIVATION_FUCTION";
            mutateActivationFunction();
            break;

        default:
            throw "error";
            qDebug() << "mutate(): error switch in default";
            break;
    }
    resetANN();
}

void NeuralNetwork::feed(QList<double> &input)
{
    mInit = false;
    mOutput.clear();

    qDebug() << "################################### feed";
    if(input.length() == mLayers.first().length())
    {
        for(int i = 0; i < mLayers.first().length(); i++)
        {

            mLayers.first().at(i)->init(input.at(i));
        }
    }
    else
    {
        qDebug() << "Wrong length of input";
        throw "error";
    }
}

QList<Neuron *> NeuralNetwork::neurons()
{
    return mNeurons;
}

NeuralNetwork *NeuralNetwork::copy()
{
    qDebug() << "################ Copying neural network";

    NeuralNetwork *net = new NeuralNetwork(parent());
    net->setLastId(getIdForNeuron()-1);
    for(int i = 0; i < mLayers.length(); i++)
    {
        Layer layer = mLayers.at(i);
        Layer copiedLayer;
        for(int j = 0; j < layer.length(); j++)
        {
            Neuron *neuron = layer.at(j);
            Neuron *copiedNeruon = new Neuron(neuron->id(), neuron->layer(), net);
            net->addNeuron(copiedNeruon);
            copiedLayer.append(copiedNeruon);

            if(i == 0) // we need to set initial num of connection for input layer, adding connections will take care about remaining neurons
            {
                copiedNeruon->setNumConnections(neuron->numConnections());
            }

            if(mLayers.length() == i+1) // if we are in last layer then add connection to neural network output slot
            {
                connect(copiedNeruon, &Neuron::proccesed, net, &NeuralNetwork::newNeuronOutput, Qt::QueuedConnection);
            }
        }
        net->addLayer(copiedLayer);
    }

    net->setScape(mScape->getNewInstance(net));
    net->setMode(mode());
    QList<Connection> connection = mConnectionsManager->connections();
    net->copyConnections(connection);

    qDebug() << "################ Copying done";

    return net;
}

void NeuralNetwork::addNeuron(Neuron *neuron)
{
    mNeurons.append(neuron);
}

void NeuralNetwork::addLayer(Layer layer)
{
    mLayers.append(layer);
}

void NeuralNetwork::copyConnections(QList<Connection> &connections)
{
    foreach (Connection connection, connections)
    {
        int fromNeuronId = connection.from->id();
        int toNeuronId = connection.to->id();
        Neuron *from = 0;
        Neuron *to = 0;
        foreach (Neuron *neuron, mNeurons)
        {
            if(neuron->id() == fromNeuronId)
                from = neuron;
            else if(neuron->id() == toNeuronId)
                to = neuron;
        }
        if(from == 0 || to == 0)
        {
            qDebug() << "error copy" << fromNeuronId << toNeuronId;
            throw "error copy";
        }

        mConnectionsManager->addConnection(from, to, connection.weight);
    }
}

void NeuralNetwork::setMode(Mode mode)
{
    mMode = mode;
}

NeuralNetwork::Mode NeuralNetwork::mode()
{
    return mMode;
}

void NeuralNetwork::setScore(double score)
{
    mScore = score;
}

double NeuralNetwork::score()
{
    return mScore;
}

void NeuralNetwork::setScape(IScapeInterface *scape)
{
    mScape = scape;
}

IScapeInterface *NeuralNetwork::scape()
{
    return mScape;
}

void NeuralNetwork::scoreNN(int attempts)
{
    resetANN();
    qDebug() << "init done";
    mInit = false;
    mTraining = true;
    mScore = 0;
    mTrainingInputs.clear();
    mBaseAttempts = attempts;
    mAttempts = attempts;
    testNN();


}

QList<double> NeuralNetwork::output()
{
    return mOutput;
}

void NeuralNetwork::newNeuronOutput(double output)
{

    mOutput.append(output);
    if(mOutput.length() == mLayers.last().length())
    {
        emit newNeuralNetworkOutput(mOutput);
    }

}

void NeuralNetwork::neuralNetworkOutput(QList<double> outputs)
{

    if(!mInit)
    {
        foreach (double output, outputs)
        {
            qDebug() << "output" << output;

            qDebug() << "########################################################";

        }
    }

    if(mTraining && !mInit)
    {
        qDebug() << "input" << mTrainingInputs << " score" << mScape->getScore(mTrainingInputs, outputs);
        mScore += mScape->getScore(mTrainingInputs, outputs);

        testNN();
    }

}

void NeuralNetwork::mutateActivationFunction()
{
    int numNeuron = qrand() % mNeurons.length();
    if(numNeuron > (mNeurons.length() / 2))
        numNeuron = round((mNeurons.length() / 2));
    for(int i = 0; i< numNeuron; i++)
    {
        int randomNeuron = qrand() % mNeurons.length();
        Neuron *neuron = mNeurons.at(randomNeuron);
        neuron->mutateActivationFunction();
    }
}

void NeuralNetwork::testNN()
{
    mTrainingInputs.clear();

    if(mAttempts == 0)
    {
        mScore = mScore/mBaseAttempts;
        mTraining = false;
        emit trainingDone();
    }
    else
    {
        mAttempts--;
        mTrainingInputs = mScape->getInput();
        foreach (double input, mTrainingInputs)
        {
            qDebug() << "input" << input;
        }
        this->feed(mTrainingInputs);
    }


}

void NeuralNetwork::initANN()
{
    mConnectionsManager->initRecurrentNeurons();
}

void NeuralNetwork::resetANN()
{
    mInit = true;
    foreach (Neuron *neuron, mNeurons)
    {
        neuron->reset();
    }
    qDebug() << "initing";
    initANN();
}

void NeuralNetwork::addConnection()
{

    //get 2 random neurons and connect them
    if(mMode == RECURRENT)
    {

        Neuron *from = mNeurons.at(qrand() % mNeurons.size());
        Neuron *to = mNeurons.at(qrand() % mNeurons.size());
        while(from->layer() == to->layer())
        {
            from = mNeurons.at(qrand() % mNeurons.size());
            to = mNeurons.at(qrand() % mNeurons.size());
        }
        addConnection(from, to);
    }
    else
    {
        Neuron *from = mNeurons.at(qrand() % mNeurons.size());

        while(from->layer() == mLayers.last().first()->layer())
            from = mNeurons.at(qrand() % mNeurons.size());

        Neuron *to = mNeurons.at(qrand() % mNeurons.size());
        while(to->layer() <= from->layer())
            to = mNeurons.at(qrand() % mNeurons.size());

        addConnection(from, to);
        qDebug() << "add connection from layer" << from->layer() << "to layer" << to->layer();


    }


}

void NeuralNetwork::addConnection(Neuron *from, Neuron *to)
{
    mConnectionsManager->addConnection(from, to);
}

void NeuralNetwork::addConnectionFrom(Neuron *from)
{

    if(mMode == RECURRENT)
    {

        Neuron *to = mNeurons.at(qrand() % mNeurons.size());
        while(from->layer() == to->layer())
            to = mNeurons.at(qrand() % mNeurons.size());

        addConnection(from, to);
    }
    else
    {
        Neuron *to = mNeurons.at(qrand() % mNeurons.size());
        while(to->layer() <= from->layer())
            to = mNeurons.at(qrand() % mNeurons.size());

        addConnection(from, to);
        qDebug() << "add connection from layer" << from->layer() << "to layer" << to->layer();


    }

}

void NeuralNetwork::addConnectionTo(Neuron *to)
{
    if(mMode == RECURRENT)
    {

        Neuron *from = mNeurons.at(qrand() % mNeurons.size());
        while(from->layer() == to->layer())
            from = mNeurons.at(qrand() % mNeurons.size());

        addConnection(from, to);
    }
    else
    {
        Neuron *from = mNeurons.at(qrand() % mNeurons.size());

        while(from->layer() == mLayers.last().first()->layer() || from->layer() >= to->layer())
            from = mNeurons.at(qrand() % mNeurons.size());

        addConnection(from, to);
    }
}

void NeuralNetwork::addNeuron()
{
    // choose random layer 1-lastLayer
    int randomLayerIndex = qrand() % mLayers.size();
    while(randomLayerIndex == mLayers.indexOf(mLayers.last()) || randomLayerIndex == 0)
        randomLayerIndex = qrand() % mLayers.size();

    Neuron *neuron = new Neuron(getIdForNeuron(), (double)randomLayerIndex, this);
    Layer layer = mLayers.at(randomLayerIndex);
    layer.append(neuron);
    mNeurons.append(neuron);
    mLayers.replace(randomLayerIndex, layer);

    addConnectionTo(neuron);
    addConnectionFrom(neuron);
//    addConnection(mLayers.last().first(), neuron);
//    addConnection(neuron, mLayers.first().first());


}

void NeuralNetwork::mutateWeight()
{
    //choose random connection change weight
    int length = mConnectionsManager->connections().length();


    int random = (qrand() % length);

    qDebug() << "random connection id" <<random << "rec connection" << mConnectionsManager->numRecurrentConnection();
    Connection connection = mConnectionsManager->connections().at(random);
    mConnectionsManager->setWeight(connection.from, connection.to, mConnectionsManager->randomWeight());
    qDebug() << "rec connection" << mConnectionsManager->numRecurrentConnection();
}

void NeuralNetwork::connectNeuralNewtork()
{
    for(int i = 0; i < mLayers.length(); i++)
    {
        if(!(mLayers.length() == (i+1)))
        {

            for(int j = 0; j < mLayers.at(i).length(); j++)
            {
                foreach (Neuron *to, mLayers.at(i+1))
                {
                    Neuron *from = mLayers.at(i).at(j);
                    addConnection(from, to);

                }
            }
        }
        else if(mLayers.length() == (i+1))
        {
            foreach (Neuron *neuron, mLayers.at(i))
            {
                connect(neuron, &Neuron::proccesed, this, &NeuralNetwork::newNeuronOutput, Qt::QueuedConnection);
            }
        }
    }
}

int NeuralNetwork::getIdForNeuron()
{
    mIdForNeuron +=1;
    return mIdForNeuron;
}
