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
#include "parameterswindow.h"


#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>


ParametersWindow::ParametersWindow(Function *pMyFunction,
                                   QString title,
                                   QWidget *parent)
    : QWidget(parent)
    , pFunction(pMyFunction)
{
    setWindowFlags(Qt::WindowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(Qt::WindowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::WindowFlags() |  Qt::WindowMinMaxButtonsHint);

    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowTitle(title);
    setWindowIcon(QIcon(":/plot.png"));

    buttonClose.setText("Close");
    buttonSaveData.setText("Save Data");

    connect(&buttonClose, SIGNAL(clicked()),
            this, SLOT(onClose()));
    connect(&buttonSaveData, SIGNAL(clicked()),
            this, SLOT(onSaveData()));

    pParamLayout = new QVBoxLayout();
    pParamLayout->addWidget(new ParameterLine());

    pButtonLayout = new QHBoxLayout();
    pButtonLayout->addWidget(&buttonSaveData);
    pButtonLayout->addWidget(&buttonClose);

    pGeneralLayout = new QVBoxLayout();
    pGeneralLayout ->addLayout(pParamLayout);
    pGeneralLayout ->addLayout(pButtonLayout);
    setLayout(pGeneralLayout);

    nParams = 0;
    getSettings();
}


ParametersWindow::~ParametersWindow() {
    saveSettings();
}


void
ParametersWindow::getSettings() {
    restoreGeometry(settings.value(QString("Parameters")).toByteArray());
    sDataDir = settings.value("Data_Dir", QDir::currentPath()).toString();
    nParams = 0;
    QString sName;
    double val;
#ifdef Q_OS_WINDOWS
    sName = QString("ni");
#else
    sName = QString("ν");
#endif
    val = settings.value(sName, 1.0).toDouble();
    Add(sName, val);
    sName = QString("R");
    val = settings.value(sName, 2.0).toDouble();
    Add(sName, val);
    sName = QString("Pm");
    val = settings.value(sName, 3.0).toDouble();
    Add(sName, val);
#ifdef Q_OS_WINDOWS
    sName = QString("TetaD");
#else
    sName = QString("Θd");
#endif
    val = settings.value(sName, 4.0).toDouble();
    Add(sName, val);
    sName = QString("Tstart");
    val = settings.value(sName, 5.0).toDouble();
    Add(sName, val);
    sName = QString("Tend");
    val = settings.value(sName, 6.0).toDouble();
    Add(sName, val);
    sName = QString("Tstep");
    val = settings.value(sName, 0.1).toDouble();
    Add(sName, val);

    pFunction->Plot(upar);
}


void
ParametersWindow::saveSettings() {
    settings.setValue(QString("Parameters"), saveGeometry());
    settings.setValue("Data_Dir", sDataDir);
    for(int i=0; i< nParams; i++)
        settings.setValue(parLine.at(i)->getName(), parLine.at(i)->getValue());
}


void
ParametersWindow::Add(QString sName, double Val) {
    ParameterLine* newPar = new ParameterLine(nParams, sName, Val);
    parLine.push_back(newPar);
    upar.push_back(Val);
    pParamLayout->addWidget(parLine.back());
    nParams++;

    connect(newPar, SIGNAL(valueChanged(int)),
            this, SLOT(onValueChanged(int)));
}


QVector<double>
ParametersWindow::getParams() {
    return upar;
}


void
ParametersWindow::onSaveData() {
    QString outFileName = QFileDialog::getSaveFileName(this,
                                                       "Save Data",
                                                       sDataDir,
                                                       "Simulated Data (*.dat);All Files (*)");
    if(outFileName == QString())
        return;

    QFile outFile(outFileName);
    if(!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this,
                                 "Unable to open file",
                                 outFile.errorString());
        return;
    }

    outFile.write(QString("Parameters:\n").toLocal8Bit());
    for(qsizetype i=0; i<parLine.size(); i++) {
        outFile.write(QString("%1 = %2\n")
                      .arg(QString(parLine.at(i)->getName()), 7)
                      .arg(parLine.at(i)->getValue(), 12)
                      .toLocal8Bit());
    }

    pFunction->saveData(&outFile);
}


void
ParametersWindow::onClose() {
    emit closing();
}


void
ParametersWindow::onValueChanged(int paramNum) {
    bool bOk = true;
    if(paramNum == 6) {
        if((parLine.at(paramNum)->getValue()<0.1)) {
            bOk = false;
            parLine.at(paramNum)->setError(true);
        }
    }
    if(paramNum == 3) {
        if((parLine.at(paramNum)->getValue()<40.0) || (parLine.at(paramNum)->getValue()>1000.0)) {
            bOk = false;
            parLine.at(paramNum)->setError(true);
        }
    }
    if(bOk) {
        parLine.at(paramNum)->setError(false);
        upar.replace(paramNum, parLine.at(paramNum)->getValue());
        pFunction->Plot(upar);
    }
}

