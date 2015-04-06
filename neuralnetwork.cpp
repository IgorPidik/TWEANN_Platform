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
    mInit = true;
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
            Neuron *neuron = new Neuron(getIdForNeuron(), (double)i, this);
            layer.append(neuron);
            mNeurons.append(neuron);
        }
        mLayers.append(layer);
    }
    foreach (Neuron *neuron, mLayers.first())
    {
        neuron->setNumConnections(1);
    }
    connectNeuralNewtork();

}

void NeuralNetwork::construct(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file: "<< filePath;
        return;
    }
    QTextStream in(&file);
    QString stringTopology = in.readLine();
    QStringList list = stringTopology.split(" ");
    list.removeLast();
    QVector<int> topology;
    foreach(QString neurons, list)
    {
        topology.append(neurons.toInt());
    }
    for(int i = 0; i < topology.length(); i++)
    {
        Layer layer;
        for (int j = 0; j < topology.at(i); j++)
        {
            int id = in.readLine().toInt();
            Neuron *neuron = new Neuron(id, (double)i, this);
            layer.append(neuron);
            mNeurons.append(neuron);
        }
        mLayers.append(layer);
    }
    foreach (Neuron *neuron, mLayers.first())
    {
        neuron->setNumConnections(1);
    }
    while (!in.atEnd())
    {
        QStringList connection = in.readLine().split(" ");
        int idFrom = connection.at(0).toInt();
        int idTo = connection.at(1).toInt();
        double weight = connection.at(2).toDouble();
        qDebug() << "connect" << idFrom << idTo << weight;
        addConnection(idFrom, idTo, weight);
    }
    foreach(Neuron *neuron, mLayers.last())
    {
        connect(neuron, &Neuron::proccesed, this, &NeuralNetwork::newNeuronOutput);
    }

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
            qDebug() << "[*] Mutation #### ADD_CONNECTION";
            addConnection();
            break;
        case ADD_NEURON:
            qDebug() << "[*] Mutation #### ADD_NEURON";
            addNeuron();
            break;
        case MUTATE_WEIGHT:
            qDebug() << "[*] Mutation #### MUTATE_WEIGHT";
            mutateWeight();
            break;

        case MUTATE_ACTIVATION_FUNCTION:
            qDebug() << "[*] Mutaion #### MUTATE_ACTIVATION_FUCTION";
            mutateActivationFunction();
            break;

        default:
            qDebug() << "[*] Error: mutate(): error switch in default";
            throw "error";
            break;
    }
    resetANN();
}

void NeuralNetwork::feed(QList<double> &input)
{
    mInit = false;
    mOutput.clear();

    qDebug() << "[*] Feeding";
    if(input.length() == mLayers.first().length())
    {
        for(int i = 0; i < mLayers.first().length(); i++)
        {

            mLayers.first().at(i)->init(input.at(i));
        }
    }
    else
    {
        qDebug() << "[*] Wrong length of input";
        throw "error";
    }
}

QList<Neuron *> NeuralNetwork::neurons()
{
    return mNeurons;
}

NeuralNetwork *NeuralNetwork::copy()
{
    qDebug() << "[*] Copying neural network";

    NeuralNetwork *net = new NeuralNetwork(parent());

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

    qDebug() << "[*] Copying done";

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
        addConnection(fromNeuronId, toNeuronId, connection.weight);
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
    qDebug() << "[*] Init done";
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

void NeuralNetwork::saveNN(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "[*] Error, cannot open file: "<< filePath;
        return;
    }
    QVector<int> topology;
    foreach (Layer layer, mLayers)
    {
        topology.append(layer.length());
    }

    QTextStream out(&file);
    foreach(int neurons, topology)
    {
        out << neurons << " ";
    }
    out << "\n";
    foreach(Neuron *neuron, mNeurons)
    {
        out << neuron->id() << "\n";
    }
    foreach (Connection connection, mConnectionsManager->connections())
    {
        out << connection.from->id() << " " << connection.to->id() << " " << connection.weight << "\n";
    }
    out.flush();
    file.close();

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
    if(mTraining && !mInit)
    {
        qDebug() << "[*] Input: " << mTrainingInputs << "Output: "<< outputs << "Score: " << mScape->getScore(mTrainingInputs, outputs);
        qDebug() << "########################################################";
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
            qDebug() << "[*] Input" << input;
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
    qDebug() << "[*] Initing";
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
        qDebug() << "[*] Adding connection from layer" << from->layer() << "to layer" << to->layer();


    }


}

void NeuralNetwork::addConnection(int fromId, int toId, double weight)
{
    Neuron *from = 0;
    Neuron *to = 0;
    foreach (Neuron *neuron, mNeurons)
    {
        if(neuron->id() == fromId)
            from = neuron;
        else if(neuron->id() == toId)
            to = neuron;
    }
    if(from == 0 || to == 0)
    {
        qDebug() << "[*] Error adding connection no neurons with these ids:" << fromId << toId;
        throw "error copy";
    }

    mConnectionsManager->addConnection(from, to, weight);
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

    Connection connection = mConnectionsManager->connections().at(random);
    mConnectionsManager->setWeight(connection.from, connection.to, mConnectionsManager->randomWeight());

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
    int maxId = 0;
    foreach (Neuron *neuron, mNeurons)
    {
        if(neuron->id() > maxId)
            maxId = neuron->id();
    }

    return (maxId+1);
}
