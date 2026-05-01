#pragma once
#include "KM2AMCEvent.hh"
#include "KM2ARecEvent.hh"
#include <optional>

class pyKM2AEvent{
    public:
        pyKM2AEvent() = default;
        ~pyKM2AEvent() = default;

        std::optional<KM2AMCEvent> mc_event;
        std::optional<KM2ARecEvent> rec_event;


};