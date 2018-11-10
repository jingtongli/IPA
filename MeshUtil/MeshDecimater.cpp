#include "MeshDecimater.h"
#include <iostream>

namespace MeshDecimater
{

	void DecimaterQuadricT(MyMesh &mesh, double max_err, double err_tolerance_factor)
	{
	    Decimater decimater(mesh);
	    HModQuadric hModQuadric;
	    decimater.add(hModQuadric);
	    // std::cout << decimater.module( hModQuadric ).name() << std::endl;

	    decimater.module(hModQuadric).set_max_err(max_err);
	    decimater.module(hModQuadric).set_error_tolerance_factor(err_tolerance_factor);
	    decimater.module(hModQuadric).set_binary(false);

	    bool succ=decimater.initialize();
	    if(!succ){std::cout<<"\nMeshSimplify error."; exit(1); }
	    decimater.decimate();
	    mesh.garbage_collection();
   	}

	void DecimaterProjectionT(MyMesh &mesh, int min_projection)
	{
	    mesh.request_vertex_normals();
	    mesh.request_face_normals();
	    mesh.update_normals();

	    Decimater decimater(mesh);
	    HModProjection hModProjection;
	    decimater.add(hModProjection);
	    // std::cout << decimater.module( hModQuadric ).name() << std::endl;

	    decimater.module(hModProjection).set_num_edge(min_projection);
	    decimater.module(hModProjection).set_binary(false);

	    bool succ=decimater.initialize();
	    if(!succ){std::cout<<"\nMeshSimplify error."; exit(1); }
	    decimater.decimate();
	    mesh.garbage_collection();
   	}
}
