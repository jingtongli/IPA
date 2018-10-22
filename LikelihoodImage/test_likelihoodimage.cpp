#include "LikelihoodImage.h" 
#include <string>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "../IOList/IOList.h"

int main ( void )
{

	// Example file
    	//std::string filename("/media/data/mathias/Zuerich_ObliquePenta/LikelihoodFloat/mb_class_data_spatial_roof_nb5_20130424121239_61010_posteriors_adjusted_scores_likeli.txt");

    	std::string outputpath("/media/data/mathias/labelflip_data/Tower/Converted/img");

	// Load 
	IOList imglist("/media/data/mathias/labelflip_data/Tower/likeli/imglist.txt");

	for(int i=0; i<imglist.size(); i++)
	{
	    	std::cout<<"\nProcessing image: "<< imglist.getNameWithoutEnding(i);
		LikelihoodImage limg;
		limg.loadImageMaros(imglist.getElement(i));
		limg.checkisOne();
		std::string currentname(outputpath);
		currentname.append("/");
		currentname.append(imglist.getNameWithoutEnding(i));
		currentname.append(".bin");
		limg.saveImage(currentname);

		int size=limg.rows()*limg.cols();
		unsigned char arr[size];
		cv::Mat cvimg(limg.rows(), limg.cols(), CV_8UC1, arr);
		int id=0;
		for(int y=0;y<limg.rows();y++){
			for(int x=0;x<limg.cols();x++){
		 	   arr[id]=(limg.getData(y,x,0)*255.0); id++;
			}
		}
		std::string imname("/media/data/mathias/labelflip_data/South/Converted/test.png");
		cv::imwrite(imname,cvimg );

		LikelihoodImage limg2;
		limg2.loadImage(currentname);
		limg2.checkisOne();
}
	/*
	std::cout<<"\nImage loaded";
	std::cout<<"\n data:" << limg.getData(1,1,2) <<" "<<limg.getData(100,100,5) << " " << limg.getData(500,688,2);

    	std::string savename("/media/data/mathias/Zuerich_ObliquePenta/test_bitch.txt");
	limg.saveImage(savename);
	std::cout<<"\nImage saved";

	int size=limg.rows()*limg.cols();
	unsigned char arr[size];
	cv::Mat cvimg(limg.rows(), limg.cols(), CV_8UC1, arr);
	int id=0;
	for(int y=0;y<limg.rows();y++){
		for(int x=0;x<limg.cols();x++){
		    arr[id]=(limg.getData(y,x,0)*255.0); id++;
		}

	std::string imname("/media/data/mathias/Zuerich_ObliquePenta/img.png");
	cv::imwrite(imname,cvimg );

	LikelihoodImage limg2;
	limg2.loadImage(savename);

	std::cout<<"\nImage loaded 2";

	std::cout<<"\n data:" << limg2.getData(1,1,2) <<" "<<limg2.getData(100,100,5) << " " << limg2.getData(500,688,2);
	*/
	return 1;
}


