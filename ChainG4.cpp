#ifndef INCLUDE_CHAING4
#define INCLUDE_CHAING4 1

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using std::cout;
using std::endl;
using std::vector;
using std::string;

#include "TFile.h"
#include <TTree.h>
#include <TChain.h>
#include "TNtuple.h"
#include "TBranch.h"  // do I need this?

//#include "treeql_replacement.cpp"
#include "location.cpp"

bool use_only_summed_forchain = true;

/*
//#define metachain_on_trinatdaq 1

//#define XSTR(x) #x
//#define STR(x) XSTR(x)

#ifdef on_trinatdaq  // define this (or not) in whatever code calls this.  
	string br_path = "/data/trinat/S1188_2014_blinded/";
	string be_path = "/data/trinat/S1188_2014_blinded/";
	string bf_path = "/home/trinat/anholm/Friends/";  // BAD!!

	string ur_path = "/home/trinat/online/analyzedFiles_2014/";
	string ue_path = "/home/trinat/online/analyzedFiles_2014/";
	string uf_path = "/home/trinat/anholm/Friends/";  

	string g4_path  = "/home/trinat/anholm/G4_Output/";
	string g4f_path = "/home/trinat/anholm/G4_Output/";
	string metadata_name = "/home/trinat/anholm/G4_Output/MetaData.txt";
#else
	string br_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Recoils_2014/";
	string be_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
	string bf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Friends_2014/";

	string ur_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Recoils_2014/";
	string ue_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Electrons_2014/";
	string uf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Friends_2014/";

	string g4_path  = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";
	string g4f_path = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/Friends/";
	string metadata_name = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/MetaData.txt";
#endif

string g4f_path = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/Friends/";
string metadata_name = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/MetaData.txt";
#endif
*/

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

// ====================================== // ====================================== //
// Newer TChains for Simulations:
string get_simfilename(string path, int runno)  // don't use this.
{	
	string fname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << path << "output_" << runno;
	ss << ".root";
	
	fname = ss.str();
	
	return fname;
}

string get_simfilename(TTree * MetaTree, int runno)  // from the MetaTree.
{	
	string namestub = "";
	string fname;
	
	int this_run = 0;
	MetaTree -> SetBranchAddress("Run", &this_run);
	char this_filename[256];
	MetaTree -> SetBranchAddress("Filename", &this_filename);

	int nentries = MetaTree -> GetEntries();
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if(runno==this_run)
		{
			namestub = string(this_filename);
			break;
		}
	}
	fname = g4_path + namestub;
	if(namestub!=string(""))
	{
		return fname;
	}
	else
	{
		cout << "ERROR:  Couldn't find run number " << runno << " in the metadata." << endl;
		return namestub;
	}
}

string get_matched_runletter(TTree * MetaTree, int runno)  //
{
	string this_set = "";
	
	int this_run = 0;
	MetaTree -> SetBranchAddress("Run", &this_run);
	char this_matchedset[256];
	MetaTree -> SetBranchAddress("matches_runset", &this_matchedset);
	
	int nentries = MetaTree -> GetEntries();
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if(runno==this_run)
		{
			this_set = string(this_matchedset);
			break;
		}
	}
	//
	if(this_set==string(""))
	{
		cout << "ERROR:  Couldn't find run number " << runno << " in the metadata." << endl;
	}
	return this_set;
}


string get_simfriendname(string path, int runno)
{	
	string fname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << path << "friend_" << runno;
	ss << ".root";

	fname = ss.str();
	
	return fname;
}

TTree * load_metadata_tree(string metadatafilename)
{
	TTree *MetaTree = new TTree();
	int nentries = MetaTree -> ReadFile(metadatafilename.c_str());
	return MetaTree;
}

TChain * get_single_simtree(int runno)
{
//	string metadatafilename = metadata_name; //g4_path + metadata_namestub;
	TTree *MetaTree = load_metadata_tree(metadata_name);
	
	string filename = get_simfilename(MetaTree, runno);
	string friendname = get_simfriendname(g4f_path, runno);
	
	TChain * tree_chain   = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	tree_chain -> Add(filename.c_str());
	friend_chain -> Add(friendname.c_str());
	
	tree_chain -> AddFriend(friend_chain);
	delete MetaTree;
	return tree_chain;
}


