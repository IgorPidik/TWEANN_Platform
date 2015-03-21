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
//    int a = qrand() % 2;
//    int b = qrand() % 2;
//    if(a == 0)
//    {
//        a = 1;
//        b = 1;
//    }
//    qDebug() << "dsa";
    QList<double> inputs;
    int a,b;
    switch (next)
    {
    case 0:
        a = 0;
        b = 0;
        next++;
        break;
    case 1:
        a = 1;
        b = 0;
        next++;
        break;
    case 2:
        a = 0;
        b = 1;
        next++;
        break;
    case 3:
        a = 1;
        b = 1;
        next = 0;
        break;
    default:
        break;
    }
    inputs.append((double)a);
    inputs.append((double)b);
    return inputs;
}

double Scape::getScore(QList<double> &input, QList<double> &output)
{
   // qDebug() << "input length " << input.length() << "output length " << output.length();
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

