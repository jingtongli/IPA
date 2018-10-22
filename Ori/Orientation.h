//author: mathias, 2017, mathias.rothermel@googlemail.com
#pragma once

#include <eigen3/Eigen/Dense>
#include <string>
 
class Orientation
{
    public:

	// Constructors
	Orientation();
	Orientation(const std::string &filename);
	~Orientation();

	// IO operation
	bool writeFile(const std::string &filename) const;
	bool readFile(const std::string &filename);

	// Getters
	Eigen::Matrix3d getR( void ) const;
	Eigen::Matrix3d getK( void ) const;
	Eigen::Vector3d getC( void ) const;
	Eigen::MatrixXd getP( void ) const;
	int cols( void ) const;
	int rows( void ) const;

	// Setters
	void setR( Eigen::Matrix3d &R );
	void setK( Eigen::Matrix3d &K );
	void setC( Eigen::Vector3d &C );
	void setP( Eigen::MatrixXd &P );
	void setName( const std::string &name );
	void setCols( int cols );
	void setRows( int rows );

	void compensateOffset( const Eigen::Vector3d &O);
	// Scaling
	void downscalePyr(const int numberofpyrs);
	void downscaleByFac(const float fac);
    private:

	void reset();
	void downscaleOnePyr();

	std::string _name;
	int _cols;
	int _rows;

	Eigen::Matrix3d _R;
	Eigen::Matrix3d _K;
	Eigen::Vector3d _C;
};
