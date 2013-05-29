#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest3;

const int MyModel::max_score = 500;

MyModel::MyModel()
:effective_average(max_score)
,log_prob_geq(max_score)
,log_prob_equal(max_score)
{
}

void MyModel::assemble()
{
	// Compute effective average as a function of score
	for(int i=0; i<max_score; i++)
		effective_average[i] = mu1 + (mu0 - mu1)*exp(-i/L);

	// Compute log[P(X >= x)]
	log_prob_geq[0] = 0.;
	for(int i=1; i<max_score; i++)
	{
		log_prob_geq[i] = log_prob_geq[i-1] +
		log(effective_average[i-1]/(effective_average[i-1] + 1.));
	}

	// Compute log[P(X=x)] = log[P(X >= x) - P(X >= x+1)]
	double g1, g2, f, max;
	for(int i=0; i<max_score - 1; i++)
	{
		g1 = log_prob_geq[i];
		g2 = log_prob_geq[i+1];
		max = g1;
		if(g2 > g1)
			max = g2;
		f = exp(g1 - max) - exp(g2 - max);
		log_prob_equal[i] = log(f) + max;
	}
	log_prob_equal[max_score - 1] = log_prob_geq[max_score - 1];
}

void MyModel::fromPrior()
{
	mu0 = exp(log(3.) + log(60./3.)*randomU());
	mu1 = exp(log(3.) + log(60./3.)*randomU());
	L = exp(log(0.1) + log(100./0.1)*randomU());

	assemble();
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
	assemble();
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

	for(int i=0; i<Data::get_instance().get_N(); i++)
	{
		if(Data::get_instance().get_out(i))
		{
			logL += log_prob_equal
				[Data::get_instance().get_score(i)];
		}
		else
		{
			logL += log_prob_geq
				[Data::get_instance().get_score(i)];
		}
	}

	return logL;
}

