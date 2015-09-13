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
#include <math.h> 
#include <random>
#include <algorithm> 
#include <vector>
//#include <string>
//#include <cstring>
//#include <TFile.h>
//#include <THStack.h>
//#include <TLorentzVector.h>
//#include <TString.h>
//#include <TPad.h>
//#include <TFitResult.h>
//#include "myClassStyle.h"
#include "hsphere.h"


using std::cout; using std::endl;
//using std::cin;
//using std::cerr;
//using std::string;
//using std::pow;

bool Overlap(double D, hsphere existing_sphere, hsphere test_sphere);


int main(){
  cout << endl;

//////////////////////////////////////////////////////////////////
// Initialization

  int seed = 7;

  // TODO adjust parameters
  int n = 10;
  double L = 1000.0; // Length
  double D = 10.0; // Ball diameter

  double std_dev = L/4.0;
  double int_std_dev = L/6.0;
 
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
        if ( Overlap(D, spheres.at(j), spheres.at(i)) ){break;} // can stop looking if one overlaps
        passed++; // no overlap
      }
    }while(passed<i); // keep going till it passes all of them
    spheres.at(i).Accept(); // Accept it
  }

//////////////////////////////////////////////////////////////////
  // Run sweeps till spatial distribution is approx uniform
 
// do TODO

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
        if ( Overlap(D, spheres.at(j), spheres.at(i)) ){break;} // can stop looking if one overlaps
        passed++; // no overlap
      }
    }while(passed<n); // keep going till it passes all of them
    spheres.at(i).Accept(); // Accept it
  }

//////////////////////////
  // Check if the spatial distribution is approx uniform yet

// TODO
 
//while TODO


//////////////////////////////////////////////////////////////////
// Clean up and exit 

  cout << endl << "Success!" << endl << endl;

  return 0;
}


bool Overlap(double D, hsphere existing_sphere, hsphere test_sphere){
  if ( existing_sphere.GetID() == test_sphere.GetID()){
     return false;
  }
  else{
//     cout << "OVERLAP" << endl; // TODO debug
     return std::abs( existing_sphere.GetX() - test_sphere.GetTestX() ) < D;
  }
}

