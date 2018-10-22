// author: mathias, 2018, mathias.rothermel@geod.baug.ethz.ch
#include "GeomUtil.h"

namespace GeomUtil
{

void intersect( const Orientation& ori0, const Eigen::Vector3d x0,
		const Orientation& ori1, const Eigen::Vector3d x1,
		Eigen::Vector3d& X)

{
	Eigen::MatrixXd P0=ori0.getP();
	Eigen::MatrixXd P1=ori1.getP();

	// H&Z page 312.
    	Eigen::Matrix4d A;
	A.block(0,0,4,1)=x0(0)*P0.block(2,0,4,1)-P0.block(0,0,4,1);
	A.block(1,0,4,1)=x0(1)*P0.block(2,0,4,1)-P0.block(1,0,4,1);
	A.block(2,0,4,1)=x1(0)*P1.block(2,0,4,1)-P1.block(0,0,4,1);
	A.block(3,0,4,1)=x1(1)*P1.block(2,0,4,1)-P1.block(1,0,4,1);

	// Now solve Ax=0
	// Check: https://stackoverflow.com/questions/45945481/fastest-way-to-solve-small-homogeneous-linear-systems-with-eigen
	Eigen::Vector4d helper=A.jacobiSvd(Eigen::ComputeFullV).matrixV().col( A.rows() - 1 );
	helper/=helper(3,0);
	X<<helper(0),helper(1),helper(2);

}

// Plane Equation ax+by+cz+d=0
void planeFrom3Pts(const Eigen::Vector3d& pt0, const Eigen::Vector3d& pt1, const Eigen::Vector3d& pt2, Eigen::Vector4d& plane)
{
	Eigen::Vector3d pt10=pt1-pt0;
	Eigen::Vector3d pt20=pt2-pt0;
	Eigen::Vector3d n=pt10.cross(pt20);
	n.normalize();

	plane(3)=-1.0*(n(0)*pt0(0)+n(1)*pt0(1)+n(2)*pt0(2));
	plane(0)=n(0); plane(1)=n(1);plane(2)=n(2);

}

void intersectRayPlane(const Eigen::Vector3d& X0, const Eigen::Vector3d& X1, const Eigen::Vector4d& plane, Eigen::Vector3d& pt)
{

   	Eigen::Vector3d d=X1-X0;

	double alpha=-1.0* 	(plane(0)*X0(0)+plane(1)*X0(1)+plane(2)*X0(2)+plane(3) ) /
	    			(plane(0)*d(0)+plane(1)*d(1)+plane(2)*d(2));

	pt=X0+alpha*d;
}

} // end namespace
