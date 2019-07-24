// This is nonstandard coding practice buuuuut....
// this file should *only* be included via "location.h".
//#ifndef include_location
//#include "location.h"
//#endif
#ifndef include_location
#define include_location 1

#include <cstdlib>
#include <sstream>
//#include <cstdio>
#include <iostream>

//#include <fstream>
//#include <iostream>
//#include <string>
//#include <vector>
//#include <sstream>


//const char* this_location = std::getenv("LOCATION");
//if(const char* env_p = std::getenv("LOCATION"))
//std::cout << "Your LOCATION is:  " << env_p << std::endl;

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

/*
//#define on_trinatdaq 1
#define on_trinat02 1
*/

// do these things even work on wisely??
string br_path_wisely = "/Users/anholm/Desktop/Anal-Ysis/Blinded_Recoils_2014/";
string be_path_wisely = "/Users/anholm/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
string bf_path_wisely = "/Users/anholm/Desktop/Anal-Ysis/Blinded_Friends_2014/";

string ur_path_wisely = "/Users/anholm/Desktop/Anal-Ysis/ReAnalyzed_2014/";
string ue_path_wisely = "/Users/anholm/Desktop/Anal-Ysis/ReAnalyzed_2014/";
string uf_path_wisely = "/Users/anholm/Desktop/Anal-Ysis/ReAnalyzedFriends_2014/";

string g4_path_wisely  = "/Users/anholm/Desktop/Trinat_Geant/build/Output/";
//string g4_path_wisely  = "/Users/anholm/Desktop/Anal-Ysis/G4_Data/";
string bb1_prefix_wisely = "/Users/anholm/Packages/MiscLibs/BB1/";
//
string br_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Recoils_2014/";
string be_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
string bf_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Friends_2014/";

//string ur_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Recoils_2014/";
//string ue_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Electrons_2014/";
//string uf_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Friends_2014/";

string ur_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/ReAnalyzed_2014/";
string ue_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/ReAnalyzed_2014/";
string uf_path_fifteen = "/Users/spiffyzha/Desktop/Anal-Ysis/ReAnalyzedFriends_2014/";

//string g4_path_fifteen  = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";
string g4_path_fifteen  = "/Users/spiffyzha/Desktop/Anal-Ysis/G4_Data/";
string bb1_prefix_fifteen = "/Users/spiffyzha/Packages/MiscLibs/BB1/";
//
string br_path_trinatdaq = "/data/trinat/S1188_2014_blinded/";
string be_path_trinatdaq = "/data/trinat/S1188_2014_blinded/";
string bf_path_trinatdaq = "/home/trinat/anholm/Friends/";  // BAD!!

string ur_path_trinatdaq = "/home/trinat/online/analyzedFiles_2014/";
string ue_path_trinatdaq = "/home/trinat/online/analyzedFiles_2014/";
string uf_path_trinatdaq = "/home/trinat/anholm/Friends/";  

string g4_path_trinatdaq  = "/home/trinat/anholm/G4_Output/";
string bb1_prefix_trinatdaq = "/home/trinat/anholm/MiscLibs/BB1/";
//
string br_path_trinat02 = "/data/trinat/S1188_2014_blinded/";  // bad
string be_path_trinat02 = "/data/trinat/S1188_2014_blinded/";  // bad
string bf_path_trinat02 = "/home1/trinat/anholm/Friends/";  // bad.

string ur_path_trinat02 = "/home1/trinat/anholm/online/analyzedFiles_2014/";  // bad
string ue_path_trinat02 = "/home1/trinat/anholm/online/analyzedFiles_2014/";  // bad
string uf_path_trinat02 = "/home1/trinat/anholm/Friends/";  // bad.

string g4_path_trinat02  = "/home1/trinat/anholm/K37/build/Output/";
string bb1_prefix_trinat02;
//
string br_path_trinat01 = "/pool/anholm/Data_2014/Blinded_Recoils_2014/";  // bad
string be_path_trinat01 = "/pool/anholm/Data_2014/Blinded_Electrons_2014/";  // bad
string bf_path_trinat01 = "/pool/anholm/Data_2014/Blinded_Friends_2014/";  // bad.

string ur_path_trinat01 = "/pool/anholm/Data_2014/Unblinded_Recoils_2014/";  // bad
string ue_path_trinat01 = "/pool/anholm/Data_2014/Unblinded_Electrons_2014/";  
string uf_path_trinat01 = "/pool/anholm/Data_2014/Unblinded_Friends_2014/";  

string g4_path_trinat01  = "/pool/anholm/Output/";
string bb1_prefix_trinat01 = "/home/trinat/anholm/Packages/MiscLibs/BB1/";

//
string br_path = br_path_fifteen;
string be_path = be_path_fifteen;
string bf_path = bf_path_fifteen;

string ur_path = ur_path_fifteen;
string ue_path = ue_path_fifteen;
string uf_path = uf_path_fifteen;

string rb_path = "/pool/trinat/analyzedFiles_2018/";
string rbfpath = "/pool/trinat/analyzedFiles_2018/Friends/";

string g4_path  = g4_path_fifteen;
string g4f_path = g4_path+"Friends/";
string metadata_name = g4_path+"MetaData.txt";

