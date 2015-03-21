#include "scape.h"

Scape::Scape(QObject *parent) : QObject(parent)
{
    next = 0;
}

Scape::~Scape()
{

}

QList<double> Scape::getInput()
{
    int a = qrand() % 2;
    int b = qrand() % 2;
    QList<double> input;
    input.append(a);
    input.append(b);
    return input;

}

double Scape::getScore(QList<double> &input, QList<double> &output)
{
    double a = input.at(0);
    double b = input.at(1);
    double correctOut = ((int)a ^ (int)b);

    double score = 1 - (qAbs(output.first() - correctOut));

    return score;
}

Scape *Scape::getNewInstance(QObject *parent)
{
    return new Scape(parent);
}

