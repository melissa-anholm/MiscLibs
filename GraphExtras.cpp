#ifndef INCLUDE_GRAPHEXTRAS
#define INCLUDE_GRAPHEXTRAS 1

#include <cmath> // sqrt
#include <vector>
using std::vector;

#include <TGraph.h>
#include <TGraphErrors.h>

#include <TH1.h>
//#include <HistExtras.cpp>
#include "Pythonic.cpp"

//double v1192_to_ns = 100.0/1024.0;
using std::string;
using std::cout;
using std::endl;
using std::min;


/*
TGraphErrors * make_TGraphErrors_like(TH1D * hist)
{
	TGraphErrors * graph = new TGraphErrors(hist);  
	// above: copies all the parameters and data points, but not the name.  or title.
	graph -> SetNameTitle(hist->GetName(), hist->GetTitle());
	graph -> SetMarkerColor(hist->GetLineColor());
	
	return graph;
}
*/

/*
TGraphErrors * set_graph_values(TGraphErrors * thisgraph, int n_points, double*x, double*x_err, double*y, double* y_err)
{
	// Set new shizzle.
	for(int i=0; i<n_points; i++)
	{
		thisgraph -> SetPoint(i, x[i], y[i]);
		thisgraph -> SetPointError(i, x_err[i], y_err[i]);
	}
	return thisgraph;
}
*/

struct set_of_points
{
	vector<double> x_vec;
	vector<double> y_vec;
};
set_of_points sort_these_vectors(vector<double> x_in, vector<double> y_in)
{
	set_of_points outgoing_sop;
	int N_points = min(x_in.size(), y_in.size());
	if( x_in.size() != y_in.size() )
	{ 
		cout << "* WARNING:  input vectors have non-consistent sizes.  Creating graph anyway." << endl;
		outgoing_sop.x_vec = x_in;
		outgoing_sop.y_vec = y_in;
		return outgoing_sop;
	}
	vector<double> x_out;
	vector<double> y_out;
	
	double use_this_xval = x_in.at(0);
	double use_this_yval = y_in.at(0);
	int use_this_i = 0;
	
	for(int j=0; j<N_points; j++)
	{
		use_this_xval = x_in.at(0);
		use_this_yval = y_in.at(0);
		use_this_i = 0;
		for(int i=1; i<x_in.size(); i++)
		{
			// Look for the smallest x-values.
			if( x_in.at(i) < use_this_xval )
			{
				use_this_xval = x_in.at(i);
				use_this_yval = y_in.at(i);
				use_this_i = i;
			}
		}
		x_out.push_back(x_in.at(use_this_i));
		y_out.push_back(y_in.at(use_this_i));
		x_in.erase(x_in.begin()+use_this_i);
		y_in.erase(y_in.begin()+use_this_i);
	}
	
//	for(int i=0; i<N_points; i++)
//	{
//		cout << "i=" << i << ", \t x=" << x_out.at(i) << ", \t y=" << y_out.at(i) << endl;
//	}
//	cout << endl;
	
	outgoing_sop.x_vec = x_out;
	outgoing_sop.y_vec = y_out;
	
	return outgoing_sop;
}

struct set_of_errpoints
{
	vector<double> x_vec;
	vector<double> y_vec;
	vector<double> dx_vec;
	vector<double> dy_vec;
};

set_of_errpoints sort_these_vectors(vector<double> x_in, vector<double> y_in, vector<double> dx_in, vector<double> dy_in)
{
	set_of_errpoints outgoing_soep;
	int N_points = min( min(x_in.size(), y_in.size()), min(dx_in.size(), dy_in.size()) );
	if( (x_in.size() != y_in.size()) || (x_in.size() != dx_in.size()) || (dx_in.size() != dy_in.size()))
	{ 
		cout << "* WARNING:  input vectors have non-consistent sizes.  Creating graph anyway." << endl;
		return outgoing_soep;
	}
	
	vector<double> x_out;
	vector<double> y_out;
	vector<double> dx_out;
	vector<double> dy_out;
	
	double use_this_xval  =  x_in.at(0);
	double use_this_yval  =  y_in.at(0);
	double use_this_dxval = dx_in.at(0);
	double use_this_dyval = dy_in.at(0);
	int use_this_i = 0;

	for(int j=0; j<N_points; j++)
	{
		use_this_xval = x_in.at(0);
		use_this_yval = y_in.at(0);
		use_this_dxval = dx_in.at(0);
		use_this_dyval = dy_in.at(0);
		use_this_i = 0;
		
		for(int i=1; i<x_in.size(); i++)
		{
			// Look for the smallest x-values.
			if( x_in.at(i) < use_this_xval )
			{
				use_this_xval  =  x_in.at(i);
				use_this_yval  =  y_in.at(i);
				use_this_dxval = dx_in.at(i);
				use_this_dyval = dy_in.at(i);
				use_this_i = i;
			}
		}
		x_out.push_back(x_in.at(use_this_i));
		y_out.push_back(y_in.at(use_this_i));
		dx_out.push_back(dx_in.at(use_this_i));
		dy_out.push_back(dy_in.at(use_this_i));
		x_in.erase(x_in.begin()+use_this_i);
		y_in.erase(y_in.begin()+use_this_i);
		dx_in.erase(dx_in.begin()+use_this_i);
		dy_in.erase(dy_in.begin()+use_this_i);
	}
	outgoing_soep.x_vec  =  x_out;
	outgoing_soep.y_vec  =  y_out;
	outgoing_soep.dx_vec = dx_out;
	outgoing_soep.dy_vec = dy_out;
	
	return outgoing_soep;
}


