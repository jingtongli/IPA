// autor: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch
#include "OriGeom.h"
#include "Orientation.h"
#include <math.h>
#include <Eigen/Dense>
#include <iostream>
#include "GeomUtil.h"

namespace OriGeom
{

	void computeRelative( const Orientation &identity, const Orientation &other, Orientation &out )
	{
	    	// Copy
	    	out=other;

		// First Cam (after transform this is identity)
		Eigen::Vector3d C0=identity.getC();
		Eigen::Matrix3d R0=identity.getR();
		Eigen::Vector3d t0=(-1.0)*R0*C0;

		// Second cam
		Eigen::Vector3d C1=other.getC();
		Eigen::Matrix3d R1=other.getR();
		Eigen::Matrix3d K1=other.getK();
		Eigen::Vector3d t1=(-1.0)*R1*C1;

		// New cam
		Eigen::Matrix3d R10=R1*R0.transpose();
		Eigen::Vector3d C10=R1*C0+t1;

		out.setK(K1);
		out.setR(R10);
		out.setC(C10);
	}

	void computeSnippet( 	const Orientation& original,
	    		 	const int uplx,
			 	const int uply,
			 	const int newcols,
			 	const int newrows,
			 	Orientation& out)
	{
	    	if(uplx+newcols>=original.cols()){ std::cout<<"\nOrientation: New cols not matching image"; }
	    	if(uply+newrows>=original.rows()){ std::cout<<"\nOrientation: New rows not matching image"; }

		out=original;

		// Update cols
		out.setCols(newcols);
		out.setRows(newrows);

		// Update rows
		Eigen::Matrix3d K=out.getK();
		K(0,2)=K(0,2)-uplx;
		K(1,2)=K(1,2)-uply;
	}

    	double angleBetweenCams(const Orientation& ori0, const Orientation& ori1)
	{
	    	Eigen::Matrix3d R0=ori0.getR();
	    	Eigen::Matrix3d R1=ori1.getR();

		double nx0,ny0,nz0,nx1,ny1,nz1,angle;
		Eigen::Vector3d v0; v0<<R0(2,0),R0(2,1),R0(2,2);
		Eigen::Vector3d v1; v1<<R1(2,0),R1(2,1),R1(2,2);

		angle=acos((v0.dot(v1))/sqrt(v0.dot(v0))/sqrt(v1.dot(v1)));

		// Now we have to determine if its convergent or not. Intersect points 0 0 1 (z-axis for each cam)
		Eigen::Vector3d x; x<<0.0,0.0,1.0;
		Eigen::Vector3d X;
		GeomUtil::intersect(ori0,x,ori1,x,X);

		// Transform the point into the cam cordinate systems
		//X0=ori0.getR()*(X-ori0.getC());
		//X1=ori1.getR()*(X-ori1.getC());

		// Now check if the 

		return angle;

		// Now we have to check if convergent or not.
		//Eigen::Vector3d bl;
		//bl=v1-v0;

		// Normal of plane holding bl and v0
		//Eigen::Vector3d n;
		//n=bl.cross(v0);

		// Projection of v0 on plane
		//Eigen::Vector3d p1;
		//p1=v1-(v1.dot(n)/sqrt(n.dot(n))*n);

		// Now check if
		//a0=acos((v0.dot(bl))/sqrt(v0.dot(v0))/sqrt(bl.dot(bl)));
		//a1=acos((p1.dot(bl))/sqrt(p1.dot(p1))/sqrt(bl.dot(bl)));

		// Diverging?

	}
}
