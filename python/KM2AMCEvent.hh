#pragma once
#include "LHEvent.h"
#include "TClonesArray.h"



/**
 * @brief 合并event_info和LHEvent信息的结构
 */
 struct KM2AMCEvent {
    // 来自event_info树
    int eve_id = -1;
    int eve_trig = -1;
    double eve_ene = 0.0;
    double eve_corex = 0.0;
    double eve_corey = 0.0;

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