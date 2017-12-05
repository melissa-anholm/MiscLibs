#ifndef INCLUDE_PYTHONIC
#define INCLUDE_PYTHONIC 1

#include <vector>

vector<double> zeros(int N)
{
	vector<double> vec;
	for(int i=0; i<N; i++)
	{
		vec.push_back(0.0);
	}
	return vec;
}
vector<double> ones(int N)
{
	vector<double> vec;
	for(int i=0; i<N; i++)
	{
		vec.push_back(1.0);
	}
	return vec;
}

vector<double> vector_scale(double a, vector<double> vec)
{
	int length = vec.size();
	for(int i=0; i<length; i++)
	{
		vec[i] = a*vec[i];
	}
	return vec;
}
vector<double> vector_scale(vector<double> vec, double a)
{
	return vector_scale(a, vec);
}

vector<double> zeros_like(vector<double> in_vec)
{
	int N = in_vec.size();
	
	vector<double> out_vec;
	for(int i=0; i<N; i++)
	{
		out_vec.push_back(0.0);
	}
	return out_vec;
}
vector<double> ones_like(vector<double> in_vec)
{
	int N = in_vec.size();
	
	vector<double> out_vec;
	for(int i=0; i<N; i++)
	{
		out_vec.push_back(1.0);
	}
	return out_vec;
}

vector<int> zeros_like(vector<int> in_vec)
{
	int N = in_vec.size();
	
	vector<int> out_vec;
	for(int i=0; i<N; i++)
	{
		out_vec.push_back(0.0);
	}
	return out_vec;
}
vector<int> ones_like(vector<int> in_vec)
{
	int N = in_vec.size();
	
	vector<int> out_vec;
	for(int i=0; i<N; i++)
	{
		out_vec.push_back(1.0);
	}
	return out_vec;
}

vector<double> termbyterm_multiply(vector<double> vec_a, vector<double> vec_b)
{
	vector<double> empty;
	if( vec_a.size() != vec_b.size())
	{
		std::cout << "Missized vectors!  Can't multiply." << std::endl;
		return empty;
	}
	
	int length = std::min(vec_a.size(), vec_b.size());
	vector<double> vec_c = zeros(length);
	for(int i=0; i<length; i++)
	{
		vec_c[i] = vec_a[i] * vec_b[i];
	}
	
	return vec_c;
}

#endif
