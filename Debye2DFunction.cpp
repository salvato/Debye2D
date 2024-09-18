#include "Debye2DFunction.h"
#include "plot2d.h"
#include "float.h"

#include <config.h>
#include <gsl/gsl_ieee_utils.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>

#include <QDir>
#include <QDebug>


double ni;
double R;
double Pm;
double TetaD;

double Tstart;
double Tend;
double Tstep;
double T;

double z;

double integranda(double z, void *params);

gsl_function
make_function (double (*f) (double, void*), double* p);

gsl_function
make_function (double (*f) (double, void*), double* p) {
    gsl_function f_new;

    f_new.function = f ;
    f_new.params   = p ;

    return f_new;
}


void
my_error_handler (const char *reason, const char *file, int line, int err) {
    if(true)
        qDebug() << "(caught ["
                 << file << ":"
                 << line << ":"
                 << reason << "(" << err  << ")]";
    //%s:%d: %s (%d)])\n", file, line, reason, err) ;
}

Debye2DFunction::Debye2DFunction()
    : pPlot(nullptr)
{
    gsl_ieee_env_setup ();
    gsl_set_error_handler (&my_error_handler);
    getSettings();
    f1 = make_function(&integranda, nullptr);

    pPlot = new Plot2D(nullptr, "Debye 2D");
    pPlot->SetLimits(0.0, 0.1, 0.0, 1.0, true, true, false, false);
    pPlot->UpdatePlot();
    pPlot->show();
}


Debye2DFunction::~Debye2DFunction(){
    if(pPlot) delete pPlot;
}


void
Debye2DFunction::getSettings() {
    sDataDir = settings.value("Data_Dir", QDir::currentPath()).toString();
}


void
Debye2DFunction::saveSettings() {
    settings.setValue("Data_Dir", sDataDir);
}


double
Debye2DFunction::operator()(const QVector<double>& par, double _T) const {
    Q_UNUSED(par);
    double result = 0, abserr = 0;
    size_t neval  = 0;
    double epsabs = 1.0e-12, epsrel = 1.0e-6;

    ni     = par[0];
    R      = par[1];
    Pm     = par[2];
    TetaD  = par[3];

    Tstart = par[4];
    Tend   = par[5];
    Tstep  = par[6];

    double factor = (4.0*ni*R)/(Pm*(TetaD*TetaD));

    gsl_integration_qng(&f1, 0.0, TetaD/_T, epsabs, epsrel,
                        &result, &abserr, &neval);

    return factor * result;
}


void
Debye2DFunction::Plot(const QVector<double>& par) const
{
    double result = 0, abserr = 0;
    size_t neval  = 0;
    double epsabs = 1.0e-12, epsrel = 1.0e-6;

    if(!pPlot) return;

    pPlot->ClearDataSet(1);
    pPlot->NewDataSet(1, 3, QColor(255, 255,   0), Plot2D::iline, "C/T^2");

    ni     = par[0];
    R      = par[1];
    Pm     = par[2];
    TetaD  = par[3];

    Tstart = par[4];
    Tend   = par[5];
    Tstep  = par[6];

    double factor = (4.0*ni*R)/(Pm*(TetaD*TetaD));

    T = Tstart;
    while(T <= Tend) {
        gsl_integration_qng(&f1, 0.0, TetaD/T, epsabs, epsrel,
                            &result, &abserr, &neval);
        pPlot->NewPoint(1, T, factor*result);
        T += Tstep;
    }

    pPlot->SetShowTitle(1, true);
    pPlot->SetShowDataSet(1, true);
    pPlot->UpdatePlot();
}


bool
Debye2DFunction::saveData(QFile* pOutFile) {
    double result = 0, abserr = 0;
    size_t neval  = 0;
    double epsabs = 1.0e-12, epsrel = 1.0e-6;

    // QVector2D* pData = pPlot->GetDataSet(1); <<== Da Implementare !!! Manca !!!

    pOutFile->write(QString("%1 %2 %3\n")
                        .arg("Temperature", 12)
                        .arg("Debye2D", 12)
                        .toLocal8Bit());
    double f;
    double factor = (4.0*ni*R)/(Pm*(TetaD*TetaD));
    T = Tstart;
    while(T <= Tend) {
        gsl_integration_qng(&f1, 0.0, TetaD/T, epsabs, epsrel,
                            &result, &abserr, &neval);
        f = factor * result;
        abserr *= factor;
        pOutFile->write(QString("%1 %2 %3\n")
                            .arg(T,      12, 'g', 6, ' ')
                            .arg(f,      12, 'g', 6, ' ')
                            .arg(f,      12, 'g', 6, ' ')
                            .arg(abserr, 12, 'g', 6, ' ')
                            .toLocal8Bit());
        T += Tstep;
    }
    return true;
}


double
integranda(double z, void* params) {
    (void) params;
    if(z > DBL_MAX_10_EXP) {
        // qDebug() << z << DBL_MAX_10_EXP;
        return 0.0;
    }
    else
        return (pow(z, 3.0) * exp(z)) / ((exp(z)-1.0)*(exp(z)-1.0));
}

