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

void meta_hadd_physlist(string use_this_physlist)  // this function is just a kludge.  Still.
{
	string original_fname = metadata_name;//g4_path+metadata_namestub;
//	string archive_fname  = g4_path+"MetaData_old.txt";
	TTree * MetaTree = load_metadata_tree(original_fname);
	
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

// ====================================== //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#endif