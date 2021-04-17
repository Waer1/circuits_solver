#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <complex>
#include "Comp.h"
#include "vsrc.h"
#include "vcvs.h"
#include "cap.h"
#include "cccs.h"
#include "ccvs.h"
#include "dependant.h"
#include "ind.h"
#include "independant.h"
#include "isrc.h"
#include "Node.h"
#include "res.h"
#include "resistors.h"
#include "Sources.h"
#include "vccs.h"
#include "vcvs.h"
#include "vsrc.h"
#include "Node.h"

using namespace Eigen;
using namespace std;

class Manager
{
	//assing the value of omega
	double omega;
	//here we will create vector have 
	vector <Node*> Nodes;
	//here the array of component that have all components on it
	vector <Comp*> comps;
	// vector of the passive element
	vector <resistors *> passive;

	//pair of nodes and its index which will arrang the matrix of unknown
	vector< pair<Node*, int> >nodesMatrixIndices;

	// no of colomuns
	int NoOfColumns;

	//the vector of of the super battry
	vector <Comp*> ndSup;

	//Data members for IsVoltSource()
	vector<Node*>RelatedNodes;
	
	//array of the other nodes that have super node relations
	vector<Node*>OtherTerminals;

	// vector of the node will treat as normal nodes
	vector <Node*> Normal;

public:
	//the function that recive input
	void reciveinput();
	//the function that implement the recived input
	void ReadUserInput(string type, string name, int end, int start, double value = -1.0, double phase = -1, int endre = -1, int startre = -1, string namere = "hamada", double coff = -1);
	//set the omega to it's
	void setomega(double);
	//set the index of unknowns in theres places
	void setIndices();
	// give the node aand return it's his index in the matrix 
	int getNodeIndex(Node*);
	//set the ground 
	void setground(int n);
	// check if the node exist and ret
	Node* checkNode(int n);
	double getomega();
	void print();
	void printnodes();
	complex<double> getImpedance(Node* endNode, Node* startNode);
	complex<double> getImpedance(int endNode, int startNode);
	vector<string> getType(int node1, int node2);
	string getType2(Comp* comp);
	vector<Comp*> getComp(int node1, int node2);
	Comp* getOneComp(int node1, int node2);

	vector<Node*> getrelatedNodetocomp(Comp* target);
	vector<Node*> getRelatedNodes(Node* target);


	bool isSuperRelated(Node * ,Node *);
	
	
	vector<Node*> getoppsiteNodes(Node* target);
		
	vector < vector<Node*> > getsupercol();
	vector<Node*> getSorted(vector<Node*> & kl);
	vector<Node*> getbasNodes(Node *);


	vector< complex<double> > getVsrcEqu(vsrc * vcomp );
	vector< complex<double> > getVcvsEqu(vcvs* vcomp);
	vector< complex<double> > getCcvsEqu(ccvs* vcomp);

	vector<Node* > gae(vector<Node* > & target);

	Node* getNodeOfIndex(int);


	pair <vector<complex<double>>, complex<double> > getsupeq(vector<Node*> needcol);
	pair < vector<complex<double>>, complex<double> > getNoreq(Node * target);

	vector < vector<complex<double>> > Equations();

	MatrixXcd EQNsolve(vector<vector<complex<double>>> V);
	void printresults(Eigen::MatrixXcd);



};