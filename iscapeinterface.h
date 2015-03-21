#ifndef ISCAPEINTERFACE
#define ISCAPEINTERFACE

#include <QList>
#include <QtPlugin>

class IScapeInterface
{
public:
    /**
     * @brief getInput
     * @return QList<double> with inputs
     *
     * Needed to train neural network,
     * provides inputs for neural network
     */
    virtual QList<double> getInput() = 0;

    /**
     * @brief getScore
     * @param input
     * @param output
     * @return score (differences in input and output)
     *
     * Needed to train neural network,
     * returns score
     */
    virtual double getScore(QList<double> &input, QList<double> &output) = 0;


    /**
     * @brief getNewInstance
     * @param parent
     *
     * Returns new instance of object
     */
    virtual IScapeInterface *getNewInstance(QObject* parent) = 0;

};

Q_DECLARE_INTERFACE(IScapeInterface, "com.NeuralNetwork.IScapeInterface")

#endif // ISCAPEINTERFACE

