QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AxisFrame.cpp \
    AxisLimits.cpp \
    DataSetProperties.cpp \
    Debye2DFunction.cpp \
    axesdialog.cpp \
    datastream2d.cpp \
    function.cpp \
    krab.cpp \
    main.cpp \
    mainwindow.cpp \
    parameterline.cpp \
    parameterswindow.cpp \
    plot2d.cpp \
    plotpropertiesdlg.cpp

HEADERS += \
    AxisFrame.h \
    AxisLimits.h \
    DataSetProperties.h \
    Debye2DFunction.h \
    axesdialog.h \
    config.h \
    datastream2d.h \
    function.h \
    krab.h \
    mainwindow.h \
    parameterline.h \
    parameterswindow.h \
    plot2d.h \
    plotpropertiesdlg.h

FORMS += \
    mainwindow.ui

LIBS += \
    -lgsl \# To include libgsl.so from /usr/local/lib
    -lgslcblas \# To include libgslcblas.so from /usr/local/lib
    -lm

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../MinFits/plot.png \
    plot.png
