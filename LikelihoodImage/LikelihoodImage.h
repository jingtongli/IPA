#pragma once
#include <string>
#include <opencv2/core/core.hpp>

class LikelihoodImage
{

    public:
	LikelihoodImage();
	LikelihoodImage(const int rows, const int cols, const int labels);
	~LikelihoodImage();

	float& getData(int y, int x, int label);

	void loadImage(const std::string &name);
	void saveImage(const std::string &name);
	void loadImageMaros(const std::string &name);

	int cols( void ) const;
	int rows( void ) const;
	int labels( void ) const;

	void setLayer(const int layer, cv::Mat &img);
	void getLayer(const int layer, cv::Mat &img);

	void downscale(const int level);

	// Debug function
	void checkisOne( void );
    private:

	void delocateImage();
	void allocateImage(const int rows, const int cols, const int labels);

	int _cols;
	int _rows;
	int _labels;
	float*** _data;

};