vector<int> get_runlist_from_rho(TTree * MetaTree, double rho, int maxrun=0)
{
	vector<int> set_of_runs;
	
	int run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
	double this_rho = 0.0;
	MetaTree -> SetBranchAddress("Rho", &this_rho);
	int has_been_summed = 0;
	MetaTree -> SetBranchAddress("has_been_summed", &has_been_summed);
	int is_a_sum = 0;
	MetaTree -> SetBranchAddress("is_a_sum", &is_a_sum);

//	use_only_summed_forchain
	
	int nentries = MetaTree -> GetEntries();
	if(maxrun != 0)
	{ 
		nentries = maxrun+1; 
	}
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if(this_rho == rho)
		{
			if(has_been_summed==0)
			{
				if( is_a_sum==1 || !use_only_summed_forchain )
				{
					set_of_runs.push_back(run);
					cout << "adding " << run << " to the runlist vector." << endl;
				}
				else if(is_a_sum==0)
				{
					cout << "skipping " << run << ", since we're only looking at summed runs." << endl;
				}
			}
			else
			{
				cout << "skipping run " << run << " because it's already been put into a sum." << endl;
			}
		}
	}

	return set_of_runs;
}


TChain * get_chain_from_rho(TTree * MetaTree, double rho, int maxrun=0)
{
	cout << "rho = " << rho << endl;
	string path       = g4_path;
	string friendpath = g4f_path;
	
	int nentries = MetaTree -> GetEntries();
	if(maxrun != 0)
	{ 
		nentries = maxrun+1; 
	}
	
	int run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
	double this_rho = 0.0;
	MetaTree -> SetBranchAddress("Rho", &this_rho);
	int this_neventsgenerated = 0;
	int this_neventssaved = 0;
	MetaTree -> SetBranchAddress("EventsGenerated", &this_neventsgenerated);
	MetaTree -> SetBranchAddress("EventsSaved",     &this_neventssaved);
	int has_been_summed = 0;
	MetaTree -> SetBranchAddress("has_been_summed",  &has_been_summed);
	int is_a_sum = 0;
	MetaTree -> SetBranchAddress("is_a_sum", &is_a_sum);

	int total_events_generated = 0;
	int total_events_recorded = 0;

	TChain * tree_chain   = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	string filename;
	string friendname;
	cout << "nentries = " << nentries << endl;
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if(this_rho == rho /* && has_been_summed==0 */ )
		{
			if(has_been_summed==0)
			{
				if(is_a_sum==1 || !use_only_summed_forchain)
				{
					cout << "Using run " << run << "  (i=" << i << "),\trho=" << this_rho << "\tN_gen=" << this_neventsgenerated << ",\tN_saved=" << this_neventssaved << endl;
					total_events_generated = total_events_generated + this_neventsgenerated;
					total_events_recorded = total_events_recorded + this_neventssaved;
				
					filename   = get_simfilename( (TChain*)MetaTree->Clone(), run);
					friendname = get_simfriendname(friendpath, run);  // check if friend exists???
					tree_chain -> Add(filename.c_str());
					friend_chain -> Add(friendname.c_str());
		
					filename = string();
					filename.clear();
					friendname = string();
					friendname.clear();
				}
				else if(is_a_sum==0)
				{
					cout << "skipping " << run << "  (i=" << i << ",\trho=" << this_rho <<"), since we're only looking at summed runs." << endl;
				}
			}
			else
			{
				cout << "* Skipping run " << run << "  (i=" << i << ",\trho=" << this_rho <<"), because it has already been put into a sum." << endl;
			}
		}
	}
	
	cout << "rho=" << rho << ", N_generated=" << total_events_generated << ", \tN_saved=" << total_events_recorded << endl;
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}


// ====================================== //
// TChains for Simulations (do I ever even use these?):

string make_simfilename(string namestub, int runno)  // no, don't..
{
	string fname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << g4_path << namestub << runno << ".root";
	fname = ss.str();
//	cout << "created simfilename:  " << fname << endl;
//	cout << "see, because g4_path=" << g4_path << ", namestub=" << namestub << ", runno=" << runno << endl;
	return fname;
}



/*
TTree * MetaTuple::LoadMetaData()
{
	TTree *MetaTree = new TTree();
//	int nrows = toftree -> ReadFile(metadatafilename.c_str());
	nentries = MetaTree -> ReadFile(metadatafilename.c_str());
	return MetaTree;
}
*/
// Load up the metadata as an ntuple.


// ====================================== //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

void printout_list(double rho)
{
	TTree * MetaTree = load_metadata_tree(metadata_name);
	vector<int> list = get_runlist_from_rho(MetaTree, rho);
	
	int length = list.size();
	for(int i=0; i<length; i++)
	{
		cout << list.at(i) << endl;
	}
	return;
}

void printout(vector<int> list)
{
	int length = list.size();
	for(int i=0; i<length; i++)
	{
		cout << list.at(i) << endl;
	}
	return;
}

void printout(vector<double> list)
{
	int length = list.size();
	for(int i=0; i<length; i++)
	{
		cout << list.at(i) << endl;
	}
	return;
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#endif