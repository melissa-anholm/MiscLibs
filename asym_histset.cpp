// ==================================================================== //
// Code by Melissa Anholm
// June 2017 - 
// 
// consolidate_and_fit.cpp inherits from consolidate_results3.cpp
// LineshapeConvolution.h inherits from RhoLibs.h
// ==================================================================== //
#include <stdlib.h>
#include <string>
#include <sstream> 
//#include <iostream>
//#include <iomanip>   // do I need this?
//#include <algorithm> // to manipulate strings.
//#include <utility>

#include <TObject.h>
#include <TFile.h>
#include <TH1D.h>
#include "TF1.h"
#include <TStyle.h>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

#include "HistExtras.cpp"         // in AsymmetryCanvasLibs
#include "MetaChain.cpp"
#include "SomeFunctions.cpp"

string asymhist_file_path = "~/Desktop/Anal-Ysis/Abeta2/Files/";

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

class asym_histset : public TObject
{
public:
	double rho;
	string descriptor;
	TH1D* h_asym;
	TH1D* h_counts;
	
	asym_histset();
	asym_histset(TH1D* this_h_asym, TH1D* this_h_counts, double);
	asym_histset(string filename, string hname_asym, string hname_counts, double);

	void set_name(string);
	void set_color(int);
	void set_range_user(double, double);
	void set_xaxis_name(string);
	void set_rho(double);
	
};
asym_histset::asym_histset()
{
	rho = 0.0;
	h_asym   = new TH1D();
	h_counts = new TH1D();
}
asym_histset::asym_histset(TH1D* this_h_asym, TH1D* this_h_counts, double this_rho=0)
{
	rho = this_rho;
	h_asym   = new TH1D();
	h_counts = new TH1D();
	
	h_asym = (TH1D*)this_h_asym -> Clone();
	h_counts = (TH1D*)this_h_counts -> Clone();
}
asym_histset::asym_histset(string filename, string hname_asym, string hname_counts, double this_rho=0)
{
	rho = this_rho;
	TFile * file = new TFile( (asymhist_file_path+filename).c_str() );
	
	h_asym   = (TH1D*)file->Get( hname_asym.c_str());    // 
	h_counts = (TH1D*)file->Get( hname_counts.c_str());  // 
}
void asym_histset::set_name(string newname)
{
	descriptor = newname;
	h_asym -> SetTitle(newname.c_str() );
	h_asym -> SetName( newname.c_str() );

	h_counts -> SetTitle( (string("N_Events for ") + newname).c_str() );
	h_counts -> SetName(  (string("N_Events for ") + newname).c_str() );
}
void asym_histset::set_color(int newcolor)
{
	h_asym -> SetMarkerColor(newcolor);
	h_asym -> SetLineColor(newcolor);
	
	h_counts -> SetMarkerColor(newcolor);
	h_counts -> SetLineColor(newcolor);
}
void asym_histset::set_range_user(double lowlimit, double highlimit)
{
	h_asym   -> GetXaxis() -> SetRangeUser(lowlimit, highlimit);
	h_counts -> GetXaxis() -> SetRangeUser(lowlimit, highlimit);
}
void asym_histset::set_xaxis_name(string newname)
{
	h_asym   -> GetXaxis() -> SetName(newname.c_str());
	h_counts -> GetXaxis() -> SetName(newname.c_str());
}
string make_g4_asymhistpair_filename(double rho, string setletter)
{
	string rhostring = double_to_string(rho);
	rhostring.erase( std::remove(rhostring.begin(), rhostring.end(), '.'), rhostring.end() );
	
	stringstream ss;
	ss << "Abeta_g4_rho" << rhostring << setletter << ".root";
	string filename = ss.str();
	return filename;
}
string make_g4_asymhistpair_title(double rho, string setletter)
{
	string rhostring = double_to_string(rho);
	stringstream ss;
	ss << "G4 A_beta (rho=" << rhostring << ", Set " << setletter << ")";
	string title = ss.str();
	return title;
}


// --- // --- //

asym_histset * get_pair_ben_B()
{
	string string_asym   = "Ben HEXpos recombination";
	string string_counts = "Ncounts- Ben HEXpos recombination";
	
	asym_histset * pair_benB  = new asym_histset("Abeta_bencompare_B.root",  string_asym, string_counts);
	pair_benB -> set_name("Ben Set B" );
	pair_benB -> set_xaxis_name("Scintillator Energy [keV]");
	pair_benB -> set_range_user(0.0, 5100.0);
	pair_benB->h_asym -> GetYaxis() -> SetRangeUser(-0.63, -0.42);
	pair_benB->h_asym -> GetListOfFunctions() -> Delete();
	pair_benB -> set_color(kBlack);
	pair_benB->h_asym -> SetMarkerStyle(33);
	pair_benB->h_asym -> SetMarkerSize(1.5);
	pair_benB->h_asym -> SetLineWidth(1);
	
	return pair_benB;
}
asym_histset * get_pair_ben_C()
{
	string string_asym   = "Ben HEXpos recombination";
	string string_counts = "Ncounts- Ben HEXpos recombination";
	
	asym_histset * pair_benC  = new asym_histset("Abeta_bencompare_C.root",  string_asym, string_counts);
	pair_benC -> set_name("Ben Set C" );
	pair_benC -> set_xaxis_name("Scintillator Energy [keV]");
	pair_benC -> set_range_user(0.0, 5100.0);
	pair_benC->h_asym -> GetYaxis() -> SetRangeUser(-0.63, -0.42);
	pair_benC->h_asym -> GetListOfFunctions() -> Delete();
	pair_benC -> set_color(kBlack);
	pair_benC->h_asym -> SetMarkerStyle(33);
	pair_benC->h_asym -> SetMarkerSize(1.5);
	pair_benC->h_asym -> SetLineWidth(1);
	
	return pair_benC;
}
asym_histset * get_pair_ben_D()
{
	string string_asym   = "Ben HEXpos recombination";
	string string_counts = "Ncounts- Ben HEXpos recombination";
	
	asym_histset * pair_benD  = new asym_histset("Abeta_bencompare_D.root",  string_asym, string_counts);
	pair_benD -> set_name("Ben Set D" );
	pair_benD -> set_xaxis_name("Scintillator Energy [keV]");
	pair_benD -> set_range_user(0.0, 5100.0);
	pair_benD->h_asym -> GetYaxis() -> SetRangeUser(-0.63, -0.42);
	pair_benD->h_asym -> GetListOfFunctions() -> Delete();
	pair_benD -> set_color(kBlack);
	pair_benD->h_asym -> SetMarkerStyle(33);
	pair_benD->h_asym -> SetMarkerSize(1.5);
	pair_benD->h_asym -> SetLineWidth(1);
	
	return pair_benD;
}
asym_histset * get_g4_asymhist_pair(double rho, string setletter)
{
	string string_asym = "Super-er A_beta (from E_scint)";
	string string_counts = "N_Events for A_beta";
	string filename = make_g4_asymhistpair_filename(rho, setletter);
	asym_histset * this_pair = new asym_histset(filename, string_asym, string_counts, rho);
	string pair_title = make_g4_asymhistpair_title(rho, setletter);
	this_pair -> set_name( pair_title );

	return this_pair;
}


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
