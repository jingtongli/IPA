// author: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch
#include "MyMesh.h"

// Standard header stuff
namespace MeshDivide
{
	void subDivCatClark(MyMesh &mesh, int iterations);

	void subDivLoop(MyMesh &mesh, int iterations, int weights=50);

	void subDivEdgeLength(MyMesh &mesh, int iterations, float edgelength);
	
	void subDivideEqual(MyMesh& mesh);
}

