/*
 *
Copyright (C) 2021  Gabriele Salvato

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

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QSettings>

#include "function.h"
#include "parameterline.h"


QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)



class ParametersWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ParametersWindow(Function* pMyFunction,
                              QString title="Function Parameters",
                              QWidget *parent = nullptr);
    ~ParametersWindow();
public:
    void Add(QString sName, double Val);
    QVector<double> getParams();

protected:
    void getSettings();
    void saveSettings();

signals:
    void closing();

public slots:
    void onClose();
    void onSaveData();
    void onValueChanged(int paramNum);


private:
    int nParams;
    QVector<double> upar;
    QVBoxLayout* pParamLayout;
    QHBoxLayout* pButtonLayout;
    QVBoxLayout* pGeneralLayout;
    QPushButton buttonClose;
    QPushButton buttonSaveData;
    Function* pFunction;
    QVector<ParameterLine*> parLine;
    QString sDataDir;
    QSettings settings;
};
