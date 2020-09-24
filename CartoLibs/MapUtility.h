#ifndef INCLUDE_MAPUTILITY
#define INCLUDE_MAPUTILITY 1


// ==================================================================== //
// Code by Melissa Anholm
// 
// ==================================================================== //

#include <vector>
#include <cmath>
#include <sstream> 
#include <iostream>  // cout

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;

#include <TH1.h>
#include <TChain.h>
#include <TFile.h>
#include "TStyle.h"
//#include "FitUtility.cpp"

#include "/Users/anholm/Packages/MiscLibs/HistExtras.h" 
#include "/Users/anholm/Packages/MiscLibs/ColorExtras.h"


#undef NDEBUG
#include<assert.h>


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
extern double get_costheta(double rhit, double zhit);
extern string make_mapname_from_monoenergy(string namestub, double monoenergy);
extern string make_mapnamekludge_from_monoenergy(string namestub, double monoenergy);
extern string make_mapname_from_monoenergy(double monoenergy);
extern string make_fullspectrum_name(string namestub);
extern string make_mapnamekludge_from_monoenergy(double monoenergy);
extern string int_to_string(int the_int);


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
class MapSetup
{
public:
	MapSetup() 
		{ filename = "map_out.root"; };
	~MapSetup();
	
	void LoadFromTree(TChain * the_tree, int N_rebin_hists=1);
	void LoadSimpleFromTree(TChain * the_tree, int N_rebin_hists=1);
	
	void LoadFromFile(string filename_);
	void AdjustTheColors();
	void save_to_file(string filename_);
	void save_to_file();
	void CloneToFile(TFile * the_file, int verbose=1, bool leaveopen=false);
	TFile * RecreateAndLeaveOpen();  // it doesn't really update, it recreates.
	
	//
	TH1D* naive_EnergyT_p_hist;
	TH1D* naive_EnergyB_p_hist;
	TH1D* naive_EnergyT_m_hist;
	TH1D* naive_EnergyB_m_hist;
	
	TH1D* naive_EnergyT_p_hist_rcut155;
	TH1D* naive_EnergyB_p_hist_rcut155;
	TH1D* naive_EnergyT_m_hist_rcut155;
	TH1D* naive_EnergyB_m_hist_rcut155;
	
	//
	TH1D* measured_EnergyT_p_hist;
	TH1D* measured_EnergyB_p_hist;
	TH1D* measured_EnergyT_m_hist;
	TH1D* measured_EnergyB_m_hist;

	TH1D* measured_EnergyT_p_hist_samedet;
	TH1D* measured_EnergyB_p_hist_samedet;
	TH1D* measured_EnergyT_m_hist_samedet;
	TH1D* measured_EnergyB_m_hist_samedet;
	
	TH1D* measured_EnergyT_p_hist_oppdet;
	TH1D* measured_EnergyB_p_hist_oppdet;
	TH1D* measured_EnergyT_m_hist_oppdet;
	TH1D* measured_EnergyB_m_hist_oppdet;
	
	//
	TH1D* measured_EnergyT_p_bb1agree;
	TH1D* measured_EnergyB_p_bb1agree;
	TH1D* measured_EnergyT_m_bb1agree;
	TH1D* measured_EnergyB_m_bb1agree;
	
	TH1D* measured_EnergyT_p_bb1agree_one;
	TH1D* measured_EnergyB_p_bb1agree_one;
	TH1D* measured_EnergyT_m_bb1agree_one;
	TH1D* measured_EnergyB_m_bb1agree_one;

	TH1D* measured_EnergyT_p_bb1agree_two;
	TH1D* measured_EnergyB_p_bb1agree_two;
	TH1D* measured_EnergyT_m_bb1agree_two;
	TH1D* measured_EnergyB_m_bb1agree_two;

	TH1D* measured_EnergyT_p_bb1agree_samedet_one;
	TH1D* measured_EnergyB_p_bb1agree_samedet_one;
	TH1D* measured_EnergyT_m_bb1agree_samedet_one;
	TH1D* measured_EnergyB_m_bb1agree_samedet_one;
	
	TH1D* measured_EnergyT_p_bb1agree_samedet_two;
	TH1D* measured_EnergyB_p_bb1agree_samedet_two;
	TH1D* measured_EnergyT_m_bb1agree_samedet_two;
	TH1D* measured_EnergyB_m_bb1agree_samedet_two;
	
	TH1D* measured_EnergyT_p_bb1agree_oppdet_one;
	TH1D* measured_EnergyB_p_bb1agree_oppdet_one;
	TH1D* measured_EnergyT_m_bb1agree_oppdet_one;
	TH1D* measured_EnergyB_m_bb1agree_oppdet_one;
	
