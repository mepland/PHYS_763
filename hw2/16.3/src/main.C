#include <TROOT.h>
//#include <TF1.h>
//#include <TF2.h>
//#include <TMinuit.h>
//#include <TH1.h>
//#include <TH2.h>
//#include <TCanvas.h>
//#include <TLegend.h>
//#include <TPaveStats.h>
//#include <TMath.h>
#include <iostream>
#include <iomanip>
//#include <stdio.h>
//#include <fstream>
#include <stdlib.h>
//#include <cmath>
#include <random>
//#include <vector>
//#include <string>
//#include <cstring>
//#include <TFile.h>
//#include <THStack.h>
//#include <TLorentzVector.h>
//#include <TString.h>
//#include <TPad.h>
//#include <TFitResult.h>
//#include "myClassStyle.h"
#include "sequence.h"


using std::cout; using std::endl;
//using std::cin;
//using std::cerr;
//using std::string;
//using std::pow;

//Double_t g2(Double_t *x, Double_t *par);
//Double_t fun2(Double_t *x, Double_t *par);


int main(){
  cout << endl;

//////////////////////////////////////////////////////////////////

  int seed = 7;
  int n = 10000;
     
  cout << "Seed = " << seed << endl;
  cout << "n = " << n << endl;

  // Set up the random number generator/seed
  std::default_random_engine engine(seed);
  std::normal_distribution<double> normal(0.0,1.0);

  double s0 = normal(engine);

//////////////////////////////////////////////////////////////////
  // Generate sequences


  sequence seq1(0.5, n, engine, normal, s0);

  cout.precision(5);
  cout << endl << "Seq1" << endl << "Alpha = " << std::setw(3) << seq1.GetAlpha() << " Mean: " << std::setw(8) << seq1.GetMean() << " Tau: " << std::setw(8) << seq1.GetTau() << " Sigma_M: " << std::setw(8) << seq1.GetSigma_M() << " Usual_Var: " << std::setw(8) << seq1.GetUsual_Variance() << endl;



//////////////////////////////////////////////////////////////////
// Clean up and exit 

  cout << endl << "Success!" << endl << endl;

  return 0;
}

/*

Double_t g2(Double_t *x, Double_t *par) {
   Double_t r1 = Double_t((x[0]-par[1])/par[2]);
   Double_t r2 = Double_t((x[1]-par[3])/par[4]);
   return par[0]*TMath::Exp(-0.5*(r1*r1+r2*r2));
}   

Double_t fun2(Double_t *x, Double_t *par) {
   Double_t *p1 = &par[0];
   Double_t result = g2(x,p1);
   return result;
}

*/