TGraphErrors * set_attributes_like(TGraphErrors * thisgraph, TH1D * hist)
{
//	TGraphErrors * graph = new TGraphErrors(hist);  
	thisgraph -> SetNameTitle( (string("Graph: ")+string(hist->GetName())).c_str(), hist->GetTitle());
	thisgraph -> SetMarkerColor(hist->GetLineColor());
	thisgraph -> SetLineColor(hist->GetLineColor());
	
	thisgraph -> SetMarkerStyle(hist->GetMarkerStyle());
	thisgraph -> SetMarkerSize(hist->GetMarkerSize());
	thisgraph -> SetLineStyle(hist->GetLineStyle());
	thisgraph -> SetLineWidth(hist->GetLineWidth());
	
	thisgraph -> GetXaxis()->SetTitle(hist->GetXaxis()->GetTitle());
	thisgraph -> GetYaxis()->SetTitle(hist->GetYaxis()->GetTitle());
//	thisgraph -> GetXaxis()->SetRangeUser(hist->GetXaxis()->GetRangeUser());  // GetRangeUser isn't a thing.
//	thisgraph -> GetYaxis()->SetRangeUser(hist->GetYaxis()->GetRangeUser());

	return thisgraph;
}

TGraphErrors * make_TGraphErrors(int N_points, double * x_avg, double * y_avg, double * delta_x, double * delta_y)
{
	TGraphErrors * my_TGraphErrors;
	my_TGraphErrors = new TGraphErrors(N_points, x_avg, y_avg, delta_x, delta_y);
	
	my_TGraphErrors->SetFillStyle(0);  // 0 =  not filled.
	my_TGraphErrors->SetFillColor(kWhite);  // the box around the thing is white.

	return my_TGraphErrors;
}


TGraphErrors * make_TGraphErrors(vector<double> x_avg, vector<double> y_avg, vector<double> delta_x, vector<double> delta_y, int color=kBlack, string name="")
{
	int N_points = std::min( std::min(x_avg.size(), y_avg.size()), std::min(delta_x.size(), delta_y.size()) );
	
	if( x_avg.size() != y_avg.size() || delta_x.size() != delta_y.size() || x_avg.size() != delta_x.size() )
		{ std::cout << "* WARNING:  TGraphErrors input vectors have non-constant sizes.  Creating graph anyway." << std::endl; }

	double * x = &x_avg[0];
	double * y = &y_avg[0];
	double * x_err = &delta_x[0];
	double * y_err = &delta_y[0];
	
	TGraphErrors * my_TGraphErrors;
	my_TGraphErrors = new TGraphErrors(N_points, x, y, x_err, y_err);
	
	my_TGraphErrors->SetFillStyle(0);  // 0 =  not filled.
	my_TGraphErrors->SetFillColor(kWhite);  // the box around the thing is white.
	my_TGraphErrors->SetMarkerColor(color);
	my_TGraphErrors->SetLineColor(color);
	if(name.size()>0) // by default, we don't set the name here, so it'll have the default name "Graph".
	{
		my_TGraphErrors->SetName(name.c_str());
		my_TGraphErrors->SetTitle(name.c_str());
	}
	
	return my_TGraphErrors;
}