	TH1D* measured_EnergyT_p_bb1agree_oppdet_two;
	TH1D* measured_EnergyB_p_bb1agree_oppdet_two;
	TH1D* measured_EnergyT_m_bb1agree_oppdet_two;
	TH1D* measured_EnergyB_m_bb1agree_oppdet_two;
	
	//
	TH1D* measured_EnergyT_p_bb1_r155;
	TH1D* measured_EnergyB_p_bb1_r155;
	TH1D* measured_EnergyT_m_bb1_r155;
	TH1D* measured_EnergyB_m_bb1_r155;
	
	TH1D* measured_EnergyT_p_bb1_r155_one;
	TH1D* measured_EnergyB_p_bb1_r155_one;
	TH1D* measured_EnergyT_m_bb1_r155_one;
	TH1D* measured_EnergyB_m_bb1_r155_one;

	TH1D* measured_EnergyT_p_bb1_r155_two;
	TH1D* measured_EnergyB_p_bb1_r155_two;
	TH1D* measured_EnergyT_m_bb1_r155_two;
	TH1D* measured_EnergyB_m_bb1_r155_two;
	
	TH1D* measured_EnergyT_p_bb1_r155_samedet_one;  // v2
	TH1D* measured_EnergyB_p_bb1_r155_samedet_one;  // v2
	TH1D* measured_EnergyT_m_bb1_r155_samedet_one;  // v2
	TH1D* measured_EnergyB_m_bb1_r155_samedet_one;  // v2
	
	TH1D* measured_EnergyT_p_bb1_r155_samedet_two;
	TH1D* measured_EnergyB_p_bb1_r155_samedet_two;
	TH1D* measured_EnergyT_m_bb1_r155_samedet_two;
	TH1D* measured_EnergyB_m_bb1_r155_samedet_two;
	
	TH1D* measured_EnergyT_p_bb1_r155_oppdet_one;
	TH1D* measured_EnergyB_p_bb1_r155_oppdet_one;
	TH1D* measured_EnergyT_m_bb1_r155_oppdet_one;
	TH1D* measured_EnergyB_m_bb1_r155_oppdet_one;

	TH1D* measured_EnergyT_p_bb1_r155_oppdet_two;
	TH1D* measured_EnergyB_p_bb1_r155_oppdet_two;
	TH1D* measured_EnergyT_m_bb1_r155_oppdet_two;
	TH1D* measured_EnergyB_m_bb1_r155_oppdet_two;
	
	//
	TH1D* measured_EnergyT_p_bb1_r105;
	TH1D* measured_EnergyB_p_bb1_r105;
	TH1D* measured_EnergyT_m_bb1_r105;
	TH1D* measured_EnergyB_m_bb1_r105;
	
	TH1D* measured_EnergyT_p_bb1_r105_one;
	TH1D* measured_EnergyB_p_bb1_r105_one;
	TH1D* measured_EnergyT_m_bb1_r105_one;
	TH1D* measured_EnergyB_m_bb1_r105_one;
	
	TH1D* measured_EnergyT_p_bb1_r105_two;
	TH1D* measured_EnergyB_p_bb1_r105_two;
	TH1D* measured_EnergyT_m_bb1_r105_two;
	TH1D* measured_EnergyB_m_bb1_r105_two;
	
	TH1D* measured_EnergyT_p_bb1_r105_samedet_one;
	TH1D* measured_EnergyB_p_bb1_r105_samedet_one;
	TH1D* measured_EnergyT_m_bb1_r105_samedet_one;
	TH1D* measured_EnergyB_m_bb1_r105_samedet_one;
	
	TH1D* measured_EnergyT_p_bb1_r105_samedet_two;
	TH1D* measured_EnergyB_p_bb1_r105_samedet_two;
	TH1D* measured_EnergyT_m_bb1_r105_samedet_two;
	TH1D* measured_EnergyB_m_bb1_r105_samedet_two;
	
	TH1D* measured_EnergyT_p_bb1_r105_oppdet_one;
	TH1D* measured_EnergyB_p_bb1_r105_oppdet_one;
	TH1D* measured_EnergyT_m_bb1_r105_oppdet_one;
	TH1D* measured_EnergyB_m_bb1_r105_oppdet_one;

	TH1D* measured_EnergyT_p_bb1_r105_oppdet_two;
	TH1D* measured_EnergyB_p_bb1_r105_oppdet_two;
	TH1D* measured_EnergyT_m_bb1_r105_oppdet_two;
	TH1D* measured_EnergyB_m_bb1_r105_oppdet_two;

	
	TH2D* costheta_v_costheta_p;
	TH2D* costheta_v_costheta_m;
	
private:
	string filename;
};


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#endif
