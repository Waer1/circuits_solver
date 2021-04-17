#include "Manager.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
#include <vector>
#include <complex>
#include <fstream>

using namespace Eigen;
using namespace std;

void Manager::ReadUserInput(string type, string name, int end, int start, double value, double phase, int endre, int startre, string namere, double coff) {

	Comp* comp = NULL;
	Node* stratN;
	Node* endN;
	Node* stratreN;
	Node* endreN;


	endN = checkNode(end);
	stratN = checkNode(start);


	if (type == "vsrc") {
		
		comp = new vsrc(name, endN, stratN, value , phase);

	} else if (type == "isrc") {

		comp = new isrc(name, endN, stratN, value, phase);
	}
	else if (type == "res") {
	
		comp = new res(name, endN, stratN, value);
	}else if (type == "cap") {
		
		double hamda = -1.0 / (value * omega * 1.0);
		comp = new cap(name, endN, stratN, hamda);
	}else if (type == "ind") {
		double hamda = (value * omega * 1.0);
		comp = new ind(name, endN, stratN, hamda);

	}else if (type == "vcvs") {
		endreN = checkNode(endre);
		stratreN = checkNode(startre);

		comp = new vcvs(name, endN, stratN , endreN, stratreN , coff);
	}else if (type == "ccvs") {

		endreN = checkNode(endre);
		stratreN = checkNode(startre);

		comp = new ccvs(name, endN, stratN, endreN, stratreN, namere, coff);
	}else if (type == "cccs") {

		endreN = checkNode(endre);
		stratreN = checkNode(startre);

		comp = new cccs(name, endN, stratN, endreN, stratreN, namere ,coff );
	}else if (type == "vccs") {
		endreN = checkNode(endre);
		stratreN = checkNode(startre);

		comp = new vccs(name, endN, stratN, endreN, stratreN, coff);
	}

	comps.push_back(comp);
	if (type == "ind" || type == "cap" || type == "res")
	{
		passive.push_back((resistors *)comp);
	}

}

pair <vector<complex<double>>, complex<double>> Manager::getsupeq(vector<Node*> needcol)
{

	//To be returned
	pair<vector<complex<double>>, complex<double>> Final;
	vector<Node*> loopednode = gae(needcol);

	//Temporary array for scalars of our linear system
	complex<double>* arr = new complex<double>[NoOfColumns];

	// assing intial values to this temp array 
	for (int w = 0; w < NoOfColumns; w++)
	{
		arr[w] = 0.0;
	}
	vector < complex<double> > ham;
	complex <double> scaler;

	for (int m = 0; m < needcol.size(); m++) {


		for (int p = 0; p < loopednode.size(); p++) {

			vector<Comp*> hello = getComp(needcol[m]->getnum(), loopednode[p]->getnum());
			vector <string> stringofcomps;

			//Setting the related node to our super node as used
			loopednode[p]->setusednode();


			for (int s = 0; s < hello.size(); s++) {
				stringofcomps.push_back(getType2(hello[s]));
			}

			for (int r = 0; r < stringofcomps.size(); r++)
			{
				//This is the component we're dealing with hello[r]
				//The component between two nodes:
				//loopenode[p] and needcol[m]

				if (stringofcomps[r] == "vsrc" || stringofcomps[r] == "vcvs" || stringofcomps[r] == "ccvs") {}
				else if (stringofcomps[r] == "isrc")
				{
					isrc* temp = (isrc*)hello[r];
					if (needcol[m]->getnum() == hello[r]->getEndNode())
						scaler += temp->getCurrent();
					else
						scaler -= temp->getCurrent();
				}
				else if (stringofcomps[r] == "cccs")
				{
					cccs* temp = (cccs*)hello[r];
					int endRef = getNodeIndex(temp->getEndNoderef());
					int strtRef = getNodeIndex(temp->getstartNoderef());
					double coeff = temp->getCoff();
					Node * refend = temp->getEndNoderef();
					Node * refstrat = temp->getstartNoderef();

					complex<double> imp = getImpedance(refend, refstrat);

					arr[endRef] -= (coeff / imp);
					arr[strtRef] += (coeff / imp);

				}
				else if (stringofcomps[r] == "vccs")
				{
					vccs* temp = (vccs*)hello[r];
					int endRef = getNodeIndex(temp->getEndNoderef());
					int strtRef = getNodeIndex(temp->getstartNoderef());
					double coeff = temp->getCoff();


					arr[endRef] -= (coeff);
					arr[strtRef] += (coeff);
				}
				else if (stringofcomps[r] == "res" || stringofcomps[r] == "cap" || stringofcomps[r] == "ind")
				{
					resistors* temp = (resistors*)hello[r];
					int endIndx = getNodeIndex(needcol[m]);
					int strtIndx = getNodeIndex(loopednode[p]);
					complex<double> G = 1.0 / temp->getComplex();

					arr[endIndx] += G;

					arr[strtIndx] -= G;

				}


			}

		}

	}
	//Pushing back the temp array values into the row
	for (int w = 0; w < NoOfColumns; w++)
	{
		ham.push_back(arr[w]);
	}

	Final.first = ham;
	Final.second = scaler;

	return Final;
}

