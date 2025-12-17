/*  main reconstruction code for LHAASOFM
 *  If you find any bug please send email to
 *  chensz@ihep.ac.cn
 *    2016-04-18 
 */
#ifndef __G4KM2A_RECONSTRUCTION_HH__
#define __G4KM2A_RECONSTRUCTION_HH__

#include "G4KM2A_Geometry.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompSVD.h"
#include "math.h"
#include "LHEvent.h"
#include "LHRecEvent.h"
//#include "LHlikehood.h"
#define PI 3.14159265358979312
#include "TMinuit.h"
#include "TMath.h"
//#include "TRandom.h"
#include "LHCalibration.h"

//#include "EOSopen.h"
class G4KM2A_Reconstruction
{
  public:
     static G4KM2A_Reconstruction * GetInstance(int tFlag)
      {

       if (m_myself == 0) m_myself = new G4KM2A_Reconstruction(tFlag);
       return m_myself;
      }
     ~G4KM2A_Reconstruction();
     G4KM2A_Reconstruction() = default;
     G4KM2A_Reconstruction(int tFlag);
  protected:
  
    // G4KM2A_Reconstruction(int tFlag);
     static G4KM2A_Reconstruction * m_myself;
     //static int Flag;
     static TMinuit *minuit;//=new TMinuit(4);
     static double _np,_dirl,_dirm,_dirn,_maxR;
     static char _style[20];
     //static std::vector<float>  nED;
     //static std::vector<float>  nWCDA;
     static float nED[6000];
     static int rED[6000];
     static float rMD[2000];
     static float nMD[2000]; 
     static float nWCDA[4000];
     //LHEvent *event;
     //TClonesArray *HitsE, *HitsM, *HitsW;
     //LHHit *Hit;
     static G4KM2A_Geometry *geom;// = G4KM2A_Geometry::GetInstance(ARRAY); 
  public:
     int   SetDetetorStatus(const char file[500],int *out);
     int   SetDetetorStatus(LHCalibration *cal);
     int   Init(LHRecEvent *trec);
     int   getliveMD(LHRecEvent *trec,double rmin, double rmax);
     int   SetBadHit(TClonesArray &tHits,int np,const char *style);
     int   setnparticle(TClonesArray &tHits,int np, double pe);
     int   trigger(TClonesArray &tHits,int np,int twind,const char *style,int flag); 
     int   timefilter(TClonesArray &tHits,int np,int twind,const char *style, int flag);
     int   spacetimefilter(TClonesArray &tHits,int np,int twind,int rwind,const char *style, int flag);
     int   resetED(TClonesArray &tHits,int np,int ncut);
     int   planarfit(TClonesArray &tHits,int np,LHRecEvent *trec,const char *style);
     int   conicalfit(TClonesArray &tHits,int np,LHRecEvent *trec,float alpha,const char *style);
     void  core_centre(TClonesArray &tHits,int np,LHRecEvent *trec,const char *style);
     int   core_centre2(TClonesArray &tHits,int np,int rp,LHRecEvent *trec,const char *style);
     double compact(TClonesArray &tHits,int np,int rp,LHRecEvent *trec,const char *style);
     int   noisefilterPlanar(TClonesArray &tHits,int np,int twind,int twinu,LHRecEvent *trec,const char *style);
     int   noisefilter(TClonesArray &tHits,int np,int twind,int twinu,int rwin,LHRecEvent *trec,const char *style);
     int   setresidual(TClonesArray &tHits,int np,LHRecEvent *trec,const char *style);
     float getnp(TClonesArray &tHits,int np,int rcut,LHRecEvent *trec,const char *style);
     float getmuM(TClonesArray &tHits,int np,int rcut,LHRecEvent *trec);
     float getmuW(TClonesArray &tHits,int np,int cut,LHRecEvent *trec);
     void setEDdr(LHRecEvent *trec);
     void setMDdr(LHRecEvent *trec);
     //for likelihood method 
     //NKG function1 for likelihood1 taken into account the detectors with no hit
     static void NKGfunction1(int &npar,double *gin,double &f,double *par,int iflag);
     static void NKGfunctionMD(int &npar,double *gin,double &f,double *par,int iflag);
     void NKGchiED(LHRecEvent *trec,double rmin,double rmax,int nbin);
     void NKGchiMD(LHRecEvent *trec,double rmin,double rmax,int nbin);
     int core_likelihood(TClonesArray &tHits,int np,LHRecEvent *trec,const char *style);
     int core_likelihoodMD(TClonesArray &tHits,int np,LHRecEvent *trec);

     int   eventrecline(LHEvent *tevent,LHRecEvent *trec, int nflag);

    // LHRecEvent *rec;
};

#endif /* __G4KM2A_RECONSTRUCTION_HH__ */ 
