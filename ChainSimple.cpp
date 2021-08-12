#ifndef INCLUDE_CHAINSIMPLE
#define INCLUDE_CHAINSIMPLE 1

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#include "ChainG4.cpp"
/*
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

#include "location.cpp"
*/
//bool use_only_summed_forchain = true;


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
/*
extern string int_to_string(int);
extern string convertDouble(double);
*/
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// ====================================== // ====================================== //
// TChains for SimpleMC:

string get_simplefilename(string namestub, int runno, string path="/Users/anholm/Desktop/Anal-Ysis/Holstein/build/Output/")
{
	string fname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << path << namestub << "_" << runno;
	ss << ".root";
	
	fname = ss.str();
	
	return fname;
}
string get_simplefilename(string namestub, string path="/Users/anholm/Desktop/Anal-Ysis/Holstein/build/Output/")
{
	string fname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << path << namestub;// << "_" << runno;
	ss << ".root";
	
	fname = ss.str();
	
	return fname;
}

TChain * get_simplechain_from_runlist(vector<int> the_runlist, string namestub) 
{
	string path         = SimpleMC_path;
	TChain * tree_chain = new TChain("ntuple");
//	string namestub = "OutputB_BestParams_BSMM";
	
	int runno;
	string filename;
	
	int N_runs = the_runlist.size();
	for(int i=0; i<N_runs; i++)
	{
		runno = the_runlist.at(i);
		filename = get_simplefilename(namestub, runno, path);
		tree_chain -> Add(filename.c_str());
		//
		filename = string();
		filename.clear();
	}
	
	return tree_chain;
}


TChain * get_single_simplechain_from_namestub(string the_namestub) 
{
//	string filename = get_simplefilename(the_namestub, 1, SimpleMC_path);
	string filename = get_simplefilename(the_namestub, SimpleMC_path);
	
	TChain * tree_chain = new TChain("ntuple");
	tree_chain -> Add(filename.c_str());
	
	return tree_chain;
}


/*
TChain * get_simplechain_from_runlist(int argc, char* argv[], string namestub)
{
	vector<int> the_runlist = make_runlist_from_args(argc, argv);
	return get_simplechain_from_runlist(the_runlist, namestub);
}
*/

// ====================================== // ====================================== //
/*
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
*/
/*
TTree * load_metadata_tree(string metadatafilename)
{
	TTree *MetaTree = new TTree();
	MetaTree -> ReadFile(metadatafilename.c_str());
	return MetaTree;
}

TChain * get_single_simtree(int runno)
{
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
TChain * get_chain_from_runlist(TTree * MetaTree, vector<int> the_runlist) // this can break if the metadata has recorded more than one of the same run number.
{
	string path       = g4_path;
	string friendpath = g4f_path;

	int nentries = MetaTree -> GetEntries();

	int run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
	int this_neventsgenerated = 0;
	int this_neventssaved = 0;
	MetaTree -> SetBranchAddress("EventsGenerated", &this_neventsgenerated);
	MetaTree -> SetBranchAddress("EventsSaved",     &this_neventssaved);
	
	int total_events_generated = 0;
	int total_events_recorded = 0;

	TChain * tree_chain   = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	string filename;
	string friendname;
//	cout << "nentries = " << nentries << endl;
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		
		int run_index = check_runmatch(run, the_runlist);
		if( run_index != -1 )
		{
			cout << "Using run " << run << "  (i=" << i << "),\tN_gen=" << this_neventsgenerated << ",\tN_saved=" << this_neventssaved << endl;
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
	}
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}
TChain * get_chain_from_runlist(vector<int> the_runlist) // wrapper
{
	TTree *MetaTree = load_metadata_tree(metadata_name);
	return get_chain_from_runlist(MetaTree, the_runlist);
}
TChain * get_chain_from_runlist(int argc, char* argv[], TTree * MetaTree) // this can break if the metadata has recorded more than one of the same run number.
{
	vector<int> the_runlist = make_runlist_from_args(argc, argv);  // am I calling this correctly????
	return get_chain_from_runlist(MetaTree, the_runlist);
}
*/
/*
TChain * get_summed_simtree()
{
	vector<int> the_runlist = get_summed_simlist(false);
	TTree * MetaTree = load_metadata_tree(metadata_name);
	
	TChain * tree = get_chain_from_runlist(MetaTree, the_runlist);
	return tree;
}
*/
/*
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
	return fname;
}
*/
// ====================================== //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#endif