TGraphErrors * make_sorted_TGraphErrors(vector<double> x_avg, vector<double> y_avg, vector<double> delta_x, vector<double> delta_y, int color)
{
	int N_points = std::min( std::min(x_avg.size(), y_avg.size()), std::min(delta_x.size(), delta_y.size()) );
	
	if( x_avg.size() != y_avg.size() || delta_x.size() != delta_y.size() || x_avg.size() != delta_x.size() )
		{ cout << "* WARNING:  TGraphErrors input vectors have non-constant sizes.  Creating graph anyway." << endl; }

	set_of_errpoints sorted_soep;
	sorted_soep = sort_these_vectors(x_avg, y_avg, delta_x, delta_y);
	vector<double> anew_x(sorted_soep.x_vec);
	vector<double> anew_y(sorted_soep.y_vec);
	vector<double> anew_dx(sorted_soep.dx_vec);
	vector<double> anew_dy(sorted_soep.dy_vec);

	double * x = &anew_x[0];
	double * y = &anew_y[0];
	double * x_err = &anew_dx[0];
	double * y_err = &anew_dy[0];
	
	TGraphErrors * my_TGraphErrors;
	my_TGraphErrors = new TGraphErrors(N_points, x, y, x_err, y_err);
	// kludge:
	my_TGraphErrors->SetMarkerStyle(21);
	my_TGraphErrors->SetMarkerSize(0.4);
	my_TGraphErrors->SetMarkerColor(color);
	my_TGraphErrors->SetLineColor(color);
	my_TGraphErrors->SetFillStyle(0);  // 0 =  not filled.
	my_TGraphErrors->SetFillColor(kWhite);  // the box around the thing is white.
	return my_TGraphErrors;
}

TGraph * make_TGraph(vector<double> x_avg, vector<double> y_avg, int color=int(kBlack))
{
	int N_points = std::min(x_avg.size(), y_avg.size());
	if( N_points <= 0 ) 
	{ 
		cout << "Bad!  No points!" << endl; 
		cout << "n_xpoints = " << x_avg.size() << endl;
		cout << "n_ypoints = " << y_avg.size() << endl;
	}
	
	if( x_avg.size() != y_avg.size() )
		{ cout << "* WARNING:  TGraphErrors input vectors have non-constant sizes.  Creating graph anyway." << endl; }

	double * x = &x_avg[0];
	double * y = &y_avg[0];
//	double * x_err = &delta_x[0];
//	double * y_err = &delta_y[0];
	
	TGraph * my_TGraph;
	my_TGraph = new TGraph(N_points, x, y);
	// kludge:
	my_TGraph->SetMarkerStyle(21);
	my_TGraph->SetMarkerSize(0.4);
	my_TGraph->SetMarkerColor(color);
	my_TGraph->SetLineColor(color);
	my_TGraph->SetFillStyle(0);  // 0 =  not filled.
	my_TGraph->SetFillColor(kWhite);  // the box around the thing is white.

	return my_TGraph;
}


TGraph * make_sorted_TGraph(vector<double> x_avg, vector<double> y_avg, int color=int(kBlack))
{
	int N_points = std::min(x_avg.size(), y_avg.size());
	if( x_avg.size() != y_avg.size() )
		{ cout << "* WARNING:  TGraphErrors input vectors have non-consistent sizes.  Creating graph anyway." << endl; }
	
	// Sort the vectors!
	set_of_points sorted_sop;
	sorted_sop = sort_these_vectors(x_avg, y_avg);
	vector<double> anew_x(sorted_sop.x_vec);
	vector<double> anew_y(sorted_sop.y_vec);
	
//	cout << "ok, maybe those things are sorted?" << endl;
//	for(int i=0; i<N_points; i++)
//	{
//		cout << "i=" << i << ", \t x=" << anew_x.at(i) << ", \t y=" << anew_y.at(i) << endl;
//	}
//	cout << endl;
	
	double * x = &anew_x[0];
	double * y = &anew_y[0];
	TGraph * my_TGraph;
	my_TGraph = new TGraph(N_points, x, y);
	
	// kludge:
	my_TGraph->SetMarkerStyle(21);
	my_TGraph->SetMarkerSize(0.4);
	my_TGraph->SetMarkerColor(color);
	my_TGraph->SetLineColor(color);
	my_TGraph->SetFillStyle(0);  // 0 =  not filled.
	my_TGraph->SetFillColor(kWhite);  // the box around the thing is white.

	return my_TGraph;
}



TGraphErrors * make_TGraphErrors(vector<double> x_avg, vector<double> y_avg)
{
	return make_TGraphErrors(x_avg, y_avg, zeros_like(x_avg), zeros_like(y_avg));
}


#endif
