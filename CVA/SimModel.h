#ifndef SIMMODEL_H_
#define SIMMODEL_H_

#include <vector>
#include <string>
#include "RateCurve.h"

/*
 Contain simulation models using monte-carlo
  Step1: sim for RateCurve using Vasicek Model
  Step2: sim for FxAsset using Local Vol model
*/

class SimulateRate{
private:
    RateCurve xCurve;
    double xVol, xSimPaths;
public:
    // assuming Leg1 is fixed rate and Leg2 is floating rate
    // inputs are swap object, discount factors and fwd rates 
    SimulateRate(RateCurve curve, double vol, double simPaths);
    std::vector<RateCurve> getSimulatedCurves();
};

#endif