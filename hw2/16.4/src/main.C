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
#include <math.h> 
#include <random>
#include <algorithm> 
#include <vector>
//#include <string>
//#include <cstring>
//#include <TFile.h>
//#include <THStack.h>
//#include <TLorentzVector.h>
#include <TString.h>
//#include <TPad.h>
//#include <TFitResult.h>
#include "myClassStyle.h"
#include "hsphere.h"


using std::cout; using std::endl;
//using std::cin;
//using std::cerr;
//using std::string;
using std::pow;

bool Overlap(double L, double D, hsphere existing_sphere, hsphere test_sphere, double x, double dX); 
double my_pair_sep_expectation(std::vector<hsphere> spheres, int power);
double my_pair_sep_var(std::vector<hsphere> spheres);
double gx(double x, double D, double L, int n);
double gjx(double x, int j, double D, double L, int n);
int factorial(int n);

double pair_cor(std::vector<hsphere> spheres, double x, double dX, double L, double D, int n, int nbins);

int main(){
  cout << endl;

//////////////////////////////////////////////////////////////////
// Initialization

  int seed = 7;

  // TODO adjust parameters
  int n = 12; // Factorials of this, can't be large at all...
  double L = 1000.0; // Length
  double D = 10.0; // Ball diameter

  double std_dev = L/4.0;
  double int_std_dev = L/6.0;
 
  // halting var declarations
  std::vector<double> past_pair_sep_var;
  bool keep_going = true;
  int sweep_number = 0;
  int count_down = -1;

  // Thermal calc vars
//  double pair_correlation = 0;


  std::vector<hsphere> spheres;

  // Set up the normal random number generator/seed
  std::default_random_engine engine(seed);
  std::normal_distribution<double> normal(0.0,std_dev);// Will be setup the same way in the class when used

  // Setup srand for the random_shuffle function
  std::srand(seed+1);

  // Get pointer to the engine to pass to class
  std::default_random_engine *engine_ptr = &engine;

  // Fill the ids vector with all the spheres ids
  std::vector<int> ids;
  for (int i=0; i<n; ++i) ids.push_back(i); // 0 1 2 ...


//////////////////////////////////////////////////////////////////
  // Generate starting distribution

  // Always pass the first one  
  spheres.push_back(hsphere(0, L, int_std_dev, std_dev, engine_ptr)); 
  spheres.at(0).Int_Move();
  spheres.at(0).Accept();

  // Now do the rest
  for(int i=1; i<n; i++){
    spheres.push_back(hsphere(i, L, int_std_dev, std_dev, engine_ptr)); 
    int passed;
    do{
      passed = 0;
      spheres.at(i).Int_Move();
      for(int j=0; j<i; j++){
        if ( Overlap(L, D, spheres.at(j), spheres.at(i), -99, -99) ){break;} // can stop looking if one overlaps
        passed++; // no overlap
      }
    }while(passed<i); // keep going till it passes all of them
    spheres.at(i).Accept(); // Accept it
  }

  cout << "Starting Positions Initialized" << endl;

//////////////////////////////////////////////////////////////////
  // Run sweeps till spatial distribution is approx uniform
 
do{
//////////////////////////
  // Update every spheres position, going through them randomly

  std::random_shuffle ( ids.begin(), ids.end() ); // shuffle ids using built-in random generator

  // loop through spheres in random ids order
  for(int i : ids){
    int passed;
    do{
      passed = 0;
      spheres.at(i).Test_Move();
      for(int j=0; j<n; j++){
        if ( Overlap(L, D, spheres.at(j), spheres.at(i), -99, -99) ){break;} // can stop looking if one overlaps
        passed++; // no overlap
      }
    }while(passed<n); // keep going till it passes all of them
    spheres.at(i).Accept(); // Accept it
  }

//////////////////////////
  // Check if the spatial distribution is approx uniform yet

  // Find seperation of nearest neighbor pairs
  for(std::vector<hsphere>::iterator it1 = spheres.begin(); it1 != spheres.end(); ++it1){
    it1->SetPairSep(-99);
    double min_pair_sep = 2*L; // sqrt(2)*L < 2L, preping for 2D case
    for(std::vector<hsphere>::iterator it2 = spheres.begin(); it2 != spheres.end(); ++it2){
      double pair_sep = std::sqrt( pow(it1->GetX() - it2->GetX(),2) );
      if ( (pair_sep <= min_pair_sep) && (it1->GetID() != it2->GetID()) ){
        it1->SetPairSep(pair_sep);
        min_pair_sep = pair_sep;
      }
    }
  }

  // compute varience of the sep
  double pair_sep_var = my_pair_sep_var(spheres);

  // save past <=sweeps_to_ave sep_var's
  int sweeps_to_ave = 10;

  if( sweep_number < sweeps_to_ave ){
    past_pair_sep_var.push_back(pair_sep_var);
  }
  else if(sweep_number >=sweeps_to_ave){
    past_pair_sep_var.erase(past_pair_sep_var.begin());
    past_pair_sep_var.push_back(pair_sep_var);
  }
  // compute average of past <= sweeps_to_ave sep_var's
  double pair_sep_var_ave = 0;
  int pair_sep_var_ave_n = 0;
  for(std::vector<double>::iterator it = past_pair_sep_var.begin(); it != past_pair_sep_var.end(); ++it){
    pair_sep_var_ave_n++;
    pair_sep_var_ave += *it;
  }  

  pair_sep_var_ave = pair_sep_var_ave/static_cast<double>(pair_sep_var_ave_n);

//////////////////////////
  // shut it down 

  // check if the variance of sep between nearest neighbors has converge
  if( (pair_sep_var <= 0.9*pair_sep_var_ave) && (sweep_number > sweeps_to_ave) && (count_down < 0) ){
    cout << endl << "Halt conditions met on sweep " << sweep_number+1 << ", stopping after 10 more sweeps" << endl;
    count_down = 11;
  }
  // for good measure go 10 more sweeps past when the conditions are meet
  if( count_down > 0 ){
    count_down--;
  }
  if( count_down == 0){
    keep_going = false;
    cout << endl << "Simulation stopped after " << sweep_number+1 << " sweeps." << endl;
  }
  // always halt after 10000 sweeps no matter what
  if(sweep_number == 9999){
//  if(sweep_number == 20){ // TODO debugging
    keep_going = false;
    cout << endl << "Warning! 10,000 sweeps run without meeting halting condition, forcing a halt!!" << endl << endl;
  }

  sweep_number++;
}while( keep_going );


//////////////////////////////////////////////////////////////////
  // Create plots output directories. WARNING system dependent
  TString OUTPUT_PATH = "./output";
  system("mkdir -p "+OUTPUT_PATH);
  system("rm "+OUTPUT_PATH+"/*.pdf");
  system("rm "+OUTPUT_PATH+"/*.png");

  // Create Plots
  //TH1::SetDefaultSumw2(1); // All hist uncertainty tracking
  char buffer[300]; int tmp; (void)tmp; // Cast tmp to void to suppress unused var warning


  double bin_size = D;
  double axis_max = L;
  double axis_min= 0;
  int nbins = std::round(L/bin_size);
  bin_size = (axis_max-axis_min)/static_cast<double>(nbins);

  tmp = sprintf(buffer, "sim pair cor; #font[52]{x}; Events / %.3g", ((axis_max-axis_min)/nbins));
  TH1F *sim_pair_cor_hist = new TH1F("sim_pair_cor_hist", buffer, nbins, axis_min, axis_max);

  tmp = sprintf(buffer, "theory pair cor; #font[52]{x}; Events / %.3g", ((axis_max-axis_min)/nbins));
  TH1F *theory_pair_cor_hist = new TH1F("theory_pair_cor_hist", buffer, nbins, axis_min, axis_max);


//////////////////////////////////////////////////////////////////
// compute thermo functions

  // TODO EXPERIMENTAL PAIR COORELATION ISN'T WORKING, NOT UNDERSTOOD

/*
    pair_correlation = 0;
    for(int i=0; i<n-k; i++){ // stop at n-k so i+k doesn't go out of bounds
      pair_correlation += (spheres.at(i+k).GetX()*spheres.at(i).GetX()) ;
    }
    pair_correlation = pair_correlation/static_cast<double>(n);

    sim_pair_cor_hist->Fill(static_cast<double>(k),pair_correlation); // fill at k with weight weight = pair_correlation
*/

  // TODO fill hists
  for(int i_x=0; i_x<nbins; i_x++){
    double bin_x = axis_min+i_x*bin_size;

    double dX = D/4.0;
    sim_pair_cor_hist->Fill(bin_x, pair_cor(spheres, bin_x, dX, L, D, n, nbins));

    theory_pair_cor_hist->Fill(bin_x, gx(bin_x, D, L, n));
   }

//////////////////////////////////////////////////////////////////
// Print Graphics

  SetClassStyle(); // Set style for plots, see myClassStyle.h, adapted from the ATLAS Experiment style

  TCanvas *c1 = new TCanvas("c1", "rectangle", 800, 600); c1->cd(); c1->Clear();

  double margin = 0.18;
  gPad->SetBottomMargin(margin); gPad->SetTopMargin(margin);
  gPad->SetLeftMargin(margin); gPad->SetRightMargin(margin);

  theory_pair_cor_hist->GetXaxis()->SetTitleOffset(1.5);
  theory_pair_cor_hist->GetYaxis()->SetTitleOffset(1.5);
//  theory_pair_cor_hist->GetXaxis()->SetNdivisions(n);

  sim_pair_cor_hist->GetXaxis()->SetTitleOffset(1.5);
  sim_pair_cor_hist->GetYaxis()->SetTitleOffset(1.5);
//  sim_pair_cor_hist->GetXaxis()->SetNdivisions(n);

  theory_pair_cor_hist->Draw("hist");
  sim_pair_cor_hist->Draw("same PE");

  c1->Print(OUTPUT_PATH+"/pair_correlation.pdf", "pdf");
  c1->Print(OUTPUT_PATH+"/pair_correlation.png", "png");


//////////////////////////////////////////////////////////////////
// Clean up and exit 

  cout << endl << "Success!" << endl << endl;

  return 0;
}


