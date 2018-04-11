
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
#include "MetaChain.cpp"

int extrafilenum = 0;

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
string int_to_string(int);
int can_hadd(int, int);
int can_hadd(string, int, string, int, string);
bool do_the_hadding(int, int, int);
int try_to_hadd(int, int);
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

int main(int argc, char *argv[]) 
{
	int run1, run2;
	string fname1, fname2;
	if(argc == 3) // 2 arguments...
	{
		run1 = atoi(argv[1]);
		run2 = atoi(argv[2]);
	}
//	else if(argc == 5)
//	{
//		run1 = atoi(argv[1]);
//	//	fname1 = atoi(argv[1]);
//		run2 = atoi(argv[3]);
//	//	fname2 = atoi(argv[4]);
//		cout << "not really implemented yet..." << endl;
//		return 0;
//	}
	else
	{
		cout << "Can't hadd.  Wrong number of arguments." << endl;
		return -1;
	}
	int filenum = try_to_hadd(run1, run2);
	cout << "filenum = " << filenum << endl;
//	return 260;
	return filenum;
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


int can_hadd(int run1, int run2)
{
	return can_hadd(metadata_name, run1, metadata_name, run2, metadata_name);
}

// No friends in this one.
// Combine by explicit runno.
int can_hadd(string filename1, int run1, string filename2, int run2, string filename3)
{
	int verbose = 2;
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
	MetaTree1 -> SetBranchAddress("has_been_summed", &isbad1);
	MetaTree2 -> SetBranchAddress("has_been_summed", &isbad2);
	MetaTree1 -> SetBranchAddress("is_a_sum", &issummed1);
	MetaTree2 -> SetBranchAddress("is_a_sum", &issummed2);
	
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
	if( rho1    != rho2)    {match = false;}
		if(verbose>1 && !match) { cout << "rho values are inconsistent." << endl;    return -1;}
	if( pol1    != pol1)    {match = false;}
		if(verbose>1 && !match) { cout << "polarizations are inconsistent." << endl; return -1;}
	if( align1  != align2)  {match = false;}
		if(verbose>1 && !match) { cout << "alignments are inconsistent." << endl;    return -1;}
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
	TTree * MetaTree3 = load_metadata_tree(filename3);
	int this_runno;
	MetaTree3 -> SetBranchAddress("Run", &this_runno);
	int is_a_sum;
	MetaTree3 -> SetBranchAddress("is_a_sum", &is_a_sum);
	
	int newrunno = 0;
	for(int i=0; i<nentries1; i++)
	{
		MetaTree3 -> GetEntry(i);
	//	cout << "i=" << i << ",\tthis_runno=" << this_runno << ",\tnewrunno=" << newrunno << endl;
		if( (newrunno == this_runno || newrunno == this_runno-extrafilenum ) && is_a_sum==1)
		{
			newrunno++;
			i=-1;
		}
	}
	cout << "Files will be hadded into a new file with run number:\t" << newrunno << endl;
	return newrunno;
}

//
bool do_the_hadding(int run1, int run2, int newrunno=0)
{ 
	bool verbose = false;
	
// run1 and run2 are the run numbers, not the i_line for the metadata file.
// this function breaks if run number isn't the first entry in every line.
	int position_of_eventsgenerated = -1;
	int position_of_eventssaved = -1;
	int position_of_runno = -1;
	int position_of_issummed = -1;
	int position_of_isbad = -1;
	int position_of_filename = -1;
//	Run/I:Filename/C:has_been_summed/I:Efield/D:Rho/D:EventsGenerated/I:EventsSaved/I:SaveEventTypes/C:Polarization/D:Alignment/D:MinCosTheta/D:Efield_Uniformity/O:StepperType/I:StepperName/C:StepMax_mm/D:PhysicsListName/C:is_a_sum/I:Trap_x_mm/D:Trap_y_mm/D:Trap_z_mm/D:Trap_sigma_x/D:Trap_sigma_y/D:Trap_sigma_z/D:Temperature/D:ExpandBeforePolarized_s/D:OP_CycleTime_s/D:SailVelocity_x_mm_per_ms/D:SailVelocity_y_mm_per_ms/D:SailVelocity_z_mm_per_ms/D:ChargeState/I

	
	string this_line;
	int this_runno;
	
	// Figure out the position where the event counts are kept.
	size_t positioncounter = 0;
	size_t last_position = 0;
	int n_found = 0;
	vector<int> delimiter_positions;
//	EventsGenerated/I:EventsSaved/I:
//	Run/I:Filename/C:has_been_summed/I:
//	is_a_sum/I:
	
	std::ifstream this_ifilestream;
	this_ifilestream.open(metadata_name.c_str());
	if( !this_ifilestream.is_open() )
	{
		cout << "Couldn't open the file for reading." << endl;
		return false;
	}
	
	std::getline(this_ifilestream, this_line);
	std::istringstream iss(this_line);
	if(verbose) cout << "this_line:  " << endl << this_line << endl;
	int size_of_string = this_line.size();
	if(verbose) cout << "size_of_string = " << size_of_string << endl;
	
	while ( positioncounter<size_of_string )
	{
		positioncounter = this_line.find(":", last_position+1);
		if(positioncounter<size_of_string)
		{
			last_position = positioncounter;
			delimiter_positions.push_back(last_position);
			n_found++;
		}
	}
	if(verbose) cout << "** delimiter n_found = " << n_found << ",\tlast_position = " << last_position << endl;
	
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
	if(verbose) cout << "position_of_eventsgenerated = " << position_of_eventsgenerated << endl;
	if(verbose) cout << "position_of_eventssaved = " << position_of_eventssaved << endl;
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
		positioncounter = this_line.find("has_been_summed", 0);
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
		positioncounter = this_line.find("is_a_sum", 0);
		last_position = positioncounter;
		if(positioncounter<size_of_string)
		{
			position_of_issummed = positioncounter;
			break;
		}
	}
	if(verbose) cout << "position_of_runno = " << position_of_runno << endl;
	if(verbose) cout << "position_of_isbad = " << position_of_isbad << endl;
	if(verbose) cout << "position_of_issummed = " << position_of_issummed << endl;
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
	if(verbose) cout << "position_of_filename = " << position_of_filename << endl;
	if(verbose) cout << "--" << endl;
	
	// ok, we know where the EventsGenerated and EventsSaved entries are.  
	// also Run, has_been_summed, and is_a_sum.
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
	if(verbose) cout << "branchpos_eventsgenerated = " << branchpos_eventsgenerated << endl;
	if(verbose) cout << "branchpos_eventssaved = " << branchpos_eventssaved << endl;
	if(verbose) cout << "branchpos_runno = " << branchpos_runno << endl;
	if(verbose) cout << "branchpos_isbad = " << branchpos_isbad << endl;
	if(verbose) cout << "branchpos_issummed = " << branchpos_issummed << endl;
	if(verbose) cout << "branchpos_filename = " << branchpos_filename << endl;
	if(verbose) cout << "--" << endl;
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
			if(verbose) cout << "i_line1=" << i_line1 << "\tfirstrunline:  " << endl;
			if(verbose) cout << firstrunline << endl;
		}
		if(this_runno == run2)
		{
			i_line2 = i_line;
			secondrunline = this_line;
			if(verbose) cout << "i_line2=" << i_line2 << "\tsecondrunline:  " << endl;
			if(verbose) cout << secondrunline << endl;
		}
	}
	
	if(verbose) cout << "final i_line = " << i_line << endl;
	if(verbose) cout << endl;
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
	if(verbose) cout << "--" << endl;
	
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
	if(verbose) cout << "n_eventssaved1 = " << n_eventssaved1 << endl;
	ss2 >> n_eventssaved2;
	if(verbose) cout << "n_eventssaved2 = " << n_eventssaved2 << endl;
	
	int total_ngenerated = n_eventsgenerated1 + n_eventsgenerated2;
	int total_nsaved = n_eventssaved1 + n_eventssaved2;
	if(verbose) cout << "--" << endl;
	if(verbose) cout << "total_ngenerated = " << total_ngenerated << endl;
	if(verbose) cout << "total_nsaved = " << total_nsaved << endl;
	
	std::stringstream ss_out;
	ss_out.str( std::string() );
	ss_out.clear();
	ss_out << firstrunline;
	
	std::stringstream ss_tmp;
	ss_tmp.str( std::string() );
	ss_tmp.clear();
	
	// assign a new run number?  somehow?!
	if(verbose) cout << "newrunno = " << newrunno << endl;

	string newstringterm = "";
	string newstringline = "";
	
	for(int i=0; i<n_found+1; i++)
	{
		if( !ss_out.good() ) 
		{ 
			cout << "Entry is shorter than expected.  Bad!" << endl;
			return false;
		//	break; 
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
		if( i != 0 )
		{
			newstringline = newstringline+"\t";
		}
		newstringline = newstringline+newstringterm;
	}
	newstringline = newstringline+"\t\n";

	if(verbose) cout << endl;
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
	i_line = 1;
	
	string this_new_line = "";
	string this_term = "";
	if(verbose) cout << "* got here!  run1 = " << run1 << ",\trun2 = " << run2 << endl;
	if(verbose) cout << "* also:  i_line=" << i_line << ",\ti_line1=" << i_line1 << ",\ti_line2=" << i_line2 << endl;
	while ( std::getline(this_ifilestream, this_line) )
	{
		i_line++;
		
		std::istringstream iss(this_line);
		iss >> this_runno;
		if( !(i_line == i_line1 || i_line == i_line2) )
		{
			filecontents = filecontents+this_line+"\n";
		}
		else
		{
			std::istringstream iss2(this_line);
			iss2 >> this_term;
			this_new_line = string(this_term);
			this_new_line = this_new_line+"\t";
		//	// Don't do this.  This makes it so that it matters whether or not there's 
		//	// trailing whitespace in the MetaData.txt entry that's being changed.
		//	int i=1;
		//	while(iss2 >> this_term)
		//	{
		//		if(i==branchpos_isbad)
		//		{
		//			this_new_line = this_new_line+"\t"+"1";
		//		}
		//		else
		//		{
		//			this_new_line = this_new_line+"\t"+this_term;
		//		}
		//		i++;
		//	}
			
			for(int i=1; i<n_found+1; i++)
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
			filecontents = filecontents+this_new_line+"\n";
		}
	}
	//
	this_ifilestream.close();

	//
	
	std::ofstream this_ofilestream;
	this_ofilestream.open(metadata_name.c_str(), std::ios::trunc);
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
	this_ofilestream << newstringline;
	this_ofilestream.close();
	
	return true;
}

//
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

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// ======================== // ======================== // ======================== //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
