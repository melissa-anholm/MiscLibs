#ifndef INCLUDE_METACHAIN
#define INCLUDE_METACHAIN 1

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
//#include <stdlib.h>
//#include <stdio.h>
//#include <ios.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

#include "TFile.h"
#include <TTree.h>
#include <TChain.h>
#include "TNtuple.h"
#include "TBranch.h"  // do I need this?

#include "treeql_replacement.cpp"
#include "location.cpp"


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
// TChains for Data:
string get_datafilename(string path, int runno, bool use_blinded=false)
{	
	string fname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << path << "output00" << runno;
	if (use_blinded)
	{
		ss << "_blinded";
	} 
	ss << ".root";
	
	fname = ss.str();
	
	return fname;
}

string get_datafriendname(string path, int runno, bool use_blinded=false)
{	
	string fname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << path << "friend00" << runno;
	if (use_blinded)
	{
		ss << "_blinded";
	} 
	ss << ".root";

	fname = ss.str();
	
	return fname;
}

string get_dataasymname(string path, int runno, bool use_blinded=false)
{	
	string fname;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << path << "/AsymTree/asymtree00" << runno;
	if (use_blinded)
	{
		ss << "_blinded";
	} 
	ss << ".root";

	fname = ss.str();
	
	return fname;
}

TChain * get_single_datatree(int runno, bool use_blinded=false)
{
	TFile * file;
	string filename;
	string friendname;
	
	set_of_runs runs;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	
	if(use_blinded==true)
	{
		cout << "* Using blinded data." << endl;
		if( runs.good_electron[runno] == true || runs.good_recoil[runno] == true )
		{
			cout << "* Using run " << runno << endl;
			if(runs.good_electron[runno] == true)
			{
				cout << "* Run " << runno << " is an electron run." << endl;
				filename = get_datafilename(be_path, runno, use_blinded);
			}
			else if(runs.good_recoil[runno] == true)
			{
				cout << "* Run " << runno << " is a recoil run." << endl;
				filename = get_datafilename(br_path, runno, use_blinded);
			}
			tree_chain -> Add(filename.c_str());
		
			friendname = get_datafriendname(bf_path, runno, use_blinded);
			friend_chain -> Add(friendname.c_str());
		}
		else
		{
			cout << "Can't use run " << runno << ".  ABORT." << endl;
		}
	}
	else
	{
		cout << "* Using unblinded data." << endl;
		if( runs.good_electron[runno] == true || runs.good_recoil[runno] == true )
		{
			cout << "* Using run " << runno << endl;
			if(runs.good_electron[runno] == true)
			{
				cout << "* Run " << runno << " is an electron run." << endl;
				filename = get_datafilename(ue_path, runno, use_blinded);
			}
			else if(runs.good_recoil[runno] == true)
			{
				cout << "* Run " << runno << " is a recoil run." << endl;
				filename = get_datafilename(ur_path, runno, use_blinded);
			}
			tree_chain -> Add(filename.c_str());
		
			friendname = get_datafriendname(uf_path, runno, use_blinded);
			friend_chain -> Add(friendname.c_str());
		}
		else
		{
			cout << "* Can't use run " << runno << ".  ABORT." << endl;
		}
	}
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}

TChain * get_chain_from_Efield(double Efield, bool use_blinded=false)
{
	bool use_shitty=false;
	
//	TFile * file;
	string filename;
	string friendname;
	
	set_of_runs runs;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	
	string path;
	string friendpath;
	
	if( !((Efield==66.0 || Efield==66.7 || Efield==67.0 ) || Efield==150.0 || Efield==395.0 || Efield==415.0 || Efield==535.0) )
	{
		cout << "Bad E-field value.  ABORT." << endl;
	}
	else
	{
		if(Efield==66.0 || Efield==67.0)
			{ Efield=66.7; }
		cout << "* Using the set of runs at E = " << Efield << endl;
		
		if(use_blinded)
		{
			cout << "* Using blinded data." << endl;
			
			if(Efield==66.7 || Efield==150.0)
			{
				path = be_path;
			}
			else if(Efield==395.0 || Efield==415.0 || Efield==535.0)
			{
				path = br_path;
			}
			friendpath = bf_path;
		}
		else
		{
			cout << "* Using un-blinded data." << endl;
			
			if(Efield==66.7 || Efield==150.0)
			{
				path = ue_path;
			}
			else if(Efield==395.0 || Efield==415.0 || Efield==535.0)
			{
				path = ur_path;
			}
			friendpath = uf_path;
		}
		//
		for(int i=302; i<runs.N_runs; i++)
		{
			if( runs.ElectricField[i] == Efield 
			 /* && runs.compton_edge_ok[i] == true */
			 && (runs.good_electron[i] == true || runs.good_recoil[i] == true) )
			{
				if( runs.good_electron[i] == true || (runs.good_recoil[i] == true /* && runs.usable[i]==true*/ ) )
				{
					cout << "Using run " << i << endl;
					filename = get_datafilename(path, i, use_blinded);
					tree_chain -> Add(filename.c_str());
					
					friendname = get_datafriendname(friendpath, i, use_blinded);
					friend_chain -> Add(friendname.c_str());
				}
			}
		}
		tree_chain -> AddFriend(friend_chain);
	}
	return tree_chain;
}

TChain * get_electron_chain_from_letter(string runset_letter, bool use_blinded=false) // case sensitive.
{
	set_of_runs runs;
	
	string filename;
	string friendname;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	
	string path;
	string friendpath;
	
	if( !( runset_letter.compare(string("A"))==0 || runset_letter.compare(string("B"))==0 || runset_letter.compare(string("C"))==0 || runset_letter.compare(string("D"))==0 ) )
	{
		cout << "Nope.  \"" << runset_letter << "\" is not a recognized electron runset."  << endl;
		return tree_chain;
	}
	else
	{
		cout << "* Using electron runset " << runset_letter << endl;
	}
	
	if(use_blinded==true)
	{
		cout << "* Using blinded dataset." << endl;
		path = be_path;
		friendpath = bf_path;
	}
	else
	{
		cout << "* Using un-blinded dataset." << endl;
		path = ue_path;
		friendpath = uf_path;
	}
	
	for(int i=302; i<runs.N_runs; i++)
	{
		if(runs.good_electron[i] == true)
		{
			if( runset_letter.compare( runs.runset_letter[i] )==0 )
			{
				cout << "Using run " << i;// << endl;
				cout << ":  runset_letter[i] = " << runs.runset_letter[i] << endl;
				filename = get_datafilename(path, i, use_blinded);
				tree_chain -> Add(filename.c_str());
			
				friendname = get_datafriendname(friendpath, i, use_blinded);
				friend_chain -> Add(friendname.c_str());
			}
		}
	}
	
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}

TChain * get_electron_asym_chain_from_letter(string runset_letter, bool use_blinded=false) // case sensitive.
{
	set_of_runs runs;
	
	string filename;
	string friendname;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	string other_friendname;
	TChain * other_friend_chain = new TChain("cycle_counters");
	TChain * asym_chain = new TChain("use_eq_time");
	
	string path;
	string friendpath;
	
	if( !( runset_letter.compare(string("A"))==0 || runset_letter.compare(string("B"))==0 || runset_letter.compare(string("C"))==0 || runset_letter.compare(string("D"))==0 ) )
	{
		cout << "Nope.  \"" << runset_letter << "\" is not a recognized electron runset."  << endl;
		return tree_chain;
	}
	else
	{
		cout << "* Using electron runset " << runset_letter << endl;
	}
	
	if(use_blinded==true)
	{
		cout << "* Using blinded dataset." << endl;
		path = be_path;
		friendpath = bf_path;
	}
	else
	{
		cout << "* Using un-blinded dataset." << endl;
		path = ue_path;
		friendpath = uf_path;
	}
	
	for(int i=302; i<runs.N_runs; i++)
	{
		if(runs.good_electron[i] == true)
		{
			if( runset_letter.compare( runs.runset_letter[i] )==0 )
			{
				cout << "Using run " << i;// << endl;
				cout << ":  runset_letter[i] = " << runs.runset_letter[i] << endl;
				filename = get_datafilename(path, i, use_blinded);
				tree_chain -> Add(filename.c_str());
			
				friendname = get_datafriendname(friendpath, i, use_blinded);
				friend_chain -> Add(friendname.c_str());
				
				other_friendname = get_dataasymname(friendpath, i, use_blinded);
				other_friend_chain -> Add(other_friendname.c_str());
				asym_chain -> Add(other_friendname.c_str());
			}
		}
	}
	
	tree_chain -> AddFriend(friend_chain);
	tree_chain -> AddFriend(other_friend_chain);
	tree_chain -> AddFriend(asym_chain);
	return tree_chain;
}