bool Overlap(double L, double D, hsphere existing_sphere, hsphere test_sphere, double x, double dX){
  if ( existing_sphere.GetID() == test_sphere.GetID()){
     return false;
  }
    double test_x[9];
    test_x[0] = test_sphere.GetTestX();
    test_x[1] = test_x[0];
    test_x[2] = test_x[0];
    test_x[3] = test_x[0]+L;
    test_x[4] = test_x[0]+L;
    test_x[5] = test_x[0]+L;
    test_x[6] = test_x[0]-L;
    test_x[7] = test_x[0]-L;
    test_x[8] = test_x[0]-L;

    for(int i=0; i<9; i++){
      double sep2 = pow((test_x[i]-existing_sphere.GetX()),2); 
      if ( ((x < 0) && (dX <= 0)) && (sep2 <= D*D) ) return true; // One of the test points 9 representations landed within the 2r=D circle around the existing one
      if ( ((x >= 0) && (dX > 0)) && (sep2 >= (x-dX)*(x-dX)) && (sep2 <= (x+dX)*(x+dX)) ) return true; // One of the test points 9 representations landed within the ring width 2dx radius x away from the existing one, for pair correlation
    }

  return false; // if it made it here they don't overlap

}


double my_pair_sep_expectation(std::vector<hsphere> spheres, int power){
  double sum = 0;
  for(std::vector<hsphere>::iterator it = spheres.begin(); it != spheres.end(); ++it){
    if( it->GetPairSep()>0 ) sum += pow(it->GetPairSep(), power); // drop any sphere that didn't find it's pair, ie sep = -99
  }
  return sum/static_cast<double>(spheres.size());
}

