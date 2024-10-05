#pragma once
#include <iostream>

class testClass{
public:
	int a;
	std::string test;
	testClass(int b) : a(b){
		std::cout<<"Constructing based on int: "<<b<<std::endl;
	}
	~testClass(){
		std::cout<<"Deleting! "<<test<<std::endl;
	}
	void doSomething(float var, bool testVar){
		std::cout<<"Something is being done here based on this float: "<<var<<" and this bool: "<<testVar<<std::endl;
	}
};
