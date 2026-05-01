/**
 * @file KM2ARecWriter.hh
 * @brief Write KM2ARecEvent rows to a ROOT file (TTree "Rec" + KM2A scalar branches)
 */

#ifndef __KM2ARECWRITER_HH__
#define __KM2ARECWRITER_HH__

#include <memory>
#include <stdexcept>
#include <string>

#include "RtypesCore.h"
#include "TFile.h"
#include "TTree.h"

#include "KM2ARecEvent.hh"
#include "LHRecEvent.h"

namespace KM2ARecWriterDetail {

/** Copy LHRecEvent data members (TObject assignment is not used). */
inline void copyLHRecEvent(LHRecEvent& dst, const LHRecEvent& src) {
    dst.ev_n = src.ev_n;
    dst.mjd = src.mjd;
    dst.dt = src.dt;
#ifdef _IS_MC
    dst.id = src.id;
    dst.E = src.E;
    dst.theta = src.theta;
    dst.phi = src.phi;
    dst.corex = src.corex;
    dst.corey = src.corey;
    dst.pNpE = src.pNpE;
    dst.pNpW = src.pNpW;
    dst.pNuM = src.pNuM;
    dst.pNuW = src.pNuW;
    dst.pNuW2 = src.pNuW2;
#endif
    dst.rec_x = src.rec_x;
    dst.rec_y = src.rec_y;
    dst.rec_theta = src.rec_theta;
    dst.rec_phi = src.rec_phi;
    dst.rec_a = src.rec_a;
    dst.rec_c0 = src.rec_c0;
    dst.rec_sigma = src.rec_sigma;
    dst.rec_Etheta_p = src.rec_Etheta_p;
    dst.rec_Ephi_p = src.rec_Ephi_p;
    dst.rec_Ec0_p = src.rec_Ec0_p;
    dst.rec_Esigma_p = src.rec_Esigma_p;
    dst.rec_Wtheta_p = src.rec_Wtheta_p;
    dst.rec_Wphi_p = src.rec_Wphi_p;
    dst.rec_Wc0_p = src.rec_Wc0_p;
    dst.rec_Wsigma_p = src.rec_Wsigma_p;
    dst.rec_Etheta_c = src.rec_Etheta_c;
    dst.rec_Ephi_c = src.rec_Ephi_c;
    dst.rec_Ec0_c = src.rec_Ec0_c;
    dst.rec_Esigma_c = src.rec_Esigma_c;
    dst.rec_Ea = src.rec_Ea;
    dst.rec_Wtheta_c = src.rec_Wtheta_c;
    dst.rec_Wphi_c = src.rec_Wphi_c;
    dst.rec_Wc0_c = src.rec_Wc0_c;
    dst.rec_Wsigma_c = src.rec_Wsigma_c;
    dst.rec_Wa = src.rec_Wa;
    dst.rec_Esize = src.rec_Esize;
    dst.rec_Wsize = src.rec_Wsize;
    dst.rec_Msize = src.rec_Msize;
    dst.rec_Msize2 = src.rec_Msize2;
    dst.rec_Eage = src.rec_Eage;
    dst.rec_Wage = src.rec_Wage;
    dst.rec_Mage = src.rec_Mage;
    dst.rec_Mage2 = src.rec_Mage2;
    dst.rec_Mrm = src.rec_Mrm;
    dst.rec_Echi = src.rec_Echi;
    dst.rec_Endf = src.rec_Endf;
    dst.rec_Mchi = src.rec_Mchi;
    dst.rec_Mchi2 = src.rec_Mchi2;
    dst.rec_Mndf = src.rec_Mndf;
    dst.rec_Ex = src.rec_Ex;
    dst.rec_Ey = src.rec_Ey;
    dst.rec_Ez = src.rec_Ez;
    dst.rec_Wx = src.rec_Wx;
    dst.rec_Wy = src.rec_Wy;
    dst.rec_Wz = src.rec_Wz;
    dst.NhitE = src.NhitE;
    dst.NhitM = src.NhitM;
    dst.NhitW = src.NhitW;
    dst.NtrigE = src.NtrigE;
    dst.NtrigE2 = src.NtrigE2;
    dst.NtrigW = src.NtrigW;
    dst.NfiltE = src.NfiltE;
    dst.NfiltM = src.NfiltM;
    dst.NfiltW = src.NfiltW;
    dst.NliveM1 = src.NliveM1;
    dst.NliveM2 = src.NliveM2;
    dst.NliveM3 = src.NliveM3;
    dst.Redge = src.Redge;
    dst.NpE1 = src.NpE1;
    dst.NpE2 = src.NpE2;
    dst.NpE3 = src.NpE3;
    dst.NpW = src.NpW;
    dst.NuM1 = src.NuM1;
    dst.NuM2 = src.NuM2;
    dst.NuM3 = src.NuM3;
    dst.NuM4 = src.NuM4;
    dst.NuM5 = src.NuM5;
    dst.NuW1 = src.NuW1;
    dst.NuW2 = src.NuW2;
    dst.NuW3 = src.NuW3;
}

}  // namespace KM2ARecWriterDetail

