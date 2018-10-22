#include "MeshDivide.h"
#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/LoopT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/SubdividerT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/LongestEdgeT.hh>
#include <iostream>

namespace MeshDivide
{

	void subDivCatClark(MyMesh &mesh, int iterations)
	{
		// Init subdivider
		OpenMesh::Subdivider::Uniform::CatmullClarkT<MyMesh> cc;
		cc.attach(mesh);
		bool succ=cc( iterations,true);
		if(!succ){std::cout<<"\nSubdivision problems."; exit(1); }
		cc.detach();
   	}

	void subDivLoop(MyMesh &mesh, int iterations, int weights)
	{
		// Init subdivider
		OpenMesh::Subdivider::Uniform::LoopT<MyMesh> loop;
		loop.attach(mesh);
		loop.init_weights(weights);
		bool succ=loop( iterations );
		if(!succ){std::cout<<"\nSubdivision problems."; exit(1); }
		loop.detach();
   	}

	void subDivEdgeLength(MyMesh &mesh, int iterations, float edgelength )
	{
		// Init subdivider
		OpenMesh::Subdivider::Uniform::LongestEdgeT<MyMesh> le;
		le.attach(mesh);
		le.set_max_edge_length((double)edgelength);
		bool succ=le( iterations,true );
		if(!succ){std::cout<<"\nSubdivision problems."; exit(1); }
		le.detach();
   	}

	void subDivideEqual(MyMesh& mesh)
	{
		MyMesh newmesh;
		MyMesh::Point mpt;
		MyMesh::Point pt1;
		MyMesh::Point pt2;
		int numnewverts=mesh.n_vertices()+mesh.n_edges();
		int numnewfaces=mesh.n_faces()*4;
		int numoldverts=mesh.n_vertices();
		int numoldfaces=mesh.n_faces();
		MyMesh::FaceIter f_it;
		MyMesh::FaceHalfedgeIter fe_it;
		MyMesh::FaceVertexIter fv_it;
		int idx=0;
		int vertid0,vertid1,vertid2;
		int edgeid0,edgeid1,edgeid2,edgeid;

		MyMesh::VertexHandle* vhandles=new MyMesh::VertexHandle[numnewverts];

		// Iterate over faces and than push_back points
    		for(f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
		{
		    	// Skim edges - add midpoints
		   	for(fe_it=mesh.fh_iter(*f_it); fe_it.is_valid(); ++fe_it)
		    	{
			    	edgeid=(mesh.edge_handle(*fe_it)).idx()+numoldverts;
				if(vhandles[edgeid].idx()!=-1) { continue; }
				else
				{
					pt1=mesh.point(mesh.to_vertex_handle(*fe_it));
					pt2=mesh.point(mesh.from_vertex_handle(*fe_it));

					mpt[0]=(pt1[0]+pt2[0])*0.5;
					mpt[1]=(pt1[1]+pt2[1])*0.5;
					mpt[2]=(pt1[2]+pt2[2])*0.5;
				    	vhandles[edgeid]=newmesh.add_vertex(MyMesh::Point(mpt[0],mpt[1],mpt[2]));
				}
			}

			// Skim verts - add old verts
			for(fv_it=mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			{
			    	idx=fv_it->idx();
				if(vhandles[idx].idx()==-1){ vhandles[idx]=newmesh.add_vertex(mesh.point(*fv_it)); }

			}
		}

		// Now add the faces
		std::vector<MyMesh::VertexHandle> face_vhandles(3);
    		for(f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
		{
	
		    	// Skim edges - collect points
	   		fe_it=mesh.fh_iter(*f_it);

			// Edge 1
		    	edgeid0=mesh.edge_handle(*fe_it).idx()+numoldverts;
			vertid0=mesh.from_vertex_handle(*fe_it).idx();
			fe_it++;

			// Edge 2
		    	edgeid1=mesh.edge_handle(*fe_it).idx()+numoldverts;
			vertid1=mesh.from_vertex_handle(*fe_it).idx();
			fe_it++;

			// Edge 3
		    	edgeid2=mesh.edge_handle(*fe_it).idx()+numoldverts;
			vertid2=mesh.from_vertex_handle(*fe_it).idx();

			// First face
			face_vhandles[0]=vhandles[edgeid2];
			face_vhandles[1]=vhandles[vertid0];
			face_vhandles[2]=vhandles[edgeid0];
			newmesh.add_face(face_vhandles);

			// Second face
			face_vhandles[0]=vhandles[edgeid1];
			face_vhandles[1]=vhandles[vertid2];
			face_vhandles[2]=vhandles[edgeid2];
			newmesh.add_face(face_vhandles);

			// Third face
			face_vhandles[0]=vhandles[edgeid0];
			face_vhandles[1]=vhandles[vertid1];
			face_vhandles[2]=vhandles[edgeid1];
			newmesh.add_face(face_vhandles);

			// Mid face 
			face_vhandles[0]=vhandles[edgeid0];
			face_vhandles[1]=vhandles[edgeid1];
			face_vhandles[2]=vhandles[edgeid2];
			newmesh.add_face(face_vhandles);

		}
		delete[] vhandles;
		mesh=newmesh;
	}
}
