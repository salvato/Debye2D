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
#include "parameterline.h"
#include <QHBoxLayout>

ParameterLine::ParameterLine(int num,
                             QString sName,
                             QString sValue,
                             QWidget *parent)
    : QWidget(parent)
    , number(num)
{
    SetStyles();

    editName.setText(sName);
    editValue.setText(sValue);

    SetLayout();
    ConnectSignals();
}


ParameterLine::ParameterLine(int num,
                             QString sName,
                             double Value,
                             QWidget *parent)
    : QWidget(parent)
    , number(num)
{
    SetStyles();

    editName.setText(sName);
    editValue.setText(QString("%1").arg(Value));

    SetLayout();
    ConnectSignals();
}


ParameterLine::ParameterLine() {
    SetStyles();

    editName.setStyleSheet(sHeaderStyle);
    editValue.setStyleSheet(sHeaderStyle);

    editName.setText("Name");
    editValue.setText("Value");

    editName.setReadOnly(true);
    editValue.setReadOnly(true);

    editName.setAlignment( Qt::AlignCenter);
    editValue.setAlignment( Qt::AlignCenter);

    SetLayout();
}


void
ParameterLine::SetLayout() {
    auto* mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(&editName);
    mainLayout->addWidget(&editValue);
    editName.setReadOnly(true);
    setLayout(mainLayout);
}


void
ParameterLine::ConnectSignals() {
    connect(&editValue, SIGNAL(textChanged(QString)),
            this, SLOT(onValueChanged(QString)));
}


void
ParameterLine::SetStyles() {
    sNormalStyle = editValue.styleSheet();

    sErrorStyle  = "QLineEdit { ";
    sErrorStyle +=     "color: rgb(255, 255, 255);";
    sErrorStyle +=     "background: rgb(255, 0, 0);";
    sErrorStyle +=     "selection-background-color: rgb(128, 128, 255);";
    sErrorStyle += "}";

    sHeaderStyle  = "QLineEdit { ";
    sHeaderStyle +=     "color: rgb(223, 223, 255);";
    sHeaderStyle +=     "background: rgb(8, 8, 128);";
    sHeaderStyle +=     "selection-background-color: rgb(128, 128, 255);";
    sHeaderStyle += "}";
}


void
ParameterLine::onValueChanged(QString sNewValue) {
    bool ok;
    sNewValue.toDouble(&ok);
    if(ok) {
        editValue.setStyleSheet(sNormalStyle);
        emit valueChanged(number);
    }
    else {
        editValue.setStyleSheet(sErrorStyle);
    }
}


void
ParameterLine::setError(bool bError) {
    if(bError)
        editValue.setStyleSheet(sErrorStyle);
    else
        editValue.setStyleSheet(sNormalStyle);
}


void
ParameterLine::setValue(double newValue) {
    editValue.setText(QString("%1").arg(newValue));
}


QString
ParameterLine::getName() {
    return editName.text();
}


double
ParameterLine::getValue() {
    return editValue.text().toDouble();
}

