// ==================================================================== //
// Code by Melissa Anholm
// 
// ==================================================================== //

#include <vector>
#include <cmath>
#include <sstream> 
#include <iostream>  // cout


#include <TH1.h>
#include <TChain.h>
#include <TFile.h>
#include "TStyle.h"

#include "MapUtility.h"

#undef NDEBUG
#include<assert.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
/*
struct chamber_geometry // naive numbers indirectly imported from G4.  we'll just kludge them in here, but remember that you can only use this if the trap is centred.
{
	// distance to the *front* of the dssd is 102.127357.
	G4double vdistance_center_to_dssd = 103.627357*mm; // this is probably a bit too precise.  wev.  it's in mm.
	G4double dssd_width_x = 40.0*mm; // mm.
	G4double dssd_width_y = 40.0*mm; // mm.
	G4double dssd_cut_radius = 15.5*mm; // mm.  probably it's only good for post-processing...  unused.
};
*/

double get_costheta(double rhit, double zhit)
{
	double tantheta = rhit/zhit;
	double costheta = cos( atan(tantheta) );
	if(zhit<0) { costheta *= -1.0; }
	return costheta;
}

string make_mapname_from_monoenergy(string namestub, double monoenergy)  // energy in MeV.
{
	int monoenergy_int = int(monoenergy*1000.0);
	std::stringstream ss;
	
	if(monoenergy != -10 && monoenergy != -20)
	{
		ss << namestub << "_" << monoenergy_int << ".root";
	}
	else if(monoenergy == -20 )
	{
		ss << namestub << "_simple.root";
	}
	else
	{
		ss << namestub << "_full.root";
	}
	
	string thename = ss.str();
	return thename;
}
string make_mapname_from_monoenergy(double monoenergy)
{
	string namestub = "map_out";
	return make_mapname_from_monoenergy(namestub, monoenergy);
}
string make_fullspectrum_name(string namestub)
{
	std::stringstream ss;
	ss << namestub << ".root";
	
	string thename = ss.str();
	return thename;
}

