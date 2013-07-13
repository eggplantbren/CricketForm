#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
{
}

void MyModel::fromPrior()
{
	mu1 = exp(log(1.) + log(100./1.)*randomU());
	mu2 = exp(log(1.) + log(100./1.)*randomU());
	w = randomU();

	if(mu1 > mu2)
	{
		double temp = mu1;
		mu1 = mu2;
		mu2 = temp;
	}
}

double MyModel::perturb()
{
	double logH = 0.;

	int which = randInt(3);
	if(which == 0)
	{
		mu1 = log(mu1);
		mu1 += log(100./1.)*pow(10., 1.5 - 6.*randomU())*randn();
		mu1 = mod(mu1 - log(1.), log(100.)) + log(1.);
		mu1 = exp(mu1);
	}
	else if(which == 1)
	{
		mu2 = log(mu2);
		mu2 += log(100./1.)*pow(10., 1.5 - 6.*randomU())*randn();
		mu2 = mod(mu2 - log(1.), log(100.)) + log(1.);
		mu2 = exp(mu2);
	}
	else if(which == 2)
	{
		w += pow(10., 1.5 - 6.*randomU())*randn();
		w = mod(w, 1.);
	}

	// Enforce order
	if(mu1 > mu2)
	{
		double temp = mu1;
		mu1 = mu2;
		mu2 = temp;
	}

	return logH;
}

void MyModel::print(std::ostream& out) const
{
	out<<mu1<<' '<<mu2<<' '<<w<<' ';
}

string MyModel::description() const
{
	string result = "mu1, mu2, w";
	return result;
}

double MyModel::logLikelihood() const
{
	double logL = 0.;
	double logL1, logL2;
	for(int i=0; i<Data::get_instance().get_N(); i++)
	{
		if(Data::get_instance().get_out(i))
		{
			logL1 = log(w) - log(mu1 + 1.)
					+ Data::get_instance().get_score(i)*log(mu1/(mu1 + 1.));
			logL2 = log(1. - w) - log(mu2 + 1.)
					+ Data::get_instance().get_score(i)*log(mu2/(mu2 + 1.));
			logL += logsumexp(logL1, logL2);
		}
		else
		{
			logL1 = log(w) + Data::get_instance().get_score(i)*log(mu1/(mu1 + 1.));
			logL2 = log(1. - w)
					+ Data::get_instance().get_score(i)*log(mu2/(mu2 + 1.));
			logL += logsumexp(logL1, logL2);
		}
	}

	return logL;
}

