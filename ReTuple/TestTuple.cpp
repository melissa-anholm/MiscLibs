// ==================================================================== //
// Author:  Melissa Anholm, 2016
// ==================================================================== //
// This will look at an existing friendtuple.  
// Call with the run number to be used as the argument.  
// 
// This may not work quite the way I intended anymore...
// Instead, try this:
// 
// root [0] TFile *file = new TFile("/home/trinat/online/analyzedFiles_2014/output00411.root");
// root [1] TTree * tree = (TTree*)file->Get("ntuple");
// root [2] tree -> AddFriend("friendtuple", "/home/trinat/anholm/Friends/friend00411.root");
// 
// root [3] tree -> Print("all")
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

//string d_path = "/data/trinat/S1188_2014/";
string d_path = "/home/trinat/online/analyzedFiles_2014/";
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

TTree * Load_BlindTreeWithFriend(int runno)
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

TTree * Load_TreeWithFriend(int runno)
{
	cout << "Got here." << endl;
	string filename = make_rootfilename(d_path+"output00", runno);
	cout << "Got here too." << endl;
	string friendname = make_rootfilename(f_path+"friend00",runno);
	cout << "And here." << endl;

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


void TestTuple()
{
	main(411);  
}


//int main(int argc, char *argv[]) // compiled.
int main(int arg) // ROOTCINT
{
	int runno;
	/*
//	// Below:  use this if you're running as a compiled program.  
//	// otherwise, comment it out.
//	if(argc==2)
//	{
//		runno = atoi(argv[1]);
//	}
//	else
//	{
//		cout << "argc = " << argc << endl;
//		cout << "Must call with a run number." << endl;
//		return 0;
//	}
	*/
	// Below:  use this if you're running from within ROOTCINT.
	// otherwise, comment it out.
	runno = arg;
	
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

	// Backscatter Event Classification:
	/*
	Bool_t is_type1a_t = kFALSE;
	Bool_t is_type1a_b = kFALSE;
	TBranch *is_type1a_t_branch = friend_tree -> Branch("is_type1a_t", &is_type1a_t);  
	TBranch *is_type1a_b_branch = friend_tree -> Branch("is_type1a_b", &is_type1a_b);  
	Bool_t is_type1b_t = kFALSE;
	Bool_t is_type1b_b = kFALSE;
	TBranch *is_type1b_t_branch = friend_tree -> Branch("is_type1b_t", &is_type1b_t);  
	TBranch *is_type1b_b_branch = friend_tree -> Branch("is_type1b_b", &is_type1b_b);  
	Bool_t is_type1c = kFALSE;
	TBranch *is_type1c_branch = friend_tree -> Branch("is_type1c", &is_type1c);  
	Bool_t is_type2a_t = kFALSE;
	Bool_t is_type2a_b = kFALSE;
	TBranch *is_type2a_t_branch = friend_tree -> Branch("is_type2a_t", &is_type2a_t);  
	TBranch *is_type2a_b_branch = friend_tree -> Branch("is_type2a_b", &is_type2a_b);  
	Bool_t is_type2b_t = kFALSE;
	Bool_t is_type2b_b = kFALSE;
	TBranch *is_type2b_t_branch = friend_tree -> Branch("is_type2b_t", &is_type2b_t);  
	TBranch *is_type2b_b_branch = friend_tree -> Branch("is_type2b_b", &is_type2b_b);  
	Bool_t is_type3a_t = kFALSE;
	Bool_t is_type3a_b = kFALSE;
	TBranch *is_type3a_t_branch = friend_tree -> Branch("is_type3a_t", &is_type3a_t);  
	TBranch *is_type3a_b_branch = friend_tree -> Branch("is_type3a_b", &is_type3a_b);  
	Bool_t is_type3b_t = kFALSE;
	Bool_t is_type3b_b = kFALSE;
	TBranch *is_type3b_t_branch = friend_tree -> Branch("is_type3b_t", &is_type3b_t);  
	TBranch *is_type3b_b_branch = friend_tree -> Branch("is_type3b_b", &is_type3b_b);  
	Bool_t is_type4_t = kFALSE;
	Bool_t is_type4_b = kFALSE;
	*/
	
	Bool_t is_normal_t = kFALSE;
	Bool_t is_normal_b = kFALSE;
	tree -> SetBranchAddress("is_normal_t", &is_normal_t);  
	tree -> SetBranchAddress("is_normal_b", &is_normal_b);  
	Bool_t is_other = kFALSE;
	tree -> SetBranchAddress("is_other", &is_other);  

	int N_hits_scint_t = 0;
	int N_hits_scint_b = 0;
	tree -> SetBranchAddress("N_hits_scint_t", &N_hits_scint_t);
	tree -> SetBranchAddress("N_hits_scint_b", &N_hits_scint_b);
	int N_hits_bb1_t = 0;
	int N_hits_bb1_b = 0;
	tree -> SetBranchAddress("N_hits_bb1_t", &N_hits_bb1_t);
	tree -> SetBranchAddress("N_hits_bb1_b", &N_hits_bb1_b);



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
		
		// backscatter:
		if(N_hits_scint_t>0 || N_hits_scint_b>0)
		{
			cout << "N_PMT_T=" << N_hits_scint_t  << ";  N_PMT_B=" << N_hits_scint_b;
			cout << ";  N_BB1_T=" << N_hits_bb1_t << ";  N_BB1_B=" << N_hits_bb1_b;
			cout << endl;
			cout << "\tis_normal_t=" << is_normal_t << ";  is_normal_b=" << is_normal_b;
			cout << ";  is_other=" << is_other;
			cout << endl;
		}
	}
	
	cout << "Done." << endl;
	return 0;
}