TChain * get_recoil_chain_from_letter(string runset_letter, bool use_blinded=false)  // case sensitive.
{
	set_of_runs runs;
	
	string filename;
	string friendname;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	
	string path;
	string friendpath;

	if( !(runset_letter.compare(string("A"))==0 || runset_letter.compare(string("B"))==0 || runset_letter.compare(string("C"))==0 || runset_letter.compare(string("D"))==0 || runset_letter.compare(string("E"))==0 ) )
	{
		cout << "Nope.  \"" << runset_letter << "\" is not a recognized recoil runset."  << endl;
		return tree_chain;
	}
	else
	{
		cout << "* Using recoil runset " << runset_letter << endl;
	}

	if(use_blinded==true)
	{
		cout << "* Using blinded dataset." << endl;
		path = br_path;
		friendpath = bf_path;
	}
	else
	{
		cout << "* Using un-blinded dataset." << endl;
		path = ur_path;
		friendpath = uf_path;
	}

	for(int i=302; i<runs.N_runs; i++)
	{
		if(runs.good_recoil[i] == true)
		{
			if( runset_letter.compare( runs.runset_letter[i] )==0 )
			{
				cout << "Using run " << i;// << endl;
				cout << ":  runset_letter[i] = " << runs.runset_letter[i] << endl;
				
				filename = get_datafilename(path, i, use_blinded);
				tree_chain -> Add(filename.c_str());
			
				friendname = get_datafriendname(friendpath, i, use_blinded);
				friend_chain -> Add(friendname.c_str());
			}
		}
	}
	
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}

TChain * get_electron_chain_from_runnos(vector<int> use_these_runs, bool use_blinded=false)
{
	string filename;
	string friendname;

	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	
	string path;
	string friendpath;

	set_of_runs runs;
	int N_runs_to_use = use_these_runs.size();
	
	if(use_blinded==true)
	{
		cout << "* Using blinded electron dataset." << endl;
		path = be_path;
		friendpath = bf_path;
	}
	else
	{
		cout << "* Using un-blinded electron dataset." << endl;
		path = ue_path;
		friendpath = uf_path;
	}

	for(int i=302; i<runs.N_runs; i++)
	{
		for(int j=0; j<N_runs_to_use; j++)
		{
			if( use_these_runs[j] == i )
			{
				if(runs.good_electron[i] == true)
				{
					cout << "Using run " << i << endl;
					filename = get_datafilename(path, i, use_blinded);
					tree_chain -> Add(filename.c_str());
					
					friendname = get_datafriendname(friendpath, i, use_blinded);
					friend_chain -> Add(friendname.c_str());
					
					cout << "ntuple:  " << filename << "\nfriendtuple:  " << friendname << endl;
					filename = string();
					filename.clear();
					friendname = string();
					friendname.clear();
				}
				else
				{
					cout << "Can't use run " << i << endl;
				}
			}
		}
	}	
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}

TChain * get_electron_asym_chain_from_runnos(vector<int> use_these_runs, bool use_blinded=false)
{
	string filename;
	string friendname;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	
	string other_friendname;
	TChain * other_friend_chain = new TChain("cycle_counters");
	TChain * asym_chain = new TChain("use_eq_time");

	string path;
	string friendpath;

	set_of_runs runs;
	int N_runs_to_use = use_these_runs.size();
	
	if(use_blinded==true)
	{
		cout << "* Using blinded electron dataset." << endl;
		path = be_path;
		friendpath = bf_path;
	}
	else
	{
		cout << "* Using un-blinded electron dataset." << endl;
		path = ue_path;
		friendpath = uf_path;
	}

	for(int i=302; i<runs.N_runs; i++)
	{
		for(int j=0; j<N_runs_to_use; j++)
		{
			if( use_these_runs[j] == i )
			{
				if(runs.good_electron[i] == true)
				{
					cout << "Using run " << i << endl;
					filename = get_datafilename(path, i, use_blinded);
					tree_chain -> Add(filename.c_str());
					
					friendname = get_datafriendname(friendpath, i, use_blinded);
					friend_chain -> Add(friendname.c_str());
					
					other_friendname = get_dataasymname(friendpath, i, use_blinded);
					other_friend_chain -> Add(other_friendname.c_str());
					asym_chain -> Add(other_friendname.c_str());
					
					cout << "ntuple:  " << filename << "\nfriendtuple:  " << friendname;// << endl;
					cout << "\tother_friend_chain/asym_chain:  " << other_friend_chain << endl;
					
					filename = string();
					filename.clear();
					friendname = string();
					friendname.clear();
					other_friendname = string();
					other_friendname.clear();
				}
				else
				{
					cout << "Can't use run " << i << endl;
				}
			}
		}
	}	
	tree_chain -> AddFriend(friend_chain);
	tree_chain -> AddFriend(other_friend_chain);
	tree_chain -> AddFriend(asym_chain);
	return tree_chain;
}

TChain * get_electron_chain_for_all(bool use_blinded=false)
{
	set_of_runs runs;
	
	string filename;
	string friendname;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	
	string path;
	string friendpath;
	if(use_blinded==true)
	{
		cout << "* Using blinded dataset." << endl;
		path = be_path;
		friendpath = bf_path;
	}
	else
	{
		cout << "* Using un-blinded dataset." << endl;
		path = ue_path;
		friendpath = uf_path;
	}
	cout << endl;
	for(int i=302; i<runs.N_runs; i++)
	{
		if(runs.good_electron[i] == true)  // if compton edge is bad, the whole run is marked as bad now.
		{
			cout << "Adding run " << i << endl;
			filename = get_datafilename(path, i, use_blinded);
			tree_chain -> Add(filename.c_str());
		
			friendname = get_datafriendname(friendpath, i, use_blinded);
			friend_chain -> Add(friendname.c_str());
		}
	}
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;

}

TChain * get_electron_asym_chain_for_all(bool use_blinded=false)
{
	set_of_runs runs;
	
	string filename;
	string friendname;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	string other_friendname;
	TChain * other_friend_chain = new TChain("cycle_counters");
	TChain * asym_chain = new TChain("use_eq_time");

	string path;
	string friendpath;
	if(use_blinded==true)
	{
		cout << "* Using blinded dataset." << endl;
		path = be_path;
		friendpath = bf_path;
	}
	else
	{
		cout << "* Using un-blinded dataset." << endl;
		path = ue_path;
		friendpath = uf_path;
	}
	cout << endl;
	for(int i=302; i<runs.N_runs; i++)
	{
		if(runs.good_electron[i] == true)  // if compton edge is bad, the whole run is marked as bad now.
		{
			cout << "Adding run " << i << endl;
			filename = get_datafilename(path, i, use_blinded);
			tree_chain -> Add(filename.c_str());
		
			friendname = get_datafriendname(friendpath, i, use_blinded);
			friend_chain -> Add(friendname.c_str());
				
			other_friendname = get_dataasymname(friendpath, i, use_blinded);
			other_friend_chain -> Add(other_friendname.c_str());
			asym_chain -> Add(other_friendname.c_str());
		}
	}
	tree_chain -> AddFriend(friend_chain);
	tree_chain -> AddFriend(other_friend_chain);
	tree_chain -> AddFriend(asym_chain);

	return tree_chain;
}


TChain * get_recoil_chain_for_all(bool use_blinded=false)
{
	set_of_runs runs;
	
	string filename;
	string friendname;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	
	string path;
	string friendpath;
	if(use_blinded==true)
	{
		cout << "* Using blinded dataset." << endl;
		path = br_path;
		friendpath = bf_path;
	}
	else
	{
		cout << "* Using un-blinded dataset." << endl;
		path = ur_path;
		friendpath = uf_path;
	}
	cout << endl;
	for(int i=302; i<runs.N_runs; i++)
	{
		if(runs.good_recoil[i] == true)  // if compton edge is bad, the whole run is marked as bad now.
		{
			cout << "Adding run " << i << endl;
			filename = get_datafilename(path, i, use_blinded);
			tree_chain -> Add(filename.c_str());
		
			friendname = get_datafriendname(friendpath, i, use_blinded);
			friend_chain -> Add(friendname.c_str());
		}
	}
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}


// ====================================== // ====================================== //
// Newer TChains for Simulations:
string get_simfilename(string path, int runno)
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

string get_simfilename(TTree * MetaTree, int runno)
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
//	char*  filename = new char[256];
//	MetaTree -> SetBranchAddress("Filename", filename);
	double this_rho = 0.0;
	MetaTree -> SetBranchAddress("Rho", &this_rho);
