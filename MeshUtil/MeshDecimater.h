#include "MyMesh.h"
#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>

namespace MeshDecimater
{
	typedef OpenMesh::Decimater::DecimaterT<MyMesh> Decimater;
	typedef OpenMesh::Decimater::ModQuadricT<MyMesh>::Handle HModQuadric;

	void DecimaterQuadricT(MyMesh &mesh, double max_err, double err_tolerance_factor);

}
