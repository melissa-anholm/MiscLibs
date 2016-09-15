#ifndef INCLUDE_GRAPHEXTRAS
#define INCLUDE_GRAPHEXTRAS 1

#include <vector>
using std::vector;

#include <TGraph.h>
#include <TGraphErrors.h>

//#include <HistExtras.cpp>
#include <Pythonic.cpp>

//double v1192_to_ns = 100.0/1024.0;

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

TGraphErrors * set_attributes_like(TGraphErrors * thisgraph, TH1D * hist)
{
//	TGraphErrors * graph = new TGraphErrors(hist);  
	thisgraph -> SetNameTitle( (string("Graph: ")+string(hist->GetName())).c_str(), hist->GetTitle());
	thisgraph -> SetMarkerColor(hist->GetLineColor());
	thisgraph -> SetLineColor(hist->GetLineColor());
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
	
	return my_TGraphErrors;
}


TGraphErrors * make_TGraphErrors(vector<double> x_avg, vector<double> y_avg, vector<double> delta_x, vector<double> delta_y)
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
	
	return my_TGraphErrors;
}

TGraphErrors * make_TGraphErrors(vector<double> x_avg, vector<double> y_avg)
{
	return make_TGraphErrors(x_avg, y_avg, zeros_like(x_avg), zeros_like(y_avg));
}


#endif