void Manager::setIndices()
{
	NoOfColumns = Nodes.size() - 1;
	int k{0};	//Index of the columns
	
	// loop to find the nodes except the ground and put them into parswith numbers
	for (int i = 0; i < Nodes.size(); i++)
	{
		complex<double>hamada(0.0, 0.0);
		if (Nodes[i]->getVolt() != hamada)
		{
			pair<Node*, int>norm(Nodes[i], k);
			nodesMatrixIndices.push_back(norm);
			k++;
		}
	}

	for (int i = 0; i < Nodes.size(); i++)
	{
		vector <Node*> relatedoo;
		relatedoo = getRelatedNodes(Nodes[i]);

		for (int w = 0 ; w < relatedoo.size();w++)
		{
			vector <Comp*> relatedcomps;
			relatedcomps = getComp(Nodes[i]->getnum(), relatedoo[w]->getnum());

			for (int y = 0; y < relatedcomps.size() ;y++){
				string typo = getType2(relatedcomps[y]);

				if (typo == "vsrc" || typo == "vcvs" ||typo == "ccvs") {
					Nodes[i]->setnormal();
				}
			
			}
			
		

		}



	}




}

int Manager::getNodeIndex(Node* random)
{
	for (int i = 0; i < nodesMatrixIndices.size(); i++)
	{
		if (random->getnum() == nodesMatrixIndices[i].first->getnum())
			return nodesMatrixIndices[i].second;
	}
	return -1;	//If the node is Ground
}

void Manager::setomega(double omegav)
{
	omega = omegav;
}

void Manager::setground(int n)
{

	for (int i = 0; i < Nodes.size(); i++) {
	
		if (Nodes[i]->getnum() == n) {
			Nodes[i]->setVoltage(0.0);
		}

	}

}

Node* Manager::checkNode(int n)
{

	for (int i = 0; i < Nodes.size(); i++) {
		if (Nodes[i]->getnum() == n) {
			return Nodes[i];
		}
	
	}

	Node* nod = new Node(n);
	Nodes.push_back(nod);
	
	return nod;

}

double Manager::getomega()
{
	return omega;
}

void Manager::print()
{

	for (int i = 0; i < comps.size(); i++)
	{
		comps[i]->print();
	}

}

void Manager::printnodes()
{
	cout << Nodes.size() << endl;
}

