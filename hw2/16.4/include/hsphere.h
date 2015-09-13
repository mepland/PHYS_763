#ifndef HSPHERE_H
#define HSPHERE_H

#include <math.h> 

class hsphere{
private:
 
    hsphere() { } // private default constructor

    double m_id;

    double m_X;
    double m_testX;

    double m_std_dev;
    double m_int_std_dev;
    double m_L;

    std::default_random_engine* m_engine_ptr;


public:
    hsphere(int id, double L, double int_std_dev, double std_dev, std::default_random_engine* engine_ptr);

    double GetID() { return m_id; }

    double GetX() { return m_X; }
    double GetTestX() { return m_testX; }

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
   std::normal_distribution<double> normal(m_L/2.0,m_int_std_dev);
   m_testX = fmod(normal(*m_engine_ptr), m_L);
}


// generate a test move, prior to checking for overlaps
void hsphere::Test_Move(){
   std::normal_distribution<double> normal(0.0,m_std_dev);
   m_testX = fmod((m_X + normal(*m_engine_ptr)), m_L); 
}

// After there are no overlaps, accept the move 
void hsphere::Accept(){
   m_X = m_testX;
}



#endif