//	TChain * tree_chain = new TChain("ntuple");

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
		//	cout << "Using run " << run << "  (i=" << i << ")" << endl;
		//	filename = get_datafilename(g4_path, run);
		//	tree_chain -> Add(filename.c_str());
			set_of_runs.push_back(run);
		}
	}

	return set_of_runs;
}



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

	int total_events_generated = 0;
	int total_events_recorded = 0;

	TChain * tree_chain   = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");
	string filename;// = get_simfilename(path, run);
	string friendname = get_simfilename(path, run);
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if(this_rho == rho)
		{
			cout << "Using run " << run << "  (i=" << i << "),\trho=" << this_rho << "\tN_gen=" << this_neventsgenerated << ",\tN_saved=" << this_neventssaved << endl;
			total_events_generated = total_events_generated + this_neventsgenerated;
			total_events_recorded = total_events_recorded + this_neventssaved;
			
			filename   = get_simfilename(path, run);
			friendname = get_simfriendname(friendpath, run);
			tree_chain -> Add(filename.c_str());
			friend_chain -> Add(friendname.c_str());
		
			filename = string();
			filename.clear();
			friendname = string();
			friendname.clear();
		}
	}
	
	cout << "rho=" << rho << ", N_generated=" << total_events_generated << ", \tN_saved=" << total_events_recorded << endl;
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}