/**
 * @brief Append KM2ARecEvent to a ROOT file (same LHRecEvent branch layout as LHAASOFit).
 */
class KM2ARecWriter {
public:
    explicit KM2ARecWriter(const std::string& path,
                           const std::string& treeName = "Rec",
                           const std::string& treeTitle = "LHAASO Reconstructed Data")
        : filename_(path) {
        file_ = std::unique_ptr<TFile>(TFile::Open(path.c_str(), "RECREATE"));
        if (!file_ || file_->IsZombie()) {
            throw std::runtime_error("Cannot create ROOT file: " + path);
        }
        file_->SetCompressionSettings(101);
        file_->cd();
        tree_ = new TTree(treeName.c_str(), treeTitle.c_str());
        rec_ptr_ = &rec_buffer_;
        tree_->Branch("Rec", "LHRecEvent", &rec_ptr_, 64000, 1);
        tree_->Branch("rec_energy", &rec_energy_);
        tree_->Branch("direction_error", &direction_error_);
        tree_->Branch("nkg_density", &nkg_density_);
        tree_->Branch("valid", &valid_);
        tree_->Branch("event_id", &event_id); // Optional: store event ID as a separate branch
        tree_->SetAutoSave(1000000);
    }

    KM2ARecWriter(const KM2ARecWriter&) = delete;
    KM2ARecWriter& operator=(const KM2ARecWriter&) = delete;
    KM2ARecWriter(KM2ARecWriter&&) = default;
    KM2ARecWriter& operator=(KM2ARecWriter&&) = default;

    ~KM2ARecWriter() { close(); }

    void operator()(const KM2ARecEvent& ev) {
        if (!tree_ || !file_) {
            throw std::runtime_error("KM2ARecWriter: file is closed");
        }
        //KM2ARecWriterDetail::copyLHRecEvent(rec_buffer_, ev.recEvent);
        rec_buffer_ = ev.recEvent; // 直接赋值整个结构体
        rec_energy_ = ev.rec_energy;
        direction_error_ = ev.direction_error;
        nkg_density_ = ev.nkg_density;
        valid_ = ev.valid;
        event_id = ev.event_id;
        tree_->Fill();
    }

    void close() {
        if (!file_) {
            return;
        }
        file_->cd();
        tree_->BuildIndex("event_id");
        file_->Write();
        file_->Close();
        tree_ = nullptr;
        file_.reset();
    }

    const std::string& filename() const { return filename_; }

private:
    std::string filename_;
    std::unique_ptr<TFile> file_;
    TTree* tree_ = nullptr;
    LHRecEvent rec_buffer_{};
    LHRecEvent* rec_ptr_ = nullptr;
    Double_t rec_energy_ = 0;
    Double_t direction_error_ = 0;
    Double_t nkg_density_ = 0;
    Double_t valid_ = 0;
    Int_t event_id = 0;
};

#endif
