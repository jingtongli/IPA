// author: mathias, 2018, mathias.rothermel@geod.baug.ethz.ch
#include "OriListIO.h"
#include <sstream>
#include <istream>
#include <iterator>
#include <fstream>
#include <iostream>
#include <iomanip>

OriListIO::OriListIO()
{

}

OriListIO::~OriListIO()
{
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
        std::ostringstream out;
	out << std::fixed <<std::setprecision(n) << a_value;
	return out.str();
}

void OriListIO::convertOriToString(const Orientation& ori, std::string& s)
{
	Eigen::MatrixXd P=ori.getP();

    	s.clear();
    	s="";

	// Getting P in
    	for(int y=0; y<P.rows(); y++)
    	{
    		for(int x=0; x<P.cols(); x++)
    		{
		    s.append(to_string_with_precision(P(y,x),12));
		    s.append(" ");
		}
	}

	// Getting rows / cols in
	s.append(std::to_string(ori.rows()));
	s.append(" ");
	s.append(std::to_string(ori.cols()));
}

void OriListIO::convertStringToOri(const std::string& s, Orientation&ori)
{
	std::string name=s.substr(0,s.find(" "));
	std::string rest=s.substr(s.find(" "));

	std::istringstream iss(rest);
	std::vector<double> results((std::istream_iterator<double>(iss)),std::istream_iterator<double>());

	// Contruct P
	Eigen::MatrixXd P(3,4);
	for(int y=0;y<3;y++)
	{
		for(int x=0;x<4;x++)
		{
			P(y,x)=results[y*4+x];
		}
	}
	ori.setP(P);
	ori.setRows(results[12]);
	ori.setCols(results[13]);
	ori.setName(name);
}

// Do the saving
void OriListIO::saveList(const std::vector<Orientation>& orivec, const std::vector<std::string>& namevec, const std::string& savename)
{
    	if(namevec.size()!=orivec.size()){std::cout<<"\nOriLsitIO: vectors must be same size!"; exit(1); }
    	std::string oristring;

	// Openfile
	std::ofstream ofs;
	ofs.open(savename, std::ofstream::out);
	if(!ofs.is_open()){ std::cout<<"Problems saving OriListIO file"; exit(1); }
	else
	{
		for(int i=0; i<orivec.size(); i++)
		{
			ofs<<namevec[i]<<" ";
			convertOriToString(orivec[i],oristring);
			ofs<<oristring;
			if(i!=orivec.size()-1){ ofs<<"\n"; }
		}
		ofs.close();
	}
}

// Do the loading
void OriListIO::loadList(std::vector<Orientation>& orivec, const std::string& savename)
{

	Orientation ori;
    	std::string oristring;
	orivec.clear();

	// Openfile
	std::ifstream ifs;
	ifs.open(savename, std::ifstream::in);
	if(!ifs.is_open()){ std::cout<<"Problems openeing OriListIO file"; exit(1); }
	else
	{
	    	while(ifs.peek()!= EOF)
		{
		    	std::getline(ifs,oristring);
			convertStringToOri(oristring,ori);
			orivec.push_back(ori);
		}
		ifs.close();
	}
}