/*
string int_to_string(int this_int)
{
	string this_string;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	
	ss << this_int;
	this_string = ss.str();
	return this_string;
}

// No friends in this one.
// Combine by explicit runno.
int can_hadd(int run1, int run2)
{
	int verbose = 2;
//	Run/I:Filename/C:BadFlag/I:Efield/D:Rho/D:EventsGenerated/I:EventsSaved/I:SaveEventTypes/C:Polarization/D:Alignment/D:MinCosTheta/D:Efield_Uniformity/O:StepperType/I:StepperName/C:StepMax_mm/D:PhysicsListName/C:is_summed/I:Trap_x_mm/D:Trap_y_mm/D:Trap_z_mm/D:Trap_sigma_x/D:Trap_sigma_y/D:Trap_sigma_z/D:Temperature/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:SailVelocity_x_mm_per_ms/D:SailVelocity_y_mm_per_ms/D:SailVelocity_z_mm_per_ms/D:ChargeState/I
	if(verbose>1)
	{
		cout << "Looking at " << metadata_name << endl;
	}
	
	TTree * MetaTree1 = load_metadata_tree(metadata_name);
	TTree * MetaTree2 = load_metadata_tree(metadata_name);
	int this_run1 = 0;
	int this_run2 = 0;
	MetaTree1 -> SetBranchAddress("Run", &this_run1);
	MetaTree2 -> SetBranchAddress("Run", &this_run2);
	
	// more parameters that I'll care about later, but don't care about now.
//Trap_x_mm/D:Trap_y_mm/D:Trap_z_mm/D:Trap_sigma_x/D:Trap_sigma_y/D:Trap_sigma_z/D:Temperature/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:SailVelocity_x_mm_per_ms/D:SailVelocity_y_mm_per_ms/D:SailVelocity_z_mm_per_ms/D:ChargeState/I	
	int nentries1 = MetaTree1 -> GetEntries();
	int i_run1 = -1;
	int i_run2 = -1;
	bool foundrun_1 = false;
	bool foundrun_2 = false;
	for(int i=0; i<nentries1; i++)
	{
		MetaTree1 -> GetEntry(i);
		if( this_run1 == run1 && !foundrun_1 )
		{
			i_run1 = i;
			foundrun_1 = true;
		}
		else if( this_run1 == run1 && foundrun_1)
		{
			cout << "This is bad.  Multiple runs with run number " << this_run1 << endl;
			return -1;
		}
	}
	for(int j=0; j<nentries1; j++)
	{
		MetaTree2 -> GetEntry(j);
		if( this_run2 == run2 && !foundrun_2 )
		{
			i_run2 = j;
			foundrun_2 = true;
		}
		else if( this_run2 == run2 && foundrun_2)
		{
			cout << "This is bad.  Multiple runs with run number " << this_run2 << endl;
			return -1;
		}
	}
	if(i_run1 == -1 || i_run2 == -1)
	{
		cout << "Bad!  Can't find runs to add." << endl;
		return -1;
	}
	else if(i_run1 == i_run2)
	{
		cout << "Bad!  You know those are literally the same run, right?" << endl;
		return -1;
	}
	// 
	// Load up the rest of the branches...
	char this_filename1[256];
	char this_filename2[256];
	MetaTree1 -> SetBranchAddress("Filename", &this_filename1);
	MetaTree2 -> SetBranchAddress("Filename", &this_filename2);
	int n_eventsgenerated1, n_eventsgenerated2;
	int n_eventssaved1, n_eventssaved2;
	MetaTree1 -> SetBranchAddress("EventsGenerated", &n_eventsgenerated1);
	MetaTree1 -> SetBranchAddress("EventsSaved",     &n_eventssaved1);
	MetaTree2 -> SetBranchAddress("EventsGenerated", &n_eventsgenerated2);
	MetaTree2 -> SetBranchAddress("EventsSaved",     &n_eventssaved2);
	//
	int isbad1, isbad2;
	int issummed1, issummed2;
	MetaTree1 -> SetBranchAddress("BadFlag", &isbad1);
	MetaTree2 -> SetBranchAddress("BadFlag", &isbad2);
	MetaTree1 -> SetBranchAddress("is_summed", &issummed1);
	MetaTree2 -> SetBranchAddress("is_summed", &issummed2);
	
	//
	double Efield1, Efield2;
	double rho1, rho2;
	double pol1, pol2;
	double align1, align2;
	MetaTree1 -> SetBranchAddress("Efield",         &Efield1);
	MetaTree1 -> SetBranchAddress("Rho",            &rho1);
	MetaTree1 -> SetBranchAddress("Polarization",   &pol1);
	MetaTree1 -> SetBranchAddress("Alignment",      &align1);
	MetaTree2 -> SetBranchAddress("Efield",         &Efield2);
	MetaTree2 -> SetBranchAddress("Rho",            &rho2);
	MetaTree2 -> SetBranchAddress("Polarization",   &pol2);
	MetaTree2 -> SetBranchAddress("Alignment",      &align2);
	//
	double stepmax1, stepmax2;
	double mincostheta1, mincostheta2;
	int steppertype1, steppertype2;
	int chargestate1, chargestate2;
	MetaTree1 -> SetBranchAddress("StepMax_mm",      &stepmax1);
	MetaTree1 -> SetBranchAddress("MinCosTheta",     &mincostheta1);
	MetaTree1 -> SetBranchAddress("StepperType",     &steppertype1);
	MetaTree1 -> SetBranchAddress("ChargeState",     &chargestate1);
	MetaTree2 -> SetBranchAddress("StepMax_mm",      &stepmax2);
	MetaTree2 -> SetBranchAddress("MinCosTheta",     &mincostheta2);
	MetaTree2 -> SetBranchAddress("StepperType",     &steppertype2);
	MetaTree2 -> SetBranchAddress("ChargeState",     &chargestate2);
	char this_StepperName1[256];
	char this_SaveEventTypes1[256];
	char this_PhysicsListName1[256];
	char this_StepperName2[256];
	char this_SaveEventTypes2[256];
	char this_PhysicsListName2[256];
	MetaTree1 -> SetBranchAddress("StepperName",     &this_StepperName1);
	MetaTree1 -> SetBranchAddress("SaveEventTypes",  &this_SaveEventTypes1);
	MetaTree1 -> SetBranchAddress("PhysicsListName", &this_PhysicsListName1);
	MetaTree2 -> SetBranchAddress("StepperName",     &this_StepperName2);
	MetaTree2 -> SetBranchAddress("SaveEventTypes",  &this_SaveEventTypes2);
	MetaTree2 -> SetBranchAddress("PhysicsListName", &this_PhysicsListName2);
	//
	double trap_x1, trap_y1, trap_z1;
	double trap_x2, trap_y2, trap_z2;
	MetaTree1 -> SetBranchAddress("Trap_x_mm",       &trap_x1);
	MetaTree1 -> SetBranchAddress("Trap_y_mm",       &trap_y1);
	MetaTree1 -> SetBranchAddress("Trap_z_mm",       &trap_z1);
	MetaTree2 -> SetBranchAddress("Trap_x_mm",       &trap_x2);
	MetaTree2 -> SetBranchAddress("Trap_y_mm",       &trap_y2);
	MetaTree2 -> SetBranchAddress("Trap_z_mm",       &trap_z2);
	double temp1, temp2;
	MetaTree1 -> SetBranchAddress("Temperature",     &temp1);
	MetaTree2 -> SetBranchAddress("Temperature",     &temp2);
	double trap_sigmax1, trap_sigmay1, trap_sigmaz1;
	double trap_sigmax2, trap_sigmay2, trap_sigmaz2;
	MetaTree1 -> SetBranchAddress("Trap_sigma_x",    &trap_sigmax1);
	MetaTree1 -> SetBranchAddress("Trap_sigma_y",    &trap_sigmay1);
	MetaTree1 -> SetBranchAddress("Trap_sigma_z",    &trap_sigmaz1);
	MetaTree2 -> SetBranchAddress("Trap_sigma_x",    &trap_sigmax2);
	MetaTree2 -> SetBranchAddress("Trap_sigma_y",    &trap_sigmay2);
	MetaTree2 -> SetBranchAddress("Trap_sigma_z",    &trap_sigmaz2);
	double sail_x1, sail_y1, sail_z1;
	double sail_x2, sail_y2, sail_z2;
	MetaTree1 -> SetBranchAddress("SailVelocity_x_mm_per_ms",   &sail_x1);
	MetaTree1 -> SetBranchAddress("SailVelocity_y_mm_per_ms",   &sail_y1);
	MetaTree1 -> SetBranchAddress("SailVelocity_z_mm_per_ms",   &sail_z1);
	MetaTree2 -> SetBranchAddress("SailVelocity_x_mm_per_ms",   &sail_x2);
	MetaTree2 -> SetBranchAddress("SailVelocity_y_mm_per_ms",   &sail_y2);
	MetaTree2 -> SetBranchAddress("SailVelocity_z_mm_per_ms",   &sail_z2);

//	/C:BadFlag/I/D:/D:Efield_Uniformity/O:/C:is_summed/I:/D:/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:

	// Now check to see whether *all* of the things are the same..
	MetaTree1 -> GetEntry(i_run1);
	MetaTree2 -> GetEntry(i_run2);
	bool match = true;
	//
	if(isbad1 || isbad2)       {match = false;}
		if(verbose>1 && !match) { cout << "One or more of the runs is 'bad'." << endl;         return match;}
	if(issummed1 || issummed2) {match = false;}
		if(verbose>1 && !match) { cout << "One or more of the runs is already a sum." << endl; return match;}
	//
	if( Efield1 != Efield2) {match = false;}
		if(verbose>1 && !match) { cout << "Efields are inconsistent." << endl;       return match;}
	if( rho1    != rho2)    {match = false;}
		if(verbose>1 && !match) { cout << "rho values are inconsistent." << endl;    return match;}
	if( pol1    != pol1)    {match = false;}
		if(verbose>1 && !match) { cout << "polarizations are inconsistent." << endl; return match;}
	if( align1  != align2)  {match = false;}
		if(verbose>1 && !match) { cout << "alignments are inconsistent." << endl;    return match;}
	//
	if( stepmax1     != stepmax2)     {match = false;}
	if( mincostheta1 != mincostheta2) {match = false;}
	if( steppertype1 != steppertype2) {match = false;}
	if( chargestate1 != chargestate2) {match = false;}
	//
	if( strcmp(this_StepperName1,    this_StepperName2)     !=0 ) {match=false;}
	if( strcmp(this_SaveEventTypes1, this_SaveEventTypes2)  !=0 ) {match=false;}
	if( strcmp(this_PhysicsListName1,this_PhysicsListName2) !=0 ) {match=false;}
	//
	if( trap_x1      != trap_x2)      {match = false;}
	if( trap_y1      != trap_y2)      {match = false;}
	if( trap_z1      != trap_z2)      {match = false;}
	if( temp1        != temp2)        {match = false;}
	if( trap_sigmax1 != trap_sigmax2) {match = false;}
	if( trap_sigmay1 != trap_sigmay2) {match = false;}
	if( trap_sigmaz1 != trap_sigmaz2) {match = false;}
	
	if (match && verbose>0)
	{
		cout << "Runs " << run1 << " and " << run2 << " are compatible and may be added!" << endl;
	}
	else if(!match && verbose>0)
	{
		cout << "Runs " << run1 << " and " << run2 << " are not compatible.  :(" << endl;
	}
	if (!match) { return -1; }
	
	
	// if it matches, figure out what new run number to use:
	// MELISSA LOOK HERE!!!  
	TTree * MetaTree3 = load_metadata_tree(metadata_name);
	int this_runno;
	MetaTree3 -> SetBranchAddress("Run", &this_runno);
	int is_summed;
	MetaTree3 -> SetBranchAddress("is_summed", &is_summed);
	
	int newrunno = 0;
	int maxcalls = 10000;
	bool match_this_loop = true;
	for(int j=0; j<nentries1; j++)  // I think n^2 is enough...
	{
		if(match_this_loop == true)
		{
			for(int i=0; i<nentries1; i++)
			{
				MetaTree3 -> GetEntry(i);
				match_this_loop = false;
				if(newrunno == this_runno && is_summed==1)
				{
					newrunno++;
					match_this_loop = true;
					break; // breaks out of the 'i' for-loop.  I think.
				}
			}
		}
		else
		{
			break;
		//	match_this_loop = false;
		}
	}
	cout << "Files will be hadded into a new file with run number:\t" << newrunno << endl;
	// MELISSA, FIX THE THING ABOVE!  
	// THIS IS 'PROBABLY' FINE, BUT NEEDS TO ACTUALLY BE TESTED.
	
	
//	if(match) { return newrunno; }
//	is_summed/I
//	else { return -1; }
//	return match;
//	return true;
	return newrunno;
}

bool do_the_hadding(int run1, int run2, int newrunno=0)
{ 
// run1 and run2 are the run numbers, not the i_line for the metadata file.
// this function breaks if run number isn't the first entry in every line.
	int position_of_eventsgenerated = -1;
	int position_of_eventssaved = -1;
	int position_of_runno = -1;
	int position_of_issummed = -1;
	int position_of_isbad = -1;
	int position_of_filename = -1;
//	Run/I:Filename/C:BadFlag/I:Efield/D:Rho/D:EventsGenerated/I:EventsSaved/I:SaveEventTypes/C:Polarization/D:Alignment/D:MinCosTheta/D:Efield_Uniformity/O:StepperType/I:StepperName/C:StepMax_mm/D:PhysicsListName/C:is_summed/I:Trap_x_mm/D:Trap_y_mm/D:Trap_z_mm/D:Trap_sigma_x/D:Trap_sigma_y/D:Trap_sigma_z/D:Temperature/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:SailVelocity_x_mm_per_ms/D:SailVelocity_y_mm_per_ms/D:SailVelocity_z_mm_per_ms/D:ChargeState/I

	
	string this_line;
//	string this_miniline;
	int this_runno;
	
	// Figure out the position where the event counts are kept.
	size_t positioncounter = 0;
	size_t last_position = 0;
	int n_found = 0;
	vector<int> delimiter_positions = 0;
//	EventsGenerated/I:EventsSaved/I:
//	Run/I:Filename/C:BadFlag/I:
//	is_summed/I:
	
	std::ifstream this_ifilestream;
	this_ifilestream.open(metadata_name.c_str());
	if( !this_ifilestream.is_open() )
	{
		cout << "Couldn't open the file for reading." << endl;
		return false;
	}
	
	std::getline(this_ifilestream, this_line);
	std::istringstream iss(this_line);
	cout << "this_line:  " << endl << this_line << endl;
	int size_of_string = this_line.size();
	cout << "size_of_string = " << size_of_string << endl;
	
	while (positioncounter<size_of_string)
	{
		positioncounter = this_line.find(":", last_position+1);
		if(positioncounter<size_of_string)
		{
			last_position = positioncounter;
			delimiter_positions.push_back(last_position);
			n_found++;
		}
	}
	cout << "** delimiter n_found = " << n_found << ",\tlast_position = " << last_position << endl;
	
	// ok, we know where the delimiters are.
	positioncounter = 0;
	while (positioncounter<size_of_string)
	{
		positioncounter = this_line.find("EventsGenerated", 0);
		last_position = positioncounter;
		if(positioncounter<size_of_string)
		{
			position_of_eventsgenerated = positioncounter;
			break;
		}
	}
	positioncounter = 0;
	while (positioncounter<size_of_string)
	{
		positioncounter = this_line.find("EventsSaved", 0);
		last_position = positioncounter;
		if(positioncounter<size_of_string)
		{
			position_of_eventssaved = positioncounter;
			break;
		}
	}
	cout << "position_of_eventsgenerated = " << position_of_eventsgenerated << endl;
	cout << "position_of_eventssaved = " << position_of_eventssaved << endl;
	positioncounter = 0;
	while (positioncounter<size_of_string)
	{
		positioncounter = this_line.find("Run", 0);
		last_position = positioncounter;
		if(positioncounter<size_of_string)
		{
			position_of_runno = positioncounter;
			break;
		}
	}
	positioncounter = 0;
	while (positioncounter<size_of_string)
	{
		positioncounter = this_line.find("BadFlag", 0);
		last_position = positioncounter;
		if(positioncounter<size_of_string)
		{
			position_of_isbad = positioncounter;
			break;
		}
	}
	positioncounter = 0;
	while (positioncounter<size_of_string)
	{
		positioncounter = this_line.find("is_summed", 0);
		last_position = positioncounter;
		if(positioncounter<size_of_string)
		{
			position_of_issummed = positioncounter;
			break;
		}
	}
	cout << "position_of_runno = " << position_of_runno << endl;
	cout << "position_of_isbad = " << position_of_isbad << endl;
	cout << "position_of_issummed = " << position_of_issummed << endl;
	positioncounter = 0;
	while (positioncounter<size_of_string)
	{
		positioncounter = this_line.find("Filename", 0);
		last_position = positioncounter;
		if(positioncounter<size_of_string)
		{
			position_of_filename = positioncounter;
			break;
		}
	}
	cout << "position_of_filename = " << position_of_filename << endl;
	cout << "--" << endl;
	
	// ok, we know where the EventsGenerated and EventsSaved entries are.  
	// also Run, BadFlag, and is_summed.
	// now figure out where they go wrt the delimiters.
	int branchpos_eventsgenerated = 0;
	int branchpos_eventssaved = 0;
	int branchpos_runno = 0;
	int branchpos_isbad = 0;
	int branchpos_issummed = 0;
	int branchpos_filename = 0;
	for(int i=0; i<size_of_string; i++)
	{
		if( position_of_eventsgenerated > delimiter_positions.at(i) ) { branchpos_eventsgenerated++; }
		else { break; }
	}
	for(int i=0; i<size_of_string; i++)
	{
		if( position_of_eventssaved > delimiter_positions.at(i) ) { branchpos_eventssaved++; }
		else { break; }
	}
	for(int i=0; i<size_of_string; i++)
	{
		if( position_of_runno > delimiter_positions.at(i) ) { branchpos_runno++; }
		else { break; }
	}
	for(int i=0; i<size_of_string; i++)
	{
		if( position_of_isbad > delimiter_positions.at(i) ) { branchpos_isbad++; }
		else { break; }
	}
	for(int i=0; i<size_of_string; i++)
	{
		if( position_of_issummed > delimiter_positions.at(i) ) { branchpos_issummed++; }
		else { break; }
	}
	for(int i=0; i<size_of_string; i++)
	{
		if( position_of_filename > delimiter_positions.at(i) ) { branchpos_filename++; }
		else { break; }
	}
	cout << "branchpos_eventsgenerated = " << branchpos_eventsgenerated << endl;
	cout << "branchpos_eventssaved = " << branchpos_eventssaved << endl;
	cout << "branchpos_runno = " << branchpos_runno << endl;
	cout << "branchpos_isbad = " << branchpos_isbad << endl;
	cout << "branchpos_issummed = " << branchpos_issummed << endl;
	cout << "branchpos_filename = " << branchpos_filename << endl;
	cout << "--" << endl;
	// numbering starts at "0".
	// k, we're done fucking around with the first line.
	
	//
	// Look for the line with info on "run1", and also the line with info on "run2".
	string firstrunline  = "";
	string secondrunline = "";
	int i_line1 = 0;
	int i_line2 = 0;
	int i_line = 1;  // already getline-ed the first line.
	while ( std::getline(this_ifilestream, this_line) )
	{
		i_line++;
		std::istringstream iss(this_line);
		iss >> this_runno;
		if(this_runno == run1)
		{
			i_line1 = i_line;
			firstrunline = this_line;
			cout << "i_line1=" << i_line1 << "\tfirstrunline:  " << endl;
			cout << firstrunline << endl;
		}
		if(this_runno == run2)
		{
			i_line2 = i_line;
			secondrunline = this_line;
			cout << "i_line2=" << i_line2 << "\tsecondrunline:  " << endl;
			cout << secondrunline << endl;
		}
	}
	
	cout << "final i_line = " << i_line << endl;
	cout << endl;
	this_ifilestream.close();

	// this_ifilestream.close();
	// ok.  I've gotten the info that I need out of this file.  I'm done with it for now.  
	// Later, I'll have to open that file back up to do things to it, but not now.
	
	// ok, now read out the info from those lines, and put them into a new line.
	std::stringstream ss1;
	ss1.str( std::string() );
	ss1.clear();
	ss1 << firstrunline;
//	cout << "ss1:  " << endl; 
//	cout << firstrunline << endl;
	std::stringstream ss2;
	ss2.str( std::string() );
	ss2.clear();
	ss2 << secondrunline;
//	cout << "ss2:  " << endl; 
//	cout << secondrunline << endl;
	cout << "--" << endl;
	
	string throwparam = "";
	for(int i=0; i<branchpos_eventsgenerated; i++)
	{
		ss1 >> throwparam;
		ss2 >> throwparam;
	//	cout << "i = " << i << ",\tthrowparam=" << throwparam << endl;
	}
	int n_eventsgenerated1;
	int n_eventsgenerated2;
	ss1 >> n_eventsgenerated1;
	cout << "n_eventsgenerated1 = " << n_eventsgenerated1 << endl;
	ss2 >> n_eventsgenerated2;
	cout << "n_eventsgenerated2 = " << n_eventsgenerated2 << endl;
	
	ss1.str( std::string() );
	ss1.clear();
	ss1 << firstrunline;
//	cout << "ss1:  " << endl; 
//	cout << firstrunline << endl;
	ss2.str( std::string() );
	ss2.clear();
	ss2 << secondrunline;
//	cout << "ss2:  " << endl; 
//	cout << secondrunline << endl;
	for(int i=0; i<branchpos_eventssaved; i++)
	{
		ss1 >> throwparam;
		ss2 >> throwparam;
	//	cout << "i = " << i << ",\tthrowparam=" << throwparam << endl;
	}
	int n_eventssaved1;
	int n_eventssaved2;
	ss1 >> n_eventssaved1;
	cout << "n_eventssaved1 = " << n_eventssaved1 << endl;
	ss2 >> n_eventssaved2;
	cout << "n_eventssaved2 = " << n_eventssaved2 << endl;
	
	int total_ngenerated = n_eventsgenerated1 + n_eventsgenerated2;
	int total_nsaved = n_eventssaved1 + n_eventssaved2;
	cout << "--" << endl;
	cout << "total_ngenerated = " << total_ngenerated << endl;
	cout << "total_nsaved = " << total_nsaved << endl;
	
	std::stringstream ss_out;
	ss_out.str( std::string() );
	ss_out.clear();
	ss_out << firstrunline;
	
	std::stringstream ss_tmp;
	ss_tmp.str( std::string() );
	ss_tmp.clear();
	
	// assign a new run number?  somehow?!
	cout << "newrunno = " << newrunno << endl;

	string newstringterm = "";
	string newstringline = "";
	int i=0; 
	while( ss_out.good() )
	{
		ss_out >> newstringterm;
		if( !ss_out.good() ) { break; }
		
		if(i == branchpos_eventsgenerated)
		{
			newstringterm = "";
			ss_tmp.str( std::string() );
			ss_tmp.clear();
			ss_tmp << total_ngenerated;
			ss_tmp >> newstringterm;
		}
		if(i == branchpos_eventssaved)
		{
			newstringterm = "";
			ss_tmp.str( std::string() );
			ss_tmp.clear();
			ss_tmp << total_nsaved;
			ss_tmp >> newstringterm;
		}
		if(i == branchpos_runno)
		{
			newstringterm = "";
			ss_tmp.str( std::string() );
			ss_tmp.clear();
			ss_tmp << newrunno;
			ss_tmp >> newstringterm;
		}
//		if(i == branchpos_isbad)
//		{
//			
//		}
		if(i == branchpos_issummed)
		{
			newstringterm = "";
			ss_tmp.str( std::string() );
			ss_tmp.clear();
			ss_tmp << 1;
			ss_tmp >> newstringterm;
		}
		if(i == branchpos_filename)
		{
			newstringterm = "";
			ss_tmp.str( std::string() );
			ss_tmp.clear();
			ss_tmp << "summedoutput_" << newrunno << ".root";
			ss_tmp >> newstringterm;
		}
		if( i != 0 )
		{
			newstringline = newstringline+"\t";
		}
		newstringline = newstringline+newstringterm;
		i++;
	}
	newstringline = newstringline+"\n";

	cout << endl;
	cout << "newstringline:" << endl; 
	cout << newstringline;// << endl;
	
	//
	this_ifilestream.open(metadata_name.c_str());
	if( !this_ifilestream.is_open() )
	{
		cout << "Couldn't open the file for reading the second time around.." << endl;
		return false;
	}
	// read in the whole file into a .... buffer of some sort.
	string filecontents = "";
	std::getline(this_ifilestream, this_line);
	filecontents = filecontents+this_line+"\n";
	
	string this_new_line = "";
	string this_term = "";
	cout << "* got here!  run1 = " << run1 << ",\trun2 = " << run2 << endl;
	while ( std::getline(this_ifilestream, this_line) )
	{
		std::istringstream iss(this_line);
		iss >> this_runno;
		cout << "* here?" << endl;
		if( !(this_runno == run1 || this_runno == run2) )
		{
		//	cout << "* here??  this_runno = " << this_runno << endl;
			filecontents = filecontents+this_line+"\n";
		}
		
		else
		{
		//	cout << "* blarg. ... this_runno = " << this_runno << endl;
			
			std::istringstream iss2(this_line);
			iss2 >> this_term;
			cout << "this_term = " << this_term << endl;
			this_new_line = string(this_term);
			
			for(int i=1; i<branchpos_isbad; i++)
			{
				iss2 >> this_term;
				cout << "this_term = " << this_term << endl;
				//int_to_string(int this_int)
			//	this_new_line = this_new_line+"\t"+int_to_string(this_term);
				this_new_line = this_new_line+"\t"+this_term;
				cout << "this_new_line:\n" << this_new_line << endl;
			}
		//	ss_tmp << 1;
			this_new_line = this_new_line+"\t"+this_term;
			cout << "ok?" << endl;
			for(int i=branchpos_isbad+1; i<n_found-1; i++)
			{
		//		ss_tmp << iss << "\t";
				iss2 >> this_term;
				cout << "i=" << i << ", this_term = " << this_term << ", this_new_line=" << this_new_line << endl;
				cout << << "i=" << i << ", this_new_line:\n" << this_new_line << endl;
			//	this_new_line = this_new_line+"\t"+int_to_string(this_term);
				cout << << "i=" << i << ", this_new_line:\n" << this_new_line << endl;
			}
			ss_tmp << iss;
			this_new_line = ss_tmp.str();
			cout << "this_new_line:" << endl;
			cout << this_new_line << endl;
		}
		cout << "so did I get here or what?" << endl;
		filecontents = filecontents+this_new_line+"\n";
	//	cout << "maybe now?" << endl;
		//
	}

	this_ifilestream.close();

	//
	std::ofstream this_ofilestream;
	this_ofilestream.open(metadata_name.c_str(), ios::app);
	if( !this_ofilestream.is_open() )
	{
		cout << "Couldn't open the file for writing." << endl;
		return false;
	}
	else
	{
		cout << "Yay, the file's open for writing!" << endl;
	}
	
//	string newstringline = "";
//	newstringline = "This is a new string line!\n";
	this_ofilestream << newstringline;
	
		
//	FILE *io_file;
//	io_file = fopen(metadata_name.c_str(), "r+");
//	char * delim = ":";
//	cout << "delim = " << delim << endl;
//	while ( (std::getline(this_filestream, this_line, "D") && positioncounter < 200) )
//	{
//		cout << positioncounter << ":\t" << this_line << endl;
//		positioncounter++;
//	}
//	fclose(io_file);
//	
//	std::ifstream this_filestream;
//	this_filestream.open(metadata_name.c_str());


//	fclose(io_file);
//	io_file = fopen(GetConfigurationFileName(), "a+");
//	// put the new entry here.
//	fclose(io_file);

//	
//	this_line = "";
////	if(this_ifilestream.seekg(std::ios::beg))
//	if(this_ifilestream.seekg(this_ifilestream.beg))
//	{
//		std::getline(this_ifilestream, this_line))
//		cout << "this_line:  " << endl << this_line << endl;
//	}
//	else 
//	{
//		if( this_ifilestream.good() ) {cout << "Good." << endl; }
//		else { cout << "Not good." << endl; }
//		//
//		if(this_ifilestream.seekg(this_ifilestream.beg)==std::ios::eof) {cout << "eofbit" << endl;}
//		if(this_ifilestream.seekg(this_ifilestream.beg)==failbit) {cout << "failbit" << endl;}
//		cout << ":(\n";
//	}
//	std::getline(this_ifilestream, this_line);
//	cout << "this_line:  " << endl << this_line << endl;
//	

	this_ofilestream.close();
	return true;
}

bool try_to_hadd(int run1, int run2)
{
	int can_hadd = -1;
//	bool can_hadd = false;
	bool is_hadded = false;
	
	can_hadd = can_hadd(run1, run2);
	if(can_hadd>=0) 
	{ 
		is_hadded = do_the_hadding(run1, run2, can_hadd); 
	}
	else
	{
		cout << "Can't hadd." << endl;
	}
	return is_hadded;
}
*/
// in hadder.cpp ??
void meta_hadd_physlist(string use_this_physlist)  // this function is just a kludge.  Still.
{
	string original_fname = metadata_name;//g4_path+metadata_namestub;
//	string archive_fname  = g4_path+"MetaData_old.txt";
	TTree * MetaTree = load_metadata_tree(original_fname);

//Run/I:Filename/C:BadFlag/I:Efield/D:Rho/D:EventsGenerated/I:EventsSaved/I:SaveEventTypes/C:Polarization/D:Alignment/D:MinCosTheta/D:Efield_Uniformity/O:StepperType/I:StepperName/C:StepMax_mm/D:PhysicsListName/C:Trap_x_mm/D:Trap_y_mm/D:Trap_z_mm/D:Trap_sigma_x/D:Trap_sigma_y/D:Trap_sigma_z/D:Temperature/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:SailVelocity_x_mm_per_ms/D:SailVelocity_y_mm_per_ms/D:SailVelocity_z_mm_per_ms/D:ChargeState/I
	// above:  that's old.
	
	int this_run = 0;
	MetaTree -> SetBranchAddress("Run", &this_run);
	vector<int> set_of_runs;

	char this_filename[256];
	MetaTree -> SetBranchAddress("Filename", &this_filename);
	vector<string> set_of_filenames;
	
	int n_eventsgenerated;
	int n_eventssaved;
	MetaTree -> SetBranchAddress("EventsGenerated", &n_eventsgenerated);
	MetaTree -> SetBranchAddress("EventsSaved",     &n_eventssaved);

	char this_PhysicsListName[256];
	MetaTree -> SetBranchAddress("PhysicsListName", &this_PhysicsListName);
	
	int total_eventsgenerated = 0;
	int total_eventssaved = 0;
	
	cout << "Looking for runs with this PhysicsList:  " << use_this_physlist << endl;
	int nentries = MetaTree -> GetEntries();
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		
	//	cout << "Run = " << this_run << "\tPhysicsListName = " << this_PhysicsListName << endl;
		if(strcmp(this_PhysicsListName, use_this_physlist.c_str()) == 0)
		{
			total_eventsgenerated += n_eventsgenerated;
			total_eventssaved += n_eventssaved;
			set_of_runs.push_back(this_run);
			set_of_filenames.push_back( string(this_filename) );
		//	cout << "this_filename = " << this_filename << endl;
		}
	}
	cout << "Generated " << total_eventsgenerated << " events in total." << endl;
	cout << "Expect to have " << total_eventssaved << " events saved." << endl;
	cout << "Use these runs:  " << endl;
	for(int i=0; i<set_of_runs.size(); i++)
	{
	//	cout << "i = " << i << " \tUse Run: " << set_of_runs[i] << endl;
		cout << set_of_runs[i] << ", ";
	}
	cout << endl;
	cout << "Try this command:  " << endl;
	cout << "hadd output_10000.root ";
	for(int i=0; i<set_of_runs.size(); i++)
	{
	//	cout << "output_" << set_of_runs[i] << ".root ";
		cout << set_of_filenames[i] << " ";
	}
	cout << endl;
}

