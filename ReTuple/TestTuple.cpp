// ==================================================================== //
// Author:  Melissa Anholm, 2016
// ==================================================================== //
// This will look at an existing friendtuple.  
// Call with the run number to be used as the argument.  
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
string d_path = "/data/trinat/S1188_2014/";
string f_path = "/home/trinat/anholm/Friends/";

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

TTree * Load_TreeWithFriend(int runno)
{
	string filename = make_rootfilename(d_path+"output00", runno, "_blinded");
	string friendname = make_rootfilename(f_path+"friend00",runno, "_blinded");

	TFile *file = new TFile(filename.c_str(), "READ");
	if(!(file->IsOpen()))
		{ cout << "Error:  File " << filename << " can't be opened." << endl; }
	TFile *friendfile = new TFile(friendname.c_str(), "READ");
	if(!(friendfile->IsOpen()))
		{ cout << "Error:  File " << friendname << " can't be opened." << endl; }

	TTree *tree = new TTree;
	tree = (TTree*)file->Get("ntuple");
	tree -> AddFriend("friendtuple", friendname.c_str());

	cout << "* Using run " << runno << endl;
	return tree;
}

//
int main(int argc, char *argv[]) 
{
	int runno;
	if(argc==2)
	{
		runno = atoi(argv[1]);
	}
	else
	{
		cout << "Must call with a run number." << endl;
		return 0;
	}
		
	TTree * tree = new TTree;
	tree = Load_TreeWithFriend(runno);
	
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
	vector<double> *dl_x_pos = 0;
	vector<double> *dl_z_pos = 0;
//	tree -> SetBranchAddress("dl_x_pos", &dl_x_pos);  // owned by friendtuple.
//	tree -> SetBranchAddress("dl_z_pos", &dl_z_pos);  // owned by friendtuple.
	Bool_t is_polarized;
	tree -> SetBranchAddress("is_polarized", &is_polarized);  // owned by friendtuple.

	int ion_count = 0;
	int x1_count = 0;
	int x2_count = 0;
	int z1_count = 0;
	int z2_count = 0;
	
	int x_count = 0;
	int z_count = 0;
		
	Long64_t nentries = tree->GetEntries();
	cout << "nentries = " << nentries << endl;	
	for(int i=0; i<nentries; i++)
	{
		if( (i % 100000) == 0) { cout<<"Reached entry "<< i << endl; }
		tree -> GetEntry(i);
		
		ion_count = ion_events->size();
		x1_count = x1_dla->size();
		x2_count = x2_dla->size();
		z1_count = z1_dla->size();
		z2_count = z2_dla->size();
		
		// Now do stuff with the tree.
		if(ion_count>0 && x1_count>0 && x2_count>0 && z1_count>0 && z2_count>0)
		{
			cout << "x1=" << (*x1_dla)[0] << ";  is_polarized=" << is_polarized << endl;
		}
	}
	
	cout << "Done." << endl;
	return 0;
}