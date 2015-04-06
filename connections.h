#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <QObject>
#include "neuron.h"

/**
 * @brief The Connection struct
 *
 * Struct for holding connection info
 *          connection from which Neuron to which Neuron,
 *          weight of connection
 *
 */

struct Connection
{
    Connection(Neuron *fromNeuron, Neuron *toNeuron, double connectionWeight)
    {
        from = fromNeuron;
        to = toNeuron;
        weight = connectionWeight;
    }

    Neuron *from;
    Neuron *to;
    double weight;

    bool operator ==(const Connection &connection) const
    {
        return (from == connection.from && to == connection.to);
    }
};

class Connections : public QObject
{
    Q_OBJECT
public:
    explicit Connections(QObject *parent = 0);
    ~Connections();

    /**
     * @brief addConnection
     * @param from
     * @param to
     *
     * @see addConnection(Neuron *from, Neuron *to, double weight);
     *
     * Creates new connection between neurons and sets random weight
     */
    void addConnection(Neuron *from, Neuron *to);

    /**
     * @brief addConnection
     * @param from
     * @param to
     * @param weight
     *
     * @see addConnection(Neuron *from, Neuron *to);
     *
     * Creates new connection between neurons with ceratain weight
     */
    void addConnection(Neuron *from, Neuron *to, double weight);

    /**
     * @brief setWeight
     * @param from
     * @param to
     * @param weight
     *
     * Sets weight between two neurons
     */
    void setWeight(Neuron *from, Neuron *to, double weight);

    /**
     * @brief deleteConnection
     * @param from
     * @param to
     *
     * Disconnects neurons
     */
    void deleteConnection(Neuron *from, Neuron *to);

    /**
     * @brief getConnectionWeight
     * @param from
     * @param to
     * @return connection's weight
     *
     * Returns weight of a connection if exists
     */
    double getConnectionWeight(Neuron *from, Neuron *to);

    /**
     * @brief initRecurrentNeurons
     *
     * Sents initial input (0) to recurrently connected neurons
     */
    void initRecurrentNeurons();

    /**
     * @brief randomWeight
     * @return generated weight
     *
     * Generates random weight
     */
    double randomWeight();

    /**
     * @brief connections
     * @return list of connections
     *
     * Returns list of connections
     */
    QList<Connection> connections();


private:

    /**
     * @brief getConnectionId
     * @param from
     * @param to
     * @return id of connection if exists
     *
     * Returns id of connection if exists
     */
    int getConnectionId(Neuron *from, Neuron *to);

    /**
     * @brief getRecurrentConnectionId
     * @param from
     * @param to
     * @return id if exists
     *
     * Returns id of reccurent connection if exists
     */
    int getRecurrentConnectionId(Neuron *from, Neuron *to);

    /**
     * @brief isConnectionRecurrent
     * @param from
     * @param to
     * @return true | false
     *
     * Returns information about recurrency of connection
     */
    bool isConnectionRecurrent(Neuron *from, Neuron *to);



    QList<Connection> mConnections;

    QList<Connection> mRecurrentConnections;

};

#endif // CONNECTIONS_H
