#ifndef INCLUDE_METACHAIN
#define INCLUDE_METACHAIN 1

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

#include "ChainData.cpp"
#include "ChainG4.cpp"
#include "ChainSimple.cpp"


extern string int_to_string(int thisint);
/*
string int_to_string(int thisint)
{
	string outstring;
	std::stringstream ss;
	ss.str( std::string() );
	ss.clear();
	ss << thisint;
	outstring = ss.str();
	return outstring;
}
*/

//extern string convertDouble(double value);
string convertDouble(double value) 
{
  std::ostringstream o;
  if (!(o << value))
    return "";
  return o.str();
}


#endif