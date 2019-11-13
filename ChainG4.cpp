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
//#define XSTR(x) #x
//#define STR(x) XSTR(x)
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


vector<int> get_runlist_from_rho(TTree * MetaTree, double rho, string runset_string, int maxrun=0)
{
	vector<int> set_of_runs;
	const char * this_runset = runset_string.c_str();
	
	int run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
	double this_rho = 0.0;
	MetaTree -> SetBranchAddress("Rho", &this_rho);
	int has_been_summed = 0;
	MetaTree -> SetBranchAddress("has_been_summed", &has_been_summed);
	int is_a_sum = 0;
	MetaTree -> SetBranchAddress("is_a_sum", &is_a_sum);
	char runset[256];
	MetaTree -> SetBranchAddress("matches_runset", &runset);
//	if( strcmp(this_matchedrunset1,this_matchedrunset2) !=0 ) {match=false;}	
	
	int nentries = MetaTree -> GetEntries();
	if(maxrun != 0)
	{ 
		nentries = maxrun+1; 
	}
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if(this_rho == rho && !strcmp(this_runset, runset))
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

TChain * get_chain_from_rho(TTree * MetaTree, double rho, string runset_string, int maxrun=0)
{
	cout << "rho = " << rho << endl;
	string path       = g4_path;
	string friendpath = g4f_path;
	
	const char * this_runset = runset_string.c_str();
//	if( strcmp(this_matchedrunset1,this_matchedrunset2) !=0 ) {match=false;}	
	
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
	char runset[256];
	MetaTree -> SetBranchAddress("matches_runset", &runset);

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
		if(this_rho == rho /* && has_been_summed==0 */ && !strcmp(this_runset, runset) )
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

vector<int> make_runlist_from_args( int argc, char* argv[] )
{
	vector<int> the_runlist;
	int thisrun;
	for (int k=1; k<argc; k++)
	{
		thisrun = atoi(argv[k]);  
		the_runlist.push_back(thisrun);
	}
	return the_runlist;
}

int check_runmatch(int the_run, vector<int> the_list) // returns the index of matching in the list, or -1 if there's no match.
{
	int have_match = -1;
	int listlength = the_list.size();
	
	for(int i=0; i<listlength; i++)
	{
		if(the_run == the_list.at(i) )
		{
	//		cout << "It's a match!  run " << the_run << " is run " << the_list.at(i) << endl;
			have_match = i;
			return have_match;
		}
		else
		{
	//		cout << "run " << the_run << " isn't run " << the_list.at(i) << endl;
		}
	}
	return -1;
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
TChain * get_chain_from_runlist(vector<int> the_runlist) // this can break if the metadata has recorded more than one of the same run number.
{
	TTree *MetaTree = load_metadata_tree(metadata_name);
	return get_chain_from_runlist(MetaTree, the_runlist);
}
TChain * get_chain_from_runlist(int argc, char* argv[], TTree * MetaTree) // this can break if the metadata has recorded more than one of the same run number.
{
	vector<int> the_runlist = make_runlist_from_args(argc, argv);  // am I calling this correctly????
	return get_chain_from_runlist(MetaTree, the_runlist);
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




// ====================================== //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

void printout_list(double rho, string runset_string)
{
	TTree * MetaTree = load_metadata_tree(metadata_name);
	vector<int> list = get_runlist_from_rho(MetaTree, rho, runset_string);
	
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