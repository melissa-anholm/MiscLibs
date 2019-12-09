
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
using std::stringstream;


#include "TFile.h"
#include <TTree.h>
#include <TChain.h>
#include "TNtuple.h"
#include "TBranch.h"  // do I need this?

#include "treeql_replacement.cpp"
#include "location.cpp"
//#include "MetaChain.cpp"
#include "ChainG4.cpp"

int extrafilenum = 10000;

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
struct hadder_runlist
{
	int newfilenum;
	vector<int> runlist;
};
struct lineinfo
{
	int linenumber;
	string linestring;
};

//
string int_to_string(int);
int can_hadd(int, int);
int can_hadd(int, int, bool);
int can_hadd(string, int, string, int, string);
int can_hadd(string, int, string, int, string, bool);
bool do_the_hadding(int, int, int);
bool do_the_hadding(int, vector<int>);
//int try_to_hadd(int, int);
hadder_runlist check_hadd_multi();
int get_newrunno(string);
string generate_hadd_command(int, vector<int>, string, string, string);
string generate_hadd_command(int, vector<int>);
lineinfo update_runline(lineinfo, int, int);
string make_filebuffer(vector<lineinfo>, string, string);


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
int main(int argc, char *argv[])
{
	setup_location();
	
	hadder_runlist this_hadder_runlist = check_hadd_multi();
	
	vector<int> result = this_hadder_runlist.runlist;
	int newrunno = this_hadder_runlist.newfilenum;
	if(newrunno==0)
	{
		cout << "Nope, nothing to hadd." << endl;
		return 0;
	}
	
	cout << "New file number:  " << newrunno << endl;
	cout << "Runs that can be hadded: " << endl;
	printout(result);

//	cout << "I'm about to generate the hadd command." << endl;
	string hadder_command = generate_hadd_command(newrunno, result);
	
//	cout << "I'm about to check whether it worked (and do the hadding)." << endl;
	bool did_it_work = do_the_hadding(newrunno, result);
	
	cout << "Use this command:  " << endl;
	cout << hadder_command << endl << endl;
	
	if(did_it_work)
	{
		return newrunno;
	}
	return 0;
}

/*
int main(int argc, char *argv[]) // hadd two runs together.
{ 
	int run1, run2;
	string fname1, fname2;
	if(argc == 3) // 2 arguments...
	{
		run1 = atoi(argv[1]);
		run2 = atoi(argv[2]);
	}
	else
	{
		cout << "Can't hadd.  Wrong number of arguments." << endl;
		return -1;
	}
	int filenum = try_to_hadd(run1, run2);
	cout << "filenum = " << filenum << endl;
	return filenum;
}
*/

//
hadder_runlist check_hadd_multi()
{
	int verbose = 0;
	TTree * MetaTree = load_metadata_tree(metadata_name);
//	cout << "Loaded up MetaTree." << endl;
	
	bool found_unused = false;
//	bool found_unused_notasum = false;
	int firstrun = 0;
	int run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
	int has_been_summed = 0;
	MetaTree -> SetBranchAddress("has_been_summed",  &has_been_summed);
	int is_a_sum = 0;
	MetaTree -> SetBranchAddress("is_a_sum",  &is_a_sum);
	
	vector<int> unsummed_runlist;
	int nentries = MetaTree -> GetEntries();
	for(int i=0; i<nentries; i++)
	{
		MetaTree -> GetEntry(i);
		if(!has_been_summed && !found_unused && !is_a_sum)
		{
			firstrun = run;
			found_unused = true;
			cout << "First run to look at (not a sum):  " << firstrun << endl;
		}
		if(!has_been_summed)
		{
			unsummed_runlist.push_back(run);
		}
	}
	if(!found_unused)
	{
		for(int i=0; i<nentries; i++)
		{
			MetaTree -> GetEntry(i);
			if(!has_been_summed && !found_unused)
			{
				firstrun = run;
				found_unused = true;
				cout << "First run to look at (is a sum):  " << firstrun << endl;
			}
		}
	}
	// if !found_unused ..... no...
	
	//
	int length_of_unsummedlist = unsummed_runlist.size();
	if(length_of_unsummedlist <= 1)
	{
		cout << "Bad!  Length of unsummed_runlist is:  " << length_of_unsummedlist << endl;
		cout << endl << endl;
	//	return 0;
	}
	vector<int> runs_to_hadd;
	runs_to_hadd.push_back(firstrun);
	//
	int newfilenum = 0;
	for(int i=0; i<length_of_unsummedlist; i++)
	{
		int does_it_work = can_hadd(firstrun, unsummed_runlist.at(i));
		if(verbose>0)
		{
			cout << "Comparing " << firstrun << ", " << unsummed_runlist.at(i);
			cout << ":  can_hadd = " << does_it_work << endl;
		}
		if(does_it_work>0)
		{
			if(verbose>1) {cout << "It works!" << endl;}
			runs_to_hadd.push_back(unsummed_runlist.at(i));
			newfilenum = does_it_work;
		}
		else
		{
			if(verbose>1) {cout << "It doesn't work.  :(" << endl;}
		}
	}
	// ok, now I have a list of runs that can be hadded together.
	//	runs_to_hadd.insert(runs_to_hadd.begin(), newfilenum);
	if(verbose>0)
	{
		cout << endl;
		cout << "New file number:  " << newfilenum << endl;
		cout << "Runs that can be hadded: " << endl;
		printout(runs_to_hadd);
	}
	hadder_runlist this_runlist;
	this_runlist.newfilenum = newfilenum;
	this_runlist.runlist = runs_to_hadd;
	
	return this_runlist;
}


string generate_hadd_command(int newrunno, vector<int> runlist, string output_namestub, string metafilename, string path_to_files) // path_to_files should have a "/" at the end.
{
//	int verbose = 0;
	TTree * MetaTree = load_metadata_tree(metafilename);
	int nentries = MetaTree -> GetEntries();
	
	int run = 0;
	MetaTree -> SetBranchAddress("Run", &run);
	char this_filename[256];
	MetaTree -> SetBranchAddress("Filename", &this_filename);
	
	string outstring;
	outstring = "hadd "+path_to_files+output_namestub+int_to_string(newrunno)+".root";
	
	int listsize = runlist.size();
	for(int j=0; j<listsize; j++)
	{
		for(int i=0; i<nentries; i++)
		{
			MetaTree -> GetEntry(i);
			if(run == runlist.at(j))
			{
				string this_filenamestring;
				stringstream ss;
				ss << this_filename;
				ss >> this_filenamestring;
				//
				outstring = outstring + " " +path_to_files+this_filenamestring;
			}
		}
	}
	return outstring;
}