void Manager::reciveinput()
{


	fstream newfile;
	ifstream ip;

	ip.open("tpoint.txt", ios::in); //open a file to perform read operation using file object

	if (ip.is_open()) { //checking whether the file is open
		string type, name, namere, tp;
		int endnode, startnode, endre, startre,gnd;
		double omega, value, phase, coff;

		do {
			ip >> type;

			if (type == "w") {
				ip >> omega;
				setomega(omega);
			}
			else if (type == "cap")
			{
				ip >> name >> endnode >> startnode >> value;
				ReadUserInput(type, name, endnode, startnode, value);
			}
			else if (type == "ind")
			{
				ip >> name >> endnode >> startnode >> value;
				ReadUserInput(type, name, endnode, startnode, value);
			}
			else if (type == "res")
			{
				ip >> name >> endnode >> startnode >> value;
				ReadUserInput(type, name, endnode, startnode, value);
			}
			else if (type == "vsrc")
			{
				ip >> name >> endnode >> startnode >> value >> phase;
				ReadUserInput(type, name, endnode, startnode, value, phase);
			}
			else if (type == "isrc")
			{
				ip >> name >> endnode >> startnode >> value >> phase;
				ReadUserInput(type, name, endnode, startnode, value, phase);
			}
			else if (type == "vcvs")
			{
				ip >> name >> endnode >> startnode >> endre >> startre >> coff;
				ReadUserInput(type, name, endnode, startnode, -1, -1, endre, startre, "hamda", coff);
			}
			else if (type == "vccs")
			{
				ip >> name >> endnode >> startnode >> endre >> startre >> coff;
				ReadUserInput(type, name, endnode, startnode, -1, -1, endre, startre, "hamda", coff);
			}
			else if (type == "ccvs")
			{
				ip >> name >> endnode >> startnode >> endre >> startre >> namere >> coff;
				ReadUserInput(type, name, endnode, startnode, -1, -1, endre, startre, namere, coff);
			}
			else if (type == "cccs")
			{
				ip >> name >> endnode >> startnode >> endre >> startre >> namere >> coff;
				ReadUserInput(type, name, endnode, startnode, -1, -1, endre, startre, namere, coff);
			} else if (type == "node")
			{
				ip >> gnd;
				setground(gnd);
			}


		} while (getline(ip, tp));

		ip.close();

	}



}

complex<double> Manager::getImpedance(Node * endNode, Node * startNode)
{
	complex<double> z;
	vector<resistors*> psv;

	for (int i = 0; i < passive.size(); i++)
	{
		int elend = passive[i]->getEndNode();
		int elstrt = passive[i]->getStartNode();

		if ( (elend == endNode->getnum() && elstrt == startNode->getnum()) || (elend == startNode->getnum() && elstrt == endNode->getnum()) )
		{
			psv.push_back(passive[i]);
		}
	}

	if (psv.empty())
		return complex<double>(0,0);
	else
	{
		complex<double> all(0.0, 0.0);

		for (int k = 0; k < psv.size(); k++)
		{
			complex<double> temp = psv[k]->getComplex();
			all += 1.0 / temp;
		}
		z = 1.0 / all;
	}
	
	return z;
}

complex<double> Manager::getImpedance(int endNode, int startNode)
{
	complex<double> z;
	vector<resistors*> psv;

	for (int k = 0; k < passive.size(); k++)
	{
		int elend = passive[k]->getEndNode();
		int elstrt = passive[k]->getStartNode();

		if ( (elend == endNode && elstrt == startNode) || (elend == startNode && elstrt == endNode) )
		{
			psv.push_back(passive[k]);
		}
	}

	if (psv.empty())
		return complex<double>(0, 0);
	else
	{
		complex<double> all(0.0, 0.0);

		for (int k = 0; k < psv.size(); k++)
		{
			complex<double> temp = psv[k]->getComplex();
			all += 1.0 / temp;
		}
		z = 1.0 / all;
	}

	return z;
}

