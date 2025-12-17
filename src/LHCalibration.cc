// -----------------------------------------------------------------------
//                                LHAASO
//                       --- LHCalibration ---
//                        class implementation file
// -----------------------------------------------------------------------
//  Description:
//
//    Managers of the LHAASO detector calibration and status
//
// -----------------------------------------------------------------------
// History:
//
//    chen songzhan      -- 2021.4 created
//           
//========================================================================
#include "LHCalibration.h"
#include <iostream>
#include <fstream>
#include <algorithm>   
#include <iterator>
#include "math.h"
#include <string.h>
#include "EOSopen.h"
using namespace std;

LHCalibration * LHCalibration::m_myself = 0;
G4KM2A_Geometry * LHCalibration::geom=0;
LHCalibration::LHCalibration()
{
    geom = G4KM2A_Geometry::GetInstance(7);
    int i;
    fNED=0;
    fNMD=0;
    fNWCDA=0;
    fNWCDA2=0;
    for(i=0;i<NNED;i++){
         fEDdt.push_back(0);
         fEDda.push_back(1);
         fEDratio.push_back(56);
         fEDstatus.push_back(0);
         fEDcount.push_back(0);
         fEDT.push_back(-1000);
         fEDTA0.push_back(0);
         fEDTA1.push_back(0);
         fEDTR0.push_back(0);
         fEDTR1.push_back(0);
    }
    for(i=0;i<NNMD;i++){
         fMDdt.push_back(120);
         fMDda.push_back(1);
         fMDratio.push_back(103);
         fMDstatus.push_back(0);
         fMDcount.push_back(0);
    }
    for(i=0;i<NNWCDA;i++){
         fWCDAdt.push_back(0);
         fWCDAda.push_back(1);
         fWCDAratio.push_back(50);
         fWCDAstatus.push_back(0);
    }
    for(i=0;i<NNWCDA;i++){
         fWCDA2dt.push_back(0);
         fWCDA2da.push_back(1);
         fWCDA2ratio.push_back(50);
         fWCDA2status.push_back(0);
    }
}
LHCalibration::~LHCalibration()
{

}
int LHCalibration::SetTDCcalibrationKM2A(const char filename[100] )
{
    int i,j,n,m,id;
    float dtt;
    char name[20];
    FILE *calfile;
    for(i=0;i<NNED;i++){  fEDdt[i]=-1000;   }
    for(i=0;i<NNMD;i++){  fMDdt[i]=-1000;   }
    n=0;m=0;
    calfile=fopen(filename,"r");
    if(!calfile){printf("%s not exist or can't read! procedure end up!\n",filename); return 1;}
    KM2ATDCfilename=filename;
    printf("using TDC calibration file %s\n",filename);
    while(!feof(calfile)){
         fscanf(calfile, "%s %d %f\n", name,&id, &dtt);
         if(strcmp(name,"ED")==0){ fEDdt[id]=dtt; n++;}
         else if (strcmp(name,"MD")==0){ fMDdt[id]=dtt; m++; }
    }
    fNED=n;
    fNMD=m;
    printf("TDC %d ED, %d MD\n",n,m);
    fclose(calfile);
 //   SetFinalStatusKM2A();
    return 0; 
}
int LHCalibration::SetADCcalibrationKM2A(const char filename[100] ) 
{
    int i,j,n,m,id;
    float daa,drr;
    char name[20];
    FILE *calfile;
    for(i=0;i<NNED;i++){  fEDda[i]=0;  fEDratio[i]=0;  }
    for(i=0;i<NNMD;i++){  fMDda[i]=0;  fMDratio[i]=0;  }
    n=0;m=0;
    calfile=fopen(filename,"r");
    if(!calfile){printf("%s not exist or can't read! procedure end up!\n",filename); return 1;}
    KM2AADCfilename=filename;
    printf("using ADC calibration file %s\n",filename);
    while(!feof(calfile)){
        fscanf(calfile, "%s %d %f %*f %f %*f\n",name, &id, &daa,&drr);
        if(strcmp(name,"ED")==0){
            if(daa>10&&daa<50){
                fEDda[id]=28.24/daa;
                if(drr>10&&drr<150)fEDratio[id]=drr;
                else fEDratio[id]=0;
                n++;
            }
            else{
                  fEDda[id]=0;
                  fEDratio[id]=0;
            }
        }
        else if(strcmp(name,"MD")==0){
            if(daa>15&&daa<150){
                fMDda[id]=77.66/daa;
                if(drr>10&&drr<150)fMDratio[id]=drr;
                else fMDratio[id]=0;
                  m++;
            }   
            else{
                fMDda[id]=0;
                fMDratio[id]=0;
            }   
              
        }
     }
     fclose(calfile); 
     fNED=n;
     fNMD=m; 
     printf("ADC calibration set: %d ED, %d MD\n",n,m);
 //    SetFinalStatusKM2A();
     return 0;
}
//read 4*6 h average ADC and Temp effeciency result
int LHCalibration::SetADCTcalibrationKM2A(const char filename[100] )
{
    int i,j,n,m,id;
    float daa,drr,edaa,edrr,ta0,ta1,tr0,tr1;
    char name[20],sTemp[300];
    FILE *calfile;
    for(i=0;i<NNED;i++){  fEDda[i]=0;  fEDratio[i]=0;  }
    for(i=0;i<NNMD;i++){  fMDda[i]=0;  fMDratio[i]=0;  }
    n=0;m=0;
    ifstream file1;
    file1.open(filename);
    if(!file1){printf("%s not exist or can't read! procedure end up!\n",filename); return 1;}
    KM2AADCfilename=filename;
    printf("using ADC calibration file %s\n",filename);

    while(file1.good()){
        file1.getline(sTemp,300);
        i=sscanf(sTemp,"%s %d %f %f %f %f %f %*f  %f %*f  %f %*f  %f %*f\n",name, &id, &daa,&edaa,&drr,&edrr,&ta0,&ta1,&tr0,&tr1); 
        if(i>1){
          if(strcmp(name,"ED")==0){
            if(daa>10&&daa<50&&daa/edaa>50){
                fEDda[id]=28.24/daa;
                if(drr>10&&drr<150&&drr/edrr>50)fEDratio[id]=drr;
                n++;
                fEDTA0[id]=ta0; fEDTA1[id]=ta1;
                fEDTR0[id]=tr0; fEDTR1[id]=tr1;
            }
          }
          else if(strcmp(name,"MD")==0){
            if(daa>15&&daa<150){
                fMDda[id]=77.66/daa;
                if(drr>10&&drr<150)fMDratio[id]=drr;
                else fMDratio[id]=0;
                  m++;
            }
         }
       }
    }
    file1.close();
    fNED=n;
    fNMD=m;
    printf("ADC calibration set: %d ED, %d MD\n",n,m);
    return 0;
}
//read 4h ADC and Temperature result
int LHCalibration::SetADCT4hcalibrationKM2A(const char filename[100] )
{
    int i,j,n,m,id;
    float daa,drr,edaa,edrr,t,temp;
    char name[20],sTemp[300];
   // for(i=0;i<NNED;i++){  fEDda[i]=0;  fEDratio[i]=0;  }
   // for(i=0;i<NNMD;i++){  fMDda[i]=0;  fMDratio[i]=0;  }
    n=0;m=0;
    ifstream file1;
    file1.open(filename);
    if(!file1){printf("%s not exist or can't read! procedure end up!\n",filename); return 1;}
    KM2AADC4hfilename=filename;
    printf("using ADC calibration file %s\n",filename);

    while(file1.good()){
        file1.getline(sTemp,300);
        i=sscanf(sTemp,"%s %d %f %f %*f %f %f %*f %f\n",name, &id, &daa,&edaa,&drr,&edrr,&t);
        if(i>1){
          if(strcmp(name,"ED")==0){
            if(daa>10&&daa<50&&daa/edaa>50){
                fEDda[id]=28.24/daa;
                if(drr>10&&drr<150&&drr/edrr>50)fEDratio[id]=drr;
                n++;
                fEDT[id]=t; 
                if(t>-100&&t<100){
                   temp=fEDTA1[id];
                   fEDTA1[id]=daa-fEDTA0[id]*t; //update the A1 using 4h result
                   if(fabs(temp-fEDTA1[id])>0.1*temp&&temp>0){
                       printf("%d cann't update A1 %f %f\n",id, temp,fEDTA1[id]); 
                       fEDTA1[id]=temp; 
                   }
                   if(drr>10&&drr<150&&drr/edrr>50){
                      temp=fEDTR1[id];
                      fEDTR1[id]=drr-fEDTR0[id]*t; //update the R1 using 4h result
                      if(fabs(temp-fEDTR1[id])>0.1*temp&&temp>0){
                          printf("%d cann't update R1 %f %f\n",id,temp,fEDTR1[id]); 
                          fEDTR1[id]=temp;
                      }
                   } 
                }
            }
          }
          /* the MD is not updated using 4h result
          else if(strcmp(name,"MD")==0){//do we need to update MD result using 4h ?
            sscanf(sTemp,"%s %d %f %f %f %f\n",name, &id, &daa,&edaa,&drr,&edrr,&t); 
           if(daa>15&&daa<150){
                temp=fMDda[id]; 
                fMDda[id]=77.66/daa;
                if(fabs(temp-fMDda[id])>0.1*temp&&temp>0){
                    printf("SetADCT4hcalibrationKM2A: MD-A %d  %f %f\n",id,temp,fMDda[id]);
                    fMDda[id]=temp;
                }  
                if(drr>10&&drr<150){
                    temp=fMDratio[id];
                    fMDratio[id]=drr;
                    if(fabs(temp-fMDratio[id])>0.1*temp&&temp>0){
                       printf("SetADCT4hcalibrationKM2A: MD-ratio %d  %f %f\n",id,temp,fMDratio[id]);
                       fMDratio[id]=temp;
                    }
                }
                  m++;
            }
         }
        */
       }
    }
    file1.close();
    fNED=n;
   // fNMD=m;
    printf("ADC calibration set: %d ED, %d MD\n",n,m);
    return 0;
}
int LHCalibration::UpdateADCwithTED(TFile *hfile)
{
    TNtupleD *SlowTree;
    double id,T,entries;
    float da,dr,temp;
    int i,count=0;
    SlowTree = (TNtupleD*)hfile->Get("slow");
    if(!SlowTree) {
            printf("slow error !!");
            return 1;
    }
    else printf("get slow OK\n");
    SlowTree->SetBranchAddress("id", &id);
    SlowTree->SetBranchAddress("T", &T);
    entries = SlowTree->GetEntriesFast();
    printf("Slow event %.0lf\n",entries);
    for(i=0; i<entries; i++){
         SlowTree->GetEntry(i);
         if(T<100&&T>-100) {
            fEDT[id]=float(T);
         }
    }
    for(i=0;i<NNED;i++){  
       if(fEDT[i]>-100&&fEDT[i]<100){
          
          da = fEDTA0[i]*fEDT[i]+fEDTA1[i];
            
          if(da>10&&da<50&&fEDda[i]>0){
              temp=fEDda[i]; 
              fEDda[i]=28.24/da;   
              count++;    
              if(fabs(temp-fEDda[i])>0.1*temp){
                 printf("UpdateADCwithTED failed:%d %f %f %lf\n",i,28.24/temp,28.24/fEDda[i],fEDT[i]);
                 fEDda[i]=temp;
                 count--;
              }
          } 
          /* not update the AD ratio using temperature 
          dr = fEDTR0[i]*fEDT[i]+fEDTR1[i];    
          if(dr>10&&dr<150&&fEDratio[i]>0){
              temp=fEDratio[i];
              fEDratio[i]=dr;
              if(fabs(temp-fEDratio[i])>0.1*temp){
                 printf("UpdateADCwithTED ratio:%d %f %f %lf\n",i,temp,fEDratio[i],fEDT[i]);
              }
           }
          */
       }
    }
    printf("update %d ED with T \n",count);
   return 0;
}

