// ==================================================================== //
// Author:  Melissa Anholm, 2016 - 
// 
// ==================================================================== //
// 
// This will add branches that need to be added to a friendtuple file.
// (original) all_okay, accyclecount, is_pol, is_unpol, is_ac.
// 
// updated to v2, 3.May.2016.
// use 100 mus time to polarize and new scint.E calibrations 
// (upper_scint_E, lower_scint_E).  also, upper_scint_E_res, 
// lower_scint_E_res.
// also-also, dl_x_pos, dl_z_pos.
// 
// 22.8.2017:  Not sure what happened to version 4, but version 5
// features include a one-parameter *thing* to switch between my 
// own computer and trinatdaq, as well as the functionality to 
// properly friendtuple the G4 data.  
// 
// 14.11.2017:  version 6 includes BB1 hit data, which was actually 
// already there in version 5.  New this version is backscatter 
// event classification.
// 
// 11.12.2017:  version 7: use overall BB1 threshold too!  60 keV.
// 
// 26.4.2018:  version 8:  added scintillator resolution 'blur' to g4 
// output.  At some point during version 7, backscatter classifications 
// were also added.  Also at some point the setup_location() function
// was added specifically to manage the clusterfuck that arose from 
// trying to use this code from multiple different computers.
// 
// 12.6.2018:  version 9:  added BB1 detector noise and resolution to 
// g4 data processing.
// 
// 5.11.2018:  version 10:  added dl_x_pos and dl_z_pos branches to G4 output.  
// previously the branches were there, but zero, and G4 position info
// was (also still is) stored in only the DL_X_Pos (etc) branches.  
// Also added at version 10:  partial LE/TE support.  Breaks retupling on 
// files analyzed before the LE/TE analyzer, but LE/TE doesn't crash it.
// 
// still needs to just add all the "orig_LE" branch names back as "orig".
// 
// 14.8.2020:  version 12:  re-did the hex position stuff into a new set 
// of branches, now using the LE data to calculate position.  
// Updated the scintillator walk adjustments to implement a second walkfit 
// adjustment based on the runset.  We also return to using Melissa's 
// polarization check convention after having used Ben's for a while.
// Melissa's probably salvages slightly more data.
// ==================================================================== //

#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream> 
#include <utility>

#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>
#include <TObjString.h>
#include <TRandom3.h>
#include <TF1.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::make_pair;  // unused?
using std::pair;
using std::min;

//
//bool is_blinded            = false;
//bool is_g4                 = true;
//bool use_g4_metadata       = true;
//bool is_old                = false;  // before trailing edge/leading edge madness.

int version = 0;

#include "location.cpp"
#include "MetaChain.cpp"
#include "treeql_replacement.cpp"
#include "BB1/bb1_strip.h"
#include "mini_cal_maker.cpp"
#include "HistExtras.cpp" // v1192_to_ns.

#include "ReHexPos.cpp"

string make_rootfilename(string name, int parameter, string name2=string(""))
{
	string rootfname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << name << parameter << name2 << ".root";
	rootfname = ss.str();
	return rootfname;
}
string int_to_string(int thisint)
{
	string outstring;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	ss << thisint;
	outstring = ss.str();
	return outstring;
}
TObjString * make_tstring(string descriptor, int value)
{
	string tmp_string = descriptor+int_to_string(value);
	TObjString * string_obj = new TObjString(tmp_string.c_str());
	return string_obj;
}

