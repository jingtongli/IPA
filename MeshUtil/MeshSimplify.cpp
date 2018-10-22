#include "MeshSimplify.h"
#include <iostream>

namespace MeshSimplify
{

	void simplifyQuadric(MyMesh &mesh, float maxerror)
	{
	    Decimater dec(mesh);
	    HModQuadric hModQuadric;
	    dec.add(hModQuadric);
	    dec.module(hModQuadric).set_max_err(maxerror);
	    dec.module(hModQuadric).set_binary(false);
	    bool succ=dec.initialize();
	    if(!succ){std::cout<<"\nMeshSimplify error."; exit(1); }
	    dec.decimate();
	    mesh.garbage_collection();
   	}

	void simplifyEdge(MyMesh &mesh, float minedgelength)
	{
	    mesh.request_vertex_status();
	    mesh.request_edge_status();
	    Decimater dec(mesh);
	    HModEdgeLength hModEdgeLength;
	    dec.add(hModEdgeLength);
	    dec.module(hModEdgeLength).set_binary(false);
	    dec.module(hModEdgeLength).set_edge_length((double)minedgelength);
	    dec.module(hModEdgeLength).set_error_tolerance_factor(1.0);
	    bool succ=dec.initialize();
	    if(!succ){std::cout<<"\nMeshSimplify error."; exit(1); }
	    dec.decimate();
	    mesh.garbage_collection();
   	}
}
