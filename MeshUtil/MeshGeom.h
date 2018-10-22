// author: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch
#include "MyMesh.h"
#include <Eigen/Dense>

// Standard header stuff
namespace MeshGeom
{

    	float faceArea(float* v1, float* v2, float* v3);

 	float faceArea2D(float* v1, float* v2, float* v3);

	template <typename T> float faceArea( T &f_it, MyMesh* mesh);

	void intersectPlaneLine( Eigen::Vector3d p, Eigen::Vector3d v, Eigen::Vector4d E, Eigen::Vector3d& i);

	template <typename T> bool isInFace2D( T y, T x, T verts[3][3]);

	void centerGravOfFace(Eigen::Vector3f& pt0, Eigen::Vector3f& pt1, Eigen::Vector3f& pt2, Eigen::Vector3f& center);
}

// Template functions have to be provided in header if linked to other binaries...
namespace MeshGeom
{

	template <typename T> float faceArea(T &f_it, MyMesh* mesh )
	{
	    	float v1[3]; float v2[3]; float v3[3];
		MyMesh::FaceVertexIter fv_it=mesh->fv_iter(*f_it);
		v1[0]=mesh->point(*fv_it)[0]; v1[1]=mesh->point(*fv_it)[1]; v1[2]=mesh->point(*fv_it)[2];
		fv_it++;
		v2[0]=mesh->point(*fv_it)[0]; v2[1]=mesh->point(*fv_it)[1]; v2[2]=mesh->point(*fv_it)[2];
		fv_it++;
		v3[0]=mesh->point(*fv_it)[0]; v3[1]=mesh->point(*fv_it)[1]; v3[2]=mesh->point(*fv_it)[2];

		// Update area and neighbors
		return MeshGeom::faceArea(v1,v2,v3);
	}


	template <typename T> bool isInFace2D( T y, T x, T verts[3][3])
	{
	    	// Following discussion on:
		// http://stackoverflow.com/questions/2049582/
		// how-to-determine-if-a-point-is-in-a-2d-triangle
		T helper=	-verts[1][1]*verts[2][0] +
		    		 verts[0][1]*(verts[2][0]-verts[1][0]) +
				 verts[0][0]*(verts[1][1]-verts[2][1]) +
				 verts[1][0]*verts[2][1];

		T s=1.0/helper*(	(verts[0][1]*verts[2][0] -
					 verts[0][0]*verts[2][1] +
					 (verts[2][1]-verts[0][1])*x +
					 (verts[0][0]-verts[2][0])*y));

		T t=1.0/helper*(	(verts[0][0]*verts[1][1] -
					 verts[0][1]*verts[1][0] +
					 (verts[0][1]-verts[1][1])*x +
					 (verts[1][0]-verts[0][0])*y));

		return  s>0.0f && t>=0.0f && 1.0f-s-t>=0;

	}

}
