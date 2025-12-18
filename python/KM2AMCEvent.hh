#pragma once
#include "LHEvent.h"
#include "TClonesArray.h"



/**
 * @brief 合并event_info和LHEvent信息的结构
 */
 struct KM2AMCEvent {
    // 来自event_info树
    int eventid() const
    {
        return lhevent->GetEvN();
    }
    double energy() const
    {
        return lhevent->GetE();
    }
    double corex() const
    {
        return lhevent->GetCorex();
    }
    double corey() const
    {
        return lhevent->GetCorey();
    }
    double altitude() const
    {
        return lhevent->GetTheta();
    }
    double azimuth() const
    {
        return lhevent->GetPhi();
    }

    int NHitsE() const
    {
        return lhevent->GetNhitE();
    }
    int NHitsM() const
    {
        return lhevent->GetNhitM();
    }
    int NHitsW() const
    {
        return lhevent->GetNhitW();
    }
    // LHEvent指针（直接复用）
    LHEvent* lhevent = nullptr;
    LHHit* GetHitE(int index) {
        return (LHHit*)((*lhevent->GetHitsE())[index]);
    }
    LHHit* GetHitM(int index) {
        return (LHHit*)((*lhevent->GetHitsM())[index]);
    }
    LHHit* GetHitW(int index) {
        return (LHHit*)((*lhevent->GetHitsW())[index]);
    }
};