vector<string> Manager::getType(int node1, int node2)
{
	vector<Comp*>ptobj = getComp(node1, node2);
	vector<string> vstr;
	for (int i = 0; i < ptobj.size(); i++)
	{
		Comp* hamada1 = dynamic_cast<res*>(ptobj[i]);
		if (hamada1)
			vstr.push_back("res");

		Comp* hamada2 = dynamic_cast<cap*>(ptobj[i]);
		if (hamada2)
			vstr.push_back("cap");

		Comp* hamada3 = dynamic_cast<ind*>(ptobj[i]);
		if (hamada3)
			vstr.push_back("ind");

		Comp* hamada4 = dynamic_cast<vsrc*>(ptobj[i]);
		if (hamada4)
			vstr.push_back("vsrc");

		Comp* hamada5 = dynamic_cast<isrc*>(ptobj[i]);
		if (hamada5)
			vstr.push_back("isrc");

		Comp* hamada6 = dynamic_cast<cccs*>(ptobj[i]);
		if (hamada6)
			vstr.push_back("cccs");

		Comp* hamada7 = dynamic_cast<ccvs*>(ptobj[i]);
		if (hamada7)
			vstr.push_back("ccvs");

		Comp* hamada8 = dynamic_cast<vcvs*>(ptobj[i]);
		if (hamada8)
			vstr.push_back("vcvs");

		Comp* hamada9 = dynamic_cast<vccs*>(ptobj[i]);
		if (hamada9)
			vstr.push_back("vccs");
	}

	return vstr;
}


string Manager::getType2(Comp* comp)
{
	string type;

	Comp* hamada1 = dynamic_cast<res*>(comp);
	if (hamada1)
		return "res";

	Comp* hamada2 = dynamic_cast<cap*>(comp);
	if (hamada2)
		return "cap";

	Comp* hamada3 = dynamic_cast<ind*>(comp);
	if (hamada3)
		return "ind";

	Comp* hamada4 = dynamic_cast<vsrc*>(comp);
	if (hamada4)
		return "vsrc";

	Comp* hamada5 = dynamic_cast<isrc*>(comp);
	if (hamada5)
		return "isrc";

	Comp* hamada6 = dynamic_cast<cccs*>(comp);
	if (hamada6)
		return "cccs";

	Comp* hamada7 = dynamic_cast<ccvs*>(comp);
	if (hamada7)
		return "ccvs";

	Comp* hamada8 = dynamic_cast<vcvs*>(comp);
	if (hamada8)
		return "vcvs";

	Comp* hamada9 = dynamic_cast<vccs*>(comp);
	if (hamada9)
		return "vccs";

}

vector<Comp*> Manager::getComp(int node1, int node2)
{
	//random vector of components to be returned
	vector<Comp*>component;

	//Loops around the components
	for (int k = 0; k < comps.size(); k++)
	{
		int elend = comps[k]->getEndNode();
		int elstrt = comps[k]->getStartNode();
		//Checks if the component is between the two passed nodes
		if ((elend == node1 && elstrt == node2) || (elend == node2 && elstrt == node1))
		{
			component.push_back(comps[k]);
		}
	}
	return component;
}


vector<complex<double>> Manager::getVsrcEqu(vsrc* vcomp)
{

	//Temporary array for scalars of our linear system
	complex<double>* tempEquation = new complex<double>[NoOfColumns];

	// assing intial values to this temp array 
	for (int w = 0; w < NoOfColumns; w++)
	{
		tempEquation[w] = 0.0;
	}

	Node* end1 = checkNode(vcomp->getEndNode());
	Node* strt1 = checkNode(vcomp->getStartNode());

	end1->setusednode();
	strt1->setusednode();

	int endIndx = getNodeIndex(end1);
	int strtIndx = getNodeIndex(strt1);

	if (endIndx != -1)
		tempEquation[endIndx] += 1.0 + 0i;

	if (strtIndx != -1)
		tempEquation[strtIndx] += -1.0 + 0i;


	if (endIndx != -1 && strtIndx != -1)
		ndSup.push_back(vcomp);


	//Vector that carries the row of coefficients
	vector<complex<double>>row;

	//Pushing back the temp array values into the row
	for (int w = 0; w < NoOfColumns; w++)
	{
		row.push_back(tempEquation[w]);
	}

	return row;
}

