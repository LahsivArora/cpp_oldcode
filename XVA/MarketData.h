#ifndef MARKETDATA_H_
#define MARKETDATA_H_

#include <vector>
#include <string>
#include "RateCurve.h"
#include "CDSCurve.h"

class MarketData{
private:
    std::vector<RateCurve*>* xRateCurves;
    std::vector<CDSCurve*>* xCDSCurves;
    std::vector<double*>* xFxSpots;
public:
    static int counter;
    MarketData(std::vector<RateCurve*>* rateCurves = new std::vector<RateCurve*>, 
               std::vector<CDSCurve*>* CDSCurves = new std::vector<CDSCurve*>, std::vector<double*>* fxSpots = new std::vector<double*>);
    std::vector<RateCurve*>* getRateCurves();
    RateCurve* getRateCurve(std::string name);
    std::vector<CDSCurve*>* getCDSCurves();
    CDSCurve* getCDSCurve(std::string name);
    std::vector<double*>* getFxSpots();
    ~MarketData(){--counter;}
    MarketData* replaceRateCurve(RateCurve* changedCurve);
    MarketData* replaceFxSpot(double* changedFxSpot);
};
  
#endif