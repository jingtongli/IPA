#include "Orientation.h"
#include <iostream>

int main()
{
    	Orientation myfirstori;
	std::string name("../data/4.ori");
	//myfirstori.writeFile(name);
	myfirstori.readFile(name);
	Eigen::Vector3d vec(3,1);
	vec<< myfirstori.getC(); //4,5,6;
	myfirstori.setC(vec);
	myfirstori.writeFile("../data/no_use.ori");

	std::cout<<myfirstori.getR()(1,1)<<std::endl<<"Finished";

}