string generate_hadd_command(int newrunno, vector<int> runlist)
{
	string output_namestub = "summedoutput_";
	string path_to_files = g4_path;
	return generate_hadd_command(newrunno, runlist, output_namestub, metadata_name, path_to_files);
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// ======================== // ======================== // ======================== //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

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

// vector<int> get_runlist_from_rho(TTree * MetaTree, double rho, int maxrun=0)


//
int can_hadd(int run1, int run2)
{
	bool relax_alreadysummed=false;
	return can_hadd(metadata_name, run1, metadata_name, run2, metadata_name, relax_alreadysummed);
}
int can_hadd(int run1, int run2, bool relax_alreadysummed)
{
	return can_hadd(metadata_name, run1, metadata_name, run2, metadata_name, relax_alreadysummed);
}
int can_hadd(string filename1, int run1, string filename2, int run2, string filename3, bool relax_alreadysummed=false)  // **this function** is what we have to adjust when we add new columns/branches to the Metadata file.
{
	int verbose = 0;
//	Run/I:Filename/C:has_been_summed/I:Efield/D:Rho/D:EventsGenerated/I:EventsSaved/I:SaveEventTypes/C:Polarization/D:Alignment/D:MinCosTheta/D:Efield_Uniformity/O:StepperType/I:StepperName/C:StepMax_mm/D:PhysicsListName/C:is_a_sum/I:Trap_x_mm/D:Trap_y_mm/D:Trap_z_mm/D:Trap_sigma_x/D:Trap_sigma_y/D:Trap_sigma_z/D:Temperature/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:SailVelocity_x_mm_per_ms/D:SailVelocity_y_mm_per_ms/D:SailVelocity_z_mm_per_ms/D:ChargeState/I
	if(verbose>1)
	{
		cout << "File 1:  " << filename1 << endl;
		cout << "File 2:  " << filename2 << endl;
		cout << "Output:  " << filename3 << endl;
	}
	
	TTree * MetaTree1 = load_metadata_tree(filename1);
	TTree * MetaTree2 = load_metadata_tree(filename2);
	int this_run1 = 0;
	int this_run2 = 0;
	MetaTree1 -> SetBranchAddress("Run", &this_run1);
	MetaTree2 -> SetBranchAddress("Run", &this_run2);
	int isbad1, isbad2;
	MetaTree1 -> SetBranchAddress("has_been_summed", &isbad1);
	MetaTree2 -> SetBranchAddress("has_been_summed", &isbad2);
	
	// more parameters that I'll care about later, but don't care about now.
//ExpandBeforePolarized_s/D:OP_CycleTime_s/D:SailVelocity_x_mm_per_ms/D:SailVelocity_y_mm_per_ms/D:SailVelocity_z_mm_per_ms/D:ChargeState/I	
	int nentries1 = MetaTree1 -> GetEntries();
	int i_run1 = -1;
	int i_run2 = -1;
	bool foundrun_1 = false;
	bool foundrun_2 = false;
	for(int i=0; i<nentries1; i++)
	{
		MetaTree1 -> GetEntry(i);
		if( this_run1 == run1 && !foundrun_1 && !isbad1)
		{
			i_run1 = i;
			foundrun_1 = true;
		//	if(isbad1)
		//	{ // this shouldn't happen if it's called from check_hadd_multi(...)
		//		cout << "No, run " << run1 << " has already been summed." << endl;
		//		return -1;
		//	}
		}
		else if( this_run1 == run1 && foundrun_1 && !isbad1)
		{
			cout << "This is bad.  Multiple runs with run number " << this_run1 << endl;
			return -1;
		}
	}
	for(int j=0; j<nentries1; j++)
	{
		MetaTree2 -> GetEntry(j);
		if( this_run2 == run2 && !foundrun_2 && !isbad2)
		{
			i_run2 = j;
			foundrun_2 = true;
		//	if(isbad2)
		//	{ // this shouldn't happen if it's called from check_hadd_multi(...)
		//		cout << "No, run " << run1 << " has already been summed." << endl;
		//		return -1;
		//	}
		}
		else if( this_run2 == run2 && foundrun_2 && !isbad2)
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
		if(verbose>0) { cout << "Bad!  You know those are literally the same run, right?" << endl; }
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
//	int isbad1, isbad2;
//	int issummed1, issummed2;
//	MetaTree1 -> SetBranchAddress("has_been_summed", &isbad1);
//	MetaTree2 -> SetBranchAddress("has_been_summed", &isbad2);
//	MetaTree1 -> SetBranchAddress("is_a_sum", &issummed1);
//	MetaTree2 -> SetBranchAddress("is_a_sum", &issummed2);
	
	//
	double Efield1, Efield2;
//	double rho1, rho2;
	double pol1, pol2;
	double align1, align2;
	double MonoEnergy1, MonoEnergy2;
	
	MetaTree1 -> SetBranchAddress("Efield",         &Efield1);
//	MetaTree1 -> SetBranchAddress("Rho",            &rho1);
	MetaTree1 -> SetBranchAddress("Polarization",   &pol1);
	MetaTree1 -> SetBranchAddress("Alignment",      &align1);
	MetaTree1 -> SetBranchAddress("MonoEnergy_MeV", &MonoEnergy1);
	
	MetaTree2 -> SetBranchAddress("Efield",         &Efield2);
//	MetaTree2 -> SetBranchAddress("Rho",            &rho2);
	MetaTree2 -> SetBranchAddress("Polarization",   &pol2);
	MetaTree2 -> SetBranchAddress("Alignment",      &align2);
	MetaTree2 -> SetBranchAddress("MonoEnergy_MeV", &MonoEnergy2);

	double Mz_1, Mz2_1, Mz3_1;
	double Mz_2, Mz2_2, Mz3_2;
	MetaTree1 -> SetBranchAddress("Mz",             &Mz_1);  //
	MetaTree1 -> SetBranchAddress("Mz2",            &Mz2_1); //
	MetaTree1 -> SetBranchAddress("Mz3",            &Mz3_1); //
	MetaTree2 -> SetBranchAddress("Mz",             &Mz_2);  //
	MetaTree2 -> SetBranchAddress("Mz2",            &Mz2_2); //
	MetaTree2 -> SetBranchAddress("Mz3",            &Mz3_2); //
	
	
	//
	double stepmax1, stepmax2;
	double mincostheta1, mincostheta2;
	int steppertype1, steppertype2;
//	int chargestate1, chargestate2;
	MetaTree1 -> SetBranchAddress("StepMin_mm",      &stepmax1);
	MetaTree1 -> SetBranchAddress("MinCosTheta",     &mincostheta1);
	MetaTree1 -> SetBranchAddress("StepperType",     &steppertype1);
//	MetaTree1 -> SetBranchAddress("ChargeState",     &chargestate1);
	MetaTree2 -> SetBranchAddress("StepMin_mm",      &stepmax2);
	MetaTree2 -> SetBranchAddress("MinCosTheta",     &mincostheta2);
	MetaTree2 -> SetBranchAddress("StepperType",     &steppertype2);
//	MetaTree2 -> SetBranchAddress("ChargeState",     &chargestate2);
	char this_StepperName1[256];
	char this_SaveEventTypes1[256];
	char this_PhysicsListName1[256];
	char this_EventGenerator1[256];
	
	char this_StepperName2[256];
	char this_SaveEventTypes2[256];
	char this_PhysicsListName2[256];
	char this_EventGenerator2[256];
	MetaTree1 -> SetBranchAddress("StepperName",     &this_StepperName1);
	MetaTree1 -> SetBranchAddress("SaveEventTypes",  &this_SaveEventTypes1);
	MetaTree1 -> SetBranchAddress("PhysicsListName", &this_PhysicsListName1);
	MetaTree1 -> SetBranchAddress("EventGenerator",  &this_EventGenerator1);  //
	MetaTree2 -> SetBranchAddress("StepperName",     &this_StepperName2);
	MetaTree2 -> SetBranchAddress("SaveEventTypes",  &this_SaveEventTypes2);
	MetaTree2 -> SetBranchAddress("PhysicsListName", &this_PhysicsListName2);
	MetaTree2 -> SetBranchAddress("EventGenerator",  &this_EventGenerator2);  //
	//
	double trap_x1i, trap_y1i, trap_z1i, trap_x1f, trap_y1f, trap_z1f;
	double trap_x2i, trap_y2i, trap_z2i, trap_x2f, trap_y2f, trap_z2f;
	MetaTree1 -> SetBranchAddress("Trap_x_i_mm",       &trap_x1i);
	MetaTree1 -> SetBranchAddress("Trap_y_i_mm",       &trap_y1i);
	MetaTree1 -> SetBranchAddress("Trap_z_i_mm",       &trap_z1i);
	MetaTree1 -> SetBranchAddress("Trap_x_f_mm",       &trap_x1f);
	MetaTree1 -> SetBranchAddress("Trap_y_f_mm",       &trap_y1f);
	MetaTree1 -> SetBranchAddress("Trap_z_f_mm",       &trap_z1f);
	MetaTree2 -> SetBranchAddress("Trap_x_i_mm",       &trap_x2i);
	MetaTree2 -> SetBranchAddress("Trap_y_i_mm",       &trap_y2i);
	MetaTree2 -> SetBranchAddress("Trap_z_i_mm",       &trap_z2i);
	MetaTree2 -> SetBranchAddress("Trap_x_f_mm",       &trap_x2f);
	MetaTree2 -> SetBranchAddress("Trap_y_f_mm",       &trap_y2f);
	MetaTree2 -> SetBranchAddress("Trap_z_f_mm",       &trap_z2f);
	
	double trap_sigmax1i, trap_sigmay1i, trap_sigmaz1i, trap_sigmax1f, trap_sigmay1f, trap_sigmaz1f;
	double trap_sigmax2i, trap_sigmay2i, trap_sigmaz2i, trap_sigmax2f, trap_sigmay2f, trap_sigmaz2f;
	MetaTree1 -> SetBranchAddress("Trap_sigma_x_i_mm",    &trap_sigmax1i);
	MetaTree1 -> SetBranchAddress("Trap_sigma_y_i_mm",    &trap_sigmay1i);
	MetaTree1 -> SetBranchAddress("Trap_sigma_z_i_mm",    &trap_sigmaz1i);
	MetaTree1 -> SetBranchAddress("Trap_sigma_x_f_mm",    &trap_sigmax1f);
	MetaTree1 -> SetBranchAddress("Trap_sigma_y_f_mm",    &trap_sigmay1f);
	MetaTree1 -> SetBranchAddress("Trap_sigma_z_f_mm",    &trap_sigmaz1f);
	MetaTree2 -> SetBranchAddress("Trap_sigma_x_i_mm",    &trap_sigmax2i);
	MetaTree2 -> SetBranchAddress("Trap_sigma_y_i_mm",    &trap_sigmay2i);
	MetaTree2 -> SetBranchAddress("Trap_sigma_z_i_mm",    &trap_sigmaz2i);
	MetaTree2 -> SetBranchAddress("Trap_sigma_x_f_mm",    &trap_sigmax2f);
	MetaTree2 -> SetBranchAddress("Trap_sigma_y_f_mm",    &trap_sigmay2f);
	MetaTree2 -> SetBranchAddress("Trap_sigma_z_f_mm",    &trap_sigmaz2f);
	
	/*
	double temp1x, temp1y, temp1z;
	double temp2x, temp2y, temp2z;
	MetaTree1 -> SetBranchAddress("Temperature_x_mK",     &temp1x);
	MetaTree1 -> SetBranchAddress("Temperature_y_mK",     &temp1y);
	MetaTree1 -> SetBranchAddress("Temperature_z_mK",     &temp1z);
	MetaTree2 -> SetBranchAddress("Temperature_x_mK",     &temp2x);
	MetaTree2 -> SetBranchAddress("Temperature_y_mK",     &temp2y);
	MetaTree2 -> SetBranchAddress("Temperature_z_mK",     &temp2z);
	double sail_x1, sail_y1, sail_z1;
	double sail_x2, sail_y2, sail_z2;
	MetaTree1 -> SetBranchAddress("SailVelocity_x_mm_per_ms",   &sail_x1);
	MetaTree1 -> SetBranchAddress("SailVelocity_y_mm_per_ms",   &sail_y1);
	MetaTree1 -> SetBranchAddress("SailVelocity_z_mm_per_ms",   &sail_z1);
	MetaTree2 -> SetBranchAddress("SailVelocity_x_mm_per_ms",   &sail_x2);
	MetaTree2 -> SetBranchAddress("SailVelocity_y_mm_per_ms",   &sail_y2);
	MetaTree2 -> SetBranchAddress("SailVelocity_z_mm_per_ms",   &sail_z2);
	*/
	
	double expansiontime1, expansiontime2;
	MetaTree1 -> SetBranchAddress("ExpandBeforePolarized_ms",   &expansiontime1);
	MetaTree2 -> SetBranchAddress("ExpandBeforePolarized_ms",   &expansiontime2);
	double OP_cycletime1, OP_cycletime2;
	MetaTree1 -> SetBranchAddress("OP_CycleTime_ms",   &OP_cycletime1);
	MetaTree2 -> SetBranchAddress("OP_CycleTime_ms",   &OP_cycletime2);

	char this_matchedrunset1[256];
	char this_matchedrunset2[256];
	MetaTree1 -> SetBranchAddress("matches_runset",     &this_matchedrunset1);
	MetaTree2 -> SetBranchAddress("matches_runset",     &this_matchedrunset2);

//	/C:has_been_summed/I/D:/D:Efield_Uniformity/O:/C:is_a_sum/I:/D:/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:

	// Now check to see whether *all* of the things are the same..
	MetaTree1 -> GetEntry(i_run1);
	MetaTree2 -> GetEntry(i_run2);
	bool match = true;
	//
	if(isbad1 || isbad2)       {match = false;}
		if(verbose>1 && !match) { cout << "One or more of the runs is 'bad'." << endl;         return -1;}
//	if(issummed1 || issummed2) {match = false;}
//		if(verbose>1 && !match) { cout << "One or more of the runs is already a sum." << endl; return -1;}
	//
	if( Efield1 != Efield2) {match = false;}
		if(verbose>1 && !match) { cout << "Efields are inconsistent." << endl;       return -1;}
//	if( rho1    != rho2)    {match = false;}
//		if(verbose>1 && !match) { cout << "rho values are inconsistent." << endl;    return -1;}
	if( pol1    != pol1)    {match = false;}
		if(verbose>1 && !match) { cout << "polarizations are inconsistent." << endl; return -1;}
	if( align1  != align2)  {match = false;}
		if(verbose>1 && !match) { cout << "alignments are inconsistent." << endl;    return -1;}
	//
	if( Mz_1  != Mz_2)   {match = false;}
	if( Mz2_1 != Mz2_2)  {match = false;}
	if( Mz3_1 != Mz3_2)  {match = false;}
	//
	if( stepmax1     != stepmax2)     {match = false;}
	if( mincostheta1 != mincostheta2) {match = false;}
	if( steppertype1 != steppertype2) {match = false;}
	//
	if( strcmp(this_StepperName1,    this_StepperName2)     !=0 ) {match=false;}
	if( strcmp(this_SaveEventTypes1, this_SaveEventTypes2)  !=0 ) {match=false;}
	if( strcmp(this_PhysicsListName1,this_PhysicsListName2) !=0 ) {match=false;}
	if( strcmp(this_EventGenerator1, this_EventGenerator2)  !=0 ) {match=false;}
	//
	if( trap_x1i        != trap_x2i)        {match = false;}
	if( trap_y1i        != trap_y2i)        {match = false;}
	if( trap_z1i        != trap_z2i)        {match = false;}
	if( trap_x1f        != trap_x2f)        {match = false;}
	if( trap_y1f        != trap_y2f)        {match = false;}
	if( trap_z1f        != trap_z2f)        {match = false;}
	if( trap_sigmax1i   != trap_sigmax2i)   {match = false;}
	if( trap_sigmay1i   != trap_sigmay2i)   {match = false;}
	if( trap_sigmaz1i   != trap_sigmaz2i)   {match = false;}
	if( trap_sigmax1f   != trap_sigmax2f)   {match = false;}
	if( trap_sigmay1f   != trap_sigmay2f)   {match = false;}
	if( trap_sigmaz1f   != trap_sigmaz2f)   {match = false;}
	
//	if( temp1x         != temp2x)         {match = false;}
//	if( temp1y         != temp2y)         {match = false;}
//	if( temp1z         != temp2z)         {match = false;}
	
	if ( (MonoEnergy1 == -10 || MonoEnergy2 == -10) && (MonoEnergy1 != MonoEnergy2) )    {match = false;} // if they don't match *and* one of them is "make the whole spectrum"
	// in the future, kludge here to make it not hadd multiple monoenergies?
	
	//
	if( expansiontime1 != expansiontime2) {match = false;}
	if( OP_cycletime1  != OP_cycletime2)  {match = false;}
	if( strcmp(this_matchedrunset1,this_matchedrunset2) !=0 ) {match=false;}
	
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
	int newrunno = 0;
	newrunno = get_newrunno(filename3);
	return newrunno;
}

//
int get_newrunno(string metafilename)
{
	int verbose = 0;
	int newrunno = 1;

	// figure out what new run number to use for the new, hadded file:
	TTree * MetaTree3 = load_metadata_tree(metafilename);
	int nentries3 = MetaTree3 -> GetEntries();

	int this_runno;
	MetaTree3 -> SetBranchAddress("Run", &this_runno);
	int is_a_sum;
	MetaTree3 -> SetBranchAddress("is_a_sum", &is_a_sum);
	
	for(int i=0; i<nentries3; i++)
	{
		MetaTree3 -> GetEntry(i);
		if( (newrunno == this_runno || newrunno == this_runno-extrafilenum ) && is_a_sum==1) // if this entry is a sum, and ... what??
		{
			newrunno++;
			i=-1;
		}
	}
	//
	int mynewfilenum = extrafilenum+newrunno;
	while(mynewfilenum > 2*extrafilenum && extrafilenum>=10)
	{
		mynewfilenum = mynewfilenum - extrafilenum;
		cout << "New file number:  " << mynewfilenum << endl;
	}
	// 
	if(verbose>0)
	{
		cout << "Files will be hadded into a new file with run number:\t" << mynewfilenum << endl;
	}
	return mynewfilenum;
}

vector<int> get_delimiter_positions(string thisline)
{
	int verbose=0;
	
	size_t positioncounter = 0;
	size_t last_position = 0;
	int n_found = 0;
	vector<int> delimiter_positions;
	
	int size_of_line = thisline.size();
	while ( positioncounter<size_of_line )
	{
		positioncounter = thisline.find(":", last_position+1);
		if(positioncounter<size_of_line)
		{
			last_position = positioncounter;
			delimiter_positions.push_back(last_position);
			n_found++;
		}
	}
	// Add a fake delimiter at the end:
	delimiter_positions.push_back(size_of_line);  // +1, or not?  probably doesn't matter, because we only need to have some other delimiter after the last real delimiter.
	
	if(verbose>0) cout << "n_found = " << n_found << ",\t size=" << delimiter_positions.size() << endl;
	if(verbose>0) cout << "** delimiter n_found = " << n_found << ",\tlast_position = " << last_position << endl;
	
	return delimiter_positions;
}
int get_position_of_string(string thingtolookin, string thingtolookfor)
{
	int positioncounter = 0;
//	size_t last_position = 0;
	int size_of_string = thingtolookin.size();
	int position_of_thingimlookingfor = 0;
	while (positioncounter<size_of_string)
	{
		positioncounter = thingtolookin.find(thingtolookfor.c_str(), 0);
	//	last_position = positioncounter;
		if(positioncounter<size_of_string)
		{
			position_of_thingimlookingfor = positioncounter;
			break;
		}
	}
	return position_of_thingimlookingfor;
}
int find_branchpos(int position_of_thing, vector<int> delimiter_positions, int totalsize)
{
	int verbose=0;
	int this_branchpos = 0;
	if(verbose>0) cout << "totalsize = " << totalsize << endl;
	for(int i=0; i<totalsize; i++)
	{	
		if(verbose>1) cout << "i= " << i << ";\tposition_of_thing=" << position_of_thing << ";\tdelimiter_position=" << delimiter_positions.at(i) << endl;
		if( position_of_thing > delimiter_positions.at(i) ) { this_branchpos++; }
		else { break; }
	}
	if(verbose>0) cout << "branchpos has been found.  returning:  " << this_branchpos << endl;
	return this_branchpos;
}


lineinfo get_lineinfo_for_run(int runno, string metafilename)
{
//	bool verbose = false;
	int this_iline = 0;
	int this_runno;
	string this_linestring;
	string linestring_tokeep = "";
	//
	std::ifstream this_ifilestream;
	this_ifilestream.open(metafilename.c_str());
	//
	std::getline(this_ifilestream, this_linestring); // don't look at the first line...
	int i=1;                                         // we've already gotten the first line.
	while ( std::getline(this_ifilestream, this_linestring) )
	{
		i++;
		std::istringstream iss(this_linestring);
		iss >> this_runno;
		if(this_runno == runno)
		{
			this_iline = i;
			linestring_tokeep = this_linestring;
		//	if(verbose) cout << "i_line1=" << i_line1 << "\tfirstrunline:  " << endl;
		//	if(verbose) cout << firstrunline << endl;
		}
	}
	
	lineinfo this_lineinfo;
	this_lineinfo.linenumber = this_iline;
	this_lineinfo.linestring = linestring_tokeep;
	return this_lineinfo;
}

/*
int find_linenumber_for_run(int runno, string metafilename)
{
	bool verbose = false;
	int this_iline = 0;
	string this_line;
	int this_runno;
	//
	std::ifstream this_ifilestream;
	this_ifilestream.open(metafilename.c_str());
	//
	std::getline(this_ifilestream, this_line); // don't look at the first line...
	int i=1;                                   // we've already gotten the first line.
	while ( std::getline(this_ifilestream, this_line) )
	{
		i++;
		std::istringstream iss(this_line);
		iss >> this_runno;
		if(this_runno == runno)
		{
			this_iline = i;
		//	firstrunline = this_line;
		//	if(verbose) cout << "i_line1=" << i_line1 << "\tfirstrunline:  " << endl;
		//	if(verbose) cout << firstrunline << endl;
		}
	}
	return this_iline;
}
string getlinestring_for_run(int runno, string metafilename)
{
	bool verbose = false;
	int this_iline = 0;
	string this_line;
	int this_runno;
	string linestring_tokeep = "";
	//
	std::ifstream this_ifilestream;
	this_ifilestream.open(metafilename.c_str());
	//
	std::getline(this_ifilestream, this_line); // don't look at the first line...
	int i=1;                                   // we've already gotten the first line.
	while ( std::getline(this_ifilestream, this_line) )
	{
		i++;
		std::istringstream iss(this_line);
		iss >> this_runno;
		if(this_runno == runno)
		{
			this_iline = i;
			linestring_tokeep = this_line;
		//	if(verbose) cout << "i_line1=" << i_line1 << "\tfirstrunline:  " << endl;
		//	if(verbose) cout << firstrunline << endl;
		}
	}
	return linestring_tokeep;
}
*/

string make_thenewstringline(vector<int> delimiter_positions, int newrunno, int total_ngenerated, int total_nsaved, double total_monoenergy, string firstrunline, int branchpos_runno, int branchpos_eventsgenerated, int branchpos_eventssaved, /*int branchpos_isbad,*/ int branchpos_issummed, int branchpos_filename, int branchpos_monoenergy)
{
	bool verbose=false;

	if(verbose)
	{
		if(verbose) cout << "branchpos_runno           = " << branchpos_runno << endl;
		if(verbose) cout << "branchpos_eventsgenerated = " << branchpos_eventsgenerated << endl;
		if(verbose) cout << "branchpos_eventssaved     = " << branchpos_eventssaved << endl;
	//	if(verbose) cout << "branchpos_isbad           = " << branchpos_isbad << endl;
		if(verbose) cout << "branchpos_issummed        = " << branchpos_issummed << endl;
		if(verbose) cout << "branchpos_filename        = " << branchpos_filename << endl;
		if(verbose) cout << "branchpos_monoenergy      = " << branchpos_monoenergy << endl;
	}
	
	std::stringstream ss_out;
	ss_out.str( std::string() );
	ss_out.clear();
	ss_out << firstrunline;
	
	std::stringstream ss_tmp;
	ss_tmp.str( std::string() );
	ss_tmp.clear();
	
	string newstringterm = "";
	string newstringline = "";
	
	int n_found = delimiter_positions.size();
	for(int i=0; i<n_found; i++)
	{
		if( !ss_out.good() ) 
		{ 
			cout << "Entry is shorter than expected.  Bad!" << endl;
		//	return false;
			break; 
		}
		ss_out >> newstringterm;
		
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
		if(i == branchpos_monoenergy)
		{
			newstringterm = "";
			ss_tmp.str( std::string() );
			ss_tmp.clear();
			ss_tmp << std::fixed << std::setprecision(6) << total_monoenergy;
			ss_tmp >> newstringterm;
	//		cout << "total_monoenergy = " << total_monoenergy << ";\tnewstringterm = " << newstringterm << endl;
		}
		if( i != 0 )
		{
			newstringline = newstringline+"\t";
		}
		newstringline = newstringline+newstringterm;
		if(verbose) cout << "i=" << i << ";\tnewstringline=" << newstringline << endl;
	}
//	newstringline = newstringline+"\t\n";
	newstringline = newstringline+"\t";
	
	if(verbose) 
	{
		cout << endl;
		cout << "newstringline:" << endl; 
		cout << newstringline << endl;
	}
	return newstringline;
}

//
/*
bool do_the_hadding(int run1, int run2, int newrunno=1)
{
	bool verbose = true;
	string current_metadatafilename = metadata_name;
	
// run1 and run2 are the run numbers, not the i_line for the metadata file.
// this function breaks if run number isn't the first entry in every line.

// Run/I:Filename/C:has_been_summed/I:Efield/D:Rho/D:EventsGenerated/I:EventsSaved/I:SaveEventTypes/C:Polarization/D:Alignment/D:MinCosTheta/D:Efield_Uniformity/O:StepperType/I:StepperName/C:StepMax_mm/D:PhysicsListName/C:is_a_sum/I:Trap_x_mm/D:Trap_y_mm/D:Trap_z_mm/D:Trap_sigma_x/D:Trap_sigma_y/D:Trap_sigma_z/D:Temperature/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:SailVelocity_x_mm_per_ms/D:SailVelocity_y_mm_per_ms/D:SailVelocity_z_mm_per_ms/D:ChargeState/I
	string this_line;
	std::ifstream this_ifilestream;
	this_ifilestream.open(current_metadatafilename.c_str());
	std::getline(this_ifilestream, this_line);
	this_ifilestream.close();
	// We're done with the file for now...
	
	vector<int> delimiter_positions;
	delimiter_positions = get_delimiter_positions(this_line);
	int length_of_line = this_line.size();
	// ok, we know where the delimiters are.
//	int positioncounter = 0;
//	size_t last_position = 0;
	
	int position_of_eventsgenerated = get_position_of_string(this_line, "EventsGenerated");
	int position_of_eventssaved     = get_position_of_string(this_line, "EventsSaved");
	int position_of_runno           = get_position_of_string(this_line, "Run");
	int position_of_isbad           = get_position_of_string(this_line, "has_been_summed");
	int position_of_issummed        = get_position_of_string(this_line, "is_a_sum");
	int position_of_filename        = get_position_of_string(this_line, "Filename");
	int position_of_monoenergy      = get_position_of_string(this_line, "MonoEnergy_MeV");
	if(verbose) cout << "position_of_eventsgenerated = " << position_of_eventsgenerated << endl;
	if(verbose) cout << "position_of_eventssaved     = " << position_of_eventssaved << endl;
	if(verbose) cout << "position_of_runno           = " << position_of_runno << endl;
	if(verbose) cout << "position_of_isbad           = " << position_of_isbad << endl;
	if(verbose) cout << "position_of_issummed        = " << position_of_issummed << endl;
	if(verbose) cout << "position_of_filename        = " << position_of_filename << endl;
	if(verbose) cout << "position_of_monoenergy      = " << position_of_monoenergy << endl;
	if(verbose) cout << "--" << endl;
	
	// ok, we know where the EventsGenerated and EventsSaved entries are.  
	//    ...also Run, has_been_summed, and is_a_sum.
	// now figure out where they go wrt the delimiters.
	int branchpos_eventsgenerated = find_branchpos(position_of_eventsgenerated, \
		delimiter_positions, length_of_line);
	int branchpos_eventssaved     = find_branchpos(position_of_eventssaved, \
		delimiter_positions, length_of_line);
	int branchpos_runno           = find_branchpos(position_of_runno, \
		delimiter_positions, length_of_line);
	int branchpos_isbad           = find_branchpos(position_of_isbad, \
		delimiter_positions, length_of_line);
	int branchpos_issummed        = find_branchpos(position_of_issummed, \
		delimiter_positions, length_of_line);
	int branchpos_filename        = find_branchpos(position_of_filename, \
		delimiter_positions, length_of_line);
	int branchpos_monoenergy        = find_branchpos(position_of_monoenergy, \
		delimiter_positions, length_of_line);

	if(verbose) cout << "branchpos_eventsgenerated = " << branchpos_eventsgenerated << endl;
	if(verbose) cout << "branchpos_eventssaved     = " << branchpos_eventssaved << endl;
	if(verbose) cout << "branchpos_runno           = " << branchpos_runno << endl;
	if(verbose) cout << "branchpos_isbad           = " << branchpos_isbad << endl;
	if(verbose) cout << "branchpos_issummed        = " << branchpos_issummed << endl;
	if(verbose) cout << "branchpos_filename        = " << branchpos_filename << endl;
	if(verbose) cout << "branchpos_monoenergy      = " << branchpos_monoenergy << endl;
	if(verbose) cout << "--" << endl;
	// numbering starts at "0".
	// k, we're done fucking around with the first line.
	
	lineinfo first_lineinfo  = get_lineinfo_for_run(run1, current_metadatafilename);
	lineinfo second_lineinfo = get_lineinfo_for_run(run2, current_metadatafilename);
	
	string firstrunline  = first_lineinfo.linestring;
	string secondrunline = second_lineinfo.linestring;
	int i_line1 = first_lineinfo.linenumber;
	int i_line2 = second_lineinfo.linenumber;

	if(verbose) cout << "i_line1=" << i_line1 << "\tfirstrunline:  " << endl;
	if(verbose) cout << firstrunline << endl;
	if(verbose) cout << "i_line2=" << i_line2 << "\tsecondrunline:  " << endl;
	if(verbose) cout << secondrunline << endl;
	// ok.  I've gotten the info that I need out of this file.  I'm done with it for now.  
	// Later, I'll have to open that file back up to do things to it, but not now.
	
	// ok, now read out the info from those lines, and put them into a new line.
	std::stringstream ss1;
	ss1.str( std::string() );
	ss1.clear();
	ss1 << firstrunline;

	std::stringstream ss2;
	ss2.str( std::string() );
	ss2.clear();
	ss2 << secondrunline;
	
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
	if(verbose) cout << "n_eventsgenerated1 = " << n_eventsgenerated1 << endl;
	ss2 >> n_eventsgenerated2;
	if(verbose) cout << "n_eventsgenerated2 = " << n_eventsgenerated2 << endl;
	
	ss1.str( std::string() );
	ss1.clear();
	ss1 << firstrunline;
	ss2.str( std::string() );
	ss2.clear();
	ss2 << secondrunline;
	for(int i=0; i<branchpos_eventssaved; i++)
	{
		ss1 >> throwparam;
		ss2 >> throwparam;
	//	cout << "i = " << i << ",\tthrowparam=" << throwparam << endl;
	}
	int n_eventssaved1;
	int n_eventssaved2;
	ss1 >> n_eventssaved1;
	if(verbose) cout << "n_eventssaved1 = " << n_eventssaved1 << endl;
	ss2 >> n_eventssaved2;
	if(verbose) cout << "n_eventssaved2 = " << n_eventssaved2 << endl;
	
	int total_ngenerated = n_eventsgenerated1 + n_eventsgenerated2;
	int total_nsaved = n_eventssaved1 + n_eventssaved2;
	if(verbose) cout << "--" << endl;
	if(verbose) cout << "total_ngenerated = " << total_ngenerated << endl;
	if(verbose) cout << "total_nsaved = " << total_nsaved << endl;
	
	
	//
	ss1.str( std::string() );
	ss1.clear();
	ss1 << firstrunline;
	ss2.str( std::string() );
	ss2.clear();
	ss2 << secondrunline;
	for(int i=0; i<branchpos_monoenergy; i++)
	{
		ss1 >> throwparam;
		ss2 >> throwparam;
	}
	int this_monoenergy1;
	int this_monoenergy2;
	ss1 >> this_monoenergy1;
	if(verbose) cout << "this_monoenergy1 = " << this_monoenergy1 << endl;
	ss2 >> this_monoenergy2;
	if(verbose) cout << "this_monoenergy2 = " << this_monoenergy2 << endl;
	
	double total_monoenergy = -10;
	if(this_monoenergy1 != this_monoenergy2 && this_monoenergy1 != -10 && this_monoenergy2 != -10)
	{
		total_monoenergy = -5.0;
	}
	else if( this_monoenergy1 != this_monoenergy2 ) // this should never happen.
	{
		total_monoenergy = -3.0;
	}
	else
	{
		total_monoenergy = this_monoenergy1;
	}
	//
	
	// assign a new run number?  somehow?!
//	if(verbose) cout << "newrunno = " << newrunno << endl;

	std::stringstream ss_out;
	ss_out.str( std::string() );
	ss_out.clear();
	ss_out << firstrunline;
	
	std::stringstream ss_tmp;
	ss_tmp.str( std::string() );
	ss_tmp.clear();
	
	
	string newstringline = "";
	newstringline = make_thenewstringline(delimiter_positions, newrunno, total_ngenerated, total_nsaved, total_monoenergy, firstrunline, branchpos_runno, branchpos_eventsgenerated, branchpos_eventssaved, branchpos_issummed, branchpos_filename, branchpos_monoenergy);
	
	first_lineinfo = update_runline(first_lineinfo, branchpos_isbad, delimiter_positions.size());
	second_lineinfo = update_runline(second_lineinfo, branchpos_isbad, delimiter_positions.size());
	firstrunline  = first_lineinfo.linestring;
	secondrunline = second_lineinfo.linestring;
	i_line1 = first_lineinfo.linenumber;
	i_line2 = second_lineinfo.linenumber;
	vector<lineinfo> lineinfo_forruns;
	lineinfo_forruns.push_back(first_lineinfo);
	lineinfo_forruns.push_back(second_lineinfo);
	
	// ok, the lines are updated.  but now I need to read the whole (updated) file into a buffer...
//	string filecontents = "";
	string filecontents = make_filebuffer(lineinfo_forruns, newstringline, current_metadatafilename);

	std::ofstream this_ofilestream;
	this_ofilestream.open(current_metadatafilename.c_str(), std::ios::trunc);
	if( !this_ofilestream.is_open() )
	{
		cout << "Couldn't open the file for writing." << endl;
		return false;
	}
	else
	{
		if(verbose) cout << "Yay, the file's open for writing!" << endl;
	}
	this_ofilestream << filecontents;
//	this_ofilestream << newstringline;
	this_ofilestream.close();
	
	return true;
}
*/

bool do_the_hadding(int run1, int run2, int newrunno=1)
{
	vector<int> runlist;
	runlist.push_back(run1);
	runlist.push_back(run2);
	
	return do_the_hadding(newrunno, runlist);
}

bool do_the_hadding(int newrunno, vector<int> runlist) // should probably add an option to specify the metadata filename...
{
	int verbose = 0;
	string current_metadatafilename = metadata_name;

	string this_line;
	std::ifstream this_ifilestream;
	this_ifilestream.open(current_metadatafilename.c_str());
	std::getline(this_ifilestream, this_line);
	this_ifilestream.close();
	// We're done with the file for now...
	
	vector<int> delimiter_positions;
	delimiter_positions = get_delimiter_positions(this_line);
	if(verbose>2) printout(delimiter_positions);
	
	int length_of_line = this_line.size();
	if(verbose>1) cout << "length_of_line = " << length_of_line << endl;
	// ok, we know where the delimiters are.
	
	int position_of_eventsgenerated = get_position_of_string(this_line, "EventsGenerated");
	int position_of_eventssaved     = get_position_of_string(this_line, "EventsSaved");
	int position_of_runno           = get_position_of_string(this_line, "Run");
	int position_of_isbad           = get_position_of_string(this_line, "has_been_summed");
	int position_of_issummed        = get_position_of_string(this_line, "is_a_sum");
	int position_of_filename        = get_position_of_string(this_line, "Filename");
	int position_of_monoenergy      = get_position_of_string(this_line, "MonoEnergy_MeV");
	if(verbose>1) cout << "position_of_eventsgenerated = " << position_of_eventsgenerated << endl;
	if(verbose>1) cout << "position_of_eventssaved     = " << position_of_eventssaved << endl;
	if(verbose>1) cout << "position_of_runno           = " << position_of_runno << endl;
	if(verbose>1) cout << "position_of_isbad           = " << position_of_isbad << endl;
	if(verbose>1) cout << "position_of_issummed        = " << position_of_issummed << endl;
	if(verbose>1) cout << "position_of_filename        = " << position_of_filename << endl;
	if(verbose>1) cout << "position_of_monoenergy      = " << position_of_monoenergy << endl;
	if(verbose>1) cout << "--" << endl;
	
	// ok, we know where the EventsGenerated and EventsSaved entries are.  
	//    ...also Run, has_been_summed, and is_a_sum.
	// now figure out where they go wrt the delimiters.
	int branchpos_eventsgenerated = find_branchpos(position_of_eventsgenerated, delimiter_positions, length_of_line);
	int branchpos_eventssaved     = find_branchpos(position_of_eventssaved,     delimiter_positions, length_of_line);
	int branchpos_runno           = find_branchpos(position_of_runno,           delimiter_positions, length_of_line);
	int branchpos_isbad           = find_branchpos(position_of_isbad,           delimiter_positions, length_of_line);
	int branchpos_issummed        = find_branchpos(position_of_issummed,        delimiter_positions, length_of_line);
	int branchpos_filename        = find_branchpos(position_of_filename,        delimiter_positions, length_of_line);
	int branchpos_monoenergy      = find_branchpos(position_of_monoenergy,      delimiter_positions, length_of_line);

	if(verbose>1) cout << "branchpos_eventsgenerated = " << branchpos_eventsgenerated << endl;
	if(verbose>1) cout << "branchpos_eventssaved = " << branchpos_eventssaved << endl;
	if(verbose>1) cout << "branchpos_runno = " << branchpos_runno << endl;
	if(verbose>1) cout << "branchpos_isbad = " << branchpos_isbad << endl;
	if(verbose>1) cout << "branchpos_issummed = " << branchpos_issummed << endl;
	if(verbose>1) cout << "branchpos_filename = " << branchpos_filename << endl;
	if(verbose>1) cout << "branchpos_monoenergy = " << branchpos_monoenergy << endl;
	if(verbose>1) cout << "--" << endl;
	
	
	// numbering starts at "0".
	// k, we're done fucking around with the header line.
//	vector<int> runlist
//	lineinfo first_lineinfo  = get_lineinfo_for_run(run1, current_metadatafilename);
//	lineinfo second_lineinfo = get_lineinfo_for_run(run2, current_metadatafilename);

	int length_of_runlist = runlist.size();
	vector<lineinfo> lineinfo_forruns;
	lineinfo thisline_info;
	
	if(verbose>1) cout << "length_of_runlist = " << length_of_runlist << endl;
	for(int i=0; i<length_of_runlist; i++)
	{
	//	cout << "i = " << i << endl;
		thisline_info = get_lineinfo_for_run(runlist.at(i), current_metadatafilename);
		lineinfo_forruns.push_back(thisline_info);
	//	if(verbose>2) cout << " (i=" << i << ", linenumber=" << thisline_info.linenumber << "):  " << thisline_info.linestring << endl;
	}
	
	
	// ok, we have a vector of line info stuff.  now what?
	
	// Get total number of events generated and events saved.
	vector<double> mono_energies;
	vector<int> n_eventsgenerated;
	vector<int> n_eventssaved;
	stringstream ss;
	ss.str( std::string() );
	ss.clear();
	string throwparam = "";
	for(int i=0; i<length_of_runlist; i++)
	{
		ss.str( std::string() );
		ss.clear();
		ss << (lineinfo_forruns.at(i)).linestring;
		for(int j=0; j<branchpos_eventsgenerated; j++)
		{
			ss >> throwparam;
		}
		int this_eventsgenerated;
		ss >> this_eventsgenerated;
		n_eventsgenerated.push_back(this_eventsgenerated);
	}
	ss.str( std::string() );
	ss.clear();
	for(int i=0; i<length_of_runlist; i++)
	{
		ss.str( std::string() );
		ss.clear();
		ss << (lineinfo_forruns.at(i)).linestring;
		for(int j=0; j<branchpos_eventssaved; j++)
		{
			ss >> throwparam;
		}
		int this_eventssaved;
		ss >> this_eventssaved;
		n_eventssaved.push_back(this_eventssaved);
	}
	//
	ss.str( std::string() );
	ss.clear();
	for(int i=0; i<length_of_runlist; i++)
	{
		ss.str( std::string() );
		ss.clear();
		ss << (lineinfo_forruns.at(i)).linestring;
		for(int j=0; j<branchpos_monoenergy; j++)
		{
			ss >> throwparam;
		}
		double this_monoenergy;
		ss >> this_monoenergy;
		mono_energies.push_back(this_monoenergy);
	}
	//
	if(verbose)
	{
		cout << endl << "Vector of events generated:" << endl;
		printout(n_eventsgenerated);
		cout << endl << "Vector of events saved:" << endl;
		printout(n_eventssaved);
	}
	int total_eventsgenerated = 0;
	int total_eventssaved = 0;
	double total_monoenergy = 0;
	for(int i=0; i<length_of_runlist; i++)
	{
		total_eventsgenerated = total_eventsgenerated+n_eventsgenerated.at(i);
		total_eventssaved     = total_eventssaved+n_eventssaved.at(i);
		if(verbose>2) cout << "mono_energies.at(" << i << " << )=" << mono_energies.at(i) << endl;
		if( mono_energies.at(i) != -10.0 ) // if not all energies
		{
			if( i>=1 && mono_energies.at(i) != mono_energies.at(i-1) ) // if the mono-energies aren't the same
			{
				total_monoenergy = -5.0;
			}
		}
		else
		{
			total_monoenergy = -10.0;
		}
	}
	if(verbose)
	{
		cout << "Total events generated:  " << total_eventsgenerated << endl;
		cout << "Total events saved:  " << total_eventssaved << endl;
	}
	// done getting events generated and events saved.
	
	string newstringline;
	newstringline = make_thenewstringline(delimiter_positions, newrunno, total_eventsgenerated, total_eventssaved, total_monoenergy, (lineinfo_forruns.at(0)).linestring, branchpos_runno, branchpos_eventsgenerated, branchpos_eventssaved,  branchpos_issummed, branchpos_filename, branchpos_monoenergy);
	
//	if(verbose)
//	{
//	//	cout << "newstringline: " << endl;
//	//	cout << newstringline << endl;
//	}

	// ok, we have the new line for the run that will be hadded.  How will we update the old run lines?
	vector<lineinfo> oldstringlines;
	for(int i=0; i<length_of_runlist; i++)
	{
		thisline_info = lineinfo_forruns.at(i);
		lineinfo_forruns.at(i) = update_runline(thisline_info, branchpos_isbad, delimiter_positions.size());
	}
	// ok, now read everything ever into a buffer to put back into the file again.
	string filecontents = make_filebuffer(lineinfo_forruns, newstringline, current_metadatafilename);
	
	if(verbose>2)
	{
		cout << endl;
		cout << "New file contents:  " << endl;
		cout << filecontents << endl;
	}
	
	std::ofstream this_ofilestream;
	this_ofilestream.open(current_metadatafilename.c_str(), std::ios::trunc);
	if( !this_ofilestream.is_open() )
	{
		cout << "Couldn't open the file for writing." << endl;
		return false;
	}
	else
	{
		if(verbose) cout << "Yay, the file's open for writing!" << endl;
	}
	this_ofilestream << filecontents;
	this_ofilestream.close();
	

	return true;
}
//
string make_filebuffer(vector<lineinfo> lineinfo_forruns, string newstringline, string metafname)
{
	string filecontents = "";
	
	std::ifstream this_ifilestream;
	this_ifilestream.open(metafname.c_str());
	if( !this_ifilestream.is_open() )
	{
		cout << "Couldn't open the file for reading this time around.." << endl;
		return filecontents;
	//	return false;
	}
	// read in the whole file into a .... buffer of some sort.
	string this_line;
	std::getline(this_ifilestream, this_line);
	filecontents = filecontents+this_line+"\n";
	int i_line = 1;
	
	string this_new_line = "";
	while ( std::getline(this_ifilestream, this_line) )
	{
		i_line++;
		
		bool linematch = false;
		int i=0;
		for(i=0; i<lineinfo_forruns.size(); i++)
		{
			if(i_line == (lineinfo_forruns.at(i)).linenumber)
			{
				linematch=true;
				break;
			}
		}
		if(!linematch)
		{
			filecontents = filecontents+this_line+"\n";
		}
		else
		{
			this_new_line = (lineinfo_forruns.at(i)).linestring;
			filecontents = filecontents+this_new_line+"\n";
		}
	}
	filecontents = filecontents+newstringline+"\n";
	//
	this_ifilestream.close();
	return filecontents;
}

lineinfo update_runline(lineinfo thisline_info, int branchpos_isbad, int n_delimiters)
{
	bool verbose=false;
	string this_new_line = "";
	string this_term = "";

	std::istringstream iss2(thisline_info.linestring);
	iss2 >> this_term;
	this_new_line = string(this_term);
	this_new_line = this_new_line+"\t";

	for(int i=1; i<n_delimiters; i++)
	{
		iss2 >> this_term;
		if(i==branchpos_isbad)
		{
			this_new_line = this_new_line+"1\t";
		}
		else
		{
			this_new_line = this_new_line+this_term+"\t";
		}
	}
	
	if(verbose) cout << "this_new_line:" << endl;
	if(verbose) cout << this_new_line << endl;
	
	thisline_info.linestring = this_new_line;
	return thisline_info;
}

// Not used?  Also, unreadable?
/*
int try_to_hadd(int run1, int run2)
{
	int can_behadded = -1;
	bool is_hadded = false;
	
	can_behadded = can_hadd(run1, run2);
	
	int mynewfilenum = extrafilenum+can_behadded;
	while(mynewfilenum > 2*extrafilenum && extrafilenum>=10)
	{
		mynewfilenum = mynewfilenum - extrafilenum;
		cout << "New file number:  " << mynewfilenum << endl;
	}
	
	if(can_behadded>=0) 
	{ 
		is_hadded = do_the_hadding(run1, run2, mynewfilenum); 
		return mynewfilenum;
	}
	else
	{
		cout << "Can't hadd." << endl;
		mynewfilenum = -1;
		return mynewfilenum;
	//	return -1;
	}
	return mynewfilenum;
//	return is_hadded;
}
*/


// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// ======================== // ======================== // ======================== //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
