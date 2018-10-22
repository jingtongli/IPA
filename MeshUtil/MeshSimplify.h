// author: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch
#include "MyMesh.h"
#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>
#include <OpenMesh/Tools/Decimater/ModEdgeLengthT.hh>

namespace MeshSimplify
{
	typedef OpenMesh::Decimater::DecimaterT<MyMesh>  Decimater;
	typedef OpenMesh::Decimater::ModQuadricT<MyMesh>::Handle HModQuadric;
	typedef OpenMesh::Decimater::ModEdgeLengthT<MyMesh>::Handle HModEdgeLength;
 
	void simplifyQuadric(MyMesh &mesh, float maxerror);

	void simplifyEdge(MyMesh &mesh, float minedgelength);
}

