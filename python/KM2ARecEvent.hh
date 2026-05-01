#pragma once
#include "LHRecEvent.h"
#include <memory>

class KM2ARecEvent
{
    public:
        KM2ARecEvent() = default;
        LHRecEvent recEvent;
        double rec_energy;
        double direction_error;
        double nkg_density;
        double valid;
        int event_id  = 0; // This event id could be used to match with LACT Simulation, not used for KM2A single.
};