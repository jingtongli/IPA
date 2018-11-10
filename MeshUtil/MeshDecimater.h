#include "MyMesh.h"
#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>
#include "ModProjectionT.h"

namespace MeshDecimater
{
	typedef OpenMesh::Decimater::DecimaterT<MyMesh> Decimater;
	typedef OpenMesh::Decimater::ModQuadricT<MyMesh>::Handle HModQuadric;
	typedef OpenMesh::Decimater::ModProjectionT<MyMesh>::Handle HModProjection;

	void DecimaterQuadricT(MyMesh &mesh, double max_err, double err_tolerance_factor);
	void DecimaterProjectionT(MyMesh &mesh, int min_projection);
}
