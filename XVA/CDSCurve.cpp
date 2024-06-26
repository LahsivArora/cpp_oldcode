#include <vector>
#include "CDSCurve.h"
#include <cmath>

int CDSCurve::counter=0;

CDSCurve::CDSCurve(std::string name, double CDSSpread, double LGD, double maxMaturity, double timesteps){
    xName=name;
    xSpread=CDSSpread;
    xLGD=LGD;
    xMaturity=maxMaturity;
    xSteps=timesteps;
    ++counter;
}

std::string CDSCurve::getName(){
    return xName;
}

std::map<double,double> CDSCurve::calcMarginalPDs(){

    std::map<double,double> pds;
    double lambda = xSpread/10000.0/xLGD ; // hazard rate
    double n = xMaturity / xSteps;

    for (double i=xSteps; i<= n; i+=xSteps){
        double cumulativePDprev = 1.0 - exp(-1.0*lambda*(i-xSteps));
        double cumulativePDnow = 1.0 - exp(-1.0*lambda*i);
        double marginalPD = cumulativePDnow - cumulativePDprev;

        pds.insert(std::pair<double,double>(i,marginalPD));        
    }
    return pds;
}