double my_pair_sep_var(std::vector<hsphere> spheres){
  double mean = my_pair_sep_expectation(spheres, 1);
  return my_pair_sep_expectation(spheres, 2) - pow(mean, 2); 
}


double gx(double x, double D, double L, int n){
//  cout << "computting gx(" << x << ")" << endl; // TODO debugging
  double sum = 0.0;
  for(int j=1; j<n-1; j++){
    sum += gjx(x, j, D, L, n);
  }
  return sum;
}

double gjx(double x, int j, double D, double L, int n){

  // make sure we are in the valid range: jD <= x <= L-(n-j)D, if not return zero
  if ( (j*D <= x) && ( x <= (L-(n-j)*D) ) ){
    
    return ((L*factorial(n-1))/(n*pow(L-n*D,n-1)))*((pow(x-j*D,j-1)*pow(L-x-(n-j)*D,n-1-j))/(factorial(j-1)*factorial(n-1-j)));
  }
  else{
   // TODO debugging
//   if ( j*D > x ) cout << "j = " << j << " Lower gjx bound failed" << endl;
//   if ( x > (L-(n-j)*D) ) cout << "j = " << j << " Upper gjx bound failed" << endl;


    return 0;
  }
}

int factorial(int n){
  if(n <= 1) return 1;
  return n*factorial(n-1);
}


double pair_cor(std::vector<hsphere> spheres, double x, double dX, double L, double D, int n, int nbins){

double space_sum = 0;

for(int space_x_it=0; space_x_it<nbins; space_x_it++){
  double pair_cor_sum = 0;
  double bin_size = L/static_cast<double>(nbins);

  double space_x = 0.0+bin_size*space_x_it;

  for(int i=0; i<n; i++){ 
     for(int j=0; j<n; j++){
        bool in_space_bin = ((space_x <= spheres.at(i).GetX()) && (spheres.at(i).GetX() <= space_x+bin_size));
        if( in_space_bin && Overlap(L, D, spheres.at(i), spheres.at(j), x, dX) ) pair_cor_sum += 1;
     }
  }
  space_sum += pair_cor_sum;

}

return space_sum/static_cast<double>(nbins);


}


