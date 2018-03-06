#ifndef include_location
#define include_location 1

/*
//#define weareontrinatdaq 1

#define XSTR(x) #x
#define STR(x) XSTR(x)

#ifdef on_trinatdaq
	#define path_to_libs /home/trinat/anholm/MiscLibs
#else
	#define path_to_libs /Users/spiffyzha/Packages/MiscLibs
#endif
*/


//bool on_trinatdaq = false;
//bool on_fifteen   = false;
//bool on_trinat02  = true;

extern bool on_trinatdaq;
extern bool on_fifteen;
extern bool on_trinat02;

//
string bb1_prefix    = "";

string br_path       = "";
string be_path       = "";
string bf_path       = "";

string ur_path       = "";
string ue_path       = "";
string uf_path       = "";

string g4_path       = "";

string g4f_path      = "";
string metadata_name = "";

//
if(on_trinatdaq)
{
	bb1_prefix = "/home/trinat/anholm/MiscLibs/BB1/";

	br_path = "/data/trinat/S1188_2014_blinded/";
	be_path = "/data/trinat/S1188_2014_blinded/";
	bf_path = "/home/trinat/anholm/Friends/";  // BAD!!

	ur_path = "/home/trinat/online/analyzedFiles_2014/";
	ue_path = "/home/trinat/online/analyzedFiles_2014/";
	uf_path = "/home/trinat/anholm/Friends/";  

	g4_path  = "/home/trinat/anholm/G4_Output/";
}
else if(on_fifteen)
{
	bb1_prefix = "/Users/spiffyzha/Packages/MiscLibs/BB1/";  // 
	
	br_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Recoils_2014/";
	be_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
	bf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Friends_2014/";

	ur_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Recoils_2014/";
	ue_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Electrons_2014/";
	uf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Friends_2014/";

	g4_path  = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";
}
else if(on_trinat02)
{
	bb1_prefix = "/home1/trinat/anholm/Packages/MiscLibs/BB1/";  // 
	
	br_path = "/data/trinat/S1188_2014_blinded/";  // bad
	be_path = "/data/trinat/S1188_2014_blinded/";  // bad
	bf_path = "/home1/trinat/anholm/Friends/";  // BAD!!

	ur_path = "/home1/trinat/online/analyzedFiles_2014/";  // bad
	ue_path = "/home1/trinat/online/analyzedFiles_2014/";  // bad
	uf_path = "/home1/trinat/anholm/Friends/";   // bad

	g4_path  = "/home1/trinat/anholm/K37/build/Output/";
}
else
{
	// Location not defined.  :(
}

string g4f_path = g4_path+"Friends/";
string metadata_name = g4_path+"MetaData.txt";

void set_g4_path(string newpath)
{
	g4_path = newpath;
	g4f_path = g4_path+"Friends/";
	metadata_name = g4_path+"MetaData.txt";
}

#endif
