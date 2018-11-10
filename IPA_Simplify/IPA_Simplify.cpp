#include "../Ori/Orientation.h"
#include "../MeshUtil/MyMesh.h"
#include "../MeshUtil/MeshIO.h"
#include "../MeshUtil/MeshDecimater.h"
#include "IPA_LoadList.h"
#include "IPA_Processing.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <numeric>


#define PI 3.14159265

cv::Mat src_img, blurred_img, edge_img;
int lowThreshold = 0;
const char *SrcTitle = "Original Image";
const char *CannyTitle = "Canny Edge Image";

void CannyThreshold(int,void*)
{
	cv::blur(src_img, blurred_img, cv::Size(3, 3));
	cv::Canny(blurred_img, edge_img, lowThreshold, lowThreshold*3, 3);
	cv::imshow(CannyTitle,edge_img);
 }


int main()
{
	// Loading mesh
	MyMesh mesh;
	std::string meshloadname("../data/Mesh_dense.ply");
	MeshIO::readMesh(mesh, meshloadname, false, false);

	// Decimate mesh
	// MeshDecimater::DecimaterQuadricT(mesh,1e-8,0.5);

	// Loading Images and Oris
	vector<string> images;
	vector<string> oris;
	readfile("../data/imglist.txt",images);
	readfile("../data/orilist.txt",oris);

//	// Show images
//	cv::namedWindow(SrcTitle,cv::WINDOW_AUTOSIZE);
//	cv::namedWindow(CannyTitle,cv::WINDOW_AUTOSIZE);
//	cv::createTrackbar( "Threshold:", "Canny Edge Image", &lowThreshold, 255, CannyThreshold);
//	cv::imshow(SrcTitle, src_img);
//	CannyThreshold(0,0);
//	cv::waitKey(0);

	for (int i=0; i<images.size();i++)
	{
		src_img = cv::imread(images[i]);
		Orientation ori(oris[i]);

		// Create edge image
		cv::blur(src_img, blurred_img, cv::Size(3, 3));
		cv::Canny(blurred_img, edge_img, lowThreshold, lowThreshold*3, 3);

		// Main Processing
		float angle_threshold=cos((180-75)*PI/180.0);
		setVertexNumEdge(mesh, edge_img, ori, angle_threshold,false);

	}

	// Assign colors to vertices
	color3D(mesh);

	// Write mesh
	std::string meshsavename("/home/jingtongli/Desktop/ipa/data/Mesh_out.ply");
	MeshIO::writeMesh(mesh, meshsavename, true, false);

	std::cout << "Finished";

	return 0;
}
