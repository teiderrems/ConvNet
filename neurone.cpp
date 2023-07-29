#include "neurone.h"
#include "util.h"

cpr::Neurone::Neurone(double bias)
{
	this->b = bias;
}

double cpr::Neurone::activate(vector<double>& vec)
{
	
	if (this->Weight.size() == 0) {
		this->Weight.resize(vec.size());
		this->Weight = cpr::Util::genWeight(vec.size());
		
        this->agrega = this->sum(vec) + this->b;
		this->y = cpr::Util::sigmoid(this->agrega);
		cout << "Activate=>" << this->y << endl;
		return this->y;
	}
	else {
		
        this->agrega = this->sum(vec) + this->b;
		this->y = cpr::Util::sigmoid(this->agrega);
		cout << "Activate=>" << this->y << endl;
		return this->y;
	}
}

double cpr::Neurone::gradient(double dw, double delta)
{
	double grad = (cpr::Util::sigmoidPrime(this->agrega) * dw * delta);
	cout << "grad :" << grad << endl;
	return grad;
}

double cpr::Neurone::gradient(vector<double>& dw, vector<double>& delta)
{
	double som = 0;
	for (int i = 0; i < dw.size(); i++)
	{
		som += dw[i] * delta[i];
	}
	double grad = (cpr::Util::sigmoidPrime(this->agrega) * som);
	cout << "grad :" << grad << endl;
	cout << endl;
	return grad;
}

vector<vector<double>> cpr::Neurone::dw(vector<double>& A, vector<double>& delta)
{
	vector<vector<double>> dw;
	dw.resize(delta.size());
	for (int i = 0; i < delta.size(); i++)
	{
		dw[i].resize(A.size());
		for (int j = 0; j < A.size(); j++)
		{
			dw[i][j] = delta[i] * A[j];
		}
	}
	return dw;
}

vector<double> cpr::Neurone::dwOutput(vector<double>& A,double delta)
{
	
	vector<double> dw;
	dw.resize(A.size());
	for (int i = 0; i < A.size(); i++)
	{
		dw[i] = A[i] * delta;
	}
	return dw;
}

double cpr::Neurone::gradientOutput(double yd)
{
	double delta = (cpr::Util::lossFunctionPrime(this->y, yd) * cpr::Util::sigmoidPrime(this->agrega));
	cout <<"delta :" << delta << endl;
	return delta;
}

void cpr::Neurone::updateWeight(vector<double>& dw, double alpha)
{

	vector<double> temp;
	temp.resize(dw.size());
	
	for (int i = 0; i < this->Weight.size(); i++)
	{
		temp[i] = this->Weight[i] - alpha * dw[i];
	}
	this->setWeight(temp);
	
}

void cpr::Neurone::updateBiais(double delta, double alpha)
{
	cout << endl;
	cout << "Biais-fc before update:" << this->b << endl;
	double temp;
	temp=this->b - alpha * delta;
	this->setBiais(temp);
	cout << "Biais-fc after update:" << this->b << endl;
	cout << endl;
}
