#ifndef INCLUDE_METACHAIN
#define INCLUDE_METACHAIN 1

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
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

string br_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Recoils_2014/";
string be_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
string bf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Friends_2014/";

string ur_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Recoils_2014/";
string ue_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Electrons_2014/";
string uf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Friends_2014/";

string g4_path = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

// ====================================== //
// TChains for Data:
string get_datafilename(string path, int runno, bool use_blinded=true)
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

/*
string get_simfilename(string path, const char* )
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
*/

string get_datafriendname(string path, int runno, bool use_blinded=true)
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

TChain * get_single_datatree(int runno, bool use_blinded=true)
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
			if(runs.good_electron[runno] == true)
			{
				filename = get_datafilename(be_path, runno, use_blinded);
			}
			else if(runs.good_recoil[runno] == true)
			{
				filename = get_datafilename(br_path, runno, use_blinded);
			}
			tree_chain -> Add(filename.c_str());
		
			friendname = get_datafriendname(bf_path, runno, use_blinded);
			friend_chain -> Add(friendname.c_str());
			cout << "* Using run " << runno << endl;
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
			if(runs.good_electron[runno] == true)
			{
				filename = get_datafilename(ue_path, runno, use_blinded);
			}
			else if(runs.good_recoil[runno] == true)
			{
				filename = get_datafilename(ur_path, runno, use_blinded);
			}
			tree_chain -> Add(filename.c_str());
		
			friendname = get_datafriendname(uf_path, runno, use_blinded);
			friend_chain -> Add(friendname.c_str());
			cout << "* Using run " << runno << endl;
		}
		else
		{
			cout << "Can't use run " << runno << ".  ABORT." << endl;
		}
	}
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}

TChain * get_chain_from_Efield(double Efield, bool use_blinded=true)
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
	
	if( !((Efield==66.0 || Efield==66.7) || Efield==150.0 || Efield==395.0 || Efield==415.0 || Efield==535.0) )
	{
		cout << "Bad E-field value.  ABORT." << endl;
	}
	else
	{
		if(Efield==66.0)
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
	//			else if(runs.good_recoil[i] == true)
	//			{
	//				if( runs.usable[i]==true)
	//				{
	//					cout << "Using run " << i << endl;
	//					filename = get_datafilename(r_path, i);
	//					tree_chain -> Add(filename.c_str());
	//					
	//					friendname = get_datafriendname(f_path, i);
	//					friend_chain -> Add(friendname.c_str());
	//				}
			//		else if(use_shitty==true)
			//		{
			//			// don't do anything.
			//			cout << "Using run " << i << " (shitty)." << endl;
			//			filename = get_datafilename(r_path, i);
			//			tree_chain -> Add(filename.c_str());
			//			
			//			friendname = get_datafriendname(f_path, i);
			//			friend_chain -> Add(friendname.c_str());
			//		}
		//			else // if use_shitty==false and runs.usable[i]==false.
		//			{
		//				cout << "Skipping Run " << i << endl;
		//			}
	//			}
			}
		}
	//	tree_chain -> AddFriend("friendtuple");
		tree_chain -> AddFriend(friend_chain);
	}
	return tree_chain;
}

TChain * get_electron_chain_from_letter(string runset_letter, bool use_blinded=true) // case sensitive.
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

TChain * get_recoil_chain_from_letter(string runset_letter, bool use_blinded=true)  // case sensitive.
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
		//	else
		//	{
		//		cout << "Run " << i << ":  runs.runset_letter[i] = " << runs.runset_letter[i] << endl;
		//	}
		}
	}
	
//	cout << endl;
//	cout << "*****************************************" << endl;
//	cout << "This doesn't work yet;  fuck you Melissa!" << endl;
//	cout << "*****************************************" << endl;
//	cout << endl;
	
	tree_chain -> AddFriend(friend_chain);
	return tree_chain;
}

TChain * get_electron_chain_from_runnos(vector<int> use_these_runs, bool use_blinded=true)
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

// ====================================== //
// Newer TChains for Simulations:

TTree * load_metadata_tree(string metadatafilename)
{
	TTree *MetaTree = new TTree();
//	int nrows = toftree -> ReadFile(metadatafilename.c_str());
	int nentries = MetaTree -> ReadFile(metadatafilename.c_str());
	return MetaTree;
}

vector<int> get_runlist_from_rho(TTree * MetaTree, double rho)
{
	vector<int> set_of_runs;
	
	int nentries = MetaTree -> GetEntries();
	int run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
	char*  filename = new char[256];
	MetaTree -> SetBranchAddress("Filename", filename);
	
	double this_rho = 0.0;
	MetaTree -> SetBranchAddress("Rho", &this_rho);
	
	TChain * tree_chain = new TChain("ntuple");
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

TChain * get_chain_from_rho(TTree * MetaTree, double rho)
{
	cout << "rho = " << rho << endl;
	
	int nentries = MetaTree -> GetEntries();
	
	int run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
//	char*  filename = new char[256];
//	MetaTree -> SetBranchAddress("Filename", filename);
	double this_rho = 0.0;
	MetaTree -> SetBranchAddress("Rho", &this_rho);
	
	TChain * tree_chain = new TChain("ntuple");
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if(this_rho == rho)
		{
			cout << "Using run " << run << "  (i=" << i << ")" << endl;
			string new_filename = get_simfilename(g4_path, run);
			tree_chain -> Add(new_filename.c_str());
		}
	}
	
	return tree_chain;
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
	
	return fname;
}

TChain * make_simchain(vector<int> use_these_runs, string namestub=g4_path+string("output_") )
{
	int nruns = use_these_runs.size();
	cout << "nruns = " << nruns << endl;

	string filename = "";
	TChain * simchain = new TChain("ntuple");
	for(int i=0; i<nruns; i++)
	{
		filename = make_simfilename(namestub, use_these_runs[i]);
		simchain -> Add(filename.c_str());
		cout << "Added " << filename << endl;
	}
	
	return simchain;
}

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
//	TFile * file;
//	string filename;
	TChain * tree_chain = new TChain("ntuple");
	
//	const char* fname;
	
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if( Compare(CompTuple) )
		{
		//	fname = filename
			tree_chain -> Add( (g4_path+string(filename)).c_str() ); // filename is already associated with the correct thing...
			cout << "Adding " << filename << " to chain." << endl;
		}
	}
	
	return tree_chain;
}

TChain * MetaTuple::AddToChain(TChain* this_chain, MetaTuple CompTuple)
{
//	TFile * file;
//	string filename;
//	TChain * tree_chain = new TChain("ntuple");	
//	const char* fname;
	
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if( Compare(CompTuple) )
		{
		//	fname = filename
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
/*
void test_load_chain()
{
	
}
*/
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#endif