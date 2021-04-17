#include <iostream>
#include <string>
#include <complex>
#include "Manager.h"
#include <fstream>
#include <string>
using namespace std;

int main() {
	Manager * man = new Manager;
	Eigen::MatrixXcd solutionMatrix;
	man->reciveinput();
	man->setIndices();
	solutionMatrix = man->EQNsolve(man->Equations());
	man->printresults(solutionMatrix);
}
