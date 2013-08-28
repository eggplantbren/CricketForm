#ifndef _MyModel_
#define _MyModel_

#include "Model.h"
#include <vector>

class MyModel:public DNest3::Model
{
	private:
		// Mixture of two geometrics
		double mu1, mu2;

		// Initial weight
		double w;

		// Short term variability
		double beta;

		// Retain
		double alpha;

		// These determine the mixture weight as a function of time
		std::vector<double> hidden;

	public:

		MyModel();
		void fromPrior();
		double perturb();

		double logLikelihood() const;
		void print(std::ostream& out) const;
		std::string description() const;
};

#endif

