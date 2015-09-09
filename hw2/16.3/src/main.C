//#include <TROOT.h>
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



/*
  // Create Plots

  char buffer[300]; int tmp; (void)tmp; // Cast tmp to void to suppress unused var warning

  int nbins = 1000;
  double axis_min = -4.0;
  double axis_max = 4.0;

  TH1::SetDefaultSumw2(1); // All hist uncertainty tracking

  tmp = sprintf(buffer, "random_x vs random_y; #font[52]{x}_{Normal}; #font[52]{y}_{Normal}; Events / (%.3g x %.3g)", ((axis_max-axis_min)/nbins), ((axis_max-axis_min)/nbins));
  TH2F *square_hist = new TH2F("square_hist", buffer, nbins, axis_min, axis_max, nbins, axis_min, axis_max);

  // Fill
  for(int i=0; i<n; i++){
    square_hist->Fill(random_x[i], random_y[i]);
  }

  // Fit
  TF2 *f_2d_gaus = new TF2("f_2d_gaus", fun2,-4.0,4.0,-4.0,4.0,5);
  f_2d_gaus->SetParName(0, "amplitude");
  f_2d_gaus->SetParName(1, "x mean");
  f_2d_gaus->SetParName(2, "x std dev");
  f_2d_gaus->SetParName(3, "y mean");
  f_2d_gaus->SetParName(4, "y std dev");

  f_2d_gaus->SetParameter(0, n/(nbins*nbins));

  f_2d_gaus->SetParameter(1, 0.0);
  f_2d_gaus->SetParameter(2, 1.0);
  f_2d_gaus->SetParameter(3, 0.0);
  f_2d_gaus->SetParameter(4, 1.0);



  // Graphics

  SetClassStyle(); // Set style for plots, see myClassStyle.h, adapted from the ATLAS Experiment style

  //  TCanvas *c1 = new TCanvas("c1", "rectangle", 800, 600); c1->cd(); c1->Clear();
  TCanvas *c2 = new TCanvas("c1", "square", 800, 800); c2->cd(); c2->Clear();

  double margin = 0.18;
  gPad->SetBottomMargin(margin); gPad->SetTopMargin(margin);
  gPad->SetLeftMargin(margin); gPad->SetRightMargin(margin);

  gStyle->SetPalette(1);

  square_hist->GetXaxis()->SetTitleOffset(1.5);
  square_hist->GetYaxis()->SetTitleOffset(1.5);
  square_hist->GetZaxis()->SetTitleOffset(1.5);

  square_hist->Draw("COLZ");

//  c2->Print(OUTPUT_PATH+"/normal.pdf", "pdf");
  c2->Print(OUTPUT_PATH+"/normal.png", "png");

  cout << endl;
  TFitResultPtr r = square_hist->Fit("f_2d_gaus", "RS");

  int dof = n-1-1;

  Double_t chi2 = r->Chi2(); // to retrieve the fit chi2
  cout << endl << "Reduced Chi2 = " << chi2/static_cast<double>(dof) << endl << endl;

  c2->Clear();
//  square_hist->Draw("LEGO0");
  square_hist->Draw("SURF20Z");

//  c2->Print(OUTPUT_PATH+"/normal_fit_3d.pdf", "pdf");
  c2->Print(OUTPUT_PATH+"/normal_fit_3d.png", "png");
*/

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
