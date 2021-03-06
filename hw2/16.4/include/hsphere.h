#ifndef HSPHERE_H
#define HSPHERE_H

#include <math.h> 

class hsphere{
private:
 
    hsphere() { } // private default constructor

    int m_id;

    double m_pair_sep;

    double m_X;
    double m_testX;

    double m_std_dev;
    double m_int_std_dev;
    double m_L;

    std::default_random_engine* m_engine_ptr;


public:
    hsphere(int id, double L, double int_std_dev, double std_dev, std::default_random_engine* engine_ptr);

    int GetID() { return m_id; }

    double GetX() { return m_X; }
    double GetTestX() { return m_testX; }

    double GetPairSep() { return m_pair_sep; }
    void SetPairSep(double pair_sep) { m_pair_sep = pair_sep; }

    void Int_Move();
    void Test_Move();
    void Accept();
};
 

// hsphere constructor
hsphere::hsphere(int id, double L, double int_std_dev, double std_dev, std::default_random_engine* engine_ptr){

   // set private variables   
   m_id = id;
   m_L = L;
   m_std_dev = std_dev;
   m_int_std_dev = int_std_dev;

   m_engine_ptr = engine_ptr;

}


// generate an initial move, prior to checking for overlaps
void hsphere::Int_Move(){
   std::normal_distribution<double> normal_int(m_L/2.0,m_int_std_dev);
   double random = normal_int(*m_engine_ptr);
   while(random < 0){ random += m_L;} // Make it positive
   m_testX = fmod(random, m_L); // Make it < L
}


// generate a test move, prior to checking for overlaps
void hsphere::Test_Move(){
   std::normal_distribution<double> normal_move(0.0,m_std_dev);
   double random = m_X + normal_move(*m_engine_ptr);
   while(random < 0){ random += m_L;} // Make it positive
   m_testX = fmod(random, m_L); // Make it < L
}

// After there are no overlaps, accept the move 
void hsphere::Accept(){
   m_X = m_testX;
}



#endif