vector<complex<double>> Manager::getVcvsEqu(vcvs* vcomp)
{

	//Temporary array for scalars of our linear system
	complex<double>* tempEquation = new complex<double>[NoOfColumns];

	// assing intial values to this temp array 
	for (int w = 0; w < NoOfColumns; w++)
	{
		tempEquation[w] = 0.0;
	}

	Node* end1 = checkNode(vcomp->getEndNode());
	Node* strt1 = checkNode(vcomp->getStartNode());
	int endIndx = getNodeIndex(end1);
	int strtIndx = getNodeIndex(strt1);

	end1->setusednode();
	strt1->setusednode();

	int coeff = vcomp->getCoff();
	Node* refEnd = vcomp->getEndNoderef();
	Node* refStrt = vcomp->getstartNoderef();
	int refEndIndx = getNodeIndex(refEnd);
	int refStrtIndx = getNodeIndex(refStrt);


	if (endIndx != -1)
		tempEquation[endIndx] += 1.0 + 0i;

	if (strtIndx != -1)
		tempEquation[strtIndx] -= 1.0 + 0i;

	if (refEndIndx != -1)
		tempEquation[refEndIndx] -= coeff;

	if (refStrtIndx != -1)
		tempEquation[refStrtIndx] += coeff;

	//Vector that carries the row of coefficients
	vector<complex<double>>row;

	if (endIndx != -1 && strtIndx != -1)
		ndSup.push_back(vcomp);

	//Pushing back the temp array values into the row
	for (int w = 0; w < NoOfColumns; w++)
	{
		row.push_back(tempEquation[w]);
	}


	return row;
}

vector<complex<double>> Manager::getCcvsEqu(ccvs* vcomp)
{
	//Temporary array for scalars of our linear system
	complex<double>* tempEquation = new complex<double>[NoOfColumns];

	// assing intial values to this temp array 
	for (int w = 0; w < NoOfColumns; w++)
	{
		tempEquation[w] = 0.0;
	}

	Node* end1 = checkNode(vcomp->getEndNode());
	Node* strt1 = checkNode(vcomp->getStartNode());
	int endIndx = getNodeIndex(end1);
	int strtIndx = getNodeIndex(strt1);

	end1->setusednode();
	strt1->setusednode();

	Node* refEnd = vcomp->getEndNoderef();
	Node* refStrt = vcomp->getstartNoderef();
	int refEndIndx = getNodeIndex(refEnd);
	int refStrtIndx = getNodeIndex(refStrt);
	complex<double>coeff = (1.0 / getImpedance(refEnd, refStrt)) * vcomp->getCoff();


	if (endIndx != -1)
		tempEquation[endIndx] += 1.0 + 0i;

	if (strtIndx != -1)
		tempEquation[strtIndx] -= 1.0 + 0i;

	if (refEndIndx != -1)
		tempEquation[refEndIndx] -= coeff;

	if (refStrtIndx != -1)
		tempEquation[refStrtIndx] += coeff;


	if (endIndx != -1 && strtIndx != -1)
		ndSup.push_back(vcomp);

	//Vector that carries the row of coefficients
	vector<complex<double>>row;

	//Pushing back the temp array values into the row
	for (int w = 0; w < NoOfColumns; w++)
	{
		row.push_back(tempEquation[w]);
	}


	return row;
}

Comp* Manager::getOneComp(int node1, int node2)
{
	//random vector of components to be returned
	Comp* component;

	//Loops around the components
	for (int k = 0; k < comps.size(); k++)
	{
		int elend = comps[k]->getEndNode();
		int elstrt = comps[k]->getStartNode();
		//Checks if the component is between the two passed nodes
		if ((elend == node1 && elstrt == node2) || (elend == node2 && elstrt == node1))
		{
			component = comps[k];
			return component;
		}
	}

	return NULL;

}


bool Manager::isSuperRelated(Node* first, Node* second)
{
	Comp* alahly = getOneComp(first->getnum(), second->getnum());

	if (alahly != NULL)
		for (int h = 0; h < ndSup.size(); h++)
			if (alahly == ndSup[h])
				return 1;

	return 0;
};

vector<Node*> Manager::getrelatedNodetocomp(Comp* target)
{
	Node* endooo = checkNode(target->getEndNode());
	Node* startooo = checkNode(target->getStartNode());

	vector<Node*> realtedToComp;

	vector<Node*> realtedTostart = getRelatedNodes(startooo);
	vector<Node*> realtedToend = getRelatedNodes(endooo);

	for (int i = 0; i < realtedTostart.size(); i++)
	{
		realtedToComp.push_back(realtedTostart[i]);
	}

	for (int i = 0; i < realtedToend.size(); i++)
	{
		realtedToComp.push_back(realtedToend[i]);
	}




	return realtedToComp;
}

