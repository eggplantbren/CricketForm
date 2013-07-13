#ifndef _MyModel_
#define _MyModel_

#include "Model.h"
#include <vector>

class MyModel:public DNest3::Model
{
	private:
		// Mixture of two geometrics
		double mu1, mu2, w;

	public:

		MyModel();
		void fromPrior();
		double perturb();

		double logLikelihood() const;
		void print(std::ostream& out) const;
		std::string description() const;
};

#endif