string bb1_prefix = bb1_prefix_fifteen;

//


const char* env_location = std::getenv("LOCATION");
string char_to_string(const char* input)
{
	string output;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	ss << input;
	output = ss.str();
	return output;
}
string env_location_string = char_to_string(env_location);
//
bool setup_location()
{
//	cout << "Called setup_location ..." << endl;
	const char* this_location = std::getenv("LOCATION");
//	cout << "this_location = " << this_location << endl;
	
	//
	if(strcmp(this_location, "fifteen")==0)
	{
		cout << "Location is fifteen." << endl;
		//
		br_path = br_path_fifteen;
		be_path = be_path_fifteen;
		bf_path = bf_path_fifteen;
		
		ur_path = ur_path_fifteen;
		ue_path = ue_path_fifteen;
		uf_path = uf_path_fifteen;
		
		g4_path  = g4_path_fifteen;
		g4f_path = g4_path+"Friends/";
		metadata_name = g4_path+"MetaData.txt";

		bb1_prefix = bb1_prefix_fifteen;
		return true;
	}
	else if( strcmp(this_location, "Wisely")==0 || strcmp(this_location, "wisely")==0 )
	{
		cout << "Location is Wisely." << endl;
		br_path = br_path_wisely;
		be_path = be_path_wisely;
		bf_path = bf_path_wisely;
		
		ur_path = ur_path_wisely;
		ue_path = ue_path_wisely;
		uf_path = uf_path_wisely;
		
		g4_path  = g4_path_wisely;
		g4f_path = g4_path+"Friends/";
		metadata_name = g4_path+"MetaData.txt";

		bb1_prefix = bb1_prefix_wisely;
		return true;
	}
	else if(strcmp(this_location, "trinat01")==0)
	{
		cout << "Location is trinat01." << endl;
		//
		br_path = br_path_trinat01;
		be_path = be_path_trinat01;
		bf_path = bf_path_trinat01;
		
		ur_path = ur_path_trinat01;
		ue_path = ue_path_trinat01;
		uf_path = uf_path_trinat01;
		
		g4_path  = g4_path_trinat01;
		g4f_path = g4_path+"Friends/";
		metadata_name = g4_path+"MetaData.txt";

		bb1_prefix = bb1_prefix_trinat01;
		return true;
	}
	else
	{
		cout << "Did not recognize your location." << endl;
	}
	return false;
}

void set_g4_path(string newpath)
{
	g4_path = newpath;
	g4f_path = g4_path+"Friends/";
	metadata_name = g4_path+"MetaData.txt";
}


/*
#ifdef on_trinatdaq
string br_path = "/data/trinat/S1188_2014_blinded/";
string be_path = "/data/trinat/S1188_2014_blinded/";
string bf_path = "/home/trinat/anholm/Friends/";  // BAD!!

string ur_path = "/home/trinat/online/analyzedFiles_2014/";
string ue_path = "/home/trinat/online/analyzedFiles_2014/";
string uf_path = "/home/trinat/anholm/Friends/";  

string g4_path  = "/home/trinat/anholm/G4_Output/";

#else  // not on trinatdaq.
#ifdef on_trinat02

string br_path = "/data/trinat/S1188_2014_blinded/";  // bad
string be_path = "/data/trinat/S1188_2014_blinded/";  // bad
string bf_path = "/home1/trinat/anholm/Friends/";  // bad.

string ur_path = "/home1/trinat/anholm/online/analyzedFiles_2014/";  // bad
string ue_path = "/home1/trinat/anholm/online/analyzedFiles_2014/";  // bad
string uf_path = "/home1/trinat/anholm/Friends/";  // bad.

string g4_path  = "/home1/trinat/anholm/K37/build/Output/";


#else  // not on trinatdaq or trinat02

string br_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Recoils_2014/";
string be_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
string bf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Friends_2014/";

string ur_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Recoils_2014/";
string ue_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Electrons_2014/";
string uf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Friends_2014/";

string g4_path  = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";

//string g4_path  = "/pool/anholm/Output/";

#endif
#endif
*/



/*
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
#ifdef on_trinatdaq

string br_path = "/data/trinat/S1188_2014_blinded/";
string be_path = "/data/trinat/S1188_2014_blinded/";
string bf_path = "/home/trinat/anholm/Friends/";  // BAD!!

string ur_path = "/home/trinat/online/analyzedFiles_2014/";
string ue_path = "/home/trinat/online/analyzedFiles_2014/";
string uf_path = "/home/trinat/anholm/Friends/";  

string g4_path  = "/home/trinat/anholm/Trinat_Geant/build/Output/";
string g4f_path = "/home/trinat/anholm/Trinat_Geant/build/Output/Friends/";
//string metadatafilename = "/home/trinat/anholm/Trinat_Geant/build/Output/MetaData.txt";

#else
string br_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Recoils_2014/";
string be_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
string bf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Friends_2014/";

string ur_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Recoils_2014/";
string ue_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Electrons_2014/";
string uf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Friends_2014/";

string g4_path  = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";
string g4f_path = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/Friends/";

#endif
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
*/





#endif
