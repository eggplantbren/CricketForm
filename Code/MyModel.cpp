#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
{
}

void MyModel::fromPrior()
{
	mu0 = exp(log(3.) + log(60./3.)*randomU());
	mu1 = exp(log(3.) + log(60./3.)*randomU());
	L = exp(log(0.1) + log(100./0.1)*randomU());
}

double MyModel::perturb1()
{
	int which = randInt(3);
	if(which == 0)
	{
		mu0 = log(mu0);
		mu0 += log(60./3.)*pow(10., 1.5 - 6.*randomU())*randn();
		mu0 = mod(mu0 -  log(3.), log(60./3.)) + log(3.);
		mu0 = exp(mu0);
	}
	if(which == 1)
	{
		mu1 = log(mu1);
		mu1 += log(60./3.)*pow(10., 1.5 - 6.*randomU())*randn();
		mu1 = mod(mu1 -  log(3.), log(60./3.)) + log(3.);
		mu1 = exp(mu1);
	}
	if(which == 2)
	{
		L = log(L);
		L += log(100./0.1)*pow(10., 1.5 - 6.*randomU())*randn();
		L = mod(L -  log(0.1), log(100./0.1)) + log(0.1);
		L = exp(L);
	}
	return 0.;
}

double MyModel::perturb()
{
	double logH = 0.;
	int passes = 1 + randInt(4);
	for(int i=0; i<passes; i++)
		logH += perturb1();
	return logH;
}

void MyModel::print(std::ostream& out) const
{
	out<<mu0<<' '<<mu1<<' '<<L<<' ';
}

string MyModel::description() const
{
	string result = "mu0, mu1, L";
	return result;
}

double MyModel::logLikelihood() const
{
	double logL = 0.;

	return logL;
}

