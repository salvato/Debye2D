/*
 *
Copyright (C) 2024  Gabriele Salvato

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
#pragma once

#include "function.h"
#include <QSettings>
#include <gsl/gsl_math.h>

QT_FORWARD_DECLARE_CLASS(Plot2D)


class Debye2DFunction : public Function
{
public:
    Debye2DFunction();
    ~Debye2DFunction();
    double operator()(const QVector<double>& par, double T) const;
    void Plot(const QVector<double>& par) const;
    void saveSettings();
    bool saveData(QFile* pOutFile);

protected:
    void getSettings();

private:
    std::vector<double> theTemperatures;
    double theErrorDef;
    Plot2D*   pPlot;
    QString   sDataDir;
    QSettings settings;
    gsl_function f1;
};

