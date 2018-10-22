// autor: mathias, 2016, mathias.rothermel@geod.baug.ethz.ch  
#include "MeshGeom.h"
#include <math.h>
#include <Eigen/Dense>

namespace MeshGeom
{

 	float faceArea(float* v1, float* v2, float* v3)
	{
	    	float x[3]; float y[3];
	    	x[0]=v2[0]-v1[0]; x[1]=v2[1]-v1[1]; x[2]=v2[2]-v1[2];
	    	y[0]=v3[0]-v1[0]; y[1]=v3[1]-v1[1]; y[2]=v3[2]-v1[2];

		return 0.5*sqrt(pow( x[1]*y[2]-x[2]*y[1],2.0) +
				pow( x[2]*y[0]-x[0]*y[2],2.0) +
				pow( x[0]*y[1]-x[1]*y[0],2.0) );

	}

 	float faceArea2D(float* v1, float* v2, float* v3)
	{
		Eigen::Matrix3f M;
		M << 1.0, 1.0, 1.0, v1[0], v2[0], v3[0], v1[1], v2[1], v3[1];
		return 0.5*fabs(M.determinant());
	}

	// p point on line
	// n line direction
	// E plane xa+by+cz+d=0;
	// i output point
	void intersectPlaneLine( Eigen::Vector3d  p, Eigen::Vector3d v, Eigen::Vector4d E, Eigen::Vector3d& i)
	{
		double alpha=(-1)*(E(3)+E(0)*p(0)+E(1)*p(1)+E(2)*p(2))/(E(0)*v(0)+E(1)*v(1)+E(2)*v(2));
		i=p+alpha*v;

	}

	void centerGravOfFace(Eigen::Vector3f& pt0, Eigen::Vector3f& pt1, Eigen::Vector3f& pt2, Eigen::Vector3f& center)
	{
	    	center<< (pt0(0)+pt1(0)+pt2(0))/3.0,(pt0(1)+pt1(1)+pt2(1))/3.0,(pt0(2)+pt1(2)+pt2(2))/3.0;
	}
}
