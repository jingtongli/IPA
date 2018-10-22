#include "Orientation.h"
#include "OriListIO.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

int main()
{

    Eigen::MatrixXd m(3,4);

    m(0,0)=1.0;
    m(1,1)=1.0;
    m(2,2)=1.0;

    m(0,3)=1.0;
    m(1,3)=2.0;
    m(2,3)=3.0;

    Orientation ori1;
    Orientation ori2;
    ori1.setP(m);
    ori2.setP(m);

    std::vector<Orientation> orivec;
    orivec.push_back(ori1);
    orivec.push_back(ori2);

    std::vector<std::string> namevec;
    namevec.push_back("cool1");
    namevec.push_back("cool2");

    std::string savename("/home/mathias/AlmostTrash/orilistio.txt");
    //OriListIO::saveList(orivec,namevec,savename);
 	//exit(1);

    orivec.clear();
    OriListIO::loadList(orivec,savename);

    Eigen::Matrix3d R=orivec[1].getR();
    Eigen::Vector3d C=orivec[1].getC();


    	for(int y=0; y<R.rows(); y++)
    	{
    		for(int x=0; x<R.cols(); x++)
    		{
		    std::cout<<std::fixed<<std::setprecision(12)<<" "<<R(y,x);
		}
	}

	std::cout<<"\n"<<C(0)<<" "<<C(1)<<" "<<C(2);
    return 1;

}
