// This is nonstandard coding practice buuuuut....
// this file should *only* be included via "location.h".
//#ifndef include_location
//#include "location.h"
//#endif
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

#endif


#ifdef on_trinatdaq
string br_path = "/data/trinat/S1188_2014_blinded/";
string be_path = "/data/trinat/S1188_2014_blinded/";
string bf_path = "/home/trinat/anholm/Friends/";  // BAD!!

string ur_path = "/home/trinat/online/analyzedFiles_2014/";
string ue_path = "/home/trinat/online/analyzedFiles_2014/";
string uf_path = "/home/trinat/anholm/Friends/";  

string g4_path  = "/home/trinat/anholm/G4_Output/";
string g4f_path = "/home/trinat/anholm/G4_Output/";
//string metadatafilename = "/home/trinat/anholm/Trinat_Geant/build/Output/MetaData.txt";
string metadata_name = "/home/trinat/anholm/G4_Output/MetaData.txt";
#else
string br_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Recoils_2014/";
string be_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Electrons_2014/";
string bf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Blinded_Friends_2014/";

string ur_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Recoils_2014/";
string ue_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Electrons_2014/";
string uf_path = "/Users/spiffyzha/Desktop/Anal-Ysis/Unblinded_Friends_2014/";

//string g4_path  = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";
#ifdef quasi_on_trinatdaq
	string g4_path  = "/Users/spiffyzha/Desktop/TempMount/anholm/G4_Output/";
#else
	string g4_path  = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/";
#endif

string g4f_path = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/Friends/";
string metadata_name = "/Users/spiffyzha/Desktop/Trinat_Geant/build/Output/MetaData.txt";
#endif


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