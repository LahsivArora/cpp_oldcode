#include <vector>
#include "Pricer.h"
#include "Leg.h"
#include <memory>

int SwapPricer::counter = 0;

SwapPricer::SwapPricer(NettingSet* netSet, RateCurve* curve1, RateCurve* curve2, double FxSpot, double lag){
    xNetSet=netSet;
    xCurve1=curve1;
    xCurve2=curve2;
    xFxSpot=FxSpot;
    xLag=lag;
    ++counter;
}

SwapPricer::SwapPricer(Swap* swap, RateCurve* curve1, RateCurve* curve2, double FxSpot, double lag){
    xSwap=swap;
    xSwaps.push_back(xSwap);
    xNetSet=new NettingSet(xSwaps);
    xCurve1=curve1;
    xCurve2=curve2;
    xFxSpot=FxSpot;
    xLag=lag;
    ++counter;
}

double SwapPricer::calcLegNPV(int legNum){
    double npv = 0.0;
    Leg calcLeg = xSwap->getLeg(legNum);
    double periodAdj = 1.0/calcLeg.getLegFreq();
    double notional = (legNum==1?1.0:-1.0)*xSwap->getNotional()*(legNum==1?1.0:xSwap->getEndFxFwd()); // add conversion for xccy
    double rate = calcLeg.getLegRate();
    std::vector<double> flow = calcLeg.getLegFlows(xSwap->getMaturity());
    RateCurve* pricingCurve = new RateCurve; 
    if (calcLeg.getLegCurveName() == xCurve1->getName())
        pricingCurve = xCurve1;
    else if (calcLeg.getLegCurveName() == xCurve2->getName())
        pricingCurve = xCurve2;

    std::vector<double> xLegdisc = pricingCurve->getDiscFactors(flow);
    std::vector<double> xLegfwd = pricingCurve->getFwdRates(flow);

    if (calcLeg.getLegType() == LegType::Fixed){
        for (unsigned int i=0; i < xLegdisc.size(); i++){
            if (flow[i] > xLag){
                npv += (notional * rate * periodAdj * xLegdisc[i]);
                if(i==0 && xSwap->getNotionalExch() == NotionalExch::YES){
                    npv += (-1.0*notional);}
                if(i+1==xLegdisc.size() && xSwap->getNotionalExch() == NotionalExch::YES){
                    npv += (notional * xLegdisc[i]);}
            }
        }
    }
    else if (calcLeg.getLegType() == LegType::Float){
        for (unsigned int i=0; i < xLegdisc.size(); i++){
            if (flow[i] > xLag){
                npv += (notional * (xLegfwd[i] + rate) * periodAdj * xLegdisc[i]);
                if(i==0 && xSwap->getNotionalExch() == NotionalExch::YES){
                    npv += (-1.0*notional);}
                if(i+1==xLegdisc.size() && xSwap->getNotionalExch() == NotionalExch::YES){
                    npv += (notional * xLegdisc[i]);}
            }
        }
    }
    return npv;
}


double SwapPricer::calcTradeNPV(){

    double npv = 0.0;

    for (unsigned int i=0; i < xNetSet->getNoOfTrades(); i++){
        xSwap = xNetSet->getTrades()[i];
        if (xSwap->getTradeType() == TradeType::IrSwap)
            npv += this->calcLegNPV(1) + this->calcLegNPV(2);
        else if (xSwap->getTradeType() == TradeType::XccySwap)
            npv += this->calcLegNPV(1)*xFxSpot + this->calcLegNPV(2); // converting to Leg2 ccy. USD in this case
    }
    return npv;
}

double SwapPricer::calcFVA(RateCurve& fundCurve){
    RateCurve *FVACurve = new RateCurve;
    *FVACurve = fundCurve.nameTransform("USD.SOFR");
    SwapPricer fundPV(xNetSet,FVACurve,FVACurve,1.0);
    return fundPV.calcTradeNPV() - calcTradeNPV();}