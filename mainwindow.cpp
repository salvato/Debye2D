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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Debye2DFunction.h"

#include <QDir>
#include <QPicture>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , pUi(new Ui::MainWindow)
    , pFunction(nullptr)
    , pParams(nullptr)
{
    pUi->setupUi(this);
    getSettings();
    setWindowIcon(QIcon(":/plot.png"));
    pUi->statusbar->showMessage(QString ("© Gabriele Salvato - 2024"));
}


MainWindow::~MainWindow() {
    delete pUi;
}


void
MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event)
    saveSettings();
    if(pUi) delete pUi;
    pUi = nullptr;
    if(pParams) delete pParams;
    pParams = nullptr;
    if(pFunction) delete pFunction;
    pFunction = nullptr;
}


void
MainWindow::getSettings() {
    restoreGeometry(settings.value(QString("MainWindow_Dialog")).toByteArray());
    sDataDir = settings.value("Data_Dir",   QDir::currentPath()).toString();
}


void
MainWindow::saveSettings() {
    settings.setValue(QString("MainWindow_Dialog"), saveGeometry());
    settings.setValue("Data_Dir", sDataDir);
}


void
MainWindow::on_pushButton_clicked() {
    if(pFunction) delete pFunction;
    pFunction = new Debye2DFunction();
    QImage formula(":/Debye2D.png");
    QPixmap pixmap;
    pixmap.convertFromImage(formula);
    pUi->formula->setPixmap(pixmap);

    if(pParams) delete pParams;
    pParams = new ParametersWindow(pFunction, "Debye 2D Parameters");

    connect(pParams, SIGNAL(closing()),
            this, SLOT(onProgramDone()));

    pParams->show();
    //hide();
}


void
MainWindow::onProgramDone() {
    pParams->disconnect();
    if(pParams) delete pParams;
    pParams = nullptr;
    if(pFunction) delete pFunction;
    pFunction = nullptr;
    show();
}