// get the other node that related with targetnode and shdeda battry
vector<Node*> Manager::getRelatedNodes(Node* target)
{
	vector<Node*>Related;

	vector<Node*> lepn = getoppsiteNodes(target);

	for (int i = 0; i < lepn.size(); i++)
	{
		Comp* alahly = getOneComp(target->getnum(), lepn[i]->getnum());

		if (alahly != NULL) {
			Related.push_back(lepn[i]);
		}

	}


	return Related;
}

vector<Node*> Manager::getoppsiteNodes(Node* target)
{
	vector<Node*>Related;

	for (int i = 0; i < Nodes.size(); i++)
	{
		if (target != Nodes[i])
			Related.push_back(Nodes[i]);
	}

	return Related;
}

// algoritmh that detect the nodes that need to do super node on them  
vector<vector<Node*>> Manager::getsupercol()
{
	vector<vector<Node*>> allsup;
	vector<Node*> relatedtosheded;
	vector<Node*> bas;
	bas = getSorted(relatedtosheded);

	// push the bas node to vector
	// loop on all bas nodes then get the related nodes to sheded vs and 

	for (int i = 0; i < bas.size(); i++) {

		if (!(bas[i]->getstbas())) {

			vector <Node*> wahed;
			wahed.push_back(bas[i]);
			bas[i]->setusedbas();
			//bas[i]->setusednode();

			vector <Node*> mainbas = getbasNodes(bas[i]);

			for (int u = 0; u < mainbas.size(); u++){
					wahed.push_back(mainbas[u]);
					mainbas[u]->setusednode();
					vector <Comp*> haha = getComp(bas[i]->getnum(), mainbas[u]->getnum());

						for (int p = 0; p < haha.size(); p++) {
							haha[p]->setbattry();
						}

			}


			for (int e = 0; e < bas.size(); e++) {

				if (!(bas[e]->getstbas()) && isSuperRelated(bas[i], bas[e])) {
						wahed.push_back(bas[e]);

						bas[e]->setusedbas();

					vector <Node*> sub = getbasNodes(bas[e]);
					for (int u = 0; u < sub.size(); u++) {

						wahed.push_back(sub[u]);

						vector <Comp*> hyhy = getComp(bas[e]->getnum(), sub[u]->getnum());

						for (int p = 0; p < hyhy.size(); p++) {
							hyhy[p]->setbattry();
						}

					}
				}
			}
			allsup.push_back(wahed);
		}



	}

	for (int j = 0; j < ndSup.size(); j++) {
		if (ndSup[j]->getbattry() == 0) {
			vector <Node*> sad;
			sad.push_back(checkNode(ndSup[j]->getStartNode()));
			sad.push_back(checkNode(ndSup[j]->getEndNode()));
			allsup.push_back(sad);
			ndSup[j]->setbattry();
		}
	}

	return allsup;
}

vector<Node*> Manager::getSorted(vector<Node*>& related)
{
	vector<Node*> relatedtosheded;
	vector<Node*> bas;

	for (int y = 0; y < ndSup.size(); y++)
	{
		Node* startqq = checkNode(ndSup[y]->getStartNode());
		Node* endqq = checkNode(ndSup[y]->getEndNode());
		relatedtosheded.push_back(startqq);
		relatedtosheded.push_back(endqq);
	}
	vector<Node*> duplicate;

	sort(relatedtosheded.begin(), relatedtosheded.end());

	for (int i = 1; i < relatedtosheded.size(); i++) {
		if (relatedtosheded[i - 1] == relatedtosheded[i]) {

			if (duplicate.empty())
				duplicate.push_back(relatedtosheded[i]);
			else if (relatedtosheded[i] != duplicate.back())
				duplicate.push_back(relatedtosheded[i]);
		}
	}

	for (int i = 0; i < duplicate.size(); i++)
		bas.push_back(duplicate[i]);



	sort(relatedtosheded.begin(), relatedtosheded.end());
	relatedtosheded.erase(unique(relatedtosheded.begin(), relatedtosheded.end()), relatedtosheded.end());


	related = relatedtosheded;


	return bas;
}

