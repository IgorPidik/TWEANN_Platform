#ifndef SCAPE_H
#define SCAPE_H

#include <QObject>
#include "iscapeinterface.h"
#include <QDebug>

class Scape : public QObject, public IScapeInterface
{
    Q_OBJECT

    Q_INTERFACES(IScapeInterface)

public:
    explicit Scape(QObject *parent = 0);
    ~Scape();
    QList<double> getInput();
    double getScore(QList<double> &input, QList<double> &output);
    Scape *getNewInstance(QObject *parent);

};

#endif // SCAPE_H
