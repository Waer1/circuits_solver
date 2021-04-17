#pragma once
#include <iostream>
#include <complex>

using namespace std;

class Node
{
	int num;
	complex<double> voltage;
	int count;
	bool taken;
	bool basnodestate;
	bool normal;



public:
	Node(int n);
	int getnum();
	void setVoltage(complex<double> v);
	void print();
	void increment();
	bool IsComplex();
	complex<double> getVolt();
	bool getstnode();
	void setusednode();

	bool getstbas();
	void setusedbas();
	
	bool getnormal();
	void setnormal();

};