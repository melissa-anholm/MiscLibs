#ifndef INCLUDE_GRAPHEXTRAS_H
#define INCLUDE_GRAPHEXTRAS_H 1

#include <cmath> // sqrt
#include <vector>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::min;


#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1.h>



struct set_of_points
{
	vector<double> x_vec;
	vector<double> y_vec;
};

extern set_of_points sort_these_vectors(vector<double> x_in, vector<double> y_in);

struct set_of_errpoints
{
	vector<double> x_vec;
	vector<double> y_vec;
	vector<double> dx_vec;
	vector<double> dy_vec;
};

extern set_of_errpoints sort_these_vectors(vector<double> x_in, vector<double> y_in, vector<double> dx_in, vector<double> dy_in);
extern TGraphErrors * set_attributes_like(TGraphErrors * thisgraph, TH1D * hist);
extern TGraphErrors * make_TGraphErrors(int N_points, double * x_avg, double * y_avg, double * delta_x, double * delta_y);
extern TGraphErrors * make_TGraphErrors(vector<double> x_avg, vector<double> y_avg, vector<double> delta_x, vector<double> delta_y, int color=kBlack, string name="");
extern TGraphErrors * make_sorted_TGraphErrors(vector<double> x_avg, vector<double> y_avg, vector<double> delta_x, vector<double> delta_y, int color);

extern TGraph * make_TGraph(vector<double> x_avg, vector<double> y_avg, int color=int(kBlack));
extern TGraph * make_sorted_TGraph(vector<double> x_avg, vector<double> y_avg, int color=int(kBlack));
extern TGraphErrors * make_TGraphErrors(vector<double> x_avg, vector<double> y_avg);


#endif