//
int main(int argc, char *argv[]) 
{
	setup_location();
	
	int runno;
	if(argc==2)
	{
		runno = atoi(argv[1]);
		cout << "Using run:  " << runno << endl;
	}
	else
	{
		cout << "Must call with a run number." << endl;
		return 0;
	}
	
	
	vector<int> set_of_runs;
	set_of_runs.push_back(runno);
	TChain * tree = get_chain_from_runlist(set_of_runs);
	
	TObjString * version_string = make_tstring(string("SOEtuple version "), version);
	
	string fname             = make_rootfilename(g4_path+"output_", runno);
	string friend_fname      = make_rootfilename(g4f_path+"friend_", runno);
	string otherfriend_fname = make_rootfilename(g4f_path+"soefriend_", runno);
	
	string soetextpath = "/Users/anholm/Desktop/Anal-Ysis/Save_Ntuple_to_text/FromAlexandre/";
	string mini_fname;
	if(runno==10016)  // Hoop 4 Face:
	{
		mini_fname    = "H4Face_10016_TOF.txt";  //
	}
	else if(runno==10018)
	{
		mini_fname    = "H4Face_10018_TOF.txt";  //  New!
	}
	else if(runno==10020)
	{
		mini_fname    = "H4Face_10020_TOF.txt";  //  New!
	}
	//
	else if(runno==10017)  // Hoop 3 Face:
	{
		mini_fname    = "H3Face_10017_TOF.txt";  //
	}
	else if(runno==10019)
	{
		mini_fname    = "H3Face_10019_TOF.txt";  //  New!
	}
	else if(runno==10021)
	{
		mini_fname    = "H3Face_10021_TOF.txt";  //  New!
	}
	//
	else if(runno==10022)  // Hoop 4 Edge:
	{
		mini_fname    = "H4Edge_10022_TOF.txt";
	}
	else if(runno==10023)  // Hoop 3 Edge:
	{
		mini_fname    = "H3Edge_10023_TOF.txt";
	}
	else if(runno==10024)  // Hoop 1 Face:
	{
		mini_fname    = "H1Face_10024_TOF.txt";
	}
	else if(runno==10025)  // Hoop 2 Face:
	{
		mini_fname    = "H2Face_10025_TOF.txt";
	}
	else if(runno==10026)  // rMCP Face:
	{
		mini_fname    = "rMCP_10026_TOF.txt";
	}
	else if(runno==10027)  //
	{
		mini_fname    = "H1Edge_10027_TOF.txt";
	}
	else if(runno==10028)  //
	{
		mini_fname    = "H2Edge_10028_TOF.txt";
	}
	else if(runno==10029)  //
	{
		mini_fname    = "CRFace_10029_TOF.txt";
	}
	else if(runno==10030)  //
	{
		mini_fname    = "CREdge_10030_TOF.txt";
	}
	else
	{
		cout << "No." << endl;
		return 0;
	}
	string texttree_fname = soetextpath + mini_fname;
	
	TFile *other_friendfile = new TFile(otherfriend_fname.c_str(), "RECREATE");
	TTree *other_friendtree = new TTree("soetuple", "soetuple");

	TTree *texttree = new TTree();
	texttree -> ReadFile(texttree_fname.c_str() );

	// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
	Double_t soe_toa;
	TBranch *soe_toa_branch = other_friendtree -> Branch("SOE_TOA", &soe_toa);
	Double_t emcp_x;
	Double_t emcp_z;
	TBranch *soe_x_branch = other_friendtree -> Branch("emcp_x", &emcp_x);
	TBranch *soe_z_branch = other_friendtree -> Branch("emcp_z", &emcp_z);
	
	
	// --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // 
	Int_t this_entrynumber;
	texttree -> SetBranchAddress("Entry", &this_entrynumber);
	
	Double_t x_final_mm;
	texttree -> SetBranchAddress("x_final_mm", &x_final_mm);
	Double_t y_final_mm;
	texttree -> SetBranchAddress("y_final_mm", &y_final_mm);
	Double_t z_final_mm;
	texttree -> SetBranchAddress("z_final_mm", &z_final_mm);
	
	Double_t tof_ns;
	texttree -> SetBranchAddress("tof_ns", &tof_ns);
	
	
	// --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // --*-- // 
	// ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // 
	// ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // 
	
	Long64_t nentries_texttree = texttree->GetEntries();
	Long64_t nentries          = tree->GetEntries();
	cout << "nentries = "          << nentries << endl;
	cout << "nentries_texttree = " << nentries_texttree << endl;
	
	// ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // ---- // 
	int i = 0;
	bool done = false;
	
	for(int j=0; j<nentries_texttree; j++)
	{
		texttree -> GetEntry(j);
		done = false;
		while( i<nentries && !done )
		{
			if( i==this_entrynumber )
			{
				if( y_final_mm==-101.800)
				{
					soe_toa = tof_ns / v1192_to_ns;
					emcp_x = 1.0*x_final_mm;
					emcp_z = 1.0*z_final_mm;
				}
				else
				{
					soe_toa = -1.0;
					emcp_x = -50.0;
					emcp_z = -50.0;
				}
				done=true;
			}
			else
			{
				soe_toa = -1.0;
				emcp_x = -50.0;
				emcp_z = -50.0;
			}
			
			i++;
			other_friendtree -> Fill();
		}
	}
	
	
	other_friendtree -> GetCurrentFile() -> Write("",TObject::kOverwrite);  
	other_friendfile -> cd();
	version_string -> Write();
	other_friendtree -> GetCurrentFile() -> Close();
	
	cout << "Done." << endl;
	return 0;
}