// ==================================================================== //
// Author:  Melissa Anholm, 2016
// ==================================================================== //
// This will add the imcp calibration branches to a friendtuple file.
// Call with a run number as an argument.  eg:
//    ./calibrationator 377
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

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::make_pair;
using std::pair;
using std::min;

//#include "/Users/spiffyzha/Packages/MiscLibs/treeql_replacement.cpp"
#include "/home/trinat/anholm/Library/treeql_replacement.cpp"
#include "mini_cal_maker.cpp"

string d_path = "/data/trinat/S1188_2014/";
string o_path = "/home/trinat/anholm/Friends/";


string make_rootfilename(string name, int parameter, string name2=string(""))
{
	string rootfname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << name << parameter << name2 <<".root";
	rootfname = ss.str();
	return rootfname;
}

//
int main(int argc, char *argv[]) 
{
	int runno;
	string fname;
	string friend_fname;
	
	if(argc==2)
	{
		runno = atoi(argv[1]);
	}
	else
	{
		cout << "Must call with a run number." << endl;
		return 0;
	}
	
	fname  = make_rootfilename(d_path+"output00", runno, "_blinded");
	friend_fname = make_rootfilename(o_path+"friend00",runno, "_blinded");

	// Original Tree:
	TFile *f = new TFile(fname.c_str(), "READ");
	if(!(f->IsOpen()))
	{
		cout << "File can't be opened." << endl;
		return 0;
	}
	TTree *tree = new TTree;
	tree = (TTree*)f->Get("ntuple");

	// Friend Tree:
	TFile *friendfile = new TFile(friend_fname.c_str(), "RECREATE");
	TTree *friend_tree = new TTree("friendtuple", "friendtuple");
	
	vector<double> *ion_events = 0;
	vector<double> *x1_dla = 0;
	vector<double> *x2_dla = 0;
	vector<double> *z1_dla = 0;
	vector<double> *z2_dla = 0;
	tree -> SetBranchAddress("TDC_ION_MCP", &ion_events);
	tree -> SetBranchAddress("TDC_DL_X1",&x1_dla);
	tree -> SetBranchAddress("TDC_DL_X2",&x2_dla);
	tree -> SetBranchAddress("TDC_DL_Z1",&z1_dla);
	tree -> SetBranchAddress("TDC_DL_Z2",&z2_dla);
	int ion_count = 0;
	int x1_count = 0;
	int x2_count = 0;
	int z1_count = 0;
	int z2_count = 0;
	
	vector<double> *dl_x_pos = 0;
	vector<double> *dl_z_pos = 0;
	TBranch *x_branch = friend_tree -> Branch("dl_x_pos", &dl_x_pos);
	TBranch *z_branch = friend_tree -> Branch("dl_z_pos", &dl_z_pos);
	dl_x_pos -> clear();
	dl_z_pos -> clear();

	int nhits = 0;
	calibration * my_cals = new calibration(runno); 
	pair<double, double> coordinates = make_pair(0.0, 0.0);
	
	Long64_t nentries = tree->GetEntries();
	cout << "nentries = " << nentries << endl;
	for(int i=0; i<nentries; i++)
	{
		if( (i % 100000) == 0) 
			{ cout<<"Reached entry "<< i << endl; }
		tree -> GetEntry(i);
		
		ion_count = ion_events->size();
		x1_count = x1_dla->size();
		x2_count = x2_dla->size();
		z1_count = z1_dla->size();
		z2_count = z2_dla->size();
		
		nhits = min(ion_count, min(min(x1_count, x2_count), min(z1_count, z2_count)));
		for(int j=0; j<nhits; j++)
		{
			coordinates = my_cals -> 
				apply_calibration((*x1_dla)[0], (*x2_dla)[0], (*z1_dla)[0], (*z2_dla)[0], 6);
				
			dl_x_pos -> push_back(coordinates.first);
			dl_z_pos -> push_back(coordinates.second);
		}
		friend_tree -> Fill();
		dl_x_pos -> clear();
		dl_z_pos -> clear();
	}
	
	friend_tree -> GetCurrentFile() -> Write("",TObject::kOverwrite);  
	friend_tree -> GetCurrentFile() -> Close();
	
	cout << "Done." << endl;
	return 0;
}