#include "connections.h"
#include <QTime>

Connections::Connections(QObject *parent) : QObject(parent)
{

}

Connections::~Connections()
{

}

void Connections::addConnection(Neuron *from, Neuron *to)
{
    qDebug() << "connection" << from->layer() << "id" << from->id() << "to" << to->layer() << "id" << to->id();
    double weight = randomWeight();
    Connection connection(from, to, weight);
    if(!mConnections.contains(connection))
    {
        connect(from, &Neuron::proccesed, to, &Neuron::input, Qt::QueuedConnection);
        to->setNumConnections(to->numConnections()+1);

        mConnections.append(connection);
        if(isConnectionRecurrent(from, to))
            mRecurrentConnections.append(connection);
    }

}

void Connections::addConnection(Neuron *from, Neuron *to, double weight)
{
    Connection connection(from, to, weight);
    if(!mConnections.contains(connection))
    {
        connect(from, &Neuron::proccesed, to, &Neuron::input, Qt::QueuedConnection);
        to->setNumConnections(to->numConnections()+1);

        mConnections.append(connection);
        if(isConnectionRecurrent(from, to))
        {
            qDebug() << "recurrent connection" << from->layer() << from->id() << to->layer() << to->id();
            mRecurrentConnections.append(connection);
        }
    }
}

void Connections::setWeight(Neuron *from, Neuron *to, double weight)
{

    int connectionId = getConnectionId(from, to);
    Connection connection = mConnections.at(connectionId);
    connection.weight = weight;
    mConnections.replace(connectionId, connection);

}

void Connections::deleteConnection(Neuron *from, Neuron *to)
{
    to->setNumConnections(to->numConnections()-1);
    if(disconnect(from, &Neuron::proccesed, to, &Neuron::input))
        qDebug() << "succesfully disconnected";
    int recurrentConnectionId = getRecurrentConnectionId(from, to);
    if(recurrentConnectionId != -1)
        mRecurrentConnections.removeAt(recurrentConnectionId);
    mConnections.removeAt(getConnectionId(from, to));
}

double Connections::getConnectionWeight(Neuron *from, Neuron *to)
{
    return mConnections.at(getConnectionId(from, to)).weight;
}

void Connections::initRecurrentNeurons()
{

    foreach (Connection connection, mRecurrentConnections)
    {
        qDebug() << "recurrent";
        connection.to->init(0);
    }
}

double Connections::randomWeight()
{
    double random = (static_cast <double> (qrand()) / static_cast <double> (RAND_MAX));
    int rand = qrand() % 2;
    if(rand == 0)
    {
        random = random * -1;
    }

    return random;
}

QList<Connection> Connections::connections()
{
    return mConnections;
}

int Connections::getConnectionId(Neuron *from, Neuron *to)
{
    Connection connection(from, to, 0);
    int id = mConnections.indexOf(connection);
    if(id == -1)
    {
        qDebug() << "Connections:getConnectionId: Error connection does not exist!";
        throw "error";
    }
    return id;

}

int Connections::getRecurrentConnectionId(Neuron *from, Neuron *to)
{
    Connection connection(from, to, 0);
    int id = mRecurrentConnections.indexOf(connection);
    return id;
}

bool Connections::isConnectionRecurrent(Neuron *from, Neuron *to)
{
    return ((from->layer() > to->layer()) || (from->layer() == to->layer()));
}

