// ==================================================================== //
// Code by Melissa Anholm
// July 2018 - 
//
// MakeSomeTF1s.cpp:  A library to create TF1s that I use commonly.
// ==================================================================== //
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>   // do I need this?
#include <algorithm> // to manipulate strings.
#include <sstream> 
#include <utility>
//#include <fstream>
//#include <map>

#include <TH1D.h>
#include "TF1.h"
//#include <TLine.h>
//#include "TArrayD.h"
//#include <TObject.h>

#include <TFitResult.h>
#include <TFitResultPtr.h>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::min;

using std::setprecision;
using std::fixed;

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
string double_to_string_again(double thisnumber, int thisprecision=12)  // 
{
	std::ostringstream oss;
	oss << fixed << setprecision(thisprecision) << thisnumber;
	
	string mynumberstring = oss.str();
	return mynumberstring;
}

// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
TF1* make_Abeta_func()  // is this formula even correct?!?
{
//	TF1* Abeta_func_keV = new TF1("Abeta_func_keV", "[0]*sqrt(1.0 - 510.9989461^2/(x+510.9989461)^2)", 0.0, 5100.0);
	TF1* Abeta_func_keV = new TF1("Abeta_func_keV", "[0]*sqrt(x^2 + 2*x*510.9989461) / (x+510.9989461)", 0.0, 5100.0);
	//double m_e = 510.9989461;
	Abeta_func_keV -> SetParNames("Abeta");
//	Abeta_func_keV -> SetParameter("Abeta", -0.56);
	Abeta_func_keV -> SetParameter("Abeta", -0.5680);  // K37 default txt inputs
//	Abeta_func_keV -> SetParameter("Abeta", -0.5707);  // PRL
	Abeta_func_keV -> SetParLimits(0, -0.59, -0.52);
	
	return Abeta_func_keV;
}

TF1* make_Abeta_func_withabsorption(double E_absorbed_keV)  // comes out as a function of kinetic energy.
{
	string E_absorbed_string = double_to_string_again(E_absorbed_keV, 6);
	string func_string = "[0]*sqrt( (x+"+E_absorbed_string+"+510.9989461)^2 - (510.9989461)^2 ) / ((x+"+E_absorbed_string+")+510.9989461)";
	
//	cout << "func_string = " << func_string << endl;
	
	TF1* Abeta_func_keV = new TF1("Abeta_func_keV", func_string.c_str(), 0, 5100.0);
	//double m_e = 510.9989461;
	Abeta_func_keV -> SetParNames("Abeta");
	Abeta_func_keV -> SetParameter("Abeta", -0.5680);  // K37 default txt inputs
//	Abeta_func_keV -> SetParameter("Abeta", -0.5707);  // PRL
	Abeta_func_keV -> SetParLimits(0, -0.59, -0.52);
	
	/*
	double x;
	x = 302.0;
	cout << "A(" << x << ") = " << Abeta_func_keV->Eval(x) << endl;
	x = 500.0;
	cout << "A(" << x << ") = " << Abeta_func_keV->Eval(x) << endl;
	x = 1000.0;
	cout << "A(" << x << ") = " << Abeta_func_keV->Eval(x) << endl;
	x = 3000.0;
	cout << "A(" << x << ") = " << Abeta_func_keV->Eval(x) << endl;
	x = 5000.0;
	cout << "A(" << x << ") = " << Abeta_func_keV->Eval(x) << endl;
	x = 10.0;
	cout << "A(" << x << ") = " << Abeta_func_keV->Eval(x) << endl;
	*/
	
	return Abeta_func_keV;
}
TF1* make_Abeta_b_func()
{
//	TF1* Abeta_b_func_keV = new TF1("Abeta_b_func_keV", "[0]*sqrt(1.0 - 510.9989461^2/(x+510.9989461)^2) * ( 1.0 + [1]*(510.9989461/x) )", 0.0, 5100.0);
	TF1* Abeta_b_func_keV = new TF1("Abeta_b_func_keV", "[0]*( sqrt(x^2 + 2*x*510.9989461) / (x+510.9989461) ) / (1.0 + [1]*(510.9989461/(x+510.9989461)) )", 0.0, 5100.0);
	Abeta_b_func_keV -> SetParNames("Abeta", "bFierz");
	Abeta_b_func_keV -> SetParameter("Abeta", -0.56);
	Abeta_b_func_keV -> SetParameter("bFierz", 0.0);
	
	return Abeta_b_func_keV;
}
TF1* make_quasi_b_func()
{
	// (1.0 + [0]*(510.9989461/(x+510.9989461)) ) is exact;  divide 'A' by it.
//	TF1* b_func_keV = new TF1("b_func_keV", "(1.0 + [0]*(510.9989461/(x+510.9989461)) )", 0.0, 5100.0);
	
	// divide 'A' by (1+b_func_keV).
	TF1* b_func_keV = new TF1("b_func_keV", "[0]*(510.9989461/(x+510.9989461))", 0.0, 5100.0);
	b_func_keV -> SetParNames("bFierz");
	b_func_keV -> SetParameter("bFierz", 1.0);
	
	return b_func_keV;
}
TF1 * make_parabola_func()
{
	//double xmin = -0.590;
	//double xmax = -0.535;
	TF1* parabola = new TF1("parabola", "[1]^(-2)*(x-[0])^2 + [2]", -0.590, -0.535);
	parabola -> SetParNames("Abeta", "partial_dA", "vertex_y");
	parabola -> SetParameter("Abeta",      -0.56);
	parabola -> SetParameter("partial_dA",  0.0025);
	parabola -> SetParameter("vertex_y",   32.0);
	parabola -> SetLineColor(kGray);
	
	return parabola;
}

