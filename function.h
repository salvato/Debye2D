#ifndef FUNCTION_H
#define FUNCTION_H

#include <QVector>
#include <QFile>

class Function
{
public:
    Function();
    virtual ~Function() {};
    virtual double operator()(const QVector<double>& x, double t) const = 0;
    virtual void Plot(const QVector<double>& par) const = 0;
    virtual void saveSettings() = 0;
    virtual bool saveData(QFile* pOutFile) = 0;
};

#endif // FUNCTION_H
