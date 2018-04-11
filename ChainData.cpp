#ifndef INCLUDE_CHAINDATA
#define INCLUDE_CHAINDATA 1

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

#include "treeql_replacement.cpp"
#include "location.cpp"



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