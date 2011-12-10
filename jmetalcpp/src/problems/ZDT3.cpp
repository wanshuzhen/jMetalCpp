/*
 * ZDT3.cpp
 *
 *  Created on: 10/12/2011
 *      Author: antonio
 */

#include <ZDT3.h>

ZDT3::ZDT3(string solutionType, int numberOfVariables) {
	numberOfVariables_   = numberOfVariables;
	numberOfObjectives_  = 2;
	numberOfConstraints_ = 0;
	problemName_ 				 = "ZDT2";

	lowerLimit_ = new double[numberOfVariables_];//(double *)malloc(sizeof(double)*numberOfVariables);
	if (lowerLimit_ == NULL) {
		cout << "Impossible to reserve memory for storing the variable lower limits" << endl;
		exit(-1);
	}

	upperLimit_ = new double[numberOfVariables_];//(double *)malloc(sizeof(double)*numberOfVariables);
	if (upperLimit_ == NULL) {
		cout << "Impossible to reserve memory for storing the variable lower limits" << endl;
		exit(-1);
	}

	for (int i = 0; i < numberOfVariables_; i++) {
		lowerLimit_[i] = 0.0;
		upperLimit_[i] = 1.0;
	}

	if (solutionType.compare("BinaryReal") == 0)
		solutionType_ = new BinaryRealSolutionType(this) ;
	else if (solutionType.compare("Real") == 0) {
		solutionType_ = new RealSolutionType(this) ;
		cout << "Tipo seleccionado Real" << endl;
	}
	else if (solutionType.compare("ArrayReal") == 0)
		solutionType_ = new ArrayRealSolutionType(this) ;
	else {
		cout << "Error: solution type " << solutionType << " invalid" << endl;
		exit(-1) ;
	}
}


ZDT3::~ZDT3() {
  delete [] lowerLimit_ ;
  delete [] upperLimit_ ;
  delete solutionType_ ;
}

/**
 * Evaluates a solution
 * @param solution The solution to evaluate
 */
void ZDT3::evaluate(Solution *solution) {
	XReal * x = new XReal(solution);
	double * fx = new double[numberOfObjectives_] ;

	fx[0] = x->getValue(0) ;
  double g = evalG(x) ;
  double h = evalH(fx[0], g) ;
	fx[1] = h * g ;

	solution->setObjective(0,fx[0]);
	solution->setObjective(1,fx[1]);

	delete [] fx ;
	delete x ;
} // evaluate

double ZDT3::evalG(XReal * x) {
	double g = 0.0 ;
	for (int i = 1; i < x->getNumberOfDecisionVariables(); i++)
		g += x->getValue(i) ;

	double c = 9.0/(numberOfVariables_ - 1) ;
	g = c * g ;
  g = g + 1.0 ;
	return g;
}

double ZDT3::evalH(double f, double g) {
	double h = 0.0 ;
	h = 1.0 - sqrt(f/g) - (f/g)*sin(10.0*M_PI*f);
	return h ;
}
