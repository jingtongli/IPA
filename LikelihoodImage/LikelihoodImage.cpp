// author: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch

#include "LikelihoodImage.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>

LikelihoodImage::LikelihoodImage() :

	_cols(-1),
	_rows(-1),
	_labels(-1),
	 _data(NULL)
{
}

LikelihoodImage::LikelihoodImage(const int rows, const int cols, const int labels) :
    _cols(cols),
    _rows(rows),
    _labels(labels),
    _data(NULL)
{
	allocateImage(rows,cols,labels);
}

LikelihoodImage::~LikelihoodImage()
{
    delocateImage();
}

void LikelihoodImage::delocateImage()
{
    if(_data)
    {
	for(int y=0; y<_rows; y++)
	{
		for(int x=0; x<_cols; x++)
		{
			delete[] _data[y][x];
		}
		delete[] _data[y];
	}
	delete[] _data;
    }
    _data=NULL;
}

void LikelihoodImage::allocateImage(const int rows, const int cols, const int labels)
{
    if(!_data)
    {
	_data=new float**[rows];

	for(int y=0; y<rows; y++)
	{
	    	_data[y]=new float*[cols];
		for(int x=0; x<cols; x++)
		{
		 	_data[y][x]=new float[labels];
		}
	}
    }
    else
    {
 	delocateImage();
	allocateImage(rows,cols,labels);
    }
}

// Slow... reorganize data structure [labels][rows][cols]
void LikelihoodImage::downscale(const int level)
{
	int newcols, newrows;

    	// Copy the image to cv mats
	std::vector<cv::Mat> cvimg(_labels);
	for(int l=0;l<_labels;l++)
	{
		cvimg[l].create(_rows,_cols,CV_32F);
		cv::Mat& cur=cvimg[l];
		for(int y=0; y<_rows; y++)
		{
			for(int x=0; x<_cols; x++)
			{
				cur.at<float>(y,x)=_data[y][x][l];
			}
		}
		for(int i=0; i<level; i++)
		{
			cv::pyrDown(cur,cur);
		}
		newcols=cur.cols;
		newrows=cur.rows;
	}

	// Memory stuff
	delocateImage();
	allocateImage(newrows,newcols,_labels);

	float min=1000;
	float max=0;
	// Copy back
	for(int l=0;l<_labels;l++)
	{
		cv::Mat& cur=cvimg[l];

		for(int y=0; y<newrows; y++)
		{
			for(int x=0; x<newcols; x++)
			{
				_data[y][x][l]=cur.at<float>(y,x);
	//			if(_data[y][x][l]>max)max=_data[y][x][l];
	//			if(_data[y][x][l]<min)min=_data[y][x][l];
			}
		}
		cvimg[l].release();

	}
	_cols=newcols;
	_rows=newrows;

}

void LikelihoodImage::getLayer(const int layer, cv::Mat &img)
{
   	if (img.cols!=_cols){ std::cout<<"\nLikli: Cols mismatch ("<<_cols<<"/"<<img.cols<<"). Exit."; exit(1); }
   	if (img.rows!=_rows){ std::cout<<"\nLikli: Rows mismatch. Exit."; exit(1); }

	float* cvptr=img.ptr<float>(0);

	for(int y=0; y<_rows; y++)
	{
		for(int x=0; x<_cols; x++)
		{
		    *(cvptr++)=_data[y][x][layer];

		}
	}
}


void LikelihoodImage::setLayer(const int layer, cv::Mat &img)
{
   	if (img.cols!=_cols){ std::cout<<"\nLikli: Cols mismatch ("<<_cols<<"/"<<img.cols<<"). Exit."; exit(1); }
   	if (img.rows!=_rows){ std::cout<<"\nLikli: Rows mismatch. Exit."; exit(1); }

	float* cvptr=img.ptr<float>(0);

	for(int y=0; y<_rows; y++)
	{
		for(int x=0; x<_cols; x++)
		{
		    _data[y][x][layer]=(*cvptr++);
		}
	}
}

