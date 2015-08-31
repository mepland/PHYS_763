#ifndef  __CLASSSTYLE_H
#define __CLASSSTYLE_H

// Adapted from the ATLAS Experiment style file

#include "TStyle.h"
#include "TLatex.h"
#include "TLine.h"
#include "TPave.h"
#include "TPad.h"
#include "TMarker.h"
#include <iostream>

TStyle* ClassStyle()
{
  TStyle *classStyle = new TStyle("Class","Class style");

  // use plain black on white colors
  Int_t icol=0; // WHITE
  classStyle->SetFrameBorderMode(icol);
  classStyle->SetFrameFillColor(icol);
  classStyle->SetCanvasBorderMode(icol);
  classStyle->SetCanvasColor(icol);
  classStyle->SetPadBorderMode(icol);
  classStyle->SetPadColor(icol);
  classStyle->SetStatColor(icol);
  //classStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  classStyle->SetPaperSize(20,26);

  // set margin sizes
  classStyle->SetPadTopMargin(0.05);
  classStyle->SetPadRightMargin(0.05);
  classStyle->SetPadBottomMargin(0.16);
  classStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  classStyle->SetTitleXOffset(1.4);
  classStyle->SetTitleYOffset(1.4);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t tsize=0.05;
  classStyle->SetTextFont(font);

  classStyle->SetTextSize(tsize);
  classStyle->SetLabelFont(font,"x");
  classStyle->SetTitleFont(font,"x");
  classStyle->SetLabelFont(font,"y");
  classStyle->SetTitleFont(font,"y");
  classStyle->SetLabelFont(font,"z");
  classStyle->SetTitleFont(font,"z");

  classStyle->SetLabelSize(tsize,"x");
  classStyle->SetTitleSize(tsize,"x");
  classStyle->SetLabelSize(tsize,"y");
  classStyle->SetTitleSize(tsize,"y");
  classStyle->SetLabelSize(tsize,"z");
  classStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  classStyle->SetMarkerStyle(20);
//  classStyle->SetMarkerSize(1.2);
  classStyle->SetMarkerSize(0.8);
  classStyle->SetHistLineWidth(2.);
  classStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars 
  //classStyle->SetErrorX(0.001);
  // get rid of error bar caps
  classStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  classStyle->SetOptTitle(0);
  //classStyle->SetOptStat(1111);
  classStyle->SetOptStat(0);
  //classStyle->SetOptFit(1111);
  classStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  classStyle->SetPadTickX(1);
  classStyle->SetPadTickY(1);

  return classStyle;

}

void SetClassStyle ()
{
  static TStyle* classStyle = 0;
  std::cout << "\nApplying Class style settings...\n" << std::endl ;
  if ( classStyle==0 ) classStyle = ClassStyle();
  gROOT->SetStyle("Class");
  gROOT->ForceStyle();
}

void myText(Double_t x,Double_t y,Color_t color, const char *text) {

  //Double_t tsize=0.05;
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}
 

void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,const char *text) 
{

  Double_t tsize=0.06;

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.25*tsize;
  Double_t y2=y+0.25*tsize;
  Double_t x2=x-0.3*tsize;
  Double_t x1=x2-boxsize;

  printf("x1= %f x2= %f y1= %f y2= %f \n",x1,x2,y1,y2);

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColor(mcolor);
  mbox->SetFillStyle(1001);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(4);
  mline.SetLineColor(1);
  mline.SetLineStyle(1);
  Double_t y_new=(y1+y2)/2.;
  mline.DrawLineNDC(x1,y_new,x2,y_new);

}


void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle, const char *text,Float_t msize) 
{
  Double_t tsize=0.06;
  TMarker *marker = new TMarker(x-(0.4*tsize),y,8);
  marker->SetMarkerColor(color);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  marker->SetMarkerSize(msize);
  marker->Draw();

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);
}


#endif // __CLASSSTYLE_H

