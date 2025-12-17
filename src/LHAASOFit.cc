/*  2016-04-18: reconstruction code for LHAASO fastMC
 *  2017-11-08: modified for G4KM2A geant4 simulation
 *  If you find any bug please send email to
 *  chensz@ihep.ac.cn
 *  
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "LHEvent.h"  
#include "LHRecEvent.h"
#include "G4KM2A_Reconstruction.h"
#include "rece.h"
#include "EOSopen.h"
int main(int argc, char *argv[]) {
    int i,j,k,id,nentries,nTotalHits,nFile,Nout,arrayFlag;
    double t,pe,x,y,z,dt,Tresolution;	 
    char sTemp[80];
    if(argc<4) {
        printf("Usage: %s arrayflag outfile timecalibrationresolution maskdetector in_file ... \n", argv[0]);
        printf("arrayflag: 0:unit; 1:ybj array; 2:33ED; 3:71ED+10MD, 4:1/4 array; 5:1/2 array 6:3/4 array 7: LHAASO all\n");
        return 1;
    }
    arrayFlag=atoi(argv[1]);
    Tresolution=atof(argv[3]);
    printf("Flag=%d, Tresolution=%.2lf (ns)\n",arrayFlag,Tresolution);

    printf("***********************************************\n");
    printf("***  LHAASO reconstruction v2.2 @2022-06-01 ***\n");
    printf("***  Please email to chensz@ihep.ac.cn for  ***\n");
    printf("***  any problem or bug !                   ***\n");
    printf("***********************************************\n");

    //init the detector position firstly !!!
    G4KM2A_Geometry *geom= G4KM2A_Geometry::GetInstance(arrayFlag);

    //Calibration class 
    LHCalibration *cal= LHCalibration::GetInstance();
      if(access(argv[4],4)==0)cal->SetStatusKM2A(argv[4],0);     
      cal->SetFinalStatusKM2A();

    //Reconstruction class
    G4KM2A_Reconstruction *reconstruct=G4KM2A_Reconstruction::GetInstance(arrayFlag);
    reconstruct->SetDetetorStatus(cal);

    //rec and event class 
    LHRecEvent *rec = new LHRecEvent();
    LHEvent *event = new LHEvent();
    TClonesArray *HitsE, *HitsM;
    LHHit *Hit;

    TFile *newfile = EOSopen(argv[2],1); //0 open, 1: created
       newfile->SetCompressionSettings(101);
       TTree *EventRec = new TTree("Rec", "LHAASO Reconstructed Data");
       // EventRec->Branch("Event", "LHEvent", &event, 320000, 2);
       EventRec->Branch("Rec", "LHRecEvent", &rec, 64000, 1);
       EventRec->SetAutoSave(1000000);

    //import the files and reconstructed them
    for(nFile=5;nFile<argc;nFile++){
        TFile *hfile= EOSopen(argv[nFile],0);
        printf("inputing file -> %s\n",argv[nFile]);
        if(hfile->IsZombie()||hfile->GetEND()<50){
            printf("file error!!\n");
            continue;
        }
        //input file and tree
       // LHEvent *event = new LHEvent();
        TTree *EventTree = (TTree *)gDirectory->Get("event");
        EventTree->SetBranchAddress("Event", &event);
        nentries = Int_t(EventTree->GetEntriesFast());
        printf("Total event %d\n",nentries);
        //Nout=int(nentries/20.);
        Nout=100;
        if(Nout<1)Nout=1;
        for(i=0; i<nentries; i++) {
            EventTree->GetEntry(i);
            HitsE = event->GetHitsE();
            HitsM = event->GetHitsM();
            k=event->GetNhitE();
            for(j=0; j<k; j++){
               Hit = (LHHit *)((*HitsE)[j]);
               id=Hit->GetId();
               if(geom->Getxyz(id,x,y,z,1,"ED")<0){ Hit->SetStatus(-1); }
               else Hit->SetStatus(5);
               if(Tresolution>0){
                  dt  = Hit->GetTime()+gRandom->Gaus(0,Tresolution);
                  Hit->SetTime(dt);
               }
            }
            k=event->GetNhitM();
            for(j=0; j<k; j++){
               Hit = (LHHit *)((*HitsM)[j]);
               id=Hit->GetId();
               if(geom->Getxyz(id,x,y,z,1,"MD")<0){ Hit->SetStatus(-1); printf("errr\n");}
               else Hit->SetStatus(5);
            }

            if(event->GetNtrigE()>4){
                reconstruct->eventrecline(event,rec,10);
                rec->NuW3 = getNKGdensity(rec->rec_Eage,rec->rec_Esize, 50.);
                rec->NuW2 = recer50new3(rec->rec_Eage,rec->rec_Esize,rec->rec_theta);
                rec->mjd = GetSpaceLC(rec->theta,rec->phi,rec->rec_theta,rec->rec_phi);
                rec->NpW = rec->Redge;
                EventRec->Fill(); 
            }
            if(i%Nout==0||i==nentries-1) printf("---------->Reconstructed event %d\n",i);
        }
        hfile->Close();
    }
    newfile->Write();
    newfile->Close();
    return 0;
}