vector<Node*> Manager::getbasNodes(Node* target)
{
	vector<Node*> temp = getRelatedNodes(target);
	vector<Node*> Sended;
	for (int i = 0; i < temp.size(); i++)
	{
		Comp* alahly = getOneComp(target->getnum(), temp[i]->getnum());


		for (int h = 0; h < ndSup.size(); h++)
			if (alahly == ndSup[h])
				Sended.push_back(temp[i]);


	}


	return Sended;
}

vector<Node*> Manager::gae(vector <Node* > & targets)
{
	vector<Node*> returned;
	for (int i = 0; i < Nodes.size(); i++) {
		int checker{0};
		
		for (int q = 0; q < targets.size(); q++) 
			if ( Nodes[i] != targets[q] ) 
				checker++;
			

			if (checker < targets.size()){}
			else
			{
				returned.push_back(Nodes[i]);
			}
		
	}

	return returned;
}

Node * Manager::getNodeOfIndex(int x)
{
	for (int i = 0; i < nodesMatrixIndices.size(); i++)
	{
		if (x == nodesMatrixIndices[i].second)
			return nodesMatrixIndices[i].first;
	}
}

vector < vector<complex<double>> > Manager::Equations()
{
	vector< vector<complex<double>> >Eqs;
	vector< complex<double>> scalers;
	//for vlotage sources
	for (int i = 0; i < comps.size(); i++) {
	
		string type = getType2(comps[i]);
		
		if (type == "vsrc" ) {
			
			vsrc* vol = (vsrc *)comps[i];
			Eqs.push_back( getVsrcEqu(vol) );
			scalers.push_back(vol->getVolt());
		}
		else if(type == "ccvs")
		{
			
			ccvs* vol = (ccvs*)comps[i];
			Eqs.push_back( getCcvsEqu(vol) );
			scalers.push_back(0.0);
		}
		else if (type == "vcvs")
		{
			
			vcvs* vol = (vcvs*)comps[i];
			Eqs.push_back( getVcvsEqu(vol) );
			scalers.push_back(0.0);
		}
	}
	
	// for super nodes
	if (!ndSup.empty()) {
		vector < vector<Node*> > sucol;

		 sucol = getsupercol();

		for (int p = 0; p < sucol.size(); p++) {
			pair < vector<complex<double>> , complex<double> > pablo;
			pablo = getsupeq(sucol[p]);
			Eqs.push_back(pablo.first);
			scalers.push_back(pablo.second);


		}

	}

	//for normal point
	for (int q = 0; q < Nodes.size(); q++) {

		if (!(Nodes[q]->getnormal()) && Nodes[q]->getVolt() != 0.0) {
			pair < vector<complex<double>>, complex<double> > toti;
			toti = getNoreq(Nodes[q]);
			Eqs.push_back(toti.first);
			scalers.push_back(toti.second);
		}

	}


	for(int y = 0 ; y < Eqs.size() ;y++)
	{
		for (int h = 0; h < Eqs[y].size(); h++) {
			cout << Eqs[y][h] << " - ";
		}
		cout << scalers[y] << endl;
	}


	Eqs.push_back(scalers);
	return Eqs;
}

