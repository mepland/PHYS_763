#include <TROOT.h>
//#include <TF1.h>
#include <TF2.h>
#include <TMinuit.h>
//#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
//#include <TLegend.h>
//#include <TPaveStats.h>
//#include <TMath.h>
#include <iostream>
//#include <iomanip>
//#include <stdio.h>
//#include <fstream>
#include <stdlib.h>
#include <cmath>
//#include <string>
//#include <cstring>
//#include <TFile.h>
//#include <THStack.h>
//#include <TLorentzVector.h>
#include <TString.h>
//#include <TPad.h>
#include <TFitResult.h>
#include "myClassStyle.h"

using std::cout; using std::endl;
using std::cin;
//using std::cerr;
//using std::string;
using std::pow;
using std::rand;

double my_expectation(double* array, int n, int power);
double my_pair_correlation(double* array, int n, int k);

Double_t const_func(Double_t *x, Double_t *par);

int main(){
  cout << endl;

//////////////////////////////////////////////////////////////////

  int seed = -99;
  int n = -99;
  char run_type = 'd';

  do{
    cout << "Enter 'd' to run with default values or 'u' to user specify values" << endl;
    cin >> run_type;

    if(run_type == 'd'){
      seed = 7;
      n = 10000000;
      }
    else if(run_type == 'u'){
      cout << endl << "Enter seed: " ;
      cin >> seed;
      if(seed<=0) seed = 7;
  
      do{
        cout << endl << "Enter number of random numbers to be generated: ";
        cin >> n;
      }while(n <=0);
    }

    if( (run_type != 'd') && (run_type != 'u') ) cout << "Please enter d or u" << endl << endl;

  }while( (run_type != 'd') && (run_type != 'u') );
     
  cout << "Seed = " << seed << endl;
  cout << "n = " << n << endl;

  // Set up the random number generator/seed, default is 7 
  srand(seed);

  // Set up dynamically allocated arrays to hold the random numbers
  double* random_x = NULL;
  double* random_y = NULL;
  random_x = new double[n];
  random_y = new double[n];

  // Fill arrays with random numbers between 0 and 1
  for(int i=0; i<n; i++){
    random_x[i] = rand()/static_cast<double>(RAND_MAX);
    random_y[i] = rand()/static_cast<double>(RAND_MAX);
  }


//////////////////////////////////////////////////////////////////
  // Do 1D tests on random_x

  double mean = my_expectation(random_x, n, 1); // <x>, should be 1/2
  double variance = my_expectation(random_x, n, 2) - pow(mean, 2); // <x^2> - <x>^2, should be 1/12
 
  cout << endl << "Mean = " << mean << endl;
  cout << "Difference from 1/2 = " << mean-0.5 << endl;

  cout << endl << "Variance = " << variance << endl;
  cout << "Difference from 1/12 = " << variance-(1.0/12.0) << endl;

    int k = 1;
    double pair_correlation = 0;

    cout << endl << "Now performing pair correlation tests <x_{i+k} x_{i}>, enter invalid k (<=0) to stop." << endl;

    do{

      cout << endl << "Enter k: ";
      cin >> k;
 
      if(k>=n){
        cout << "k must be less than n, please reenter." << endl;
        continue;
        }
      else if(k<=0){
        cout << "Invalid k <= 0, pair correlation tests completed." << endl;
        continue;
        }
         
      pair_correlation = my_pair_correlation(random_x, n, k);

      cout << endl << "Pair Correlation, k = " << k << ": " << pair_correlation << endl;
      cout << "Difference from 1/4 = " << pair_correlation - 0.25 << endl;

    }while(k>0);


//////////////////////////////////////////////////////////////////
  // Create plots output directories. WARNING system dependent
  TString OUTPUT_PATH = "./output";
  system("mkdir -p "+OUTPUT_PATH);
  system("rm "+OUTPUT_PATH+"/*.pdf");
  system("rm "+OUTPUT_PATH+"/*.png");

  // Create Plots

  char buffer[300]; int tmp; (void)tmp; // Cast tmp to void to suppress unused var warning

  int nbins = 500;
  double axis_min = -0.1;
  double axis_max = 1.1;

  TH1::SetDefaultSumw2(1); // All hist uncertainty tracking

  tmp = sprintf(buffer, "random_x vs random_y; #font[52]{x}_{Uniform}; #font[52]{y}_{Uniform}; Events / (%.3g x %.3g)", ((axis_max-axis_min)/nbins), ((axis_max-axis_min)/nbins));
  TH2F *square_hist = new TH2F("square_hist", buffer, nbins, axis_min, axis_max, nbins, axis_min, axis_max);

  // Fill
  for(int i=0; i<n; i++){
    square_hist->Fill(random_x[i], random_y[i]);
  }

  // Fit
  TF2 *f_const = new TF2("f_const", const_func,0.0,1.0,0.0,1.0,1);
  f_const->SetParName(0, "Constant");
  f_const->SetParameter(0, n/(nbins*nbins));

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

//  c2->Print(OUTPUT_PATH+"/uniform.pdf", "pdf");
  c2->Print(OUTPUT_PATH+"/uniform.png", "png");

  cout << endl;
  TFitResultPtr r = square_hist->Fit("f_const", "RS");

  int dof = n-1-1;

  Double_t chi2 = r->Chi2(); // to retrieve the fit chi2
  cout << endl << "Reduced Chi2 = " << chi2/static_cast<double>(dof) << endl << endl;

  c2->Clear();
//  square_hist->Draw("LEGO0");
  square_hist->Draw("SURF20Z");

//  c2->Print(OUTPUT_PATH+"/uniform_fit_3d.pdf", "pdf");
  c2->Print(OUTPUT_PATH+"/uniform_fit_3d.png", "png");


//////////////////////////////////////////////////////////////////
// Clean up and exit 

  delete random_x;
  delete random_y;
  random_x = NULL;
  random_y = NULL;

  cout << endl << "Success!" << endl << endl;

  return 0;
}


double my_expectation(double* array, int n, int power){

  double sum = 0; 
  for(int i=0; i<n; i++){
    sum += pow(array[i], power);
  }

  return sum/static_cast<double>(n);
}


double my_pair_correlation(double* array, int n, int k){

  double sum = 0; 
  for(int i=0; i<n-k; i++){ // stop at n-k so i+k doesn't go out of bounds
    sum += (array[i+k]*array[i]) ;
  }

  return sum/static_cast<double>(n);
}

Double_t const_func(Double_t *x, Double_t *par){
 return par[0];
}  
