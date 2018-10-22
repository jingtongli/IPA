#include <string>
#include "../MeshUtil/MyMesh.h"
#include "../MeshUtil/MeshIO.h"

int main()
{

	MyMesh mesh;
	std::string loadname("/home/mathias/workspace/meshrefinement/input_data/toyPly_big.ply");
	MeshIO::readMesh(mesh,loadname,true,false);

	std::string savename("/home/mathias/workspace/urbanup/data/out.ply");
	MeshIO::writeMesh(mesh,savename,true,false);
}


