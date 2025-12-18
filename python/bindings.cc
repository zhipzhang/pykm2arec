/**
 * @file bindings.cc
 * @brief Nanobind bindings for KM2A event reading
 * 
 * Exposes LHHit, LHWave, LHEvent, KM2AMCEvent, and KM2AEventSource to Python
 */

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/make_iterator.h>

#include "LHEvent.h"
#include "KM2AMCEvent.hh"
#include "KM2AEventSource.hh"

namespace nb = nanobind;

NB_MODULE(_pykm2arec, m) {
    m.doc() = "Python bindings for KM2A MC event reading";

    // ========================================================================
    // LHHit class
    // ========================================================================
    nb::class_<LHHit>(m, "LHHit")
        .def(nb::init<>())
        .def(nb::init<Int_t, Double_t, Double_t, Int_t>(),
             nb::arg("id"), nb::arg("time"), nb::arg("pe"), nb::arg("np"))
        .def(nb::init<Int_t, Double_t, Double_t, Int_t, Int_t>(),
             nb::arg("id"), nb::arg("time"), nb::arg("pe"), nb::arg("np"), nb::arg("status"))
        // Getters
        .def("GetId", &LHHit::GetId)
        .def("GetStatus", &LHHit::GetStatus)
        .def("GetTime", &LHHit::GetTime)
        .def("GetPe", &LHHit::GetPe)
        .def("GetNp", &LHHit::GetNp)
        // Setters
        .def("SetId", &LHHit::SetId)
        .def("SetStatus", &LHHit::SetStatus)
        .def("SetTime", &LHHit::SetTime)
        .def("SetPe", &LHHit::SetPe)
        .def("SetNp", &LHHit::SetNp)
        // Properties for pythonic access
        .def_prop_ro("id", &LHHit::GetId)
        .def_prop_ro("status", &LHHit::GetStatus)
        .def_prop_ro("time", &LHHit::GetTime)
        .def_prop_ro("pe", &LHHit::GetPe)
        .def_prop_ro("np", &LHHit::GetNp)
        .def("__repr__", [](const LHHit& h) {
            return "LHHit(id=" + std::to_string(h.GetId()) + 
                   ", time=" + std::to_string(h.GetTime()) +
                   ", pe=" + std::to_string(h.GetPe()) +
                   ", status=" + std::to_string(h.GetStatus()) + ")";
        });

    // ========================================================================
    // LHWave class
    // ========================================================================
    nb::class_<LHWave>(m, "LHWave")
        .def(nb::init<>())
        .def(nb::init<Double_t, Double_t>(), nb::arg("t"), nb::arg("a"))
        // Getters
        .def("GetT", &LHWave::GetT)
        .def("GetA", &LHWave::GetA)
        // Setters
        .def("SetT", &LHWave::SetT)
        .def("SetA", &LHWave::SetA)
        // Properties
        .def_prop_ro("t", &LHWave::GetT)
        .def_prop_ro("a", &LHWave::GetA)
        .def("__repr__", [](const LHWave& w) {
            return "LHWave(t=" + std::to_string(w.GetT()) + 
                   ", a=" + std::to_string(w.GetA()) + ")";
        });

    // ========================================================================
    // LHEvent class
    // ========================================================================
    nb::class_<LHEvent>(m, "LHEvent")
        .def(nb::init<>())
        // Basic getters
        .def("GetEvN", &LHEvent::GetEvN)
        .def("GetMjd", &LHEvent::GetMjd)
        .def("GetDt", &LHEvent::GetDt)
        .def("GetNhitE", &LHEvent::GetNhitE)
        .def("GetNhitM", &LHEvent::GetNhitM)
        .def("GetNhitW", &LHEvent::GetNhitW)
        .def("GetNwaveE", &LHEvent::GetNwaveE)
        .def("GetNwaveM", &LHEvent::GetNwaveM)
        .def("GetNtrigE", &LHEvent::GetNtrigE)
        .def("GetNtrigW", &LHEvent::GetNtrigW)
        .def("GetTrigEt", &LHEvent::GetTrigEt)
        // MC getters
        .def("GetE", &LHEvent::GetE)
        .def("GetId", &LHEvent::GetId)
        .def("GetNpE", &LHEvent::GetNpE)
        .def("GetNpW", &LHEvent::GetNpW)
        .def("GetNuM", &LHEvent::GetNuM)
        .def("GetNuW", &LHEvent::GetNuW)
        .def("GetNuW2", &LHEvent::GetNuW2)
        .def("GetTheta", &LHEvent::GetTheta)
        .def("GetPhi", &LHEvent::GetPhi)
        .def("GetCorex", &LHEvent::GetCorex)
        .def("GetCorey", &LHEvent::GetCorey)
        // Properties for pythonic access
        .def_prop_ro("ev_n", &LHEvent::GetEvN)
        .def_prop_ro("mjd", &LHEvent::GetMjd)
        .def_prop_ro("dt", &LHEvent::GetDt)
        .def_prop_ro("nhit_e", &LHEvent::GetNhitE)
        .def_prop_ro("nhit_m", &LHEvent::GetNhitM)
        .def_prop_ro("nhit_w", &LHEvent::GetNhitW)
        .def_prop_ro("nwave_e", &LHEvent::GetNwaveE)
        .def_prop_ro("nwave_m", &LHEvent::GetNwaveM)
        .def_prop_ro("ntrig_e", &LHEvent::GetNtrigE)
        .def_prop_ro("ntrig_w", &LHEvent::GetNtrigW)
        .def_prop_ro("trig_et", &LHEvent::GetTrigEt)
        .def_prop_ro("energy", &LHEvent::GetE)
        .def_prop_ro("particle_id", &LHEvent::GetId)
        .def_prop_ro("theta", &LHEvent::GetTheta)
        .def_prop_ro("phi", &LHEvent::GetPhi)
        .def_prop_ro("core_x", &LHEvent::GetCorex)
        .def_prop_ro("core_y", &LHEvent::GetCorey)
        // Hit access via TClonesArray
        .def("GetHitE", [](LHEvent& ev, int i) -> LHHit* {
            auto* arr = ev.GetHitsE();
            if (!arr || i < 0 || i >= arr->GetEntries()) return nullptr;
            return static_cast<LHHit*>(arr->At(i));
        }, nb::rv_policy::reference)
        .def("GetHitM", [](LHEvent& ev, int i) -> LHHit* {
            auto* arr = ev.GetHitsM();
            if (!arr || i < 0 || i >= arr->GetEntries()) return nullptr;
            return static_cast<LHHit*>(arr->At(i));
        }, nb::rv_policy::reference)
        .def("GetHitW", [](LHEvent& ev, int i) -> LHHit* {
            auto* arr = ev.GetHitsW();
            if (!arr || i < 0 || i >= arr->GetEntries()) return nullptr;
            return static_cast<LHHit*>(arr->At(i));
        }, nb::rv_policy::reference)
        .def("GetWaveE", [](LHEvent& ev, int i) -> LHWave* {
            auto* arr = ev.GetWaveE();
            if (!arr || i < 0 || i >= arr->GetEntries()) return nullptr;
            return static_cast<LHWave*>(arr->At(i));
        }, nb::rv_policy::reference)
        .def("GetWaveM", [](LHEvent& ev, int i) -> LHWave* {
            auto* arr = ev.GetWaveM();
            if (!arr || i < 0 || i >= arr->GetEntries()) return nullptr;
            return static_cast<LHWave*>(arr->At(i));
        }, nb::rv_policy::reference)
        .def("__repr__", [](const LHEvent& ev) {
            return "LHEvent(ev_n=" + std::to_string(ev.GetEvN()) +
                   ", E=" + std::to_string(ev.GetE()) +
                   ", NhitE=" + std::to_string(ev.GetNhitE()) +
                   ", NhitM=" + std::to_string(ev.GetNhitM()) + ")";
        });

    // ========================================================================
    // KM2AMCEvent struct
    // ========================================================================
    nb::class_<KM2AMCEvent>(m, "KM2AMCEvent")
        .def(nb::init<>())
        // event_info fields
        .def_rw("eve_id", &KM2AMCEvent::eve_id)
        .def_rw("eve_trig", &KM2AMCEvent::eve_trig)
        .def_rw("eve_ene", &KM2AMCEvent::eve_ene)
        .def_rw("eve_corex", &KM2AMCEvent::eve_corex)
        .def_rw("eve_corey", &KM2AMCEvent::eve_corey)
        // LHEvent pointer
        .def_prop_ro("lhevent", [](KM2AMCEvent& ev) { return ev.lhevent; }, 
                     nb::rv_policy::reference)
        // Hit accessors
        .def("GetHitE", &KM2AMCEvent::GetHitE, nb::rv_policy::reference)
        .def("GetHitM", &KM2AMCEvent::GetHitM, nb::rv_policy::reference)
        .def("GetHitW", &KM2AMCEvent::GetHitW, nb::rv_policy::reference)
        .def("__repr__", [](const KM2AMCEvent& ev) {
            std::string repr = "KM2AMCEvent(eve_id=" + std::to_string(ev.eve_id);
            repr += ", eve_ene=" + std::to_string(ev.eve_ene);
            if (ev.lhevent) {
                repr += ", E=" + std::to_string(ev.lhevent->GetE());
                repr += ", NhitE=" + std::to_string(ev.lhevent->GetNhitE());
            }
            repr += ")";
            return repr;
        });

    // ========================================================================
    // KM2AEventSource class
    // ========================================================================
    nb::class_<KM2AEventSource>(m, "KM2AEventSource")
        .def(nb::init<const std::string&>(), nb::arg("filename"),
             "Open a ROOT file containing KM2A MC events")
        // Size methods
        .def("size", &KM2AEventSource::size, "Get the number of events")
        .def("__len__", &KM2AEventSource::size)
        .def("size_event_info", &KM2AEventSource::sizeEventInfo, 
             "Get the number of event_info entries")
        // Indexing
        .def("__getitem__", [](KM2AEventSource& src, Long64_t index) {
            if (index < 0) index += src.size();
            if (index < 0 || index >= src.size()) {
                throw nb::index_error("Event index out of range");
            }
            return src.GetEvent(index);
        })
        .def("get_event", &KM2AEventSource::GetEvent, nb::arg("index"),
             "Get event at specified index")
        // Iterator support
        .def("__iter__", [](KM2AEventSource& src) {
            return nb::make_iterator(nb::type<KM2AEventSource>(), "iterator",
                                     src.begin(), src.end());
        }, nb::keep_alive<0, 1>())
        // File info
        .def_prop_ro("filename", &KM2AEventSource::filename)
        .def("__repr__", [](const KM2AEventSource& src) {
            return "KM2AEventSource('" + src.filename() + "', " + 
                   std::to_string(src.size()) + " events)";
        });
}