Eigen::MatrixXcd Manager::EQNsolve(vector<vector<complex<double>>> V)
{
	Eigen::MatrixXcd A((V.size() - 1), (V[0].size()));   //Coeff Matrix
	Eigen::MatrixXcd B((V[V.size() - 1].size()), 1);   //Scaler Matrix
	for (int i = 0; i < V.size() - 1; i++) //A fill
	{
		for (int j = 0; j < V[i].size(); j++)
		{
			A(i, j) = V[i][j];
		}
	}
	for (int j = 0; j < (V[V.size() - 1].size()); j++)// B fill
	{
		B(j, 0) = V[V.size() - 1][j];
	}
	MatrixXcd X((V[0].size()), 1);
	//X= (inv)A B
	X = (A.inverse()) * B;



	return X;
}

 pair<vector<complex<double>>, complex<double>> Manager::getNoreq(Node* target)
 {
	 target->setusednode();
	 //To be returned
	 pair<vector<complex<double>>, complex<double>> Final;
	 vector<Node*> loopednode = getRelatedNodes(target);
	 //Temporary array for scalars of our linear system
	 complex<double>* arr = new complex<double>[NoOfColumns];

	 // assing intial values to this temp array 
	 for (int w = 0; w < NoOfColumns; w++)
	 {
		 arr[w] = 0.0;
	 }

	 vector < complex<double> > ham;
	 complex <double> scaler;


		 for (int p = 0; p < loopednode.size(); p++) {
			 vector<Comp*> hello = getComp(target->getnum(), loopednode[p]->getnum());
			 vector <string> stringofcomps;

			 //Setting the related node to our super node as used
			 for (int s = 0; s < hello.size(); s++) {
				 stringofcomps.push_back(getType2(hello[s]));
			 }

			 for (int r = 0; r < stringofcomps.size(); r++)
			 {

				 if (stringofcomps[r] == "vsrc" || stringofcomps[r] == "vcvs" || stringofcomps[r] == "ccvs") {}
				 else if (stringofcomps[r] == "isrc")
				 {
					 isrc* temp = (isrc*)hello[r];
					 if (target->getnum() == hello[r]->getEndNode())
						 scaler += temp->getCurrent();
					 else
						 scaler -= temp->getCurrent();
				 }
				 else if (stringofcomps[r] == "cccs")
				 {
					 cccs* temp = (cccs*)hello[r];
					 int endRef = getNodeIndex(temp->getEndNoderef());
					 int strtRef = getNodeIndex(temp->getstartNoderef());
					 double coeff = temp->getCoff();
					 Node* endo= temp->getEndNoderef();
					 Node* start = temp->getstartNoderef();

					 complex<double> imp = getImpedance(endo, start);


					 arr[endRef] -= (coeff / imp);
					 arr[strtRef] += (coeff / imp);

				 }
				 else if (stringofcomps[r] == "vccs")
				 {
					 vccs* temp = (vccs*)hello[r];
					 int endRef = getNodeIndex(temp->getEndNoderef());
					 int strtRef = getNodeIndex(temp->getstartNoderef());
					 double coeff = temp->getCoff();


					 arr[endRef] -= (coeff);
					 arr[strtRef] += (coeff);
				 }
				 else if (stringofcomps[r] == "res" || stringofcomps[r] == "cap" || stringofcomps[r] == "ind")
				 {
					 resistors* temp = (resistors*)hello[r];
					 int mainnode = getNodeIndex( checkNode( target->getnum()) );
					 complex<double> G = 1.0 / temp->getComplex();
					 int other;

					 if (temp->getStartNode() == target->getnum()) {
						 other = temp->getEndNode();
					 }
					 else
					 {
						 other = temp->getStartNode();
					 }

					 int otherINdex = getNodeIndex( checkNode( other ) );
					 arr[mainnode] += G;

					 arr[otherINdex] -= G;

				 }


			 }

		 }

	 
	 //Pushing back the temp array values into the row
	 for (int w = 0; w < NoOfColumns; w++)
	 {
		 ham.push_back(arr[w]);
	 }

	 Final.first = ham;
	 Final.second = scaler;

	 return Final;
 }


 void Manager::printresults(MatrixXcd results)
 {
	 ofstream myfile;
	 myfile.open("Results.txt"); // opens output txt file
	 myfile << "Voltage at each node :\n"; // write in output file
	 for (int i = 0; i < results.size(); i++)
	 {
		 myfile << "V" << getNodeOfIndex(i)->getnum() << " --->  " << results(i) << endl;
	 }
	 myfile << "\n (: End Of solution";
	 myfile.close(); // closes file
 }