// ====================================== //
// TChains for Simulations (do I ever even use these?):
string make_simfilename(string namestub, int runno)
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

TChain * make_simchain(vector<int> use_these_runs, string namestub=string("output_") )
{
	int nruns = use_these_runs.size();
	cout << "nruns = " << nruns << endl;
	
	string filename = "";
	TChain * simchain = new TChain("ntuple");
	string friendname = "";
	TChain * simfriendchain = new TChain("friendtuple");
	for(int i=0; i<nruns; i++)
	{
		filename = make_simfilename(namestub, use_these_runs[i]);
		friendname = get_simfriendname(g4f_path, use_these_runs[i]);
		simchain -> Add(filename.c_str());
		cout << "Added " << filename << " to chain." << endl;
		simfriendchain -> Add(friendname.c_str());
		cout << "Added " << filename << " to friendchain." << endl;
	}
	simchain -> AddFriend(simfriendchain);
	
	return simchain;
}

/*
class MetaTuple
{
public:
	string metafilename;
	TTree * MetaTree;
	int nentries;
//	vector<int> use_these_runs;
	
	MetaTuple();
	MetaTuple(string metadatafilename); //=g4_path+string("MetaData.txt")
	void set_allthe_branch_addresses();
//	TTree * LoadMetaData();
	void SetBools();
	
	TChain * LoadNewChain(MetaTuple CompTuple);
	TChain * AddToChain(TChain* this_chain, MetaTuple CompTuple);

	bool Compare(MetaTuple CompTuple);

	bool run_compare;
	bool filename_compare;         // //
	bool Efield_compare;
	bool Efield_Uniformity_compare;
	bool PI_Energy_compare;        //
	bool StepperType_compare;
	bool StepperName_compare;      // //
	bool StepMax_mm_compare;
	bool EventsGenerated_compare;
	bool EventsSaved_compare;
	bool SaveEventTypes_compare;   // // 
	bool trap_x_compare;
	bool trap_y_compare;
	bool trap_z_compare;
	bool trap_sigma_x_compare;
	bool trap_sigma_y_compare;
	bool trap_sigma_z_compare;
	bool Temperature_compare;
	bool Polarization_compare;
	bool Alignment_compare;
	bool ExpandBeforePolarized_s_compare;
	bool OP_CycleTime_s_compare;
	bool sailvelocity_x_compare;
	bool sailvelocity_y_compare;
	bool sailvelocity_z_compare;
	
	void set_run(int run_) {run=run_; run_compare=true;}
	void set_filename(string filename_) {filename=(char*)filename_.c_str(); filename_compare=true;}
	void set_Efield(double Efield_) {Efield=Efield_; Efield_compare=true;}
	void set_Efield_Uniformity(bool Efield_Uniformity_) 
	{
		Efield_Uniformity=Efield_Uniformity_; 
		Efield_Uniformity_compare=true;
	}
	void set_PI_Energy(double PI_Energy_) {PI_Energy=PI_Energy_; PI_Energy_compare=true;}
	void set_StepperType(int StepperType_) {StepperType = StepperType_; StepperType_compare=true;}
	void set_StepperName(string StepperName_) 
	{
		StepperName = (char*)StepperName_.c_str(); 
		StepperName_compare=true; 
	}
	void set_StepMax_mm(double StepMax_mm_) {StepMax_mm = StepMax_mm_; StepMax_mm_compare=true;}
	void set_EventsGenerated(int EventsGenerated_) 
	{
		EventsGenerated = EventsGenerated_; 
		EventsGenerated_compare=true; 
	}
	void set_EventsSaved(int EventsSaved_) {EventsSaved=EventsSaved_; EventsSaved_compare=true;}
	void set_SaveEventTypes(string SaveEventTypes_) 
	{
		SaveEventTypes = (char*)SaveEventTypes_.c_str();  
		SaveEventTypes_compare = true;
	}
	void set_trap_x(double trap_x_) {trap_x=trap_x_; trap_x_compare=true;}
	void set_trap_y(double trap_y_) {trap_y=trap_y_; trap_y_compare=true;}
	void set_trap_z(double trap_z_) {trap_z=trap_z_; trap_z_compare=true;}
	void set_trap_sigma_x(double trap_sigma_x_) 
	{
		trap_sigma_x=trap_sigma_x_; 
		trap_sigma_x_compare=true;
	}
	void set_trap_sigma_y(double trap_sigma_y_) 
	{
		trap_sigma_y=trap_sigma_y_; 
		trap_sigma_y_compare=true;
	}
	void set_trap_sigma_z(double trap_sigma_z_) 
	{
		trap_sigma_z=trap_sigma_z_; 
		trap_sigma_z_compare=true;
	}
	void set_Temperature(double Temperature_) 
	{
		Temperature=Temperature_; 
		Temperature_compare=true;
	}
	void set_Polarization(double Polarization_) 
	{
		Polarization=Polarization_; 
		Polarization_compare=true;
	}
	void set_Alignment(double Alignment_) {Alignment=Alignment_; Alignment_compare=true;}
	void set_ExpandBeforePolarized_s(double ExpandBeforePolarized_s_)
	{
		ExpandBeforePolarized_s=ExpandBeforePolarized_s_;
		ExpandBeforePolarized_s_compare=true;
	}
	void set_OP_CycleTime_s(double OP_CycleTime_s_)
	{
		OP_CycleTime_s=OP_CycleTime_s_;
		OP_CycleTime_s_compare=true;
	}
	void set_sailvelocity_x(double sailvelocity_x_)
	{
		sailvelocity_x=sailvelocity_x_;
		sailvelocity_x_compare=true;
	}
	void set_sailvelocity_y(double sailvelocity_y_)
	{
		sailvelocity_y=sailvelocity_y_;
		sailvelocity_y_compare=true;
	}
	void set_sailvelocity_z(double sailvelocity_z_)
	{
		sailvelocity_z=sailvelocity_z_;
		sailvelocity_z_compare=true;
	}
	
// private:
//	branch pointers:
	int    run;
	char*  filename;         // //
	double Efield;
	bool   Efield_Uniformity;
	double PI_Energy;        //
	int    StepperType;
	char*  StepperName;      // //
	double StepMax_mm;
	int    EventsGenerated;
	int    EventsSaved;
	char*  SaveEventTypes;   // // 
	double trap_x;
	double trap_y;
	double trap_z;
	double trap_sigma_x;
	double trap_sigma_y;
	double trap_sigma_z;
	double Temperature;
	double Polarization;
	double Alignment;
	double ExpandBeforePolarized_s;
	double OP_CycleTime_s;
	double sailvelocity_x;
	double sailvelocity_y;
	double sailvelocity_z;
};

TChain * MetaTuple::LoadNewChain(MetaTuple CompTuple)
{
	TChain * tree_chain = new TChain("ntuple");
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if( Compare(CompTuple) )
		{
			tree_chain -> Add( (g4_path+string(filename)).c_str() ); // filename is already associated with the correct thing...
			cout << "Adding " << filename << " to chain." << endl;
		}
	}
	
	return tree_chain;
}

TChain * MetaTuple::AddToChain(TChain* this_chain, MetaTuple CompTuple)
{
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if( Compare(CompTuple) )
		{
			this_chain -> Add( (g4_path+string(filename)).c_str() ); // filename is already associated with the correct thing...
			cout << "Adding " << filename << " to chain." << endl;
		}
	}
	
	return this_chain;
}

void MetaTuple::set_allthe_branch_addresses()
{
	run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
	filename = new char[256];
	MetaTree -> SetBranchAddress("Filename", filename);
	Efield = 0;
	MetaTree -> SetBranchAddress("Efield", &Efield);
	Efield_Uniformity=-1;
	MetaTree -> SetBranchAddress("Efield_Uniformity", &Efield_Uniformity);
//	PI_Energy=0.0;
//	MetaTree -> SetBranchAddress("PI_Energy_eV", PI_Energy);
	StepperType=-1;
	MetaTree -> SetBranchAddress("StepperType", &StepperType);
	StepperName = new char[256];
	MetaTree -> SetBranchAddress("StepperName", StepperName);
	StepMax_mm=0.0;
	MetaTree -> SetBranchAddress("StepMax_mm", &StepMax_mm);
	
	EventsGenerated=0;
	MetaTree -> SetBranchAddress("EventsGenerated", &EventsGenerated);
	EventsSaved=0;
	MetaTree -> SetBranchAddress("EventsSaved", &EventsSaved);
	SaveEventTypes = new char[256];
	MetaTree -> SetBranchAddress("SaveEventTypes", SaveEventTypes);
	
	trap_x = 0.0;
	MetaTree -> SetBranchAddress("Trap_x_mm", &trap_x);
	trap_y = 0.0;
	MetaTree -> SetBranchAddress("Trap_y_mm", &trap_y);
	trap_z = 0.0;
	MetaTree -> SetBranchAddress("Trap_z_mm", &trap_z);
	trap_sigma_x = 0.0;
	MetaTree -> SetBranchAddress("Trap_sigma_x", &trap_sigma_x);
	trap_sigma_y = 0.0;
	MetaTree -> SetBranchAddress("Trap_sigma_y", &trap_sigma_y);
	trap_sigma_z = 0.0;
	MetaTree -> SetBranchAddress("Trap_sigma_z", &trap_sigma_z);
	
	Temperature = 0.0;
	MetaTree -> SetBranchAddress("Temperature", &Temperature);
	Polarization = 0.0;
	MetaTree -> SetBranchAddress("Polarization", &Polarization);
	Alignment = 0.0;
	MetaTree -> SetBranchAddress("Alignment", &Alignment);

	ExpandBeforePolarized_s = 0.0;
	MetaTree -> SetBranchAddress("ExpandBeforePolarized_s", &ExpandBeforePolarized_s);
	OP_CycleTime_s = 0.0;
	MetaTree -> SetBranchAddress("OP_CycleTime_s", &OP_CycleTime_s);
	
	sailvelocity_x=0.0;
	MetaTree -> SetBranchAddress("SailVelocity_x_mm_per_ms", &sailvelocity_x);
	sailvelocity_y=0.0;
	MetaTree -> SetBranchAddress("SailVelocity_y_mm_per_ms", &sailvelocity_y);
	sailvelocity_z=0.0;
	MetaTree -> SetBranchAddress("SailVelocity_z_mm_per_ms", &sailvelocity_z);
}

void MetaTuple::SetBools()
{
	run_compare=false;
	filename_compare=false;         // //
	Efield_compare=false;
	Efield_Uniformity_compare=false;
	PI_Energy_compare=false;        //
	StepperType_compare=false;
	StepperName_compare=false;      // //
	StepMax_mm_compare=false;
	EventsGenerated_compare=false;
	EventsSaved_compare=false;
	SaveEventTypes_compare=false;   // // 
	trap_x_compare=false;
	trap_y_compare=false;
	trap_z_compare=false;
	trap_sigma_x_compare=false;
	trap_sigma_y_compare=false;
	trap_sigma_z_compare=false;
	Temperature_compare=false;
	Polarization_compare=false;
	Alignment_compare=false;
	ExpandBeforePolarized_s_compare=false;
	OP_CycleTime_s_compare=false;
	sailvelocity_x_compare=false;
	sailvelocity_y_compare=false;
	sailvelocity_z_compare=false;
}

bool MetaTuple::Compare(MetaTuple CompTuple)
{ // Use the true-est of *either* set of bools.
	bool match = true;
	
	if(run_compare==true||CompTuple.run_compare==true)
	{
		if(run!=CompTuple.run)
			{ match=false; return match; }
	}
//	if(filename_compare==true||CompTuple.filename_compare==true)
//	{
//		if(filename!=CompTuple.filename)
//			{ match=false; return match; }
//	}
	if(Efield_compare==true||CompTuple.Efield_compare==true)
	{
		if(Efield!=CompTuple.Efield)
			{ match=false; return match; }
	}
	if(Efield_Uniformity_compare==true||CompTuple.Efield_Uniformity_compare==true)
	{
		if(Efield_Uniformity!=CompTuple.Efield_Uniformity)
			{ match=false; return match; }
	}
	if(PI_Energy_compare==true||CompTuple.PI_Energy_compare==true)
	{
		if(PI_Energy!=CompTuple.PI_Energy)
			{ match=false; return match; }
	}
	if(StepperType_compare==true||CompTuple.StepperType_compare==true)
	{
		if(StepperType!=CompTuple.StepperType)
			{ match=false; return match; }
	}
//	if(StepperName_compare==true||CompTuple.StepperName_compare==true)
//	{
//		if(StepperName!=CompTuple.StepperName)
//			{ match=false; return match; }
//	}
	if(StepMax_mm_compare==true||CompTuple.StepMax_mm_compare==true)
	{
		if(StepMax_mm!=CompTuple.StepMax_mm)
			{ match=false; return match; }
	}
	if(EventsGenerated_compare==true||CompTuple.EventsGenerated_compare==true)
	{
		if(EventsGenerated!=CompTuple.EventsGenerated)
			{ match=false; return match; }
	}
	if(EventsSaved_compare==true||CompTuple.EventsSaved_compare==true)
	{
		if(EventsSaved!=CompTuple.EventsSaved)
			{ match=false; return match; }
	}
//	if(SaveEventTypes_compare==true||CompTuple.SaveEventTypes_compare==true)
//	{
//		if(SaveEventTypes!=CompTuple.SaveEventTypes)
//			{ match=false; return match; }
//	}
	if(trap_x_compare==true||CompTuple.trap_x_compare==true)
	{
		if(trap_x!=CompTuple.trap_x)
			{ match=false; return match; }
	}
	if(trap_y_compare==true||CompTuple.trap_y_compare==true)
	{
		if(trap_y!=CompTuple.trap_y)
			{ match=false; return match; }
	}
	if(trap_z_compare==true||CompTuple.trap_z_compare==true)
	{
		if(trap_z!=CompTuple.trap_z)
			{ match=false; return match; }
	}
	if(trap_sigma_x_compare==true||CompTuple.trap_sigma_x_compare==true)
	{
		if(trap_sigma_x!=CompTuple.trap_sigma_x)
			{ match=false; return match; }
	}
	if(trap_sigma_y_compare==true||CompTuple.trap_sigma_y_compare==true)
	{
		if(trap_sigma_y!=CompTuple.trap_sigma_y)
			{ match=false; return match; }
	}
	if(trap_sigma_z_compare==true||CompTuple.trap_sigma_z_compare==true)
	{
		if(trap_sigma_z!=CompTuple.trap_sigma_z)
			{ match=false; return match; }
	}
	if(Temperature_compare==true||CompTuple.Temperature_compare==true)
	{
		if(Temperature!=CompTuple.Temperature)
			{ match=false; return match; }
	}
	if(Polarization_compare==true||CompTuple.Polarization_compare==true)
	{
		if(Polarization!=CompTuple.Polarization)
			{ match=false; return match; }
	}
	if(Alignment_compare==true||CompTuple.Alignment_compare==true)
	{
		if(Alignment!=CompTuple.Alignment)
			{ match=false; return match; }
	}
	if(ExpandBeforePolarized_s_compare==true||CompTuple.ExpandBeforePolarized_s_compare==true)
	{
		if(ExpandBeforePolarized_s!=CompTuple.ExpandBeforePolarized_s)
			{ match=false; return match; }
	}
	if(OP_CycleTime_s_compare==true||CompTuple.OP_CycleTime_s_compare==true)
	{
		if(OP_CycleTime_s!=CompTuple.OP_CycleTime_s)
			{ match=false; return match; }
	}
	if(sailvelocity_x_compare==true||CompTuple.sailvelocity_x_compare==true)
	{
		if(sailvelocity_x!=CompTuple.sailvelocity_x)
			{ match=false; return match; }
	}
	if(sailvelocity_y_compare==true||CompTuple.sailvelocity_y_compare==true)
	{
		if(sailvelocity_y!=CompTuple.sailvelocity_y)
			{ match=false; return match; }
	}
	if(sailvelocity_z_compare==true||CompTuple.sailvelocity_z_compare==true)
	{
		if(sailvelocity_z!=CompTuple.sailvelocity_z)
			{ match=false; return match; }
	}
	//
	return match;
}

MetaTuple::MetaTuple()
{
	nentries=0;
	cout << "Created a MetaTuple without an associated TTree." << endl;
	SetBools();
}

MetaTuple::MetaTuple(string metadatafilename)
{
	metafilename = metadatafilename;
	MetaTree = new TTree();
	nentries = MetaTree -> ReadFile(metafilename.c_str());
	set_allthe_branch_addresses();
	SetBools();
	
	cout << "MetaTuple created.  There are " << nentries << " rows." << endl;
//	LoadMetaData();
}
*/



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

/*
TChain * get_single_datatree_fromfile(string filename, string friendname) 
{ // filename and friendname already have path included.
	TFile * file;
	TChain * tree_chain = new TChain("ntuple");
	TChain * friend_chain = new TChain("friendtuple");

	tree_chain -> Add(filename.c_str());
	friend_chain -> Add(friendname.c_str());

	tree_chain -> AddFriend(friend_chain);
	
	cout << "Using: " << filename << endl;
	cout << "Friend:  " << friendname << endl;
	
	return tree_chain;
}
TChain * get_single_datatree_fromfile(string filename) 
{ // filename and friendname already have path included.
	cout << "Warning:  loading TTree without its friendtuple."  << endl;
	
	TFile * file;
	TChain * tree_chain = new TChain("ntuple");
//	TChain * friend_chain = new TChain("friendtuple");

	tree_chain -> Add(filename.c_str());
//	friend_chain -> Add(friendname.c_str());
//	tree_chain -> AddFriend(friend_chain);
	
	cout << "Using: " << filename << endl;
//	cout << "Friend:  " << friendname << endl;
	
	return tree_chain;
}
*/
// ====================================== //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#endif