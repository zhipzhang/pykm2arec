/**
 * @file bindings.cc
 * @brief Nanobind bindings for KM2A event reading and reconstruction
 * 
 * Exposes LHHit, LHWave, LHEvent, KM2AMCEvent, KM2AEventSource,
 * LHRecEvent, KM2ARecEvent, and KM2AReconstructor to Python
 */

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/make_iterator.h>

#include "LHEvent.h"
#include "LHRecEvent.h"
#include "KM2AMCEvent.hh"
#include "KM2AEventSource.hh"
#include "KM2ARecEvent.hh"
#include "KM2AReconstructor.hh"
#include "G4KM2A_Geometry.h"

namespace nb = nanobind;

NB_MODULE(_pykm2arec, m) {
    m.doc() = "Python bindings for KM2A MC event reading and reconstruction";

    // ========================================================================
    // 配置路径设置函数
    // ========================================================================
    m.def("set_config_path", &G4KM2A_Geometry::SetConfigPath,
          nb::arg("path"),
          "Set the path to the config directory containing detector position files");
    m.def("get_config_path", &G4KM2A_Geometry::GetConfigPath,
          "Get the current config directory path");

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
        .def_prop_ro("eventid", &KM2AMCEvent::eventid)
        .def_prop_ro("energy", &KM2AMCEvent::energy)
        .def_prop_ro("corex", &KM2AMCEvent::corex)
        .def_prop_ro("corey", &KM2AMCEvent::corey)
        .def_prop_ro("altitude", &KM2AMCEvent::altitude)
        .def_prop_ro("azimuth", &KM2AMCEvent::azimuth)
        .def_prop_ro("nhitse", &KM2AMCEvent::NHitsE)
        .def_prop_ro("nhitsm", &KM2AMCEvent::NHitsM)
        .def_prop_ro("nhitsw", &KM2AMCEvent::NHitsW)
        // LHEvent pointer
        .def_prop_ro("lhevent", [](KM2AMCEvent& ev) { return ev.lhevent; }, 
                     nb::rv_policy::reference)
        // Hit accessors
        .def("GetHitE", &KM2AMCEvent::GetHitE, nb::rv_policy::reference)
        .def("GetHitM", &KM2AMCEvent::GetHitM, nb::rv_policy::reference)
        .def("GetHitW", &KM2AMCEvent::GetHitW, nb::rv_policy::reference)
        .def_prop_ro("HitsE", [](KM2AMCEvent& ev) {
            nb::list res;
            for (int i = 0; i < ev.NHitsE(); i++) {
                res.append(ev.GetHitE(i));
            }
            return res;

        }, nb::rv_policy::reference)
        .def("__repr__", [](const KM2AMCEvent& ev) {
            return "KM2AMCEvent(event_id=" + std::to_string(ev.eventid()) +
                   ", energy=" + std::to_string(ev.energy()) +
                   ", corex=" + std::to_string(ev.corex()) +
                   ", corey=" + std::to_string(ev.corey()) +
                   ", altitude=" + std::to_string(ev.altitude()) +
                   ", azimuth=" + std::to_string(ev.azimuth());
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

    // ========================================================================
    // LHRecEvent class - Reconstructed event data
    // ========================================================================
    nb::class_<LHRecEvent>(m, "LHRecEvent")
        .def(nb::init<>())
        // Basic event info
        .def_rw("ev_n", &LHRecEvent::ev_n)
        .def_rw("mjd", &LHRecEvent::mjd)
        .def_rw("dt", &LHRecEvent::dt)
        // MC truth info (only available when _IS_MC is defined)
        .def_rw("id", &LHRecEvent::id)
        .def_rw("E", &LHRecEvent::E)
        .def_rw("theta", &LHRecEvent::theta)
        .def_rw("phi", &LHRecEvent::phi)
        .def_rw("corex", &LHRecEvent::corex)
        .def_rw("corey", &LHRecEvent::corey)
        .def_rw("pNpE", &LHRecEvent::pNpE)
        .def_rw("pNpW", &LHRecEvent::pNpW)
        .def_rw("pNuM", &LHRecEvent::pNuM)
        .def_rw("pNuW", &LHRecEvent::pNuW)
        .def_rw("pNuW2", &LHRecEvent::pNuW2)
        // Reconstruction results - core position
        .def_rw("rec_x", &LHRecEvent::rec_x)
        .def_rw("rec_y", &LHRecEvent::rec_y)
        // Reconstruction results - direction
        .def_rw("rec_theta", &LHRecEvent::rec_theta)
        .def_rw("rec_phi", &LHRecEvent::rec_phi)
        .def_rw("rec_a", &LHRecEvent::rec_a)
        .def_rw("rec_c0", &LHRecEvent::rec_c0)
        .def_rw("rec_sigma", &LHRecEvent::rec_sigma)
        // ED planar fit
        .def_rw("rec_Etheta_p", &LHRecEvent::rec_Etheta_p)
        .def_rw("rec_Ephi_p", &LHRecEvent::rec_Ephi_p)
        .def_rw("rec_Ec0_p", &LHRecEvent::rec_Ec0_p)
        .def_rw("rec_Esigma_p", &LHRecEvent::rec_Esigma_p)
        // WCDA planar fit
        .def_rw("rec_Wtheta_p", &LHRecEvent::rec_Wtheta_p)
        .def_rw("rec_Wphi_p", &LHRecEvent::rec_Wphi_p)
        .def_rw("rec_Wc0_p", &LHRecEvent::rec_Wc0_p)
        .def_rw("rec_Wsigma_p", &LHRecEvent::rec_Wsigma_p)
        // ED conical fit
        .def_rw("rec_Etheta_c", &LHRecEvent::rec_Etheta_c)
        .def_rw("rec_Ephi_c", &LHRecEvent::rec_Ephi_c)
        .def_rw("rec_Ec0_c", &LHRecEvent::rec_Ec0_c)
        .def_rw("rec_Esigma_c", &LHRecEvent::rec_Esigma_c)
        .def_rw("rec_Ea", &LHRecEvent::rec_Ea)
        // WCDA conical fit
        .def_rw("rec_Wtheta_c", &LHRecEvent::rec_Wtheta_c)
        .def_rw("rec_Wphi_c", &LHRecEvent::rec_Wphi_c)
        .def_rw("rec_Wc0_c", &LHRecEvent::rec_Wc0_c)
        .def_rw("rec_Wsigma_c", &LHRecEvent::rec_Wsigma_c)
        .def_rw("rec_Wa", &LHRecEvent::rec_Wa)
        // Size and age parameters
        .def_rw("rec_Esize", &LHRecEvent::rec_Esize)
        .def_rw("rec_Wsize", &LHRecEvent::rec_Wsize)
        .def_rw("rec_Msize", &LHRecEvent::rec_Msize)
        .def_rw("rec_Msize2", &LHRecEvent::rec_Msize2)
        .def_rw("rec_Eage", &LHRecEvent::rec_Eage)
        .def_rw("rec_Wage", &LHRecEvent::rec_Wage)
        .def_rw("rec_Mage", &LHRecEvent::rec_Mage)
        .def_rw("rec_Mage2", &LHRecEvent::rec_Mage2)
        .def_rw("rec_Mrm", &LHRecEvent::rec_Mrm)
        // Chi-square and NDF
        .def_rw("rec_Echi", &LHRecEvent::rec_Echi)
        .def_rw("rec_Endf", &LHRecEvent::rec_Endf)
        .def_rw("rec_Mchi", &LHRecEvent::rec_Mchi)
        .def_rw("rec_Mchi2", &LHRecEvent::rec_Mchi2)
        .def_rw("rec_Mndf", &LHRecEvent::rec_Mndf)
        // Reconstructed positions
        .def_rw("rec_Ex", &LHRecEvent::rec_Ex)
        .def_rw("rec_Ey", &LHRecEvent::rec_Ey)
        .def_rw("rec_Ez", &LHRecEvent::rec_Ez)
        .def_rw("rec_Wx", &LHRecEvent::rec_Wx)
        .def_rw("rec_Wy", &LHRecEvent::rec_Wy)
        .def_rw("rec_Wz", &LHRecEvent::rec_Wz)
        // Hit counts
        .def_rw("NhitE", &LHRecEvent::NhitE)
        .def_rw("NhitM", &LHRecEvent::NhitM)
        .def_rw("NhitW", &LHRecEvent::NhitW)
        // Trigger counts
        .def_rw("NtrigE", &LHRecEvent::NtrigE)
        .def_rw("NtrigE2", &LHRecEvent::NtrigE2)
        .def_rw("NtrigW", &LHRecEvent::NtrigW)
        // Filtered hit counts
        .def_rw("NfiltE", &LHRecEvent::NfiltE)
        .def_rw("NfiltM", &LHRecEvent::NfiltM)
        .def_rw("NfiltW", &LHRecEvent::NfiltW)
        // Live MD counts
        .def_rw("NliveM1", &LHRecEvent::NliveM1)
        .def_rw("NliveM2", &LHRecEvent::NliveM2)
        .def_rw("NliveM3", &LHRecEvent::NliveM3)
        // Edge distance
        .def_rw("Redge", &LHRecEvent::Redge)
        // Particle counts
        .def_rw("NpE1", &LHRecEvent::NpE1)
        .def_rw("NpE2", &LHRecEvent::NpE2)
        .def_rw("NpE3", &LHRecEvent::NpE3)
        .def_rw("NpW", &LHRecEvent::NpW)
        // Muon counts
        .def_rw("NuM1", &LHRecEvent::NuM1)
        .def_rw("NuM2", &LHRecEvent::NuM2)
        .def_rw("NuM3", &LHRecEvent::NuM3)
        .def_rw("NuM4", &LHRecEvent::NuM4)
        .def_rw("NuM5", &LHRecEvent::NuM5)
        .def_rw("NuW1", &LHRecEvent::NuW1)
        .def_rw("NuW2", &LHRecEvent::NuW2)
        .def_rw("NuW3", &LHRecEvent::NuW3)
        .def("__repr__", [](const LHRecEvent& ev) {
            return "LHRecEvent(ev_n=" + std::to_string(ev.ev_n) +
                   ", rec_theta=" + std::to_string(ev.rec_theta) +
                   ", rec_phi=" + std::to_string(ev.rec_phi) +
                   ", rec_Esize=" + std::to_string(ev.rec_Esize) +
                   ", NhitE=" + std::to_string(ev.NhitE) + ")";
        });

    // ========================================================================
    // KM2ARecEvent class - Wrapper for reconstructed event with extra info
    // ========================================================================
    nb::class_<KM2ARecEvent>(m, "KM2ARecEvent")
        .def(nb::init<>())
        // Access to LHRecEvent
        .def_prop_ro("rec_event", [](KM2ARecEvent& ev) -> LHRecEvent* {
            return ev.recEvent.get();
        }, nb::rv_policy::reference, "Access to the underlying LHRecEvent")
        // Extra reconstruction results
        .def_rw("rec_energy", &KM2ARecEvent::rec_energy, "Reconstructed energy")
        .def_rw("direction_error", &KM2ARecEvent::direction_error, "Direction reconstruction error")
        .def_rw("nkg_density", &KM2ARecEvent::nkg_density, "NKG density at 50m")
        .def_rw("valid", &KM2ARecEvent::valid, "Validity flag (1=valid, 0=invalid)")
        .def("__repr__", [](const KM2ARecEvent& ev) {
            if (ev.valid == 0) {
                return std::string("KM2ARecEvent(valid=0)");
            }
            return "KM2ARecEvent(rec_theta=" + std::to_string(ev.recEvent->rec_theta) +
                   ", rec_phi=" + std::to_string(ev.recEvent->rec_phi) +
                   ", rec_x=" + std::to_string(ev.recEvent->rec_x) +
                   ", rec_y=" + std::to_string(ev.recEvent->rec_y) +
                   ", rec_energy=" + std::to_string(ev.rec_energy) +
                   ", direction_error=" + std::to_string(ev.direction_error) +
                   ", valid=1)";
        });

    // ========================================================================
    // KM2AReconstructor class - Event reconstruction
    // ========================================================================
    nb::class_<KM2AReconstructor>(m, "KM2AReconstructor")
        .def(nb::init<int>(), nb::arg("array_flag"),
             "Create a reconstructor with specified array flag (1=full array, etc.)")
        .def("__call__", &KM2AReconstructor::operator(), nb::arg("mc_event"),
             "Reconstruct an event from KM2AMCEvent, returns KM2ARecEvent")
        .def("reconstruct", &KM2AReconstructor::operator(), nb::arg("mc_event"),
             "Reconstruct an event from KM2AMCEvent, returns KM2ARecEvent")
        .def_prop_ro("array_flag", &KM2AReconstructor::getArrayFlag, "Get the array flag")
        .def("__repr__", [](const KM2AReconstructor& rec) {
            return "KM2AReconstructor(array_flag=" + std::to_string(rec.getArrayFlag()) + ")";
        });

    // ========================================================================
    // G4KM2A_Geometry class - Detector geometry and coordinates
    // ========================================================================
    nb::class_<G4KM2A_Geometry>(m, "G4KM2A_Geometry")
        .def(nb::init<int>(), nb::arg("flag"),
             "Create a geometry instance with specified flag:\n"
             "  0: calibration mode\n"
             "  1: KM2A@ybj\n"
             "  2: KM2A-33ED\n"
             "  3: KM2A-71ED+10MD\n"
             "  4: KM2A-quarter\n"
             "  5: KM2A-half\n"
             "  6: KM2A-3/4\n"
             "  7: KM2A-all for MC\n"
             "  >20210720: Data mode with date-based ED config")
        // Static methods for config path
        .def_static("set_config_path", &G4KM2A_Geometry::SetConfigPath,
                    nb::arg("path"), "Set the config directory path")
        .def_static("get_config_path", &G4KM2A_Geometry::GetConfigPath,
                    "Get the current config directory path")
        // Singleton accessor
        .def_static("get_instance", &G4KM2A_Geometry::GetInstance,
                    nb::arg("flag"), nb::rv_policy::reference,
                    "Get the singleton instance of geometry")
        // Number of detectors
        .def("get_ned", &G4KM2A_Geometry::GetNED, "Get number of ED detectors")
        .def("get_nmd", &G4KM2A_Geometry::GetNMD, "Get number of MD detectors")
        .def("get_nwcda", &G4KM2A_Geometry::GetNWCDA, "Get number of WCDA detectors")
        .def_prop_ro("ned", &G4KM2A_Geometry::GetNED, "Number of ED detectors")
        .def_prop_ro("nmd", &G4KM2A_Geometry::GetNMD, "Number of MD detectors")
        .def_prop_ro("nwcda", &G4KM2A_Geometry::GetNWCDA, "Number of WCDA detectors")
        // Get detector ID from array index
        .def("get_ed_id", &G4KM2A_Geometry::GetEDId, nb::arg("index"),
             "Get ED real ID from array index")
        .def("get_md_id", &G4KM2A_Geometry::GetMDId, nb::arg("index"),
             "Get MD real ID from array index")
        .def("get_wcda_id", &G4KM2A_Geometry::GetWCDAId, nb::arg("index"),
             "Get WCDA real ID from array index")
        // Get array index from real ID
        .def("get_ed_id2", &G4KM2A_Geometry::GetEDId2, nb::arg("real_id"),
             "Get ED array index from real ID (-1 if not found)")
        .def("get_md_id2", &G4KM2A_Geometry::GetMDId2, nb::arg("real_id"),
             "Get MD array index from real ID (-1 if not found)")
        .def("get_wcda_id2", &G4KM2A_Geometry::GetWCDAId2, nb::arg("real_id"),
             "Get WCDA array index from real ID (-1 if not found)")
        // Get detector coordinates - returns tuple (x, y, z) or None if invalid
        .def("get_ed_xyz", [](G4KM2A_Geometry& geo, int id, int flag) -> nb::object {
            double x, y, z;
            int result = geo.GetEDxyz(id, x, y, z, flag);
            if (result < 0) return nb::none();
            return nb::make_tuple(x, y, z);
        }, nb::arg("id"), nb::arg("flag") = 1,
           "Get ED coordinates (x, y, z) by ID.\n"
           "  flag=0: id is array index\n"
           "  flag=1: id is real detector ID\n"
           "Returns tuple (x, y, z) or None if ID is invalid")
        .def("get_md_xyz", [](G4KM2A_Geometry& geo, int id, int flag) -> nb::object {
            double x, y, z;
            int result = geo.GetMDxyz(id, x, y, z, flag);
            if (result < 0) return nb::none();
            return nb::make_tuple(x, y, z);
        }, nb::arg("id"), nb::arg("flag") = 1,
           "Get MD coordinates (x, y, z) by ID.\n"
           "  flag=0: id is array index\n"
           "  flag=1: id is real detector ID\n"
           "Returns tuple (x, y, z) or None if ID is invalid")
        .def("get_wcda_xyz", [](G4KM2A_Geometry& geo, int id, int flag) -> nb::object {
            double x, y, z;
            int result = geo.GetWCDAxyz(id, x, y, z, flag);
            if (result < 0) return nb::none();
            return nb::make_tuple(x, y, z);
        }, nb::arg("id"), nb::arg("flag") = 1,
           "Get WCDA coordinates (x, y, z) by ID.\n"
           "  flag=0: id is array index\n"
           "  flag=1: id is real detector ID\n"
           "Returns tuple (x, y, z) or None if ID is invalid")
        // Get all ED coordinates as list of tuples
        .def("get_all_ed_xyz", [](G4KM2A_Geometry& geo) {
            nb::list result;
            int ned = geo.GetNED();
            for (int i = 0; i < ned; i++) {
                double x, y, z;
                geo.GetEDxyz(i, x, y, z, 0);  // flag=0 for array index
                result.append(nb::make_tuple(geo.GetEDId(i), x, y, z));
            }
            return result;
        }, "Get all ED coordinates as list of (id, x, y, z) tuples")
        // Get all MD coordinates as list of tuples
        .def("get_all_md_xyz", [](G4KM2A_Geometry& geo) {
            nb::list result;
            int nmd = geo.GetNMD();
            for (int i = 0; i < nmd; i++) {
                double x, y, z;
                geo.GetMDxyz(i, x, y, z, 0);  // flag=0 for array index
                result.append(nb::make_tuple(geo.GetMDId(i), x, y, z));
            }
            return result;
        }, "Get all MD coordinates as list of (id, x, y, z) tuples")
        // Get all WCDA coordinates as list of tuples
        .def("get_all_wcda_xyz", [](G4KM2A_Geometry& geo) {
            nb::list result;
            int nwcda = geo.GetNWCDA();
            for (int i = 0; i < nwcda; i++) {
                double x, y, z;
                geo.GetWCDAxyz(i, x, y, z, 0);  // flag=0 for array index
                result.append(nb::make_tuple(geo.GetWCDAId(i), x, y, z));
            }
            return result;
        }, "Get all WCDA coordinates as list of (id, x, y, z) tuples")
        // Range getters
        .def("get_range_x", [](G4KM2A_Geometry& geo) {
            double minx, maxx;
            geo.GetRangeX(minx, maxx);
            return nb::make_tuple(minx, maxx);
        }, "Get X range as tuple (min, max)")
        .def("get_range_y", [](G4KM2A_Geometry& geo) {
            double miny, maxy;
            geo.GetRangeY(miny, maxy);
            return nb::make_tuple(miny, maxy);
        }, "Get Y range as tuple (min, max)")
        .def("get_range_z", [](G4KM2A_Geometry& geo) {
            double minz, maxz;
            geo.GetRangeZ(minz, maxz);
            return nb::make_tuple(minz, maxz);
        }, "Get Z range as tuple (min, max)")
        // ED rotation
        .def("get_rotation_ed", &G4KM2A_Geometry::GetRotationED,
             "Get ED rotation angle in degrees")
        .def_prop_ro("rotation_ed", &G4KM2A_Geometry::GetRotationED,
                     "ED rotation angle in degrees")
        // Core edge calculation
        .def("get_core_edge", &G4KM2A_Geometry::GetCoreEdge,
             nb::arg("x"), nb::arg("y"),
             "Calculate the distance to the array edge from position (x, y)")
        .def("__repr__", [](G4KM2A_Geometry& geo) {
            return "G4KM2A_Geometry(NED=" + std::to_string(geo.GetNED()) +
                   ", NMD=" + std::to_string(geo.GetNMD()) +
                   ", NWCDA=" + std::to_string(geo.GetNWCDA()) + ")";
        });
}