int LHCalibration::SetMDcalibration(const char filename[100] )
{
    int i,j,n,m,id;
    float ta,tt,tr;
    FILE *calfile;
    float MDdt2[NNMD];
    if(fNMD<10){
       for(i=0;i<NNMD;i++){  
          fMDdt[i]=-1000;
          fMDda[i]=0; 
          fMDratio[i]=0;
       }
    }
    for(i=0;i<NNMD;i++) MDdt2[i]=-1000;

    n=0;
    calfile=fopen(filename,"r");
    if(!calfile){printf("%s not exist or can't read! procedure end up!\n",filename); return 1;}
    MDfilename=filename;
    printf("using MD calibration file %s\n",filename);
    fscanf(calfile,"%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s\n");
    while(!feof(calfile)){
         fscanf(calfile,"%*s %d %f %*lf %*lf %*lf %f %*lf %f %*lf %*lf\n",&id,&ta,&tt,&tr);
         //fMDda[id]=ta;
         MDdt2[id]=tt; 
         //fMDratio[id]=tr;
         n++;
    }
    fNMD=n;
    printf("Set %d MD TDC\n",n);
    fclose(calfile);
    //reset MD dt
    n=0;
    double all=0.;
    for(i=0;i<NNMD;i++){
          if(MDdt2[i]>-900&&fMDdt[i]>-900){
               n++;
               all += MDdt2[i]-fMDdt[i];
          }      
    }
    if(n>100) all = all/n;
    else all=0.;
    printf("Reset MD TDC %.2lf ns\n",all); 
    for(i=0;i<NNMD;i++){
         if(MDdt2[i]>-900){
               fMDdt[i]=MDdt2[i]-all;
          }      
    }
 //   SetFinalStatusKM2A();
    return 0;
}
int LHCalibration::SetStatusKM2A(const char filename[100],int cflag=0){
    KM2AStatusfilename = filename;
    TFile *infile=EOSopen(filename,0);
    if(!infile){
        printf("open file error %s !!\n",filename);
        infile->Close();
        return 1;
    }
    TNtupleD *nt = (TNtupleD *)infile->Get("nt1");
    if(!nt){
        printf("get nt1 error %s !!\n",filename);
        infile->Close();
        return 1;
    }
    int i,n=0,m=0;
    for(i=0;i<NNED;i++){  fEDstatus[i]=-1; }
    for(i=0;i<NNMD;i++){  fMDstatus[i]=-1; }
    //mode:id:Frate:Foccu:Foccu2:rate:occu:dtpeak:dtmed:qamax:n:mjd:nevent
    double fmode,fid,fFrate,fFoccu,fFoccu2,dtmed,nevent;
    int mode,id,Frate,Foccu,Foccu2;
    nt->SetBranchAddress("mode",&fmode);
    nt->SetBranchAddress("id",&fid);
    nt->SetBranchAddress("Frate",&fFrate);
    nt->SetBranchAddress("Foccu",&fFoccu);
    nt->SetBranchAddress("Foccu2",&fFoccu2);
    nt->SetBranchAddress("dtmed",&dtmed);
    nt->SetBranchAddress("nevent",&nevent);
    int nTotalentry = nt->GetEntriesFast();
    for(i=0;i<nTotalentry;i++){
        nt->GetEntry(i);
        mode  =int(fmode);
        id    =int(fid);
        Frate =int(fFrate);
        Foccu =int(fFoccu);
        Foccu2=int(fFoccu2);
        if(mode==0){
             if(Frate==0&&Foccu==0){ 
                fEDstatus[id]=0;
                if(fabs(dtmed)>5)printf("ED dt: %d %lf\n",id,dtmed);
                n++;  
             }
        }
        else if(mode==1){
             if(Frate==0&&Foccu==0){
                fMDstatus[id]=0; 
                if(cflag==1&&fMDdt[id]>-900){
                   fMDdt[id]=fMDdt[id]+(dtmed-1);  
                }
                if(fabs(dtmed)>10)printf("MD dt: %d %lf\n",id,dtmed);
                m++;
             }
        }        
    }
    infile->Close();
    fNED=n;
    fNMD=m;
    printf("KM2A Status set: %d good ED, %d good MD\n",n,m);
    //SetFinalStatusKM2A();
    return 0;
}
int LHCalibration::SetFinalStatusKM2A(){
   int i,j,n=0,m=0;
   double x,y,z;
   for(i=0;i<NNED;i++){ 
      if(fEDstatus[i]==0&&(fEDdt[i]<-900||fEDda[i]==0))fEDstatus[i] = 1;
      if(geom->Getxyz(i,x,y,z,1,"ED")<0)fEDstatus[i] = 2;
      if(fEDstatus[i]==0)n++;
      //else printf("%d %lf %lf\n",fEDstatus[i],fEDdt[i],fEDda[i]);
   }
   for(i=0;i<NNMD;i++){  
      if(fMDstatus[i]==0&&(fMDdt[i]<-900||fMDda[i]==0))fMDstatus[i] = 1;
      if(geom->Getxyz(i,x,y,z,1,"MD")<0)fMDstatus[i] = 2;  
      if(fMDstatus[i]==0)m++;
   }
   fNED = n;
   fNMD = m;
   printf("KM2A Final Status set: %d good ED, %d good MD\n",n,m);
   return 0;
}
int LHCalibration::ApplyKM2AEvent(KM2AEvent *event,LHEvent *lhevent, double ft){
    int i,j,k,id,idt,tag,mode,nflag;
    double x,y,z,t,dt,mjd,pe,pd,peda,pedd;

    TClonesArray *Hits;
    KM2AHit *Hit;
    if(ft<1)lhevent->Initcsz();
    Hits = event->Hitlist();
    k = event->Nmd()+event->NHit();
    nflag=0;//to check half event
    for(j=0; j<k; j++){
         Hit = (KM2AHit *)((*Hits)[j]);
         id=Hit->Id();
         t=Hit->Time();
         if(ft>1){
             t=t-ft;
             if(t<4000&&t>0)nflag++;
             if(t<4000)continue;
         }
         if(t<4200)nflag++;
         mode=Hit->Mode();

         pe=Hit->Qa();
         pd=Hit->Qd();
         peda=Hit->Peda();
         pedd=Hit->Pedd();
         tag=Hit->Tag();
         if(mode==0){
             //if(geom->Getxyz(id,x,y,z,1,"ED")<0)continue;
             //if(fEDda[id]==0)continue;
             fEDcount[id]++;
             if(fEDstatus[id]!=0)continue;
             if(pe>0){
                 pe=pe-peda/8.;
                 pd=pd-pedd/8.;
                 if(pe>3700&&fEDratio[id]>10&&fEDratio[id]<150) pe=pd*fEDratio[id];
                 pe = pe*fEDda[id];
             }
             t=t-fEDdt[id];

             if(fEDdt[id]>-900&&fEDda[id]!=0&&pe>=0){
                 lhevent->AddHitE(id,t,pe,0);
             }
          }
          else if(mode==1){
              //if(geom->Getxyz(id,x,y,z,1,"MD")<0)continue;
              //if(fMDda[id]==0)continue;
              fMDcount[id]++;
              if(fMDstatus[id]!=0)continue;
              if(pe>0){
                   pe=(pe-peda/8.*304)*0.0319;
                   pd=(pd-pedd/8.*304)*0.0319;
                   if(tag>0&&fMDratio[id]>10&&fMDratio[id]<150) pe=pd*fMDratio[id];
                   pe = pe*fMDda[id];
              }
              t=t-fMDdt[id];
              if(fMDdt[id]>-900&&fMDda[id]!=0&&pe>=0)  {
                  idt=id;
                  if(event->Mjd()<58978.4327986){ //temporay for MD DAQ id error
                    if(id==450) idt=1236; 
                    if(id==1028)idt=1058;
                    if(id==1058)idt=1028;
                  }
                  if(event->Mjd()<59418.5202){//error corrected on 2021-7-23 run117916,
                     if(id==78)idt=103;
                     if(id==103)idt=78;
                  } 
                  lhevent->AddHitM(idt,t,pe,0);
            }
       }
    }
    return nflag;
}
int LHCalibration::GetEDcount(){
   int i,n=0;
   for(i=0;i<NNED;i++){
      if(fEDcount[i]>0)n++;
   }
   return n;
}
int LHCalibration::GetEDcountGood(){
   int i,n=0;
   for(i=0;i<NNED;i++){
      if(fEDcount[i]>0&&fEDstatus[i]==0)n++;
   }
   return n;
}
int LHCalibration::GetMDcount(){
   int i,n=0;
   for(i=0;i<NNMD;i++){
      if(fMDcount[i]>0)n++;
   }
   return n;
}
int LHCalibration::GetMDcountGood(){
   int i,n=0;
   for(i=0;i<NNMD;i++){
      if(fMDcount[i]>0&&fMDstatus[i]==0)n++;
   }
   return n;
}
