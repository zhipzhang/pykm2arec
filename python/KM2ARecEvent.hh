#pragma once
#include "LHRecEvent.h"

class KM2ARecEvent
{
    public:
        KM2ARecEvent()
        {
            recEvent = std::make_unique<LHRecEvent>();
        }
        std::unique_ptr<LHRecEvent> recEvent;
        double rec_energy;
        double direction_error;
        double nkg_density;
        double valid;
};