#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <random>
#include <vector>

#include <TH1.h>

#include "myClassStyle.h"


// TODO for loops ending values, < or <=, probably doesn't matter...
// TODO write hist gen code into class

class sequence{
private:
 
    sequence() { } // private default constructor

    double m_alpha;
    double m_n;
    double m_s0;

    double m_AA; // <A^2>
    double m_A; // <A(0)> = <A> = <s> = mean
    double m_tau; // tau
    double m_sigma_M; // sigma_M
    double m_usual_variance; // <s^s> - <s>^2 = <A^2> - <A>^2

    TH1F *m_sequence_hist;


    std::vector<double> m_sequence;


    double At(int t);
    double AtA0(int t);
    void AA();
    void mean();

    double phiAAt(int t);

    void tau();
    void sigma_M();
    void usual_variance();

public:
    sequence(double alpha, int n, std::default_random_engine engine, std::normal_distribution<double> normal, double s0);

    double GetAlpha() { return m_alpha; }
    double GetN() { return m_n; }
    double GetS0() { return m_s0; }

    double GetMean() { return m_A; }
    double GetTau() { return m_tau; }
    double GetSigma_M() { return m_sigma_M; }
    double GetUsual_Variance() { return m_usual_variance; }

    std::vector<double> GetSequence() { return m_sequence; }

    TH1F* GetSequence_hist() { return m_sequence_hist; }

};
 

// sequence constructor
sequence::sequence(double alpha, int n, std::default_random_engine engine, std::normal_distribution<double> normal, double s0){

   if((alpha <= 0 ) || (alpha >= 1)){
   std::cerr << "ERROR: Invalid alpha!" << std::endl;
   }
   if(n <= 0){
   std::cerr << "ERROR: Invalid n!" << std::endl;
   }

   // set private variables   
   m_alpha = alpha;
   m_n = n;
   m_s0 = s0;

   // setup sequence hist
   int nbins = 500;
   double axis_min = -10.0;
   double axis_max = 10.0;

   char buffer[300]; int tmp; (void)tmp; // Cast tmp to void to suppress unused var warning
   tmp = sprintf(buffer, "sequence_hist; #font[52]{s}_{j}; Counts / (%.3g)", ((axis_max-axis_min)/nbins));

   m_sequence_hist = new TH1F("sequence_hist", buffer, nbins, axis_min, axis_max);

   // set s_0
   m_sequence.push_back(m_s0);
   m_sequence_hist->Fill(m_sequence.at(0));

   // create the sequence 
   for(int i=1; i<n; i++){
   m_sequence.push_back( m_alpha*m_sequence.at(i-1) + (1.0 - m_alpha)*normal(engine));
   m_sequence_hist->Fill(m_sequence.at(i));
   }


   // calculate everything in the correct order 
   AA();
   mean();
   tau();
   sigma_M();
   usual_variance();
}




double sequence::At(int t){
   // computes <A(t)> = (1/M') Sum_j A(q_(j+t)) where M' = M - t
   // for us M = n, and A = q = s
   
   double sum = 0.0;

   // put in the -1's to match the array numbering from 0 and the equations from 1 explicitly
   for(int j=1-1; j<m_n-t-1; j++){
     sum += m_sequence.at(j+t);
   }
 
   return sum/static_cast<double>(m_n-t);
}

double sequence::AtA0(int t){
   // computes <A(t)A(0)> = (1/M') Sum_j A(q_(j+t))A(q_j) where M' = M - t
   // for us M = n, and A = q = s
   
   double sum = 0.0;

   // put in the -1's to match the array numbering from 0 and the equations from 1 explicitly
   for(int j=1-1; j<m_n-t-1; j++){
     sum += m_sequence.at(j+t)*m_sequence.at(j);
   }
 
   return sum/static_cast<double>(m_n-t);
}

void sequence::AA(){
   // computes <A^2> = (1/M) Sum_j A(j)^2
   // for us M = n, and A = q = s
   
   double sum = 0.0;

   // put in the -1's to match the array numbering from 0 and the equations from 1 explicitly
   for(int j=1-1; j<m_n-1; j++){
     sum += std::pow(m_sequence.at(j),2);
   }
 
   m_AA = sum/static_cast<double>(m_n);
}

void sequence::mean(){
   m_A = At(0); // Note: At(0) is the usual mean formula
}





double sequence::phiAAt(int t){
   // computes Phi_AA(t) = (<A(t)A(0)> - <A(t)><A(0)>) / (<A^2> - <A>^2) 

   return ( AtA0(t) - At(t)*m_A ) / ( m_AA - std::pow(m_A,2) );

}

void sequence::tau(){
   // computes tau = Sum_t Phi_AA(t), t > 0 i.e. t = 1, 2, 3... M-1 = N-1 = m_n-1

   double sum = 0.0;

   // int t is the same as the formula's t here, no shift to match vectors... 
   // Sum over the first 60% of the sample, before the wild fluctuations start to set in
   for(int t=1; t<std::round(m_n/10.0); t++){
//   for(int t=1; t<249; t++){ // for debugging, hard coded
//   for(int t=1; t<m_n-1; t++){ // naive way, over all the t's we could possibly do

//  std::cout << "t: "<<t<<" <A(t)A(0)> " << std::setw(12) << AtA0(t) << " <A(t)>: " << std::setw(12) << At(t) << " <A(0)>: " << std::setw(12) << At(0) << " phiAA(t): " << std::setw(12) << phiAAt(t) << std::endl;

   sum += phiAAt(t); 
   }
 
   m_tau = sum;

   // TODO debugging stream, get it to plot or write out to a file
/*
   for(int t=1; t<std::round(m_n-1); t++){
   std::cout << phiAAt(t) << std::endl;  
   }
*/
}

void sequence::sigma_M(){

   m_sigma_M = std::sqrt( (m_AA-std::pow(m_A,2)) / (m_n/(2*m_tau + 1)) );

}


void sequence::usual_variance(){

   m_usual_variance = m_AA-std::pow(m_A,2);

}


#endif
