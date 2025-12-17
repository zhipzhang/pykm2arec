#define PI 3.14159265358979312
double getNKGdensity(double age,double size, double r){
    double cs,ro,rm=130.; //the molliere radius is fixed to be 130m
    cs = TMath::Gamma(4.5-age)/(TMath::Gamma(age-2.5+2.)*TMath::Gamma(4.5+2.5-2.0-2.*age));
    cs = cs/(2*PI*rm*rm);
    ro=cs*size*pow(r/rm,age-2.5)*pow(1+r/rm,age-4.5);
    return ro;
}
double getzenithcorrect(double e){
   if(e<1.4) e=1.4;
   //double tt=-0.969512*e+3.13405;
   double tt=-1.30673*e+3.87289;
   if(tt<0)tt=0;
   return tt; 
}
double recer50new2(double age,double size,double theta){
   /*zenith :1/cos-1:0-0.05, 0.05-0.10,... */
  //a=-2.7, NpW>0, dr<2, ratio<-2.3
 //  double p1[12]={1.01036, 0.99026, 0.97132, 0.94702, 0.93152, 0.91419, 0.89510, 0.87000, 0.85355, 0.86067, 0.84731, 0.81870,};
 //  double p0[12]={1.75015, 1.75883, 1.77451, 1.80289, 1.83192, 1.86855, 1.91583, 1.96680, 2.01119, 2.05386, 2.09785, 2.15866,};  
   //a=-2.7, NpW>20, dr<2, ratio<-2.3
   double p1[12]={1.01406, 0.99442, 0.97522, 0.95388, 0.93582, 0.91890, 0.89844, 0.87645, 0.85806, 0.86456, 0.85011, 0.81312,};
   double p0[12]={1.73539, 1.74290, 1.75802, 1.78547, 1.81716, 1.85552, 1.90326, 1.95510, 2.00124, 2.04296, 2.08653, 2.15147,};
     	double r50,re;
   int k;
   if(age<0.6||age>2.4)return -1;
    if(theta<0||theta*57.3>51)return -1;
    k=int((1./cos(theta)-1)/0.05);
    if(k>11)k=11;
    if(k<0)k=0;
    r50=log10(getNKGdensity(age,size,50.));
    return r50*p1[k]+p0[k];
}
double recer50new3(double age,double size,double theta){
   double p2[12]={0.02503, 0.02361, 0.02489, 0.02357, 0.02149, 0.02303, 0.01852, 0.01409, 0.01289, 0.02556, 0.03406, 0.03635,};
   double p1[12]={0.96475, 0.94221, 0.91961, 0.90142, 0.88660, 0.86839, 0.86159, 0.85272, 0.84218, 0.82113, 0.79659, 0.77589};
   double p0[12]={1.74558, 1.75789, 1.77675, 1.80182, 1.83308, 1.86808, 1.91001, 1.95468, 1.99553, 2.04191, 2.09133, 2.14474};
   double r50,re;
   int k;
   if(age<0.6||age>2.4)return -1;
    if(theta<0||theta*57.3>51)return -1;
    k=int((1./cos(theta)-1)/0.05);
    if(k>11)k=11;
    if(k<0)k=0;
    r50=log10(getNKGdensity(age,size,50.));
    return pow(r50,2)*p2[k] + r50*p1[k] + p0[k];
}
double recer50(double age,double size,double theta){
    double r50;
    r50=getNKGdensity(age,size,50.);
    return 0.93600891*log10(r50)+0.91350799*pow(1./cos(theta)-1,2)+1.7790728;
}
double receNfit(double nhit){
    if(nhit>5)return 0.446739*log10(nhit)*log10(nhit)-0.159119*log10(nhit)+0.44573 ; 
    else return 0;
}
int CutKM2AErecR50(double erec,double ratio){
 int i,n,m;
   double ll;
   double cut[12]={-5.11, -5.24, -5.95, -6.08, -2.34, -2.35, -2.36, -2.36, -2.36, -2.36,-2.36,-2.36};
   if(erec<1)return -1;
     n=int((erec-1)/0.2);
     if(n>11)n=11;
     if(ratio<cut[n])return n;
     else return -1;
}
int CutKM2AErecNfit(double erec,double ratio){ //erec=log10(E/TeV)//ratio=log10(Nu+0.1/Ne)
    int i,n,m;
    double ll;
    //zenith<50 all >60% 
    //pgamma 0.81, 0.77, 0.73, 0.65, 0.64, 0.74, 0.76, 0.70, 0.87, 0.89, 
    //pback 1.30e-02, 6.90e-03, 1.74e-03, 4.48e-04, 2.33e-04, 1.56e-04, 1.30e-04, 6.41e-05, 1.42e-04, 3.23e-05, 
    //Q 7.16, 9.31, 17.40, 30.53, 41.96, 59.06, 66.98, 87.77, 72.76, 155.76, 
    double cut[10]={-2.20, -2.50, -2.60, -2.80, -2.60, -2.50, -2.50, -2.60, -2.40, -2.40};  
    //zenith<50 all >60% exclude sqrt((corex-20)**2+(corey+510)**2)<120
    //pgamma 0.81, 0.77, 0.72, 0.64, 0.69, 0.73, 0.76, 0.70, 0.87, 0.88, 
    //pback 1.19e-02, 6.35e-03, 1.61e-03, 4.28e-04, 2.60e-04, 1.49e-04, 1.29e-04, 4.95e-05, 1.37e-04, 3.38e-05, 
    //Q=7.45, 9.66, 18.01, 31.04, 42.85, 60.06, 66.76, 99.31, 73.81, 151.93, 
    //double cut[10]={-2.20, -2.50, -2.60, -2.80, -2.50, -2.50, -2.50, -2.60, -2.40, -2.40}; 
    if(erec<1)return -1;
     n=int((erec-1)/0.2);
     if(n>9)n=9;
     if(ratio<cut[n])return n;
     else return -1;
}
double rece2(double ne,double theta=-1){ //NpE2+theta+core rec energy,return energy log10(E) (TeV)
    double ene=0,ezen=0,er=0;
    if(ne>5&&theta>-0.1){ //NpE1/NpE2>2 dr<40m
        return 0.94789566*log10(ne)+0.60047041/pow(cos(theta),2)-0.91006520/cos(theta)+0.32395902;
    }
    else if(ne>5&&theta<0){ //all event
        return 0.955211*log10(ne)+0.13467;
    }
    else return -10;
};
int CutKM2AErecNe2(double erec,double ratio){ //erec=log10(E/TeV)//ratio=log10(Nu+0.1/Ne)
    int i,n,m;
    double ll;
    double cut[10]={-2.30, -2.70, -2.90, -3.00, -2.40, -2.40, -2.40, -2.30, -2.50, -2.30,};     
     if(erec<1)return -1;
     n=int((erec-1)/0.2);
     if(n>9)n=9;
     if(ratio<cut[n])return n;
     else return -1;
}
int  corefilter(double x, double y, int flag=0){
    double dr,phi;
    if(flag==0){
       dr=sqrt((x+355)*(x+355)+(y-195)*(y-195)); 
       if(dr<160)return 1;
       else return 0;
    }
    else if(flag==1) { 
        dr=sqrt(x*x+(y+20)*(y+20));
        phi=atan2(x,-(y+20))*180/PI; 
        if(phi<0)phi += 360;    
        if(dr>250&&dr<575&&phi>208&&phi<273)return 1;
        else return 0;   
    }
    else{
       printf("corefilter() flag error\n");
       return 0;
   }
};
double GetSpaceLC(double zen1, double phi1, double zen2, double phi2)
{
    if(zen1<0||zen2<0)return -1;

    double dr,dl0,dm0,dz0,dl1,dm1,dz1,cs;

       dl0= sin(zen1)*cos(phi1);
       dm0= sin(zen1)*sin(phi1);
       dz0= cos(zen1);

       dl1 = sin(zen2)*cos(phi2);
       dm1 = sin(zen2)*sin(phi2);
       dz1 = cos(zen2);
       cs=dl0*dl1+dm0*dm1+dz0*dz1;
       if(cs>1.0)cs=1.0;
     dr = acos(cs)*57.295780;
    return dr;
}