string int_to_string(int the_int)
{
	std::stringstream ss;
	ss << the_int;
	string the_string = ss.str();
	return the_string;
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
void MapSetup::AdjustTheColors()
{
	// Orange
	naive_EnergyT_p_hist -> SetLineColor(int(mOrange));
	naive_EnergyB_p_hist -> SetLineColor(int(mOrange));
	naive_EnergyT_m_hist -> SetLineColor(int(mOrange));
	naive_EnergyB_m_hist -> SetLineColor(int(mOrange));
	
	naive_EnergyT_p_hist_rcut155 -> SetLineColor(int(mOrange));
	naive_EnergyB_p_hist_rcut155 -> SetLineColor(int(mOrange));
	naive_EnergyT_m_hist_rcut155 -> SetLineColor(int(mOrange));
	naive_EnergyB_m_hist_rcut155 -> SetLineColor(int(mOrange));

	// blue.  no, now red.
	measured_EnergyT_p_bb1agree -> SetLineColor(kRed);
	measured_EnergyB_p_bb1agree -> SetLineColor(kRed);
	measured_EnergyT_m_bb1agree -> SetLineColor(kRed);
	measured_EnergyB_m_bb1agree -> SetLineColor(kRed);
		
	// red.  no, now blue.
	measured_EnergyT_p_bb1_r155 -> SetLineColor(kBlue);
	measured_EnergyB_p_bb1_r155 -> SetLineColor(kBlue);
	measured_EnergyT_m_bb1_r155 -> SetLineColor(kBlue);
	measured_EnergyB_m_bb1_r155 -> SetLineColor(kBlue);
	
	// green
	measured_EnergyT_p_bb1_r105 -> SetLineColor(kGreen);
	measured_EnergyB_p_bb1_r105 -> SetLineColor(kGreen);
	measured_EnergyT_m_bb1_r105 -> SetLineColor(kGreen);
	measured_EnergyB_m_bb1_r105 -> SetLineColor(kGreen);
	
	// purple!  
	measured_EnergyT_p_bb1_r155_samedet_one -> SetLineColor(kViolet-3);
	measured_EnergyB_p_bb1_r155_samedet_one -> SetLineColor(kViolet-3);
	measured_EnergyT_m_bb1_r155_samedet_one -> SetLineColor(kViolet-3);
	measured_EnergyB_m_bb1_r155_samedet_one -> SetLineColor(kViolet-3);
}

void MapSetup::LoadFromFile(string filename_)
{
	gStyle->SetOptStat(0);

	map_filename = filename_;
	
	TFile * f = new TFile(map_filename.c_str());
	if(f->IsZombie())
	{
		cout << "No TFile " << filename_ << " could be found." << endl;
		cout << "Hard kill." << endl;
		assert(0);
	}
	
	naive_EnergyT_p_hist = (TH1D*)f->Get("Naive Upper Energy(+)");
	naive_EnergyB_p_hist = (TH1D*)f->Get("Naive Lower Energy(+)");
	naive_EnergyT_m_hist = (TH1D*)f->Get("Naive Upper Energy(-)");
	naive_EnergyB_m_hist = (TH1D*)f->Get("Naive Lower Energy(-)");
	
	naive_EnergyT_p_hist_rcut155 = (TH1D*)f->Get("Naive Upper Energy(+) (R<=15.5mm)");
	naive_EnergyB_p_hist_rcut155 = (TH1D*)f->Get("Naive Lower Energy(+) (R<=15.5mm)");
	naive_EnergyT_m_hist_rcut155 = (TH1D*)f->Get("Naive Upper Energy(-) (R<=15.5mm)");
	naive_EnergyB_m_hist_rcut155 = (TH1D*)f->Get("Naive Lower Energy(-) (R<=15.5mm)");
	
	//
	measured_EnergyT_p_hist = (TH1D*)f->Get("Measured ScintT Energy(+)");
	measured_EnergyB_p_hist = (TH1D*)f->Get("Measured ScintB Energy(+)");
	measured_EnergyT_m_hist = (TH1D*)f->Get("Measured ScintT Energy(-)");
	measured_EnergyB_m_hist = (TH1D*)f->Get("Measured ScintB Energy(-)");
	
	measured_EnergyT_p_hist_samedet = (TH1D*)f->Get("Measured ScintT Energy(+) (same det)");
	measured_EnergyB_p_hist_samedet = (TH1D*)f->Get("Measured ScintB Energy(+) (same det)");
	measured_EnergyT_m_hist_samedet = (TH1D*)f->Get("Measured ScintT Energy(-) (same det)");
	measured_EnergyB_m_hist_samedet = (TH1D*)f->Get("Measured ScintB Energy(-) (same det)");
	
	measured_EnergyT_p_hist_oppdet = (TH1D*)f->Get("Measured ScintT Energy(+) (opp det)");
	measured_EnergyB_p_hist_oppdet = (TH1D*)f->Get("Measured ScintB Energy(+) (opp det)");
	measured_EnergyT_m_hist_oppdet = (TH1D*)f->Get("Measured ScintT Energy(-) (opp det)");
	measured_EnergyB_m_hist_oppdet = (TH1D*)f->Get("Measured ScintB Energy(-) (opp det)");
	
	//
	measured_EnergyT_p_bb1agree = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement");
	measured_EnergyB_p_bb1agree = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement");
	measured_EnergyT_m_bb1agree = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement");
	measured_EnergyB_m_bb1agree = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement");

	measured_EnergyT_p_bb1agree_one = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement (1hit)");
	measured_EnergyB_p_bb1agree_one = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement (1hit)");
	measured_EnergyT_m_bb1agree_one = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement (1hit)");
	measured_EnergyB_m_bb1agree_one = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement (1hit)");

	measured_EnergyT_p_bb1agree_two = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement (2hit)");
	measured_EnergyB_p_bb1agree_two = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement (2hit)");
	measured_EnergyT_m_bb1agree_two = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement (2hit)");
	measured_EnergyB_m_bb1agree_two = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement (2hit)");
	
	measured_EnergyT_p_bb1agree_samedet_one = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement (same det, 1hit)");
	measured_EnergyB_p_bb1agree_samedet_one = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement (same det, 1hit)");
	measured_EnergyT_m_bb1agree_samedet_one = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement (same det, 1hit)");
	measured_EnergyB_m_bb1agree_samedet_one = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement (same det, 1hit)");
	
	measured_EnergyT_p_bb1agree_samedet_two = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement (same det, 2hit)");
	measured_EnergyB_p_bb1agree_samedet_two = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement (same det, 2hit)");
	measured_EnergyT_m_bb1agree_samedet_two = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement (same det, 2hit)");
	measured_EnergyB_m_bb1agree_samedet_two = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement (same det, 2hit)");
	
	measured_EnergyT_p_bb1agree_oppdet_one = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement (opp det, 1hit)");
	measured_EnergyB_p_bb1agree_oppdet_one = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement (opp det, 1hit)");
	measured_EnergyT_m_bb1agree_oppdet_one = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement (opp det, 1hit)");
	measured_EnergyB_m_bb1agree_oppdet_one = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement (opp det, 1hit)");

	measured_EnergyT_p_bb1agree_oppdet_two = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement (opp det, 2hit)");
	measured_EnergyB_p_bb1agree_oppdet_two = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement (opp det, 2hit)");
	measured_EnergyT_m_bb1agree_oppdet_two = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement (opp det, 2hit)");
	measured_EnergyB_m_bb1agree_oppdet_two = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement (opp det, 2hit)");
	
	
	//
	measured_EnergyT_p_bb1_r155 = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=15.5");
	measured_EnergyB_p_bb1_r155 = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=15.5");
	measured_EnergyT_m_bb1_r155 = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=15.5");
	measured_EnergyB_m_bb1_r155 = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=15.5");
	
	measured_EnergyT_p_bb1_r155_one = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=15.5 (1hit)");
	measured_EnergyB_p_bb1_r155_one = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=15.5 (1hit)");
	measured_EnergyT_m_bb1_r155_one = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=15.5 (1hit)");
	measured_EnergyB_m_bb1_r155_one = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=15.5 (1hit)");

	measured_EnergyT_p_bb1_r155_two = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=15.5 (2hit)");
	measured_EnergyB_p_bb1_r155_two = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=15.5 (2hit)");
	measured_EnergyT_m_bb1_r155_two = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=15.5 (2hit)");
	measured_EnergyB_m_bb1_r155_two = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=15.5 (2hit)");

	measured_EnergyT_p_bb1_r155_samedet_one = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=15.5 (same det, 1hit)");
	measured_EnergyB_p_bb1_r155_samedet_one = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=15.5 (same det, 1hit)");
	measured_EnergyT_m_bb1_r155_samedet_one = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=15.5 (same det, 1hit)");
	measured_EnergyB_m_bb1_r155_samedet_one = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=15.5 (same det, 1hit)");
	
	measured_EnergyT_p_bb1_r155_samedet_two = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=15.5 (same det, 2hit)");
	measured_EnergyB_p_bb1_r155_samedet_two = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=15.5 (same det, 2hit)");
	measured_EnergyT_m_bb1_r155_samedet_two = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=15.5 (same det, 2hit)");
	measured_EnergyB_m_bb1_r155_samedet_two = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=15.5 (same det, 2hit)");
	
	measured_EnergyT_p_bb1_r155_oppdet_one = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=15.5 (opp det, 1hit)");
	measured_EnergyB_p_bb1_r155_oppdet_one = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=15.5 (opp det, 1hit)");
	measured_EnergyT_m_bb1_r155_oppdet_one = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=15.5 (opp det, 1hit)");
	measured_EnergyB_m_bb1_r155_oppdet_one = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=15.5 (opp det, 1hit)");

	measured_EnergyT_p_bb1_r155_oppdet_two = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=15.5 (opp det, 2hit)");
	measured_EnergyB_p_bb1_r155_oppdet_two = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=15.5 (opp det, 2hit)");
	measured_EnergyT_m_bb1_r155_oppdet_two = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=15.5 (opp det, 2hit)");
	measured_EnergyB_m_bb1_r155_oppdet_two = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=15.5 (opp det, 2hit)");
	
	
	//
	measured_EnergyT_p_bb1_r105 = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=10.5");
	measured_EnergyB_p_bb1_r105 = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=10.5");
	measured_EnergyT_m_bb1_r105 = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=10.5");
	measured_EnergyB_m_bb1_r105 = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=10.5");

	measured_EnergyT_p_bb1_r105_one = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=10.5 (1hit)");
	measured_EnergyB_p_bb1_r105_one = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=10.5 (1hit)");
	measured_EnergyT_m_bb1_r105_one = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=10.5 (1hit)");
	measured_EnergyB_m_bb1_r105_one = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=10.5 (1hit)");

	measured_EnergyT_p_bb1_r105_two = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=10.5 (1hit)");
	measured_EnergyB_p_bb1_r105_two = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=10.5 (1hit)");
	measured_EnergyT_m_bb1_r105_two = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=10.5 (1hit)");
	measured_EnergyB_m_bb1_r105_two = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=10.5 (1hit)");
	
	measured_EnergyT_p_bb1_r105_samedet_one = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=10.5 (same det, 1hit)");
	measured_EnergyB_p_bb1_r105_samedet_one = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=10.5 (same det, 1hit)");
	measured_EnergyT_m_bb1_r105_samedet_one = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=10.5 (same det, 1hit)");
	measured_EnergyB_m_bb1_r105_samedet_one = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=10.5 (same det, 1hit)");
	
	measured_EnergyT_p_bb1_r105_samedet_two = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=10.5 (same det, 2hit)");
	measured_EnergyB_p_bb1_r105_samedet_two = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=10.5 (same det, 2hit)");
	measured_EnergyT_m_bb1_r105_samedet_two = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=10.5 (same det, 2hit)");
	measured_EnergyB_m_bb1_r105_samedet_two = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=10.5 (same det, 2hit)");
	
	measured_EnergyT_p_bb1_r105_oppdet_one = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=10.5 (opp det, 1hit)");
	measured_EnergyB_p_bb1_r105_oppdet_one = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=10.5 (opp det, 1hit)");
	measured_EnergyT_m_bb1_r105_oppdet_one = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=10.5 (opp det, 1hit)");
	measured_EnergyB_m_bb1_r105_oppdet_one = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=10.5 (opp det, 1hit)");

	measured_EnergyT_p_bb1_r105_oppdet_two = (TH1D*)f->Get("Measured ScintT Energy(+) - rBB1<=10.5 (opp det, 2hit)");
	measured_EnergyB_p_bb1_r105_oppdet_two = (TH1D*)f->Get("Measured ScintB Energy(+) - rBB1<=10.5 (opp det, 2hit)");
	measured_EnergyT_m_bb1_r105_oppdet_two = (TH1D*)f->Get("Measured ScintT Energy(-) - rBB1<=10.5 (opp det, 2hit)");
	measured_EnergyB_m_bb1_r105_oppdet_two = (TH1D*)f->Get("Measured ScintB Energy(-) - rBB1<=10.5 (opp det, 2hit)");
	
	costheta_v_costheta_p = (TH2D*)f->Get("costheta v costheta (+) - BB1 Agreement");
	costheta_v_costheta_m = (TH2D*)f->Get("costheta v costheta (-) - BB1 Agreement");
	
	th2_num_Eout_v_Ein_tp = (TH2D*)f->Get("Numerical E_measured vs E_generated, Top(+)");
	th2_num_Eout_v_Ein_tm = (TH2D*)f->Get("Numerical E_measured vs E_generated, Top(-)");
	th2_num_Eout_v_Ein_bp = (TH2D*)f->Get("Numerical E_measured vs E_generated, Bottom(+)");
	th2_num_Eout_v_Ein_bm = (TH2D*)f->Get("Numerical E_measured vs E_generated, Bottom(-)");
	/*
	th2_ana_Eout_v_Ein_tp = (TH2D*)f->Get("Analytic E_measured vs E_generated, Top(+)");
	th2_ana_Eout_v_Ein_tm = (TH2D*)f->Get("Analytic E_measured vs E_generated, Top(-)");
	th2_ana_Eout_v_Ein_bp = (TH2D*)f->Get("Analytic E_measured vs E_generated, Bottom(+)");
	th2_ana_Eout_v_Ein_bm = (TH2D*)f->Get("Analytic E_measured vs E_generated, Bottom(-)");
	*/
	
	return;
}

MapSetup::~MapSetup()
{
	delete naive_EnergyT_p_hist;
	delete naive_EnergyB_p_hist;
	delete naive_EnergyT_m_hist;
	delete naive_EnergyB_m_hist;
	
	delete naive_EnergyT_p_hist_rcut155;
	delete naive_EnergyB_p_hist_rcut155;
	delete naive_EnergyT_m_hist_rcut155;
	delete naive_EnergyB_m_hist_rcut155;
	
	//
	delete measured_EnergyT_p_hist;
	delete measured_EnergyB_p_hist;
	delete measured_EnergyT_m_hist;
	delete measured_EnergyB_m_hist;
	
	delete measured_EnergyT_p_hist_samedet;
	delete measured_EnergyB_p_hist_samedet;
	delete measured_EnergyT_m_hist_samedet;
	delete measured_EnergyB_m_hist_samedet;
	
	delete measured_EnergyT_p_hist_oppdet;
	delete measured_EnergyB_p_hist_oppdet;
	delete measured_EnergyT_m_hist_oppdet;
	delete measured_EnergyB_m_hist_oppdet;
	
	//
	delete measured_EnergyT_p_bb1agree;
	delete measured_EnergyB_p_bb1agree;
	delete measured_EnergyT_m_bb1agree;
	delete measured_EnergyB_m_bb1agree;
	
	delete measured_EnergyT_p_bb1agree_one;// = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement (1hit)");
	delete measured_EnergyB_p_bb1agree_one;// = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement (1hit)");
	delete measured_EnergyT_m_bb1agree_one;// = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement (1hit)");
	delete measured_EnergyB_m_bb1agree_one;// = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement (1hit)");

	delete measured_EnergyT_p_bb1agree_two;// = (TH1D*)f->Get("Measured ScintT Energy(+) - BB1 Agreement (2hit)");
	delete measured_EnergyB_p_bb1agree_two;// = (TH1D*)f->Get("Measured ScintB Energy(+) - BB1 Agreement (2hit)");
	delete measured_EnergyT_m_bb1agree_two;// = (TH1D*)f->Get("Measured ScintT Energy(-) - BB1 Agreement (2hit)");
	delete measured_EnergyB_m_bb1agree_two;// = (TH1D*)f->Get("Measured ScintB Energy(-) - BB1 Agreement (2hit)");

	delete measured_EnergyT_p_bb1agree_samedet_one;
	delete measured_EnergyB_p_bb1agree_samedet_one;
	delete measured_EnergyT_m_bb1agree_samedet_one;
	delete measured_EnergyB_m_bb1agree_samedet_one;
	
	delete measured_EnergyT_p_bb1agree_samedet_two;
	delete measured_EnergyB_p_bb1agree_samedet_two;
	delete measured_EnergyT_m_bb1agree_samedet_two;
	delete measured_EnergyB_m_bb1agree_samedet_two;
	
	delete measured_EnergyT_p_bb1agree_oppdet_one;
	delete measured_EnergyB_p_bb1agree_oppdet_one;
	delete measured_EnergyT_m_bb1agree_oppdet_one;
	delete measured_EnergyB_m_bb1agree_oppdet_one;

	delete measured_EnergyT_p_bb1agree_oppdet_two;
	delete measured_EnergyB_p_bb1agree_oppdet_two;
	delete measured_EnergyT_m_bb1agree_oppdet_two;
	delete measured_EnergyB_m_bb1agree_oppdet_two;
	
	//
	delete measured_EnergyT_p_bb1_r155;
	delete measured_EnergyB_p_bb1_r155;
	delete measured_EnergyT_m_bb1_r155;
	delete measured_EnergyB_m_bb1_r155;
	
	delete measured_EnergyT_p_bb1_r155_one;
	delete measured_EnergyB_p_bb1_r155_one;
	delete measured_EnergyT_m_bb1_r155_one;
	delete measured_EnergyB_m_bb1_r155_one;

	delete measured_EnergyT_p_bb1_r155_two;
	delete measured_EnergyB_p_bb1_r155_two;
	delete measured_EnergyT_m_bb1_r155_two;
	delete measured_EnergyB_m_bb1_r155_two;

	delete measured_EnergyT_p_bb1_r155_samedet_one;
	delete measured_EnergyB_p_bb1_r155_samedet_one;
	delete measured_EnergyT_m_bb1_r155_samedet_one;
	delete measured_EnergyB_m_bb1_r155_samedet_one;
	
	delete measured_EnergyT_p_bb1_r155_samedet_two;
	delete measured_EnergyB_p_bb1_r155_samedet_two;
	delete measured_EnergyT_m_bb1_r155_samedet_two;
	delete measured_EnergyB_m_bb1_r155_samedet_two;
	
	delete measured_EnergyT_p_bb1_r155_oppdet_one;
	delete measured_EnergyB_p_bb1_r155_oppdet_one;
	delete measured_EnergyT_m_bb1_r155_oppdet_one;
	delete measured_EnergyB_m_bb1_r155_oppdet_one;
	
	delete measured_EnergyT_p_bb1_r155_oppdet_two;
	delete measured_EnergyB_p_bb1_r155_oppdet_two;
	delete measured_EnergyT_m_bb1_r155_oppdet_two;
	delete measured_EnergyB_m_bb1_r155_oppdet_two;
	
	//
	delete measured_EnergyT_p_bb1_r105;
	delete measured_EnergyB_p_bb1_r105;
	delete measured_EnergyT_m_bb1_r105;
	delete measured_EnergyB_m_bb1_r105;
	
	delete measured_EnergyT_p_bb1_r105_one;
	delete measured_EnergyB_p_bb1_r105_one;
	delete measured_EnergyT_m_bb1_r105_one;
	delete measured_EnergyB_m_bb1_r105_one;

	delete measured_EnergyT_p_bb1_r105_two;
	delete measured_EnergyB_p_bb1_r105_two;
	delete measured_EnergyT_m_bb1_r105_two;
	delete measured_EnergyB_m_bb1_r105_two;

	delete measured_EnergyT_p_bb1_r105_samedet_one;
	delete measured_EnergyB_p_bb1_r105_samedet_one;
	delete measured_EnergyT_m_bb1_r105_samedet_one;
	delete measured_EnergyB_m_bb1_r105_samedet_one;
	
	delete measured_EnergyT_p_bb1_r105_samedet_two;
	delete measured_EnergyB_p_bb1_r105_samedet_two;
	delete measured_EnergyT_m_bb1_r105_samedet_two;
	delete measured_EnergyB_m_bb1_r105_samedet_two;
	
	delete measured_EnergyT_p_bb1_r105_oppdet_one;
	delete measured_EnergyB_p_bb1_r105_oppdet_one;
	delete measured_EnergyT_m_bb1_r105_oppdet_one;
	delete measured_EnergyB_m_bb1_r105_oppdet_one;
	
	delete measured_EnergyT_p_bb1_r105_oppdet_two;
	delete measured_EnergyB_p_bb1_r105_oppdet_two;
	delete measured_EnergyT_m_bb1_r105_oppdet_two;
	delete measured_EnergyB_m_bb1_r105_oppdet_two;

	delete costheta_v_costheta_p;
	delete costheta_v_costheta_m;
	
	delete th2_num_Eout_v_Ein_tp;
	delete th2_num_Eout_v_Ein_tm;
	delete th2_num_Eout_v_Ein_bp;
	delete th2_num_Eout_v_Ein_bm;
	
	/*
	delete th2_ana_Eout_v_Ein_tp;
	delete th2_ana_Eout_v_Ein_tm;
	delete th2_ana_Eout_v_Ein_bp;
	delete th2_ana_Eout_v_Ein_bm;
	*/
}
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

void MapSetup::LoadSimpleFromTree(TChain * the_tree, int N_rebin_hists)
{
//	cout << "Called MapSetup::LoadSimpleFromTree(...) with N_rebin_hists=" << N_rebin_hists << endl;
	gStyle->SetOptStat(0);
	
	int parallel_color;
	int antiparallel_color;
	
	naive_EnergyT_p_hist = CreateHist( string("Naive Upper Energy(+)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyB_p_hist = CreateHist( string("Naive Lower Energy(+)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyT_m_hist = CreateHist( string("Naive Upper Energy(-)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyB_m_hist = CreateHist( string("Naive Lower Energy(-)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	naive_EnergyT_p_hist_rcut155 = CreateHist( string("Naive Upper Energy(+) (R<=15.5mm)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyB_p_hist_rcut155 = CreateHist( string("Naive Lower Energy(+) (R<=15.5mm)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyT_m_hist_rcut155 = CreateHist( string("Naive Upper Energy(-) (R<=15.5mm)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyB_m_hist_rcut155 = CreateHist( string("Naive Lower Energy(-) (R<=15.5mm)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	// just leave the rest of the histograms blank.  but create them anyway.
	measured_EnergyT_p_hist = CreateHist( string("Measured ScintT Energy(+)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_hist = CreateHist( string("Measured ScintB Energy(+)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_hist = CreateHist( string("Measured ScintT Energy(-)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_hist = CreateHist( string("Measured ScintB Energy(-)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_hist_samedet = CreateHist( string("Measured ScintT Energy(+) (same det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_hist_samedet = CreateHist( string("Measured ScintB Energy(+) (same det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_hist_samedet = CreateHist( string("Measured ScintT Energy(-) (same det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_hist_samedet = CreateHist( string("Measured ScintB Energy(-) (same det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_hist_oppdet = CreateHist( string("Measured ScintT Energy(+) (opp det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_hist_oppdet = CreateHist( string("Measured ScintB Energy(+) (opp det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_hist_oppdet = CreateHist( string("Measured ScintT Energy(-) (opp det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_hist_oppdet = CreateHist( string("Measured ScintB Energy(-) (opp det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	//
	measured_EnergyT_p_bb1agree = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1agree_one = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_one = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_one = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_one = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1agree_two = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_two = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_two = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_two = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1agree_samedet_one = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_samedet_one = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_samedet_one = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_samedet_one = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1agree_samedet_two = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_samedet_two = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_samedet_two = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_samedet_two = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1agree_oppdet_one = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_oppdet_one = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_oppdet_one = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_oppdet_one = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1agree_oppdet_two = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_oppdet_two = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_oppdet_two = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_oppdet_two = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	//
	measured_EnergyT_p_bb1_r155 = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155 = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155 = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155 = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1_r155_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1_r155_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r155_samedet_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_samedet_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_samedet_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_samedet_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r155_samedet_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_samedet_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_samedet_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_samedet_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r155_oppdet_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_oppdet_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_oppdet_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_oppdet_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1_r155_oppdet_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_oppdet_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_oppdet_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_oppdet_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	//
	measured_EnergyT_p_bb1_r105 = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105 = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105 = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105 = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_samedet_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_samedet_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_samedet_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_samedet_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_samedet_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_samedet_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_samedet_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_samedet_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_oppdet_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_oppdet_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_oppdet_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_oppdet_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_oppdet_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_oppdet_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_oppdet_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_oppdet_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	costheta_v_costheta_p = CreateHist2d( string("costheta v costheta (+) - BB1 Agreement"), string("costheta"), string("costheta") );
		costheta_v_costheta_p -> GetXaxis() -> SetTitle("Generated Cos(theta)");
		costheta_v_costheta_p -> GetYaxis() -> SetTitle("Observed Cos(theta)");
	costheta_v_costheta_m = CreateHist2d( string("costheta v costheta (-) - BB1 Agreement"), string("costheta"), string("costheta") );
		costheta_v_costheta_m -> GetXaxis() -> SetTitle("Generated Cos(theta)");
		costheta_v_costheta_m -> GetYaxis() -> SetTitle("Observed Cos(theta)");
		
//	cout << "Trying to create the excess maphists now.." << endl;
	th2_num_Eout_v_Ein_tp = CreateHist2d( string("Numerical E_measured vs E_generated, Top(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_num_Eout_v_Ein_tm = CreateHist2d( string("Numerical E_measured vs E_generated, Top(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_num_Eout_v_Ein_bp = CreateHist2d( string("Numerical E_measured vs E_generated, Bottom(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_num_Eout_v_Ein_bm = CreateHist2d( string("Numerical E_measured vs E_generated, Bottom(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	
	th2_num_Eout_v_Ein_tp -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_num_Eout_v_Ein_bp -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_num_Eout_v_Ein_tm -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_num_Eout_v_Ein_bm -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_num_Eout_v_Ein_tp -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_num_Eout_v_Ein_bp -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_num_Eout_v_Ein_tm -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_num_Eout_v_Ein_bm -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	

	/*
	th2_ana_Eout_v_Ein_tp = CreateHist2d( string("Analytic E_measured vs E_generated, Top(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_tm = CreateHist2d( string("Analytic E_measured vs E_generated, Top(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_bp = CreateHist2d( string("Analytic E_measured vs E_generated, Bottom(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_bm = CreateHist2d( string("Analytic E_measured vs E_generated, Bottom(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	*/
	/*
	cout << "We'll try... N_rebin_hists=" << N_rebin_hists << endl;
	th2_ana_Eout_v_Ein_tp = CreateHist2d( string("Huh? Top(+)"),    string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_tm = CreateHist2d( string("Huh? Top(-)"),    string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_bp = CreateHist2d( string("Huh? Bottom(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_bm = CreateHist2d( string("Huh? Bottom(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	*/
	/*
	th2_ana_Eout_v_Ein_tp -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_ana_Eout_v_Ein_bp -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_ana_Eout_v_Ein_tm -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_ana_Eout_v_Ein_bm -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_ana_Eout_v_Ein_tp -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_ana_Eout_v_Ein_bp -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_ana_Eout_v_Ein_tm -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_ana_Eout_v_Ein_bm -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	*/
//	cout << "LoadSimpleFromTree is done creating mapping hists.  ...for now." << endl;
	
	Double_t naive_hit_t;
	Double_t naive_hit_b;
	the_tree -> SetBranchAddress("naive_hit_t", &naive_hit_t);
	the_tree -> SetBranchAddress("naive_hit_b", &naive_hit_b);
	double gen_Tbeta;
	the_tree -> SetBranchAddress("gen_Tbeta", &gen_Tbeta);

//	double gen_Tbeta;
//	the_tree -> SetBranchAddress("gen_Tbeta", &gen_Tbeta);
	
//	double gen_xhit_t;
//	double gen_yhit_t;
//	double gen_xhit_b;
//	double gen_yhit_b;
//	the_tree -> SetBranchAddress("bb1_top_x", &gen_xhit_t);
//	the_tree -> SetBranchAddress("bb1_top_y", &gen_yhit_t);
//	the_tree -> SetBranchAddress("bb1_bottom_x", &gen_xhit_b);
//	the_tree -> SetBranchAddress("bb1_bottom_y", &gen_yhit_b);
	
	// in the future, this will come pre-pixellated.  but for now, we'll pixellate it here.  ...or not.  we should pixellate it sometime.
	double gen_rhit_t;
	double gen_rhit_b;
	the_tree -> SetBranchAddress("gen_rhit_t", &gen_rhit_t);
	the_tree -> SetBranchAddress("gen_rhit_b", &gen_rhit_b);


	int TTLBit_SigmaPlus;
	the_tree -> SetBranchAddress("TTLBit_SigmaPlus", &TTLBit_SigmaPlus);
	
	Double_t ScintT;
	Double_t ScintB;
	the_tree -> SetBranchAddress("upper_scint_E", &ScintT);
	the_tree -> SetBranchAddress("lower_scint_E", &ScintB);
	
	// BB1s:  
	vector<double> * bb1_t_x = 0;
	vector<double> * bb1_t_y = 0;
//	vector<double> * bb1_t_E = 0;
	vector<double> * bb1_t_r = 0;
	vector<double> * bb1_b_x = 0;
	vector<double> * bb1_b_y = 0;
//	vector<double> * bb1_b_E = 0;
	vector<double> * bb1_b_r = 0;
	
	
	// set branch addresses whether we're using a cut or not.
	the_tree -> SetBranchAddress("bb1_top_x", &bb1_t_x);
	the_tree -> SetBranchAddress("bb1_top_y", &bb1_t_y);
//	the_tree -> SetBranchAddress("bb1_top_E", &bb1_t_E);
	the_tree -> SetBranchAddress("bb1_top_r", &bb1_t_r);
	the_tree -> SetBranchAddress("bb1_bottom_x", &bb1_b_x);
	the_tree -> SetBranchAddress("bb1_bottom_y", &bb1_b_y);
//	the_tree -> SetBranchAddress("bb1_bottom_E", &bb1_b_E);
	the_tree -> SetBranchAddress("bb1_bottom_r", &bb1_b_r);
	
	double gen_t_r=0;
	double gen_b_r=0;
//	the_tree -> SetBranchAddress("gen_rhit_t", &gen_t_r);
//	the_tree -> SetBranchAddress("gen_rhit_b", &gen_b_r);
	double gen_costheta;
	the_tree -> SetBranchAddress("gen_costheta", &gen_costheta);
	double zhit=103.627357;
	
	int n_hits_t = 0;
	int n_hits_b = 0;
	
	cout << "branch addresses are set." << endl;
	
	int nentries = the_tree->GetEntries();
	cout << "nentries = " << nentries << endl;
	for(int i=0; i<nentries; i++)
	{
		the_tree -> GetEntry(i);
		if( (i % 100000) == 0) { cout << "Reached entry "<< i << endl; }
		
	//	n_hits_t = bb1_t_x->size();
	//	n_hits_b = bb1_b_x->size();
		/*
		// * // // // * // // // * // // // * // 
		if(TTLBit_SigmaPlus==1)
		{
		//	cout << "+" << endl;
			// naive hits:  
			if(naive_hit_t==1)  // pol:  p
			{
				naive_EnergyT_p_hist -> Fill(ScintT);
				gen_rhit_t = sqrt( pow(bb1_t_x->at(0), 2) + pow(bb1_t_y->at(0), 2) );
				if(gen_rhit_t<=15.5) { naive_EnergyT_p_hist_rcut155 -> Fill(ScintT); }
				//   --- * ---   //
			}
			if(naive_hit_b==1) // pol:  p
			{
				naive_EnergyB_p_hist -> Fill(ScintB);
				gen_rhit_b = sqrt(pow(bb1_b_x->at(0), 2) + pow(bb1_b_y->at(0), 2) );
				if(gen_rhit_b<=15.5) { naive_EnergyB_p_hist_rcut155 -> Fill(ScintB); }
				//   --- * ---   //
			}
			
			//   --- * ---   //   --- * ---   //
			// full spectra, for events going in all the directions:
			//
		}
		// * // // // * // // // * // // // * // 
		else if(TTLBit_SigmaPlus==0)
		{
		//	cout << "-" << endl;
			// naive hits:
			if(naive_hit_t==1) // pol:  m
			{
				naive_EnergyT_m_hist -> Fill(ScintT);
				gen_rhit_t = sqrt( pow(bb1_t_x->at(0), 2) + pow(bb1_t_y->at(0), 2) );
				if(gen_rhit_t<=15.5) { naive_EnergyT_m_hist_rcut155 -> Fill(ScintT); }
				
				//   --- * ---   //
			}
			if(naive_hit_b==1) // pol:  m
			{
				naive_EnergyB_m_hist -> Fill(ScintB);
				gen_rhit_b = sqrt(pow(bb1_b_x->at(0), 2) + pow(bb1_b_y->at(0), 2) );
				if(gen_rhit_b<=15.5) { naive_EnergyB_m_hist_rcut155 -> Fill(ScintB); }
				//   --- * ---   //
			}
			
			//   --- * ---   //   --- * ---   //
			// full spectra, for events going in all the directions:
			//
		}
		else
		{
			cout << "It's broken." << endl;
			assert(0);
			return;
		}
		*/
		
		if(naive_hit_t==1)
		{
			n_hits_t=1;
			n_hits_b=0;
		}
		else if(naive_hit_b==1)
		{
			n_hits_b=1;
			n_hits_t=0;
		}
		
		if(TTLBit_SigmaPlus==1)
		{
			if(naive_hit_t==1)  // pol:  p
			{
			//	"naive_EnergyT_p_hist -> Fill(gen_Tbeta);"  would go here.
				if( ScintT>0 && n_hits_t==1 && n_hits_b==0 ) 
				{
					if(bb1_t_r->at(0) <= 15.5)
					{
					//	"th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, ScintT);" would go here.
						th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, ScintT);
					}
					else
					{
						th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, 5500);
					}
				}
				else
				{
					th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, 5500);
				}
			}
			if(naive_hit_b==1)  // pol:  p
			{
			//	"naive_EnergyB_p_hist -> Fill(gen_Tbeta);"  would go here.
				if( ScintB>0 && n_hits_b==1 && n_hits_t==0 ) 
				{
					if(bb1_b_r->at(0) <= 15.5)
					{
						th2_num_Eout_v_Ein_bp->Fill(gen_Tbeta, ScintB);
					}
					else
					{
						th2_num_Eout_v_Ein_bp->Fill(gen_Tbeta, 5500);
					}
				}
				else
				{
					th2_num_Eout_v_Ein_bp->Fill(gen_Tbeta, 5500);
				}
			}
		}
		else if(TTLBit_SigmaPlus==0)
		{
			if(naive_hit_t==1)  // pol:  m
			{
				if( ScintT>0 && n_hits_t==1 && n_hits_b==0 ) 
				{
					if(bb1_t_r->at(0) <= 15.5)
					{
						th2_num_Eout_v_Ein_tm->Fill(gen_Tbeta, ScintT);
					}
					else
					{
						th2_num_Eout_v_Ein_tm->Fill(gen_Tbeta, 5500);
					}
				}
				else
				{
					th2_num_Eout_v_Ein_tm->Fill(gen_Tbeta, 5500);
				}
			}
			if(naive_hit_b==1)  // pol:  m
			{
				if( ScintB>0 && n_hits_b==1 && n_hits_t==0 ) 
				{
					if(bb1_b_r->at(0) <= 15.5)
					{
						th2_num_Eout_v_Ein_bm->Fill(gen_Tbeta, ScintB);
					}
					else
					{
						th2_num_Eout_v_Ein_bm->Fill(gen_Tbeta, 5500);
					}
				}
				else
				{
					th2_num_Eout_v_Ein_bm->Fill(gen_Tbeta, 5500);
				}
			}
		}
		else
		{
			cout << "It's broken." << endl;
		}
		
		if(TTLBit_SigmaPlus==1)
		{
			// naive hits:  
			if(naive_hit_t==1)  // pol:  p
			{
				naive_EnergyT_p_hist -> Fill(gen_Tbeta);
				if(gen_rhit_t<=15.5) { naive_EnergyT_p_hist_rcut155 -> Fill(gen_Tbeta); }
				//   --- * ---   //
				if(ScintT>0) // has a scint hit.
				{
					measured_EnergyT_p_hist_samedet -> Fill(ScintT); 
					if(n_hits_t>0) // naive tp, hits t
					{
						if(n_hits_t==1)
						{
							measured_EnergyT_p_bb1agree_samedet_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
					//			th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, ScintT);
								measured_EnergyT_p_bb1_r155_samedet_one -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_p_bb1_r105_samedet_one -> Fill(ScintT);
								}
							}
						}
					//	if(n_hits_t>=2)
					//	{
					//		measured_EnergyT_p_bb1agree_samedet_two -> Fill(ScintT);
					//		if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					//		{
					//			measured_EnergyT_p_bb1_r155_samedet_two -> Fill(ScintT);
					//			if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
					//			{
					//				measured_EnergyT_p_bb1_r105_samedet_two -> Fill(ScintT);
					//			}
					//		}
					//	}
					}
				}
				//
				if(ScintB>0) 
				{
					measured_EnergyB_p_hist_oppdet -> Fill(ScintB); 
					if(n_hits_b>0) // naive tp, hits b
					{
						if(n_hits_b==1)
						{
							measured_EnergyB_p_bb1agree_oppdet_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyB_p_bb1_r155_oppdet_one -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyB_p_bb1_r105_oppdet_one -> Fill(ScintB);
								}
							}
						}
					//	if(n_hits_b>=2)
					//	{
					//		measured_EnergyB_p_bb1agree_oppdet_two -> Fill(ScintB);
					//		if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					//		{
					//			measured_EnergyB_p_bb1_r155_oppdet_two -> Fill(ScintB);
					//			if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
					//			{
					//				measured_EnergyB_p_bb1_r105_oppdet_two -> Fill(ScintB);
					//			}
					//		}
					//	}
					}
				}
			}
			if(naive_hit_b==1) // pol:  p
			{
				naive_EnergyB_p_hist -> Fill(gen_Tbeta);
				if(gen_rhit_b<=15.5) { naive_EnergyB_p_hist_rcut155 -> Fill(gen_Tbeta); }
				//   --- * ---   //
				if(ScintT>0) 
				{
					measured_EnergyT_p_hist_oppdet -> Fill(ScintT); 
					if(n_hits_t>0) // naive bp, hits t
					{
						if(n_hits_t==1)
						{
							measured_EnergyT_p_bb1agree_oppdet_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyT_p_bb1_r155_oppdet_one -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_p_bb1_r105_oppdet_one -> Fill(ScintT);
								}
							}
						}
					//	if(n_hits_t>=2)
					//	{
					//		measured_EnergyT_p_bb1agree_oppdet_two -> Fill(ScintT);
					//		if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					//		{
					//			measured_EnergyT_p_bb1_r155_oppdet_two -> Fill(ScintT);
					//			if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
					//			{
					//				measured_EnergyT_p_bb1_r105_oppdet_two -> Fill(ScintT);
					//			}
					//		}
					//	}
					}
				}
				//
				if(ScintB>0) 
				{
					measured_EnergyB_p_hist_samedet -> Fill(ScintB); 
					if(n_hits_b>0) // naive bp, hits b
					{
						if(n_hits_b==1)
						{
							measured_EnergyB_p_bb1agree_samedet_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
						//		th2_num_Eout_v_Ein_bp->Fill(gen_Tbeta, ScintB);
								measured_EnergyB_p_bb1_r155_samedet_one -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyB_p_bb1_r105_samedet_one -> Fill(ScintB);
								}
							}
						}
					//	if(n_hits_b>=2)
					//	{
					//		measured_EnergyB_p_bb1agree_samedet_two -> Fill(ScintB);
					//		if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					//		{
					//			measured_EnergyB_p_bb1_r155_samedet_two -> Fill(ScintB);
					//			if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
					//			{
					//				measured_EnergyB_p_bb1_r105_samedet_two -> Fill(ScintB);
					//			}
					//		}
					//	}
					}
				}
			}
			
			//   --- * ---   //   --- * ---   //
			// full spectra, for events going in all the directions:
			if(ScintT>0) // has a scint hit.
			{
				measured_EnergyT_p_hist -> Fill(ScintT); 
				if(n_hits_t>0) // has a corresponding bb1 hit
				{
					measured_EnergyT_p_bb1agree -> Fill(ScintT);
					costheta_v_costheta_p -> Fill(gen_costheta, get_costheta(bb1_t_r->at(0), zhit));
					if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					{
						measured_EnergyT_p_bb1_r155 -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
						{
							measured_EnergyT_p_bb1_r105 -> Fill(ScintT);
						}
					}
					if(n_hits_t==1)
					{
						measured_EnergyT_p_bb1agree_one -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyT_p_bb1_r155_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyT_p_bb1_r105_one -> Fill(ScintT);
							}
						}
					}
				//	if(n_hits_t>=2)
				//	{
				//		measured_EnergyT_p_bb1agree_two -> Fill(ScintT);
				//		if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
				//		{
				//			measured_EnergyT_p_bb1_r155_two -> Fill(ScintT);
				//			if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
				//			{
				//				measured_EnergyT_p_bb1_r105_two -> Fill(ScintT);
				//			}
				//		}
				//	}
				}
			}
			if(ScintB>0) 
			{
				measured_EnergyB_p_hist -> Fill(ScintB); 
				if(n_hits_b>0)
				{
					measured_EnergyB_p_bb1agree -> Fill(ScintB); 
					costheta_v_costheta_p -> Fill(gen_costheta, get_costheta(bb1_b_r->at(0), -1.0*zhit));
					if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					{
						measured_EnergyB_p_bb1_r155 -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
						{
							measured_EnergyB_p_bb1_r105 -> Fill(ScintB);
						}
					}
					if(n_hits_b==1)
					{
						measured_EnergyB_p_bb1agree_one -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyB_p_bb1_r155_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyB_p_bb1_r105_one -> Fill(ScintB);
							}
						}
					}
				//	if(n_hits_b>=2)
				//	{
				//		measured_EnergyB_p_bb1agree_two -> Fill(ScintB);
				//		if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
				//		{
				//			measured_EnergyB_p_bb1_r155_two-> Fill(ScintB);
				//			if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
				//			{
				//				measured_EnergyB_p_bb1_r105_two -> Fill(ScintB);
				//			}
				//		}
				//	}
				}
			}
			//
		}
		
		// * // // // * // // // * // // // * // 
		else if(TTLBit_SigmaPlus==0)
		{
			// naive hits:
			if(naive_hit_t==1) // pol:  m
			{
				naive_EnergyT_m_hist -> Fill(gen_Tbeta);
				if(gen_rhit_t<=15.5) { naive_EnergyT_m_hist_rcut155 -> Fill(gen_Tbeta); }
				
				//   --- * ---   //
				if(ScintT>0) 
				{
					measured_EnergyT_m_hist_samedet -> Fill(ScintT); 
					if(n_hits_t>0) // naive tm, hits t
					{
						if(n_hits_t==1)
						{
							measured_EnergyT_m_bb1agree_samedet_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
						//		th2_num_Eout_v_Ein_tm->Fill(gen_Tbeta, ScintT);
								measured_EnergyT_m_bb1_r155_samedet_one -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_m_bb1_r105_samedet_one -> Fill(ScintT);
								}
							}
						}
					//	if(n_hits_t>=2)
					//	{
					//		measured_EnergyT_m_bb1agree_samedet_two -> Fill(ScintT);
					//		if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					//		{
					//			measured_EnergyT_m_bb1_r155_samedet_two -> Fill(ScintT);
					//			if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
					//			{
					//				measured_EnergyT_m_bb1_r105_samedet_two -> Fill(ScintT);
					//			}
					//		}
					//	}
					}
				}
			//	if(ScintB>0) 
			//	{
			//		measured_EnergyB_m_hist_oppdet -> Fill(ScintB); 
			//		if(n_hits_b>0) // naive tm, hits b
			//		{
			//			if(n_hits_b==1)
			//			{
			//				measured_EnergyB_m_bb1agree_oppdet_one -> Fill(ScintB);
			//				if( bb1_b_r->at(0) <= 15.5 )
			//				{
			//					measured_EnergyB_m_bb1_r155_oppdet_one -> Fill(ScintB);
			//					if( bb1_b_r->at(0) <= 10.5 ) 
			//					{
			//						measured_EnergyB_m_bb1_r105_oppdet_one -> Fill(ScintB);
			//					}
			//				}
			//			}
			//		//	if(n_hits_b>=2)
			//		//	{
			//		//		measured_EnergyB_m_bb1agree_oppdet_two -> Fill(ScintB);
			//		//		if( bb1_b_r->at(0) <= 15.5 )
			//		//		{
			//		//			measured_EnergyB_m_bb1_r155_oppdet_two -> Fill(ScintB);
			//		//			if( bb1_b_r->at(0) <= 10.5 ) 
			//		//			{
			//		//				measured_EnergyB_m_bb1_r105_oppdet_two -> Fill(ScintB);
			//		//			}
			//		//		}
			//		//	}
			//		}
			//	}
			}
			if(naive_hit_b==1) // pol:  m
			{
				naive_EnergyB_m_hist -> Fill(gen_Tbeta);
				if(gen_rhit_b<=15.5) { naive_EnergyB_m_hist_rcut155 -> Fill(gen_Tbeta); }
				//   --- * ---   //
			//	if(ScintT>0) 
			//	{
			//		measured_EnergyT_m_hist_oppdet -> Fill(ScintT); 
			//		if(n_hits_t>0) // naive bm, hits t
			//		{
			//			if(n_hits_t==1)
			//			{
			//				measured_EnergyT_m_bb1agree_oppdet_one -> Fill(ScintT);
			//				if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
			//				{
			//					measured_EnergyT_m_bb1_r155_oppdet_one -> Fill(ScintT);
			//					if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
			//					{
			//						measured_EnergyT_m_bb1_r105_oppdet_one -> Fill(ScintT);
			//					}
			//				}
			//			}
			//		//	if(n_hits_t>=2)
			//		//	{
			//		//		measured_EnergyT_m_bb1agree_oppdet_two -> Fill(ScintT);
			//		//		if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
			//		//		{
			//		//			measured_EnergyT_m_bb1_r155_oppdet_two -> Fill(ScintT);
			//		//			if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
			//		//			{
			//		//				measured_EnergyT_m_bb1_r105_oppdet_two -> Fill(ScintT);
			//		//			}
			//		//		}
			//		//	}
			//		}
			//	}
				if(ScintB>0) 
				{
					measured_EnergyB_m_hist_samedet -> Fill(ScintB); 
					if(n_hits_b>0) // naive bm, hits b
					{
						if(n_hits_b==1)
						{
							measured_EnergyB_m_bb1agree_samedet_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 )
							{
						//		th2_num_Eout_v_Ein_bm->Fill(gen_Tbeta, ScintB);
								measured_EnergyB_m_bb1_r155_samedet_one -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) 
								{
									measured_EnergyB_m_bb1_r105_samedet_one -> Fill(ScintB);
								}
							}
						}
					//	if(n_hits_b>=2)
					//	{
					//		measured_EnergyB_m_bb1agree_samedet_two -> Fill(ScintB);
					//		if( bb1_b_r->at(0) <= 15.5 )
					//		{
					//			measured_EnergyB_m_bb1_r155_samedet_two -> Fill(ScintB);
					//			if( bb1_b_r->at(0) <= 10.5 ) 
					//			{
					//				measured_EnergyB_m_bb1_r105_samedet_two -> Fill(ScintB);
					//			}
					//		}
					//	}
					}
				}
			}
			
			//   --- * ---   //   --- * ---   //
			// full spectra, for events going in all the directions:
			if(ScintT>0) 
			{
				measured_EnergyT_m_hist -> Fill(ScintT); 
				if(n_hits_t>0) 
				{
				//	cout << "i=" << i << ", ScintT=" << ScintT << ", bb1_t_r=" << bb1_t_r->at(0) << endl;
					measured_EnergyT_m_bb1agree -> Fill(ScintT);
					costheta_v_costheta_m -> Fill(gen_costheta, get_costheta(bb1_t_r->at(0), zhit));
					if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					{
						measured_EnergyT_m_bb1_r155 -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
						{
							measured_EnergyT_m_bb1_r105 -> Fill(ScintT);
						}
					}
					//
					if(n_hits_t==1)
					{
						measured_EnergyT_m_bb1agree_one -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyT_m_bb1_r155_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyT_m_bb1_r105_one -> Fill(ScintT);
							}
						}
					}
				//	if(n_hits_t>=2)
				//	{
				//		measured_EnergyT_m_bb1agree_two -> Fill(ScintT);
				//		if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
				//		{
				//			measured_EnergyT_m_bb1_r155_two -> Fill(ScintT);
				//			if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
				//			{
				//				measured_EnergyT_m_bb1_r105_two -> Fill(ScintT);
				//			}
				//		}
				//	}
				}
			}
			if(ScintB>0) 
			{
				measured_EnergyB_m_hist -> Fill(ScintB); 
				if(n_hits_b>0)
				{
					measured_EnergyB_m_bb1agree -> Fill(ScintB); 
					costheta_v_costheta_m -> Fill(gen_costheta, get_costheta(bb1_b_r->at(0), -1.0*zhit));
					if( bb1_b_r->at(0) <= 15.5 )
					{
						measured_EnergyB_m_bb1_r155 -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 10.5 ) 
						{
							measured_EnergyB_m_bb1_r105 -> Fill(ScintB);
						}
					}
					//
					if(n_hits_b==1)
					{
						measured_EnergyB_m_bb1agree_one -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyB_m_bb1_r155_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyB_m_bb1_r105_one -> Fill(ScintB);
							}
						}
					}
				//	if(n_hits_b>=2)
				//	{
				//		measured_EnergyB_m_bb1agree_two -> Fill(ScintB);
				//		if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
				//		{
				//			measured_EnergyB_m_bb1_r155_two-> Fill(ScintB);
				//			if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
				//			{
				//				measured_EnergyB_m_bb1_r105_two -> Fill(ScintB);
				//			}
				//		}
				//	}
				}
			}
			//
		}
		else
		{
			cout << "It's broken." << endl;
			assert(0);
			return;
		}
	}
	//
	cout << "Guess we're done loading from simple tree." << endl;
	return;
} // LoadSimpleFromTree

void MapSetup::LoadFromTree(TChain * the_tree, int N_rebin_hists)  // default N_rebin_hists=1 if called with no argument.
{
	cout << "Called MapSetup::LoadFromTree(...) with N_rebin_hists=" << N_rebin_hists << endl;
	
//	gROOT->ForceStyle();
	gStyle->SetOptStat(0);
	
	int parallel_color;
	int antiparallel_color;
	
	naive_EnergyT_p_hist = CreateHist( string("Naive Upper Energy(+)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyB_p_hist = CreateHist( string("Naive Lower Energy(+)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyT_m_hist = CreateHist( string("Naive Upper Energy(-)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyB_m_hist = CreateHist( string("Naive Lower Energy(-)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	naive_EnergyT_p_hist_rcut155 = CreateHist( string("Naive Upper Energy(+) (R<=15.5mm)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyB_p_hist_rcut155 = CreateHist( string("Naive Lower Energy(+) (R<=15.5mm)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyT_m_hist_rcut155 = CreateHist( string("Naive Upper Energy(-) (R<=15.5mm)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	naive_EnergyB_m_hist_rcut155 = CreateHist( string("Naive Lower Energy(-) (R<=15.5mm)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	//
	measured_EnergyT_p_hist = CreateHist( string("Measured ScintT Energy(+)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_hist = CreateHist( string("Measured ScintB Energy(+)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_hist = CreateHist( string("Measured ScintT Energy(-)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_hist = CreateHist( string("Measured ScintB Energy(-)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_hist_samedet = CreateHist( string("Measured ScintT Energy(+) (same det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_hist_samedet = CreateHist( string("Measured ScintB Energy(+) (same det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_hist_samedet = CreateHist( string("Measured ScintT Energy(-) (same det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_hist_samedet = CreateHist( string("Measured ScintB Energy(-) (same det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_hist_oppdet = CreateHist( string("Measured ScintT Energy(+) (opp det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_hist_oppdet = CreateHist( string("Measured ScintB Energy(+) (opp det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_hist_oppdet = CreateHist( string("Measured ScintT Energy(-) (opp det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_hist_oppdet = CreateHist( string("Measured ScintB Energy(-) (opp det)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	//
	measured_EnergyT_p_bb1agree = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1agree_one = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_one = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_one = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_one = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1agree_two = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_two = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_two = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_two = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1agree_samedet_one = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_samedet_one = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_samedet_one = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_samedet_one = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1agree_samedet_two = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_samedet_two = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_samedet_two = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_samedet_two = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1agree_oppdet_one = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_oppdet_one = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_oppdet_one = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_oppdet_one = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1agree_oppdet_two = CreateHist( string("Measured ScintT Energy(+) - BB1 Agreement (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1agree_oppdet_two = CreateHist( string("Measured ScintB Energy(+) - BB1 Agreement (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1agree_oppdet_two = CreateHist( string("Measured ScintT Energy(-) - BB1 Agreement (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1agree_oppdet_two = CreateHist( string("Measured ScintB Energy(-) - BB1 Agreement (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	//
	measured_EnergyT_p_bb1_r155 = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155 = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155 = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155 = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1_r155_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1_r155_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r155_samedet_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_samedet_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_samedet_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_samedet_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r155_samedet_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_samedet_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_samedet_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_samedet_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r155_oppdet_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_oppdet_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_oppdet_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_oppdet_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);

	measured_EnergyT_p_bb1_r155_oppdet_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=15.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r155_oppdet_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=15.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r155_oppdet_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=15.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r155_oppdet_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=15.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	//
	measured_EnergyT_p_bb1_r105 = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105 = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105 = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105 = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_samedet_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_samedet_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_samedet_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_samedet_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (same det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_samedet_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_samedet_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_samedet_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_samedet_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (same det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_oppdet_one = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_oppdet_one = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_oppdet_one = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_oppdet_one = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (opp det, 1hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	measured_EnergyT_p_bb1_r105_oppdet_two = CreateHist( string("Measured ScintT Energy(+) - rBB1<=10.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_p_bb1_r105_oppdet_two = CreateHist( string("Measured ScintB Energy(+) - rBB1<=10.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyT_m_bb1_r105_oppdet_two = CreateHist( string("Measured ScintT Energy(-) - rBB1<=10.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	measured_EnergyB_m_bb1_r105_oppdet_two = CreateHist( string("Measured ScintB Energy(-) - rBB1<=10.5 (opp det, 2hit)"), string("Mapping_Ebeta"), int(kBlack), N_rebin_hists);
	
	// bb1radiuscut, costheta_actual vs costheta_generated
	costheta_v_costheta_p = CreateHist2d( string("costheta v costheta (+) - BB1 Agreement"), string("costheta"), string("costheta") );
		costheta_v_costheta_p -> GetXaxis() -> SetTitle("Generated Cos(theta)");
		costheta_v_costheta_p -> GetYaxis() -> SetTitle("Observed Cos(theta)");
	costheta_v_costheta_m = CreateHist2d( string("costheta v costheta (-) - BB1 Agreement"), string("costheta"), string("costheta") );
		costheta_v_costheta_m -> GetXaxis() -> SetTitle("Generated Cos(theta)");
		costheta_v_costheta_m -> GetYaxis() -> SetTitle("Observed Cos(theta)");
	
	// ok, so what about the BB1 spectrum data??
//	BB1energyT_p_bb1agree;
//	BB1energyB_p_bb1agree;
//	BB1energyT_m_bb1agree;
//	BB1energyB_m_bb1agree;
	
	th2_num_Eout_v_Ein_tp = CreateHist2d( string("Numerical E_measured vs E_generated, Top(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_num_Eout_v_Ein_tm = CreateHist2d( string("Numerical E_measured vs E_generated, Top(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_num_Eout_v_Ein_bp = CreateHist2d( string("Numerical E_measured vs E_generated, Bottom(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_num_Eout_v_Ein_bm = CreateHist2d( string("Numerical E_measured vs E_generated, Bottom(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_num_Eout_v_Ein_tp -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_num_Eout_v_Ein_bp -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_num_Eout_v_Ein_tm -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_num_Eout_v_Ein_bm -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_num_Eout_v_Ein_tp -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_num_Eout_v_Ein_bp -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_num_Eout_v_Ein_tm -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_num_Eout_v_Ein_bm -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	
	/*
	th2_ana_Eout_v_Ein_tp = CreateHist2d( string("Analytic E_measured vs E_generated, Top(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_tm = CreateHist2d( string("Analytic E_measured vs E_generated, Top(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_bp = CreateHist2d( string("Analytic E_measured vs E_generated, Bottom(+)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	th2_ana_Eout_v_Ein_bm = CreateHist2d( string("Analytic E_measured vs E_generated, Bottom(-)"), string("Mapping_Ebeta"), string("Mapping_Ebeta"), N_rebin_hists, N_rebin_hists);
	
	th2_ana_Eout_v_Ein_tp -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_ana_Eout_v_Ein_bp -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_ana_Eout_v_Ein_tm -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_ana_Eout_v_Ein_bm -> GetXaxis() -> SetTitle("Generated Beta Kinetic Energy [keV]");
	th2_ana_Eout_v_Ein_tp -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_ana_Eout_v_Ein_bp -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_ana_Eout_v_Ein_tm -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	th2_ana_Eout_v_Ein_bm -> GetYaxis() -> SetTitle("Measured Scintillator Energy [keV]");
	*/
	
	// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	double naive_hit_t;
	double naive_hit_b;
	the_tree -> SetBranchAddress("naive_hit_t", &naive_hit_t);
	the_tree -> SetBranchAddress("naive_hit_b", &naive_hit_b);
	double gen_Tbeta;
	the_tree -> SetBranchAddress("gen_Tbeta", &gen_Tbeta);
	
	double gen_xhit_t;
	double gen_yhit_t;
	double gen_xhit_b;
	double gen_yhit_b;
	the_tree -> SetBranchAddress("gen_xhit_t", &gen_xhit_t);
	the_tree -> SetBranchAddress("gen_yhit_t", &gen_yhit_t);
	the_tree -> SetBranchAddress("gen_xhit_b", &gen_xhit_b);
	the_tree -> SetBranchAddress("gen_yhit_b", &gen_yhit_b);
	// in the future, this will come pre-pixellated.  but for now, we'll pixellate it here.  ...or not.  we should pixellate it sometime.
	double gen_rhit_t;
	double gen_rhit_b;
	the_tree -> SetBranchAddress("gen_rhit_t", &gen_rhit_t);
	the_tree -> SetBranchAddress("gen_rhit_b", &gen_rhit_b);

	
	int TTLBit_SigmaPlus;
	the_tree -> SetBranchAddress("TTLBit_SigmaPlus", &TTLBit_SigmaPlus);
	
	Double_t ScintT;
	Double_t ScintB;
	the_tree -> SetBranchAddress("upper_scint_E", &ScintT);
	the_tree -> SetBranchAddress("lower_scint_E", &ScintB);
	
	// BB1s:  
	vector<double> * bb1_t_x = 0;
	vector<double> * bb1_t_y = 0;
	vector<double> * bb1_t_E = 0;
	vector<double> * bb1_t_r = 0;
	vector<double> * bb1_b_x = 0;
	vector<double> * bb1_b_y = 0;
	vector<double> * bb1_b_E = 0;
	vector<double> * bb1_b_r = 0;
	
	
	// set branch addresses whether we're using a cut or not.
	the_tree -> SetBranchAddress("bb1_top_x", &bb1_t_x);
	the_tree -> SetBranchAddress("bb1_top_y", &bb1_t_y);
	the_tree -> SetBranchAddress("bb1_top_E", &bb1_t_E);
	the_tree -> SetBranchAddress("bb1_top_r", &bb1_t_r);
	the_tree -> SetBranchAddress("bb1_bottom_x", &bb1_b_x);
	the_tree -> SetBranchAddress("bb1_bottom_y", &bb1_b_y);
	the_tree -> SetBranchAddress("bb1_bottom_E", &bb1_b_E);
	the_tree -> SetBranchAddress("bb1_bottom_r", &bb1_b_r);
	
	double gen_t_r;
	double gen_b_r;
	the_tree -> SetBranchAddress("gen_rhit_t", &gen_t_r);
	the_tree -> SetBranchAddress("gen_rhit_b", &gen_b_r);
	double gen_costheta;
	the_tree -> SetBranchAddress("gen_costheta", &gen_costheta);
	double zhit=103.627357;
	
	int n_hits_t = 0;
	int n_hits_b = 0;
	
	int nentries = the_tree->GetEntries();
	cout << "nentries = " << nentries << endl;
//	nentries = 5000000;
	for(int i=0; i<nentries; i++)
	{
		the_tree -> GetEntry(i);
		if( (i % 100000) == 0) { cout << "Reached entry "<< i << endl; }
		
		n_hits_t = bb1_t_r->size();
		n_hits_b = bb1_b_r->size();
		
		// * // // // * // // // * // // // * // 
	//	th2_num_Eout_v_Ein_tp = (TH2D*)f->Get("Numerical E_measured vs E_generated, Top(+)");
	//	th2_num_Eout_v_Ein_tm = (TH2D*)f->Get("Numerical E_measured vs E_generated, Top(-)");
	//	th2_num_Eout_v_Ein_bp = (TH2D*)f->Get("Numerical E_measured vs E_generated, Bottom(+)");
	//	th2_num_Eout_v_Ein_bm = (TH2D*)f->Get("Numerical E_measured vs E_generated, Bottom(-)");
		if(TTLBit_SigmaPlus==1)
		{
			if(naive_hit_t==1)  // pol:  p
			{
			//	"naive_EnergyT_p_hist -> Fill(gen_Tbeta);"  would go here.
				if( ScintT>0 && n_hits_t==1 && n_hits_b==0 ) 
				{
					if(bb1_t_r->at(0) <= 15.5)
					{
					//	"th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, ScintT);" would go here.
						th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, ScintT);
					}
					else
					{
						th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, 5500);
					}
				}
				else
				{
					th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, 5500);
				}
			}
			if(naive_hit_b==1)  // pol:  p
			{
			//	"naive_EnergyB_p_hist -> Fill(gen_Tbeta);"  would go here.
				if( ScintB>0 && n_hits_b==1 && n_hits_t==0 ) 
				{
					if(bb1_b_r->at(0) <= 15.5)
					{
						th2_num_Eout_v_Ein_bp->Fill(gen_Tbeta, ScintB);
					}
					else
					{
						th2_num_Eout_v_Ein_bp->Fill(gen_Tbeta, 5500);
					}
				}
				else
				{
					th2_num_Eout_v_Ein_bp->Fill(gen_Tbeta, 5500);
				}
			}
		}
		else if(TTLBit_SigmaPlus==0)
		{
			if(naive_hit_t==1)  // pol:  m
			{
				if( ScintT>0 && n_hits_t==1 && n_hits_b==0 ) 
				{
					if(bb1_t_r->at(0) <= 15.5)
					{
						th2_num_Eout_v_Ein_tm->Fill(gen_Tbeta, ScintT);
					}
					else
					{
						th2_num_Eout_v_Ein_tm->Fill(gen_Tbeta, 5500);
					}
				}
				else
				{
					th2_num_Eout_v_Ein_tm->Fill(gen_Tbeta, 5500);
				}
			}
			if(naive_hit_b==1)  // pol:  m
			{
				if( ScintB>0 && n_hits_b==1 && n_hits_t==0 ) 
				{
					if(bb1_b_r->at(0) <= 15.5)
					{
						th2_num_Eout_v_Ein_bm->Fill(gen_Tbeta, ScintB);
					}
					else
					{
						th2_num_Eout_v_Ein_bm->Fill(gen_Tbeta, 5500);
					}
				}
				else
				{
					th2_num_Eout_v_Ein_bm->Fill(gen_Tbeta, 5500);
				}
			}
		}
		else
		{
			cout << "It's broken." << endl;
		}
		
		if(TTLBit_SigmaPlus==1)
		{
			// naive hits:  
			if(naive_hit_t==1)  // pol:  p
			{
				naive_EnergyT_p_hist -> Fill(gen_Tbeta);
				if(gen_rhit_t<=15.5) { naive_EnergyT_p_hist_rcut155 -> Fill(gen_Tbeta); }
				//   --- * ---   //
				if(ScintT>0) // has a scint hit.
				{
					measured_EnergyT_p_hist_samedet -> Fill(ScintT); 
					if(n_hits_t>0) // naive tp, hits t
					{
						if(n_hits_t==1)
						{
							measured_EnergyT_p_bb1agree_samedet_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
					//			th2_num_Eout_v_Ein_tp->Fill(gen_Tbeta, ScintT);
								measured_EnergyT_p_bb1_r155_samedet_one -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_p_bb1_r105_samedet_one -> Fill(ScintT);
								}
							}
						}
						if(n_hits_t>=2)
						{
							measured_EnergyT_p_bb1agree_samedet_two -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyT_p_bb1_r155_samedet_two -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_p_bb1_r105_samedet_two -> Fill(ScintT);
								}
							}
						}
					}
				}
				//
				if(ScintB>0) 
				{
					measured_EnergyB_p_hist_oppdet -> Fill(ScintB); 
					if(n_hits_b>0) // naive tp, hits b
					{
						if(n_hits_b==1)
						{
							measured_EnergyB_p_bb1agree_oppdet_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyB_p_bb1_r155_oppdet_one -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyB_p_bb1_r105_oppdet_one -> Fill(ScintB);
								}
							}
						}
						if(n_hits_b>=2)
						{
							measured_EnergyB_p_bb1agree_oppdet_two -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyB_p_bb1_r155_oppdet_two -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyB_p_bb1_r105_oppdet_two -> Fill(ScintB);
								}
							}
						}
					}
				}
			}
			if(naive_hit_b==1) // pol:  p
			{
				naive_EnergyB_p_hist -> Fill(gen_Tbeta);
				if(gen_rhit_b<=15.5) { naive_EnergyB_p_hist_rcut155 -> Fill(gen_Tbeta); }
				//   --- * ---   //
				if(ScintT>0) 
				{
					measured_EnergyT_p_hist_oppdet -> Fill(ScintT); 
					if(n_hits_t>0) // naive bp, hits t
					{
						if(n_hits_t==1)
						{
							measured_EnergyT_p_bb1agree_oppdet_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyT_p_bb1_r155_oppdet_one -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_p_bb1_r105_oppdet_one -> Fill(ScintT);
								}
							}
						}
						if(n_hits_t>=2)
						{
							measured_EnergyT_p_bb1agree_oppdet_two -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyT_p_bb1_r155_oppdet_two -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_p_bb1_r105_oppdet_two -> Fill(ScintT);
								}
							}
						}
					}
				}
				//
				if(ScintB>0) 
				{
					measured_EnergyB_p_hist_samedet -> Fill(ScintB); 
					if(n_hits_b>0) // naive bp, hits b
					{
						if(n_hits_b==1)
						{
							measured_EnergyB_p_bb1agree_samedet_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
						//		th2_num_Eout_v_Ein_bp->Fill(gen_Tbeta, ScintB);
								measured_EnergyB_p_bb1_r155_samedet_one -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyB_p_bb1_r105_samedet_one -> Fill(ScintB);
								}
							}
						}
						if(n_hits_b>=2)
						{
							measured_EnergyB_p_bb1agree_samedet_two -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyB_p_bb1_r155_samedet_two -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyB_p_bb1_r105_samedet_two -> Fill(ScintB);
								}
							}
						}
					}
				}
			}
			
			//   --- * ---   //   --- * ---   //
			// full spectra, for events going in all the directions:
			if(ScintT>0) // has a scint hit.
			{
				measured_EnergyT_p_hist -> Fill(ScintT); 
				if(n_hits_t>0) // has a corresponding bb1 hit
				{
					measured_EnergyT_p_bb1agree -> Fill(ScintT);
					costheta_v_costheta_p -> Fill(gen_costheta, get_costheta(bb1_t_r->at(0), zhit));
					if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					{
						measured_EnergyT_p_bb1_r155 -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
						{
							measured_EnergyT_p_bb1_r105 -> Fill(ScintT);
						}
					}
					if(n_hits_t==1)
					{
						measured_EnergyT_p_bb1agree_one -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyT_p_bb1_r155_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyT_p_bb1_r105_one -> Fill(ScintT);
							}
						}
					}
					if(n_hits_t>=2)
					{
						measured_EnergyT_p_bb1agree_two -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyT_p_bb1_r155_two -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyT_p_bb1_r105_two -> Fill(ScintT);
							}
						}
					}
				}
			}
			if(ScintB>0) 
			{
				measured_EnergyB_p_hist -> Fill(ScintB); 
				if(n_hits_b>0)
				{
					measured_EnergyB_p_bb1agree -> Fill(ScintB); 
					costheta_v_costheta_p -> Fill(gen_costheta, get_costheta(bb1_b_r->at(0), -1.0*zhit));
					if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					{
						measured_EnergyB_p_bb1_r155 -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
						{
							measured_EnergyB_p_bb1_r105 -> Fill(ScintB);
						}
					}
					
					if(n_hits_b==1)
					{
						measured_EnergyB_p_bb1agree_one -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyB_p_bb1_r155_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyB_p_bb1_r105_one -> Fill(ScintB);
							}
						}
					}
					if(n_hits_b>=2)
					{
						measured_EnergyB_p_bb1agree_two -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyB_p_bb1_r155_two-> Fill(ScintB);
							if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyB_p_bb1_r105_two -> Fill(ScintB);
							}
						}
					}
				}
			}
			//
		}
		
		// * // // // * // // // * // // // * // 
		else if(TTLBit_SigmaPlus==0)
		{
			// naive hits:
			if(naive_hit_t==1) // pol:  m
			{
				naive_EnergyT_m_hist -> Fill(gen_Tbeta);
				if(gen_rhit_t<=15.5) { naive_EnergyT_m_hist_rcut155 -> Fill(gen_Tbeta); }
				
				//   --- * ---   //
				if(ScintT>0) 
				{
					measured_EnergyT_m_hist_samedet -> Fill(ScintT); 
					if(n_hits_t>0) // naive tm, hits t
					{
						if(n_hits_t==1)
						{
							measured_EnergyT_m_bb1agree_samedet_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
						//		th2_num_Eout_v_Ein_tm->Fill(gen_Tbeta, ScintT);
								measured_EnergyT_m_bb1_r155_samedet_one -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_m_bb1_r105_samedet_one -> Fill(ScintT);
								}
							}
						}
						if(n_hits_t>=2)
						{
							measured_EnergyT_m_bb1agree_samedet_two -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyT_m_bb1_r155_samedet_two -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_m_bb1_r105_samedet_two -> Fill(ScintT);
								}
							}
						}
					}
				}
				if(ScintB>0) 
				{
					measured_EnergyB_m_hist_oppdet -> Fill(ScintB); 
					if(n_hits_b>0) // naive tm, hits b
					{
						if(n_hits_b==1)
						{
							measured_EnergyB_m_bb1agree_oppdet_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 )
							{
								measured_EnergyB_m_bb1_r155_oppdet_one -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) 
								{
									measured_EnergyB_m_bb1_r105_oppdet_one -> Fill(ScintB);
								}
							}
						}
						if(n_hits_b>=2)
						{
							measured_EnergyB_m_bb1agree_oppdet_two -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 )
							{
								measured_EnergyB_m_bb1_r155_oppdet_two -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) 
								{
									measured_EnergyB_m_bb1_r105_oppdet_two -> Fill(ScintB);
								}
							}
						}
					}
				}
			}
			if(naive_hit_b==1) // pol:  m
			{
				naive_EnergyB_m_hist -> Fill(gen_Tbeta);
				if(gen_rhit_b<=15.5) { naive_EnergyB_m_hist_rcut155 -> Fill(gen_Tbeta); }
				//   --- * ---   //
				if(ScintT>0) 
				{
					measured_EnergyT_m_hist_oppdet -> Fill(ScintT); 
					if(n_hits_t>0) // naive bm, hits t
					{
						if(n_hits_t==1)
						{
							measured_EnergyT_m_bb1agree_oppdet_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyT_m_bb1_r155_oppdet_one -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_m_bb1_r105_oppdet_one -> Fill(ScintT);
								}
							}
						}
						if(n_hits_t>=2)
						{
							measured_EnergyT_m_bb1agree_oppdet_two -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
							{
								measured_EnergyT_m_bb1_r155_oppdet_two -> Fill(ScintT);
								if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
								{
									measured_EnergyT_m_bb1_r105_oppdet_two -> Fill(ScintT);
								}
							}
						}
					}
				}
				if(ScintB>0) 
				{
					measured_EnergyB_m_hist_samedet -> Fill(ScintB); 
					if(n_hits_b>0) // naive bm, hits b
					{
						if(n_hits_b==1)
						{
							measured_EnergyB_m_bb1agree_samedet_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 )
							{
						//		th2_num_Eout_v_Ein_bm->Fill(gen_Tbeta, ScintB);
								measured_EnergyB_m_bb1_r155_samedet_one -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) 
								{
									measured_EnergyB_m_bb1_r105_samedet_one -> Fill(ScintB);
								}
							}
						}
						if(n_hits_b>=2)
						{
							measured_EnergyB_m_bb1agree_samedet_two -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 15.5 )
							{
								measured_EnergyB_m_bb1_r155_samedet_two -> Fill(ScintB);
								if( bb1_b_r->at(0) <= 10.5 ) 
								{
									measured_EnergyB_m_bb1_r105_samedet_two -> Fill(ScintB);
								}
							}
						}
					}
				}
			}
			
			//   --- * ---   //   --- * ---   //
			// full spectra, for events going in all the directions:
			if(ScintT>0) 
			{
				measured_EnergyT_m_hist -> Fill(ScintT); 
				if(n_hits_t>0) 
				{
					measured_EnergyT_m_bb1agree -> Fill(ScintT);
					costheta_v_costheta_m -> Fill(gen_costheta, get_costheta(bb1_t_r->at(0), zhit));
					if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
					{
						measured_EnergyT_m_bb1_r155 -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
						{
							measured_EnergyT_m_bb1_r105 -> Fill(ScintT);
						}
					}
					//
					if(n_hits_t==1)
					{
						measured_EnergyT_m_bb1agree_one -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyT_m_bb1_r155_one -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyT_m_bb1_r105_one -> Fill(ScintT);
							}
						}
					}
					if(n_hits_t>=2)
					{
						measured_EnergyT_m_bb1agree_two -> Fill(ScintT);
						if( bb1_t_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyT_m_bb1_r155_two -> Fill(ScintT);
							if( bb1_t_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyT_m_bb1_r105_two -> Fill(ScintT);
							}
						}
					}
				}
			}
			if(ScintB>0) 
			{
				measured_EnergyB_m_hist -> Fill(ScintB); 
				if(n_hits_b>0)
				{
					measured_EnergyB_m_bb1agree -> Fill(ScintB); 
					costheta_v_costheta_m -> Fill(gen_costheta, get_costheta(bb1_b_r->at(0), -1.0*zhit));
					if( bb1_b_r->at(0) <= 15.5 )
					{
						measured_EnergyB_m_bb1_r155 -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 10.5 ) 
						{
							measured_EnergyB_m_bb1_r105 -> Fill(ScintB);
						}
					}
					//
					if(n_hits_b==1)
					{
						measured_EnergyB_m_bb1agree_one -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyB_m_bb1_r155_one -> Fill(ScintB);
							if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyB_m_bb1_r105_one -> Fill(ScintB);
							}
						}
					}
					if(n_hits_b>=2)
					{
						measured_EnergyB_m_bb1agree_two -> Fill(ScintB);
						if( bb1_b_r->at(0) <= 15.5 ) // bb1 radius <= 15.5 mm
						{
							measured_EnergyB_m_bb1_r155_two-> Fill(ScintB);
							if( bb1_b_r->at(0) <= 10.5 ) // bb1 radius <= 10.5 mm
							{
								measured_EnergyB_m_bb1_r105_two -> Fill(ScintB);
							}
						}
					}
				}
			}
			//
		}
		else
		{
			cout << "It's broken." << endl;
			assert(0);
			return;
		}
	}
	//
	return;
}

void MapSetup::CloneToFile(TFile * f, int verbose, bool leaveopen)
{
	f -> cd();
	// Save the histograms with the data we care about directly.
//	cout << "Called CloneToFile(...).  We're starting to save histograms..." << endl;
	//
	naive_EnergyT_p_hist -> Write();
	naive_EnergyB_p_hist -> Write();
	naive_EnergyT_m_hist -> Write();
	naive_EnergyB_m_hist -> Write();
	
	naive_EnergyT_p_hist_rcut155 -> Write();
	naive_EnergyB_p_hist_rcut155 -> Write();
	naive_EnergyT_m_hist_rcut155 -> Write();
	naive_EnergyB_m_hist_rcut155 -> Write();
	
	//
	measured_EnergyT_p_hist -> Write();
	measured_EnergyB_p_hist -> Write();
	measured_EnergyT_m_hist -> Write();
	measured_EnergyB_m_hist -> Write();
	
	measured_EnergyT_p_hist_samedet -> Write();
	measured_EnergyB_p_hist_samedet -> Write();
	measured_EnergyT_m_hist_samedet -> Write();
	measured_EnergyB_m_hist_samedet -> Write();
	
	measured_EnergyT_p_hist_oppdet -> Write();
	measured_EnergyB_p_hist_oppdet -> Write();
	measured_EnergyT_m_hist_oppdet -> Write();
	measured_EnergyB_m_hist_oppdet -> Write();
	//
	measured_EnergyT_p_bb1agree -> Write();
	measured_EnergyB_p_bb1agree -> Write();
	measured_EnergyT_m_bb1agree -> Write();
	measured_EnergyB_m_bb1agree -> Write();
	
	measured_EnergyT_p_bb1agree_one -> Write();
	measured_EnergyB_p_bb1agree_one -> Write();
	measured_EnergyT_m_bb1agree_one -> Write();
	measured_EnergyB_m_bb1agree_one -> Write();

	measured_EnergyT_p_bb1agree_two -> Write();
	measured_EnergyB_p_bb1agree_two -> Write();
	measured_EnergyT_m_bb1agree_two -> Write();
	measured_EnergyB_m_bb1agree_two -> Write();
	
	measured_EnergyT_p_bb1agree_samedet_one -> Write();
	measured_EnergyB_p_bb1agree_samedet_one -> Write();
	measured_EnergyT_m_bb1agree_samedet_one -> Write();
	measured_EnergyB_m_bb1agree_samedet_one -> Write();
	
	measured_EnergyT_p_bb1agree_samedet_two -> Write();
	measured_EnergyB_p_bb1agree_samedet_two -> Write();
	measured_EnergyT_m_bb1agree_samedet_two -> Write();
	measured_EnergyB_m_bb1agree_samedet_two -> Write();
	
	measured_EnergyT_p_bb1agree_oppdet_one -> Write();
	measured_EnergyB_p_bb1agree_oppdet_one -> Write();
	measured_EnergyT_m_bb1agree_oppdet_one -> Write();
	measured_EnergyB_m_bb1agree_oppdet_one -> Write();

	measured_EnergyT_p_bb1agree_oppdet_two -> Write();
	measured_EnergyB_p_bb1agree_oppdet_two -> Write();
	measured_EnergyT_m_bb1agree_oppdet_two -> Write();
	measured_EnergyB_m_bb1agree_oppdet_two -> Write();
	//
	measured_EnergyT_p_bb1_r155 -> Write();
	measured_EnergyB_p_bb1_r155 -> Write();
	measured_EnergyT_m_bb1_r155 -> Write();
	measured_EnergyB_m_bb1_r155 -> Write();

	measured_EnergyT_p_bb1_r155_one -> Write();
	measured_EnergyB_p_bb1_r155_one -> Write();
	measured_EnergyT_m_bb1_r155_one -> Write();
	measured_EnergyB_m_bb1_r155_one -> Write();

	measured_EnergyT_p_bb1_r155_two -> Write();
	measured_EnergyB_p_bb1_r155_two -> Write();
	measured_EnergyT_m_bb1_r155_two -> Write();
	measured_EnergyB_m_bb1_r155_two -> Write();

	measured_EnergyT_p_bb1_r155_samedet_one -> Write();
	measured_EnergyB_p_bb1_r155_samedet_one -> Write();
	measured_EnergyT_m_bb1_r155_samedet_one -> Write();
	measured_EnergyB_m_bb1_r155_samedet_one -> Write();
	
	measured_EnergyT_p_bb1_r155_samedet_two -> Write();
	measured_EnergyB_p_bb1_r155_samedet_two -> Write();
	measured_EnergyT_m_bb1_r155_samedet_two -> Write();
	measured_EnergyB_m_bb1_r155_samedet_two -> Write();
	
	measured_EnergyT_p_bb1_r155_oppdet_one -> Write();
	measured_EnergyB_p_bb1_r155_oppdet_one -> Write();
	measured_EnergyT_m_bb1_r155_oppdet_one -> Write();
	measured_EnergyB_m_bb1_r155_oppdet_one -> Write();

	measured_EnergyT_p_bb1_r155_oppdet_two -> Write();
	measured_EnergyB_p_bb1_r155_oppdet_two -> Write();
	measured_EnergyT_m_bb1_r155_oppdet_two -> Write();
	measured_EnergyB_m_bb1_r155_oppdet_two -> Write();
	
	//
	measured_EnergyT_p_bb1_r105 -> Write();
	measured_EnergyB_p_bb1_r105 -> Write();
	measured_EnergyT_m_bb1_r105 -> Write();
	measured_EnergyB_m_bb1_r105 -> Write();

	measured_EnergyT_p_bb1_r105_one -> Write();
	measured_EnergyB_p_bb1_r105_one -> Write();
	measured_EnergyT_m_bb1_r105_one -> Write();
	measured_EnergyB_m_bb1_r105_one -> Write();

	measured_EnergyT_p_bb1_r105_two -> Write();
	measured_EnergyB_p_bb1_r105_two -> Write();
	measured_EnergyT_m_bb1_r105_two -> Write();
	measured_EnergyB_m_bb1_r105_two -> Write();

	measured_EnergyT_p_bb1_r105_samedet_one -> Write();
	measured_EnergyB_p_bb1_r105_samedet_one -> Write();
	measured_EnergyT_m_bb1_r105_samedet_one -> Write();
	measured_EnergyB_m_bb1_r105_samedet_one -> Write();
	
	measured_EnergyT_p_bb1_r105_samedet_two -> Write();
	measured_EnergyB_p_bb1_r105_samedet_two -> Write();
	measured_EnergyT_m_bb1_r105_samedet_two -> Write();
	measured_EnergyB_m_bb1_r105_samedet_two -> Write();
	
	measured_EnergyT_p_bb1_r105_oppdet_one -> Write();
	measured_EnergyB_p_bb1_r105_oppdet_one -> Write();
	measured_EnergyT_m_bb1_r105_oppdet_one -> Write();
	measured_EnergyB_m_bb1_r105_oppdet_one -> Write();

	measured_EnergyT_p_bb1_r105_oppdet_two -> Write();
	measured_EnergyB_p_bb1_r105_oppdet_two -> Write();
	measured_EnergyT_m_bb1_r105_oppdet_two -> Write();
	measured_EnergyB_m_bb1_r105_oppdet_two -> Write();
//	cout << "Some are saved." << endl;

	costheta_v_costheta_p -> Write();
	costheta_v_costheta_m -> Write();
	
//	cout << "Now a few more." << endl;
	th2_num_Eout_v_Ein_tp -> Write();
	th2_num_Eout_v_Ein_tm -> Write();
	th2_num_Eout_v_Ein_bp -> Write();
	th2_num_Eout_v_Ein_bm -> Write();
//	cout << "Even more have been saved." << endl;
	
	/*
	th2_ana_Eout_v_Ein_tp -> Write();
	th2_ana_Eout_v_Ein_tm -> Write();
	th2_ana_Eout_v_Ein_bp -> Write();
	th2_ana_Eout_v_Ein_bm -> Write();
	*/
//	cout << "Histograms are done being saved.  We may or may not try to close the file now." << endl;
	
	//
	if(!leaveopen)
	{
		f -> Close();
	}
	//
	if(verbose)
	{
		cout << "MapSetup data has been cloned to file." << endl;
	}
}

void MapSetup::save_to_file(string filename_)
{
//	cout << "filename_ = " << filename_ << endl;
//	map_filename = filename_;
//	cout << "We'll create a new TFile now.." << endl;
	TFile *outfile = new TFile( (filename_).c_str(), "RECREATE");
//	cout << "TFile is recreated..." << endl;
	CloneToFile(outfile, 0);  // 0=not verbose.  'no argument'=close when finished.
//	cout << "Saved data to file:  " << filename_ << ", but map_filename=" << map_filename << endl;
	cout << "Saved data to file:  " << filename_ << endl;
}

void MapSetup::save_to_file()
{
	cout << "Saving to file:  " << map_filename << endl;
	save_to_file(map_filename);
}

TFile * MapSetup::RecreateAndLeaveOpen()  // it doesn't really update, it recreates.
{
	TFile *outfile = new TFile( (map_filename).c_str(), "RECREATE");
	
	cout << "Recreating (to update) file:  " << map_filename << endl;
	CloneToFile(outfile, 0, true);
	
	return outfile;  // can I do this?  I think it's bad c++ practice, but it might be ok with root.
}

