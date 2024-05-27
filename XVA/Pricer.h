#ifndef PRICER_H_
#define PRICER_H_

#include <vector>
#include "Swap.h"
#include "RateCurve.h"

class SwapPricer{
private:
    Swap xSwap;
    RateCurve xCurve;
    double xLag;
public:
    // assuming Leg1 is fixed rate and Leg2 is floating rate
    // inputs are swap object, discount factors and fwd rates 
    SwapPricer(Swap& swap, RateCurve& curve, double lag = 0.0);
    double getLegNPV(int legNum);
    double getTradeNPV();
    double getParRate();
};

#endif