#include <TROOT.h>
//#include <TF1.h>
//#include <TF2.h>
//#include <TMinuit.h>
#include <TH1.h>
//#include <TH2.h>
#include <TCanvas.h>
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
#include <TString.h>
//#include <TPad.h>
//#include <TFitResult.h>
#include "myClassStyle.h"
#include "sequence.h"


using std::cout; using std::endl;
//using std::cin;
//using std::cerr;
//using std::string;
using std::pow;

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


  // setup the var vs alpha histogram
  char buffer[300]; int tmp; (void)tmp; // Cast tmp to void to suppress unused var warning

  double bin_size = 0.1; // TODO adjust
  double axis_max = 10;
  double axis_min= 0;
  int nbins = std::round((axis_max-axis_min)/bin_size);
  bin_size = (axis_max-axis_min)/static_cast<double>(nbins);

  tmp = sprintf(buffer, "simple var vs alpha; #font[52]{#alpha}; Events / %.3g", ((axis_max-axis_min)/nbins));
  TH1F *var_vs_alpha_hist = new TH1F("var_vs_alpha_hist", buffer, nbins, axis_min, axis_max);

  tmp = sprintf(buffer, "theory var vs alpha; #font[52]{#alpha}; Events / %.3g", ((axis_max-axis_min)/nbins));
  TH1F *theory_var_vs_alpha_hist = new TH1F("theory_var_vs_alpha_hist", buffer, nbins, axis_min, axis_max);



//////////////////////////////////////////////////////////////////
  // Generate sequences


  sequence seq1(0.505, n, engine, normal, s0); // for gauss plot and debugging, do 0.505 so the for loop doesn't hit it too...

//  cout.precision(5);
//  cout << endl << "Seq1" << endl << "Alpha = " << std::setw(3) << seq1.GetAlpha() << " Mean: " << std::setw(8) << seq1.GetMean() << " Tau: " << std::setw(8) << seq1.GetTau() << " Sigma_M: " << std::setw(8) << seq1.GetSigma_M() << " Usual_Var: " << std::setw(8) << seq1.GetUsual_Variance() << endl; // TODO debugging

  cout << endl;

  std::vector<sequence> alpha_sequences;
  int alpha_nsteps = 100-1;
//  double alpha_step_size = (1.0-0.0)/static_cast<double>(alpha_nsteps);
  double alpha_step_size = (1.0/100.0);
  double alpha_min = alpha_step_size; // Need to start indexing through the vector at 0, but alpha >0...
  for(int alpha_step=0; alpha_step < alpha_nsteps; alpha_step++){
    if(alpha_step%10 == 0) cout << "now processing alpha = " << alpha_min+alpha_step_size*alpha_step << endl;
    alpha_sequences.push_back(sequence(alpha_min+alpha_step_size*alpha_step, n, engine, normal, s0));
    var_vs_alpha_hist->Fill(alpha_sequences.at(alpha_step).GetAlpha(), alpha_sequences.at(alpha_step).GetUsual_Variance()); 
    theory_var_vs_alpha_hist->Fill(alpha_sequences.at(alpha_step).GetAlpha(), pow(alpha_sequences.at(alpha_step).GetSigma_M(),2)); 
  }

//////////////////////////////////////////////////////////////////
  // Create plots output directories. WARNING system dependent
  TString OUTPUT_PATH = "./output";
  system("mkdir -p "+OUTPUT_PATH);
  system("rm "+OUTPUT_PATH+"/*.pdf");
  system("rm "+OUTPUT_PATH+"/*.png");

  // Get Plot
  TH1F *sequence_hist = seq1.GetSequence_hist();

//////////////////////////////////////////////////////////////////
// Print Graphics

  SetClassStyle(); // Set style for plots, see myClassStyle.h, adapted from the ATLAS Experiment style

  TCanvas *c1 = new TCanvas("c1", "rectangle", 800, 600); c1->cd(); c1->Clear();

  double margin = 0.18;
  gPad->SetBottomMargin(margin); gPad->SetTopMargin(margin);
  gPad->SetLeftMargin(margin); gPad->SetRightMargin(margin);

//////////////////////////
  c1->cd(); c1->Clear();

  sequence_hist->GetXaxis()->SetTitleOffset(1.5);
  sequence_hist->GetYaxis()->SetTitleOffset(1.5);

  cout << endl << "Perform gaus fit for one sequence, mean and all statistics below: " << endl;
  sequence_hist->Fit("gaus"); // use the automatic Gaussian fitting

  sequence_hist->Draw("same PE");

  c1->Print(OUTPUT_PATH+"/pair_correlation.pdf", "pdf");
  c1->Print(OUTPUT_PATH+"/pair_correlation.png", "png");

//////////////////////////
  cout << endl;
  c1->cd(); c1->Clear();

  var_vs_alpha_hist->GetXaxis()->SetTitleOffset(1.5);
  var_vs_alpha_hist->GetYaxis()->SetTitleOffset(1.5);

  theory_var_vs_alpha_hist->GetXaxis()->SetTitleOffset(1.5);
  theory_var_vs_alpha_hist->GetYaxis()->SetTitleOffset(1.5);

  theory_var_vs_alpha_hist->Draw("hist");
  var_vs_alpha_hist->Draw("same PE");

  c1->Print(OUTPUT_PATH+"/var_vs_alpha.pdf", "pdf");
  c1->Print(OUTPUT_PATH+"/var_vs_alpha.png", "png");


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
