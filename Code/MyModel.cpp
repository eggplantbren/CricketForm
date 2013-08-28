#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:hidden(Data::get_instance().get_N())
{

}

void MyModel::fromPrior()
{
	mu1 = exp(log(1.) + log(100./1.)*randomU());
	mu2 = exp(log(1.) + log(100./1.)*randomU());
	if(mu1 > mu2)
	{
		double temp = mu1;
		mu1 = mu2;
		mu2 = temp;
	}

	w = randomU();
	beta = exp(log(1E-3) + log(1E6)*randomU());
	alpha = 0.9 + 0.1*randomU();
	for(size_t i=0; i<hidden.size(); i++)
		hidden[i] = randn();
}

double MyModel::perturb()
{
	double logH = 0.;

	int which = randInt(5);
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
	else if(which == 3)
	{
		beta = log(beta);
		beta += log(1E6)*pow(10., 1.5 - 6.*randomU())*randn();
		beta = mod(beta - log(1E-3), log(1E6)) + log(1E-3);
		beta = exp(beta);
	}
	else if(which == 4)
	{
		alpha += 0.1*pow(10., 1.5 - 6.*randomU())*randn();
		alpha = mod(alpha - 0.9, 0.1) + 0.9;
	}


	// Enforce order
	if(mu1 > mu2)
	{
		double temp = mu1;
		mu1 = mu2;
		mu2 = temp;
	}

	double chance = pow(10., 0.5 - 4.*randomU());
	double scale = pow(10., 1.5 - 6.*randomU());
	for(size_t i=0; i<hidden.size(); i++)
	{
		if(randomU() <= chance)
		{
			if(scale > 1.)
				hidden[i] = randn();
			else
			{
				logH -= -0.5*pow(hidden[i], 2);
				hidden[i] += scale*randn();
				logH += -0.5*pow(hidden[i], 2);
			}
		}
	}


	return logH;
}

void MyModel::print(std::ostream& out) const
{
	out<<mu1<<' '<<mu2<<' '<<w<<' '<<' '<<beta<<' '<<alpha<<' ';
	for(size_t i=0; i<hidden.size(); i++)
		out<<hidden[i]<<' ';
}

string MyModel::description() const
{
	string result = "mu1, mu2, w, beta, alpha, hidden";
	return result;
}

double MyModel::logLikelihood() const
{
	double logL = 0.;
	double logL1, logL2;

	double ww = w;
	for(int i=0; i<Data::get_instance().get_N(); i++)
	{
		if(Data::get_instance().get_out(i))
		{
			logL1 = log(ww) - log(mu1 + 1.)
					+ Data::get_instance().get_score(i)*log(mu1/(mu1 + 1.));
			logL2 = log(1. - ww) - log(mu2 + 1.)
					+ Data::get_instance().get_score(i)*log(mu2/(mu2 + 1.));
			logL += logsumexp(logL1, logL2);
		}
		else
		{
			logL1 = log(ww) + Data::get_instance().get_score(i)*log(mu1/(mu1 + 1.));
			logL2 = log(1. - ww)
					+ Data::get_instance().get_score(i)*log(mu2/(mu2 + 1.));
			logL += logsumexp(logL1, logL2);
		}

		// Find next weight
		double logit = alpha*log(w/(1. - w)) + beta*hidden[i];
		ww = exp(logit)/(1. + exp(logit));
	}

	return logL;
}

