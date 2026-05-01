#pragma once
#include "G4KM2A_Reconstruction.h"
#include "KM2AMCEvent.hh"
#include "KM2ARecEvent.hh"
#include "G4KM2A_Geometry.h"
#include "TClonesArray.h"
#include "rece.h"

class KM2AReconstructor
{
    public:
        KM2AReconstructor(int arrayFlag) : arrayFlag(arrayFlag)
        {
            reconstructor = std::make_unique<G4KM2A_Reconstruction>(arrayFlag);
            geometry = G4KM2A_Geometry::GetInstance(arrayFlag);
        }
        ~KM2AReconstructor() = default;
        int getArrayFlag() const { return arrayFlag; }
        KM2ARecEvent operator()(KM2AMCEvent& mcEvent)
        {
            KM2ARecEvent recEvent;
            setHitStatus(*mcEvent.lhevent->GetHitsE(), mcEvent.NHitsE(), "ED");
            setHitStatus(*mcEvent.lhevent->GetHitsM(), mcEvent.NHitsM(), "MD");
            if(mcEvent.NHitsE() < 4)
            {
                recEvent.valid = 0;
                return recEvent;
            }
            reconstructor->eventrecline(mcEvent.lhevent, &recEvent.recEvent, 10);
            recEvent.rec_energy = recer50new3(recEvent.recEvent.rec_Eage, recEvent.recEvent.rec_Esize, recEvent.recEvent.rec_theta);
            recEvent.direction_error = GetSpaceLC(mcEvent.lhevent->GetTheta(), mcEvent.lhevent->GetPhi(), recEvent.recEvent.rec_theta, recEvent.recEvent.rec_phi);
            recEvent.nkg_density = getNKGdensity(recEvent.recEvent.rec_Eage, recEvent.recEvent.rec_Esize, 50.);
            recEvent.valid = 1;
            return recEvent;
        }
    private:
        int arrayFlag;
        std::unique_ptr<G4KM2A_Reconstruction> reconstructor;
        G4KM2A_Geometry* geometry;  // 单例，不需要管理生命周期
        void setHitStatus(TClonesArray& hits, int nHits, const std::string& detector) 
        {
            for(int i = 0; i < nHits; i++)
            {
                LHHit* hit = (LHHit*)(hits[i]);
                int id = hit->GetId();
                double x, y, z;
                if(geometry->Getxyz(id, x, y, z, 1, detector.c_str()) < 0)
                {
                    hit->SetStatus(-1);
                }
                else
                {
                    hit->SetStatus(5);
                }
            }
        }
};