TF1 * make_betaEphasespace_func() // phase space as a function of *energy*. keV.
{
	// P(E) dE = C * p*E*(E0-E)^2  // params:  constant C, endpoint E0.
	// pbetac = sqrt( pow(E,2) - pow(m_e,2) );
	// m_e = 0.5109989461*1000.0;
	// E0 = 5.63646*1000.0;
	TF1* beta_phasespace = new TF1("beta_phasespace", "[0]*x*sqrt( pow(x,2) - pow(510.9989461,2) )*([1]-x)^2", 0.5109989461*1000.0, 5.63646*1000.0);
	beta_phasespace -> SetParNames("scale", "E0");
	beta_phasespace -> SetParameter("scale", 3.0e7);
	beta_phasespace -> SetParameter("E0", 5636.46);
	
	
	return beta_phasespace;
}

TF1* make_linefunc()
{
//	TF1* linefunc = new TF1("linefunc", "[0]*x + [1]", 0.0, 6000.0);
//	linefunc -> SetParNames("m", "b");
//	linefunc -> SetParameter("m", -1.0e-4);
//	linefunc -> SetParameter("b", 0.13);
	
	TF1* linefunc = new TF1("linefunc", "[0]*(x-[1])", 0.0, 6000.0);
	linefunc -> SetParNames("slope", "x_intercept");
	linefunc -> SetParameter("slope", -1.0e-8);
	linefunc -> SetParameter("x_intercept", 5700.0);
	return linefunc;
}

/*
TF1 * make_betaKEphasespace_func() // phase space as a function of *energy*. keV.
{
	// P(E) dE = C * p*E*(E0-E)^2  // params:  constant C, endpoint E0.
	// pbetac = sqrt( pow(E,2) - pow(m_e,2) );
	// m_e = 0.5109989461*1000.0;
	// E0 = 5.63646*1000.0;
	TF1* beta_phasespace = new TF1("beta_phasespace", "[0]*x*sqrt( pow(x,2) - pow(510.9989461,2) )*([1]-x)^2", 0.5109989461*1000.0, 5.63646*1000.0);
	beta_phasespace -> SetParNames("scale", "E0");
	beta_phasespace -> SetParameter("scale", 3.0e7);
	beta_phasespace -> SetParameter("E0", 5636.46);
	
	
	return beta_phasespace;
}
*/
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //
TF1 * make_Wtilde_1()  // x-axis is beta kinetic energy in keV.  "E_scint".
{
	//double amu_to_MeV = 931.4940954;        // 931.4940954 from wikipedia July 2018.
	//parent.Mass   = 36.97337589*amu_to_MeV  // 
	//daughter.Mass = 36.96677632*amu_to_MeV  // 
	//neutrino.Mass = 0.0;
	//double m_e = 510.9989461(13);
	
	// E_0 = 5.636004099063859 MeV
	// W1[T_] = p[T]*EE[T]*(E0 - EE[T])^2
	//	(5125.005152963859 - T)^2 * (510.9989461 + T) * Sqrt[ 1021.9978922*T + T^2]
	// WA[T_] = p[T]*p[T]*(E0 - EE[T])^2
	//	(5125.005152963859 - T)^2 * (1021.9978922*T + T^2)
	// Wb[T_] = p[T]*electronMass*(E0 - EE[T])^2
	//	510.9989461*(5125.005152963859 - T)^2 * Sqrt[1021.9978922 * T + T^2]
	double E_0 = 5636.004099063859;
	double m_e = 510.9989461;
	
	TF1* Wtilde_1 = new TF1("Wtilde_1", "((5125.005152963859 - x)^2) * (510.9989461 + x) * sqrt(1021.9978922*x + x^2)", 0.0, E_0 - m_e);
	Wtilde_1 -> SetLineColor(int(kGreen));
	return Wtilde_1;
}
TF1 * make_Wtilde_A()  // x-axis is beta kinetic energy in keV.  "E_scint".
{
	double E_0 = 5636.004099063859;
	double m_e = 510.9989461;
	TF1* Wtilde_A = new TF1("Wtilde_A", "((5125.005152963859 - x)^2) * (1021.9978922*x + x^2)", 0.0, E_0 - m_e);
	Wtilde_A -> SetLineColor(int(kRed));
	return Wtilde_A;
}
TF1 * make_Wtilde_b()  // x-axis is beta kinetic energy in keV.  "E_scint".
{
	double E_0 = 5636.004099063859;
	double m_e = 510.9989461;
	TF1* Wtilde_b = new TF1("Wtilde_b", "510.9989461*((5125.005152963859 - x)^2) * sqrt(1021.9978922 * x + x^2)", 0.0, E_0 - m_e);
	Wtilde_b -> SetLineColor(int(kBlue));
	return Wtilde_b;
}
// --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- // --- //

TF1* make_gaussian_func(double center, double sigma, double area)
{
	TF1* gaussian_func = new TF1("gaussian_func_keV", "[0]/(sqrt(2*pi)*[2]) * exp(-0.5*((x-[1])/[2])**2)", 0.0, 6000.0);
	gaussian_func -> SetParNames("area", "mean", "sigma");
	gaussian_func -> SetParameter("area",  area);
	gaussian_func -> SetParameter("mean",  center);
	gaussian_func -> SetParameter("sigma", sigma);
	
	return gaussian_func;
}

/*
TF1* make_gaussian_func(double center, double sigma, double area)
{
	TF1* gaussian_func = new TF1("gaussian_func_keV", )
	
}
*/


