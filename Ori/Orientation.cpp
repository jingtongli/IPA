//author: mathias, 2017, mathias.rothermel@googlemail.com
#include "Orientation.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <opencv2/calib3d/calib3d.hpp>

Orientation::Orientation()
{
	reset();
}

Orientation::Orientation(const std::string &name)
{
	reset();
	readFile(name);
}

Orientation::~Orientation()
{

}

void Orientation::reset()
{
	_name="none";
	_cols=-1;
	_rows=-1;
	_R.setZero();
	_K.setZero();
	_C.setZero();
}

// This is not tested and somewhat guessed
void Orientation::downscaleByFac(const float fac)
{
    	int oldrows=_rows;
	_cols=floor(((float)_cols)*fac);
	_rows=floor(((float)_rows)*fac);

	float factor=(float)_rows/(float)oldrows;
	_K(0,0)*=factor;
	_K(1,1)*=factor;
	_K(0,2)=(float)(_cols-1)/2.0;
	_K(1,2)=(float)(_rows-1)/2.0;
}


void Orientation::downscalePyr(const int numberofpyrs)
{
    	for(int i=0;i<numberofpyrs;i++)	{ downscaleOnePyr(); }
}

void Orientation::compensateOffset( const Eigen::Vector3d &O)
{
    _C-=O;
}
// The respective image downsampling is filter,i
// start on first pixel take every second, fine with opencv
void Orientation::downscaleOnePyr()
{
    	int oldrows=_rows;
	_cols=floor(((float)_cols+1.0)*0.5);
	_rows=floor(((float)_rows+1.0)*0.5);

	float factor=(float)_rows/(float)oldrows;
	_K*=factor;
	_K(2,2)=1.0;
}

// Getters //
int Orientation::cols( void ) const { return _cols; }
int Orientation::rows( void ) const { return _rows; }
Eigen::Matrix3d Orientation::getR( void ) const { return _R; }
Eigen::Matrix3d Orientation::getK( void ) const { return _K; }
Eigen::Vector3d Orientation::getC( void ) const { return _C; }
Eigen::MatrixXd Orientation::getP( void ) const
{
	Eigen::MatrixXd P(3,4);
	Eigen::Matrix3d KR; KR=_K*_R;
	Eigen::Vector3d RC; RC=(-1.0)*_K*_R*_C;

	P<< 	KR(0,0), KR(0,1), KR(0,2), RC(0),
 		KR(1,0), KR(1,1), KR(1,2), RC(1),
 		KR(2,0), KR(2,1), KR(2,2), RC(2);

	return P;
}

// Easy Setters //
void Orientation::setR( Eigen::Matrix3d &R ){ _R=R; }
void Orientation::setK( Eigen::Matrix3d &K ){ _K=K; }
void Orientation::setC( Eigen::Vector3d &C ){ _C=C; }
void Orientation::setCols( const int cols ){ _cols=cols; }
void Orientation::setRows( const int rows ){ _rows=rows; }
void Orientation::setName( const std::string &name ){ _name=name; }
void Orientation::setP( Eigen::MatrixXd &P )
{

    double aP[12]={	P(0,0), P(0,1), P(0,2), P(0,3),
    			P(1,0), P(1,1), P(1,2), P(1,3),
   			P(2,0), P(2,1), P(2,2), P(2,3)	};

    cv::Mat cvP(3,4,CV_64F,&aP);

    // Here we need a decomposition
    cv::Mat K,R,C;
    cv::decomposeProjectionMatrix( cvP, K, R, C);

    _R << R.at<double>(0,0), R.at<double>(0,1), R.at<double>(0,2),
	  R.at<double>(1,0), R.at<double>(1,1), R.at<double>(1,2),
	  R.at<double>(2,0), R.at<double>(2,1), R.at<double>(2,2);

    _K << K.at<double>(0,0), K.at<double>(0,1), K.at<double>(0,2),
	  K.at<double>(1,0), K.at<double>(1,1), K.at<double>(1,2),
	  K.at<double>(2,0), K.at<double>(2,1), K.at<double>(2,2);

    C/=C.at<double>(3,0);
    _C << C.at<double>(0,0), C.at<double>(1,0), C.at<double>(2,0);
}

// Writing the file yo
bool Orientation::writeFile(const std::string &filename) const
{

	FILE * fid;
	fid = fopen(filename.c_str(),"w");
	if(!fid)
	{
	    	std::cout<<"\nProblems openeing file "<<filename<<".";
		return 0;
	}
	fprintf(fid,"$id %s", _name.c_str());
	fprintf(fid,"\n$rc %16i %16i",_rows, _cols);
	fprintf(fid,"\n$R %16.8f %16.8f %16.8f %16.8f %16.8f %16.8f %16.8f %16.8f %16.8f",_R(0,0),_R(0,1),_R(0,2),_R(1,0),_R(1,1),_R(1,2),_R(2,0),_R(2,1),_R(2,2));
	fprintf(fid,"\n$K %16.8f %16.8f %16.8f %16.8f %16.8f %16.8f %16.8f %16.8f %16.8f",_K(0,0),_K(0,1),_K(0,2),_K(1,0),_K(1,1),_K(1,2),_K(2,0),_K(2,1),_K(2,2));
	fprintf(fid,"\n$C %16.6f %16.6f %16.6f",_C(0),_C(1),_C(2));

	fclose(fid);

	return 1;
}

// Reading the file yo
bool Orientation::readFile(const std::string &filename)
{

    	std::string buffer;
    	std::stringstream ss;

	// Open filestream
    	std::fstream fid;
	fid.open(filename, std::ifstream::in );
	if(!fid)
	{
	  	std::cout<<"\nError: file "<< filename <<" not found \n";
		return 0;
	}
    	// String reading buffer
    	while(fid >> buffer)
    	{
         	if(buffer=="$id")
                {
			fid>>_name; continue;
                }
        	if(buffer=="$rc")
        	{
                    	fid>>buffer;
                    	ss<<buffer;
			ss>>_rows;
                    	ss.str("");
                    	ss.clear();

			fid>>buffer;
                    	ss<<buffer;
			ss>>_cols;
                    	ss.str("");
                    	ss.clear();
		}
        	if(buffer=="$R")
        	{
			for(int y=0; y<3; y++)
			{
				for(int x=0; x<3; x++)
				{
                    			fid>>buffer;
                    			ss<<buffer;
					ss>>_R(y,x);
                    			ss.str("");
                    			ss.clear();
				}
			}
		}
        	if(buffer=="$K")
        	{
			for(int y=0; y<3; y++)
			{
				for(int x=0; x<3; x++)
				{
                    			fid>>buffer;
                    			ss<<buffer;
					ss>>_K(y,x);
                    			ss.str("");
                    			ss.clear();
				}
			}
		}
        	if(buffer=="$C")
        	{
			for(int y=0; y<3; y++)
			{
                    		fid>>buffer;
                    		ss<<buffer;
				ss>>_C(y);
                    		ss.str("");
                    		ss.clear();
			}
		}
	}
	fid.close();
	return 1;
}