void LikelihoodImage::loadImage(const std::string &name)
{

	FILE * pFile;
	long lSize;
        char * buffer;
	size_t result;

	 pFile = fopen ( name.c_str() , "rb" );
	 if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	 // obtain file size:
	 fseek (pFile , 0 , SEEK_END);
         lSize = ftell (pFile);
         rewind (pFile);

         // allocate memory to contain the whole file:
	 buffer = (char*) malloc (sizeof(char)*lSize);
	 if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	 // copy the file into the buffer:
	 result = fread (buffer,1,lSize,pFile);
	 if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	 float* ptr=(float*)buffer;
	 _rows=*ptr; ptr++;
	 _cols=*ptr; ptr++;
	 _labels=*ptr; ptr++;
	allocateImage(_rows,_cols,_labels);
	for(int y=0; y<_rows; y++)
	{
		for(int x=0; x<_cols; x++)
		{
			for(int l=0; l<_labels; l++)
			{
                    		_data[y][x][l]=*ptr;
				ptr++;
			}
		}
	}
	fclose (pFile);
	free (buffer);

}

void LikelihoodImage::saveImage(const std::string &name)
{


	float* floatbuffer=new float[_rows*_cols*_labels+(size_t)3];

	std::cout<<(size_t)_rows*_cols*_labels+(size_t)3;
	floatbuffer[0]=(float)_rows;
	floatbuffer[1]=(float)_cols;
	floatbuffer[2]=(float)_labels;
	float* ptr=floatbuffer+3;
	
	for(int y=0; y<_rows; y++)
	{
		for(int x=0; x<_cols; x++)
		{
			for(int l=0; l<_labels; l++)
			{
				*ptr=_data[y][x][l]; ptr++;
			}
		}
	}
	FILE *my_file = fopen(name.c_str(), "wb");
	fwrite( floatbuffer, sizeof(float), _cols*_rows*_labels+3, my_file);
	fclose(my_file);
   	delete[] floatbuffer;
}

void LikelihoodImage::checkisOne( void )
{
    	std::cout<<"\n<<Checking validity...";
	for(int y=0; y<_rows; y++)
	{
		for(int x=0; x<_cols; x++)
		{
		    	float sum=0;
			for(int l=0;l<_labels;l++)
			{
			    if(_data[y][x][l]>1.0 ) { _data[y][x][l]=0.0; std::cout<<"\nEntry larger 1 detected"; }
				sum+=_data[y][x][l];

			}
			if(fabs(sum-1.0)>0.001){
			    	std::cout << "\n"<<sum; 
				for(int l=0;l<_labels;l++)
				{
					std::cout<<" "<<_data[y][x][l];
				}
			}
		}
	}
	std::cout<<"\n...done";

}

void LikelihoodImage::loadImageMaros(const std::string &name)
{
	int l;
    	// Zuerich
    	//_cols=1500;
    	//_rows=1122;
    	//_labels=5;

    	// Enschede
    	_cols=1404;
    	_rows=936;
    	_labels=5;

	// Dortmund
    	//_cols=1362;
    	//_rows=1020;
    	//_labels=5;

	// Synth city
	//_cols=1416;
	//_rows=1062;
	//_labels=5;

	// South
	//_cols=768;
	//_rows=576;
	//_labels=5;

	allocateImage(_rows,_cols, _labels);

	std::fstream fid;
    	fid.open(name, std::fstream::in);
    	if(!fid)
    	{
	    std::cout<<"\nLiklihoodimage: Problem opening file: "<< name;
	    exit(1);
    	}

    	// String reading buffer
    	std::string buffer;
    	std::stringstream ss;

	// Read actual content
	for(int x=0; x<_cols; x++)
	{
		for(int y=0; y<_rows; y++)
		{
	//		for(int l=0; l<_labels; l++)
			{
			    	// Get the whole line string
                    		fid>>buffer;
                    		ss<<buffer;

				l=0;
	        		while(ss>>_data[y][x][l])
		        	{
				   // std::cout<<"\n"<<_data[y][x][l];
				    l++;
				    if (ss.peek() == ',')
					ss.ignore();
				}
                   	 	ss.str("");
                    		ss.clear();
			}
	    	}
	}
	fid.close();
}

int LikelihoodImage::cols( void ) const { return  _cols; }
int LikelihoodImage::rows( void ) const {  return _rows; }
int LikelihoodImage::labels( void ) const {  return _labels; }

float& LikelihoodImage::getData(const int y, const int x, const int l){ return _data[y][x][l]; }

